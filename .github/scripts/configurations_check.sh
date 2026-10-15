#!/usr/bin/env bash
# Checks the configurations folder: per organ one folder with the zipped
# configuration (exactly one .json inside) and its README.md, or - the form an
# upload arrives in - "<Name>.zip" with "<Name>.README.md" beside it flat in
# configurations/. Anything else fails: audio files (samples, impulse
# responses), files named like pipe samples (036-C.wav), other file types,
# anything above 40 MB. Run from the repository root; exit 1 on any error.
set -e
fail=0
zipcheck() {
  python3 - "$1" <<'EOF'
import sys, zipfile
path = sys.argv[1]
try:
    names = zipfile.ZipFile(path).namelist()
except zipfile.BadZipFile:
    print("::error file=%s::not a zip file" % path); sys.exit(1)
files = [n for n in names if not n.endswith('/')]
if len(files) != 1 or not files[0].lower().endswith('.json'):
    print("::error file=%s::the zip must hold exactly one configuration .json and nothing else (found: %s)" % (path, ', '.join(files))); sys.exit(1)
EOF
}
while IFS= read -r -d '' f; do
  name="$(basename "$f")"
  lower="$(echo "$name" | tr '[:upper:]' '[:lower:]')"
  size=$(stat -c %s "$f")
  case "$f" in
    configurations/README.md|configurations/INDEX*.md) continue ;;
    configurations/*/*)
      # An organ's folder: README.md and the zip.
      case "$lower" in
        readme.md) ;;
        *.zip) zipcheck "$f" || fail=1 ;;
        *.json) echo "::error file=$f::a bare .json opens as text on GitHub - send the zip OrganSampler writes beside a portable configuration"; fail=1 ;;
        *) echo "::error file=$f::only the zipped configuration and its README.md belong in an organ's folder ($name) - no samples, no impulse responses (they are part of the sample folders), nothing else"; fail=1 ;;
      esac ;;
    configurations/*)
      # An upload: "<Name>.zip" with "<Name>.README.md" beside it.
      case "$lower" in
        *.readme.md)
          base="${f%.README.md}"
          [ -f "$base.zip" ] || { echo "::error file=$f::no $(basename "$base").zip beside this README - both files OrganSampler wrote are needed"; fail=1; } ;;
        *.zip)
          zipcheck "$f" || fail=1
          base="${f%.zip}"
          [ -f "$base.README.md" ] || { echo "::error file=$f::no $(basename "$base").README.md beside this zip - both files OrganSampler wrote are needed"; fail=1; } ;;
        *.json) echo "::error file=$f::a bare .json opens as text on GitHub - send the zip OrganSampler writes beside a portable configuration"; fail=1 ;;
        *) echo "::error file=$f::only <Name>.zip and <Name>.README.md belong here ($name)"; fail=1 ;;
      esac ;;
  esac
  if echo "$name" | grep -Eq '^[0-9]{3}[-_.]'; then
    echo "::error file=$f::named like a pipe sample ($name) - samples are never published here"; fail=1
  fi
  if [ "$size" -gt 41943040 ]; then
    echo "::error file=$f::larger than 40 MB"; fail=1
  fi
done < <(find configurations -type f -print0)
if [ "$fail" -ne 0 ]; then
  echo "The configurations folder takes, per organ, the zipped configuration (one .json inside) and its README.md - nothing else. Samples and impulse responses are bought or downloaded elsewhere (Sonus Paradisi)."
  exit 1
fi
echo "configurations/ is clean"
