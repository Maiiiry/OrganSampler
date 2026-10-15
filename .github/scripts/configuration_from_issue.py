#!/usr/bin/env python3
"""Takes the files attached to an "Organ configuration" issue and puts them into
configurations/ as the upload pair "<Name>.zip" + "<Name>.README.md".

The issue body (environment variable ISSUE_BODY) holds the attachments as
GitHub links (https://github.com/user-attachments/files/<id>/<name> or the older
https://github.com/<owner>/<repo>/files/<id>/<name>). Exactly one .zip (the
configuration .json inside) and one .md (the README) are expected; GitHub puts
dots for spaces in attachment names, so the README is renamed after the zip.
Prints what it did; exits 1 with a message for the issue when something is
missing. Run from the repository root.
"""
import os
import re
import sys
import urllib.request
import zipfile


def main():
    body = os.environ.get('ISSUE_BODY', '')
    # GitHub attachments, or any https link straight to a .zip or .md file.
    urls = re.findall(r'https://github\.com/(?:user-attachments/files|[^/\s]+/[^/\s]+/files)/\d+/[^\s)\]>"\']+', body)
    urls += re.findall(r'https://[^\s)\]>"\']+\.(?:zip|md)\b', body)
    urls = list(dict.fromkeys(urls))   # unique, in order
    zips, readmes = [], []
    os.makedirs('configurations', exist_ok=True)
    for url in urls:
        name = url.rsplit('/', 1)[-1]
        lower = name.lower()
        if not (lower.endswith('.zip') or lower.endswith('.md')):
            continue
        target = os.path.join('issue-files', name)
        os.makedirs('issue-files', exist_ok=True)
        request = urllib.request.Request(url, headers={'User-Agent': 'OrganSampler-configurations'})
        with urllib.request.urlopen(request, timeout=60) as response, open(target, 'wb') as out:
            out.write(response.read())
        (zips if lower.endswith('.zip') else readmes).append(target)
    if len(zips) != 1 or len(readmes) != 1:
        print('The issue must carry exactly one zip (the configuration .json inside) and one README .md - '
              'the two files OrganSampler wrote beside the portable configuration. Found: %d zip, %d .md.'
              % (len(zips), len(readmes)))
        return 1
    try:
        inner = [n for n in zipfile.ZipFile(zips[0]).namelist() if not n.endswith('/')]
    except zipfile.BadZipFile:
        print('The attached zip cannot be read.')
        return 1
    if len(inner) != 1 or not inner[0].lower().endswith('.json'):
        print('The zip must hold exactly one configuration .json and nothing else (found: %s).' % ', '.join(inner))
        return 1
    base = os.path.basename(zips[0])[:-4]
    zip_target = os.path.join('configurations', base + '.zip')
    readme_target = os.path.join('configurations', base + '.README.md')
    os.replace(zips[0], zip_target)
    os.replace(readmes[0], readme_target)
    with open(readme_target, encoding='utf-8', errors='replace') as f:
        readme = f.read()
    left = re.findall(r'\(fill in:[^)]*\)', readme)
    if left:
        print('The README still has %d "(fill in: ...)" place(s) to complete: %s' % (len(left), '; '.join(left)))
        return 1
    print('configurations/%s.zip and configurations/%s.README.md' % (base, base))
    with open(os.environ.get('GITHUB_OUTPUT', os.devnull), 'a') as out:
        out.write('base=%s\n' % base)
    return 0


if __name__ == '__main__':
    sys.exit(main())
