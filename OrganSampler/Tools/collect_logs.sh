#!/bin/sh
# Collects everything a bug report needs into one zip in your home folder:
# the CrashLogs folder (activity.log, crash reports), settings.json,
# selftest.txt and a system.txt with the computer's description. No sample
# data. Attach the zip to the "Logs" field of the bug report at
# https://github.com/zurek-jiri/OrganSampler/issues/new/choose
#
# Usage: sh collect_logs.sh
set -e
case "$(uname -s)" in
  Darwin) data="$HOME/Library/Application Support/OrganSampler" ;;
  *)      data="${XDG_CONFIG_HOME:-$HOME/.config}/OrganSampler" ;;
esac
if [ ! -d "$data" ]; then echo "No OrganSampler data folder at $data - has the program been started on this computer?"; exit 1; fi
stamp="$(date +%Y-%m-%d_%H-%M)"
work="${TMPDIR:-/tmp}/OrganSampler-logs-$stamp"
out="$HOME/OrganSampler-logs-$stamp.zip"
rm -rf "$work"; mkdir -p "$work"
[ -d "$data/CrashLogs" ] && cp -R "$data/CrashLogs" "$work/"
for f in settings.json selftest.txt; do [ -f "$data/$f" ] && cp "$data/$f" "$work/"; done
{
  echo "collected: $(date)"
  echo "os: $(uname -srm)"
  if [ "$(uname -s)" = "Darwin" ]; then
    sw_vers 2>/dev/null
    sysctl -n machdep.cpu.brand_string 2>/dev/null | sed 's/^/cpu: /'
    echo "ram: $(( $(sysctl -n hw.memsize) / 1073741824 )) GB"
    system_profiler SPAudioDataType 2>/dev/null | sed 's/^/  /'
  else
    [ -f /etc/os-release ] && grep PRETTY_NAME /etc/os-release
    grep -m1 "model name" /proc/cpuinfo 2>/dev/null | sed 's/^/cpu: /'
    grep MemTotal /proc/meminfo 2>/dev/null | sed 's/^/ram: /'
    echo "alsa devices:"; aplay -l 2>/dev/null | sed 's/^/  /'
  fi
} > "$work/system.txt"
rm -f "$out"
(cd "$work" && zip -qr "$out" .)
rm -rf "$work"
echo ""
echo "Log bundle written: $out"
echo "Attach it to the bug report: https://github.com/zurek-jiri/OrganSampler/issues/new/choose"
