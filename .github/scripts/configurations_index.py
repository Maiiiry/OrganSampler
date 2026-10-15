#!/usr/bin/env python3
"""Writes configurations/INDEX.md: one line per published organ configuration -
the link to its README (which says how to download it), the divisions, the
rows of the Stop List and the perspectives, read from the configuration file
itself. Run by the Configurations check workflow on every push to main and
committed back when it changed; it can be run by hand from the repository root.

The Stop List rows are counted as the program counts them: every stop of the
voicing state that is not nested in a stop folder, plus every transmission or
extension that is not nested in one.
"""
import json
import os
import sys
import zipfile

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'configurations')


def main():
    rows = []
    for folder in sorted(os.listdir(ROOT)):
        path = os.path.join(ROOT, folder)
        if not os.path.isdir(path):
            continue
        # The configuration is the .json inside the folder's zip (a bare .json is accepted too).
        cfg = None
        for name in sorted(os.listdir(path)):
            full = os.path.join(path, name)
            if name.lower().endswith('.zip'):
                with zipfile.ZipFile(full) as z:
                    inner = [n for n in z.namelist() if n.lower().endswith('.json')]
                    if inner:
                        cfg = json.loads(z.read(inner[0]).decode('utf-8'))
                        break
            elif name.lower().endswith('.json'):
                with open(full, encoding='utf-8') as f:
                    cfg = json.load(f)
                break
        if cfg is None:
            continue
        divisions = int(cfg.get('manualCount', 0)) + 1
        manuals = cfg.get('manualKeyboards', [])[:divisions]
        # The rows nested in a stop folder: the table's child rows (the voicing
        # state marks them by groupMaster only when it was saved by a newer program).
        nested = set()
        for manual, m in enumerate(manuals):
            for e in m.get('stopBuild', {}).get('entries', []) or []:
                if e.get('child'):
                    nested.add((manual, e.get('name', '')))
        stops = cfg.get('voicingSettings', {}).get('stops', [])
        count = 0
        for s in stops:
            manual = int(s.get('primaryManual', 0))
            if manual >= divisions or s.get('groupMaster') or (manual, s.get('name', '')) in nested:
                continue
            count += 1
        for s in stops:
            for t in s.get('transmissions') or []:
                if not t.get('groupMaster') and int(t.get('manual', 0)) < divisions:
                    count += 1
        # The perspectives, in the order of the perspective patterns (the user's own).
        order = [str(p).split(';')[0].strip() for p in cfg.get('perspectivePatterns', []) or []]
        perspectives = []
        for m in manuals:
            for p in m.get('stopBuild', {}).get('perspectives', []) or []:
                if p and p not in perspectives:
                    perspectives.append(p)
        perspectives.sort(key=lambda p: next((i for i, o in enumerate(order) if o.lower() == p.lower()), len(order)))
        if not perspectives:
            perspectives = ['default']
        # The auxiliary route counts as well when a division uses it (a device saved on it).
        for m in manuals:
            for route in m.get('perspectiveAudio', []) or []:
                if str(route.get('name', '')).lower() != 'auxiliary':
                    continue
                xml = str(route.get('audioDeviceStateXml', ''))
                used = 'deviceType=' in xml or ('audioOutputDeviceName="' in xml and 'audioOutputDeviceName=""' not in xml)
                if used and 'auxiliary' not in perspectives:
                    perspectives.append('auxiliary')
        title = folder
        readme = os.path.join(path, 'README.md')
        if os.path.exists(readme):
            with open(readme, encoding='utf-8') as f:
                for line in f:
                    if line.startswith('# '):
                        title = line[2:].strip()
                        break
        rows.append((title, folder, divisions, count, ', '.join(perspectives)))

    # A Markdown table cannot be sorted by a click, so the header carries the
    # triangles as links: each opens the same table sorted by that column -
    # INDEX.md by organ ascending, INDEX-<column>-<asc|desc>.md the others.
    columns = [('organ', 'Organ', lambda r: r[0].lower()), ('divisions', 'Divisions', lambda r: r[2]),
               ('stops', 'Stops', lambda r: r[3]), ('routes', 'Routes', lambda r: r[4].lower())]
    def file_for(key, descending):
        return 'INDEX.md' if (key == 'organ' and not descending) else 'INDEX-%s-%s.md' % (key, 'desc' if descending else 'asc')
    def render(sort_key, descending):
        ordered = sorted(rows, key=lambda r: r[0].lower())
        ordered.sort(key=dict((k, f) for k, _, f in columns)[sort_key], reverse=descending)
        header = []
        for key, label, _ in columns:
            up = '▲' if (key == sort_key and not descending) else '[▲](%s)' % file_for(key, False)
            down = '▼' if (key == sort_key and descending) else '[▼](%s)' % file_for(key, True)
            header.append('%s %s%s' % (label, up, down))
        lines = [
            '# Published configurations',
            '',
            'One line per organ - the README of each (with the download of its `.json` and where to',
            'put it), the number of divisions, the rows of its Stop List (transmissions and extensions',
            'included, the rows nested in stop folders not) and the routes - the perspectives as the',
            'configuration names them (`default` = a single perspective) and `auxiliary` when a',
            'division uses that route; what each one carries, the README of the organ says. The',
            'triangles in the header sort the table by that column. Written by the `Configurations',
            'check` workflow from the configuration files themselves on every change; do not edit by',
            'hand. How to use and publish one: [README.md](README.md).',
            '',
            '| ' + ' | '.join(header) + ' |',
            '|---|---:|---:|---|',
        ]
        for title, folder, divisions, count, perspectives in ordered:
            lines.append('| [%s](%s/README.md) | %d | %d | %s |' % (title.replace('|', '\\|'), folder, divisions, count, perspectives))
        lines.append('')
        lines.append('%d configuration(s).' % len(ordered))
        lines.append('')
        return '\n'.join(lines)
    changed = 0
    for key, _, _ in columns:
        for descending in (False, True):
            name = file_for(key, descending)
            text = render(key, descending)
            out = os.path.join(ROOT, name)
            old = open(out, encoding='utf-8').read() if os.path.exists(out) else None
            if old != text:
                with open(out, 'w', encoding='utf-8', newline='\n') as f:
                    f.write(text)
                changed += 1
    print('%s: %d configuration(s), %d file(s) written' % ('INDEX.md' if changed == 0 else 'INDEX', len(rows), changed))
    return 1 if changed else 0


if __name__ == '__main__':
    sys.exit(main())
