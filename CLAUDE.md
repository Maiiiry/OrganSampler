# OrganSampler - rules for every session (Windows and Linux)

The user writes Czech; answer in Czech; append a closing report to `OrganSampler/CLAUDE_REPORTS.md`
(the copy on the NTFS disk: `F:\JUCE_PROJEKTY\OrganSamplerC` = `/mnt/pracovni/JUCE_PROJEKTY/OrganSamplerC`).

## Icons - do not touch without asking
- The icon set is one per size and lives in git: `OrganSampler/Source/Icons/icon-{16,24,32,48,64,128,256}.png`
  (16-128 hand-tuned: outlines, the smallest with thick pipes; 256 the picture), `OrganSampler/Source/AppIcon.png`
  (1024, the website picture), `OrganSampler/OrganSampler_icon.ico` (all seven sizes, PNG entries),
  `OrganSampler/Source/Icons/OrganSampler.icns` (sixteen entries: the classic RLE types is32/il32/ih32/it32 with
  their masks for 16/32/48/128 - the Finder of macOS 10.7-10.13 shows PNG data in `icp4`/`icp5` as pixel garbage,
  so those two are never written - and PNG ic07-ic14 for 128-1024 and the @2x sizes; built by
  `python OrganSampler/Tools/make_icns.py OrganSampler/Source/Icons OrganSampler/Source/AppIcon.png
  OrganSampler/Source/Icons/OrganSampler.icns` from icon-16..512.png + AppIcon.png), `site/assets/icon.png` (= AppIcon).
- JUCE (CMake and Projucer alike) would rescale two of them into blurred icons: `CMakeLists.txt` overwrites JUCE's
  generated `icon.ico` / `AppIcon.icns` with ours after `juce_add_gui_app`; the jucer's VS exporter copies the .ico
  by `postExportShellCommandWin`, which `Projucer --resave` does NOT run - after a resave on Windows copy
  `OrganSampler/OrganSampler_icon.ico` over `OrganSampler/Builds/VisualStudio2026/icon.ico` by hand.
- Never replace the icons by a rescaled or "improved" set. If a picture changes, rebuild the 256/1024 entries
  only and keep 16-128 as they are. Linux (menu entry, window icon) uses the same files.

## Versions and releases
- Every delivered build is a new version. Bump it in ALL of: `OrganSampler/OrganSampler.jucer` (`version=`),
  `CMakeLists.txt` (`project(... VERSION ...)`), `site/config.js` (`version:` - app.js writes it into the page),
  `site/index.html` (`data-version` spans), `configurations/rural-organ/README.md` ("Saved with OrganSampler ..."),
  `OrganSampler/CHANGELOG.md`. Before tagging, grep the previous version string over the repo (CHANGELOG excepted).
- Regenerate `OrganSampler/Source/ManualData.h` after any manual edit: `python OrganSampler/Tools/generate_manual_data.py`.
- Commit with `Co-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>`, push, tag `v<version>`; CI builds Windows,
  macOS (Apple silicon, macOS 11+), Intel macOS 10.11+ and Linux and publishes a pre-release; the release job deletes
  every older `v*` release and tag. The Rural Organ demo release `rural-organ-0.8` stays. Only Sonus Paradisi is ever
  named for samples; never "sample set" - say sample folders / folder tree.
- Intel macOS build (10.11+): never add `-D_LIBCPP_DISABLE_AVAILABILITY` (it makes the binary import libc++
  symbols of the build machine's macOS - dyld "Symbol not found" on 10.12-10.14). After each release download
  `OrganSampler-macos-10.11-intel.zip` into a fresh temp folder and run
  `python -I OrganSampler/Tools/check_macho_imports.py <app>/Contents/MacOS/OrganSampler`: it must end with
  "late symbols: 0".
- Windows commit messages: write the message to a file WITHOUT BOM (`[IO.File]::WriteAllText(path, text)`) and
  `git commit -F <file>`; `-F -` with a here-string does not work in PowerShell 5.1 (the text becomes pathspecs,
  the commit fails and a following `git tag` lands on the previous commit), and `Set-Content -Encoding utf8` adds
  a BOM to the subject line. Tag only after `git log -1` shows the new commit.
- Windows: `git` is not on PATH - use Visual Studio's
  (`C:\Program Files\Microsoft Visual Studio\18\Community\...\Team Explorer\Git\cmd\git.exe`); after pulling,
  run `C:\JUCE9\Projucer.exe --resave OrganSampler\OrganSampler.jucer` and copy the .ico (above), commit the regenerated
  `Builds/VisualStudio2026` and `JuceLibraryCode`. Linux clone: `~/OrganSamplerC-linux` (git cannot run on the NTFS
  checkout from Linux); Linux builds come from CI only, the binary goes to `~/Downloads/OrganSampler` by rename.

## Configurations and paths
- A configuration never stores an absolute path except a tree outside the sample library root (another disk);
  everything under the root is relative to it (`sampleLibraryRootName` / `sampleLibraryRootRelative`, pool
  `sampleTrees`, `folderRelativePath`, `extraFolderRelativePaths`, `reverbIrRelative`/`reverbIrLibraryRelative`).
  A portable configuration carries no machine settings: devices become `deviceType="default"` with the route's
  sample rate and buffer size; "none" stays none.
- Never alter audio silently; never drop user-configured content (routes, stops, folders) without asking first
  (AskUserQuestion). A row of the Stop List is never removed by the program itself - only by the user
  ("Remove empty stops", arrange mode).
- Stop folders (a container stop with indented children) are table rows: the container has no folders, the
  children `child: true`. Keep them through every save/load path.
- Transmissions grouped under a stop folder are NOT table rows: they live in the source stop's voicing entry
  (`voicingSettings.stops[].transmissions[]` with `groupMaster`). Search there before claiming they are gone.
- The transmission list of a stop belongs to the Stop List / voicing state only. A combination, crescendo step
  or registration sets on/off states and must never replace, add to or remove from the list (0.8.92).
- The voicing state keeps entries of stops the engine could not build (missing folders) and writes them back;
  never let a save drop voicing of a stop that merely failed to load.

- A configuration README is WRITTEN BY THE PROGRAM: saving a portable configuration puts `<name>.README.md`
  beside the `.json` (`MainComponent::configurationReadme`; dev: `ORGAN_SAVE_PORTABLE=<file>` with
  `ORGAN_LOAD_CONFIGURATION=<non-portable cfg>` loads and saves without the dialogs). Only the three
  "(fill in: ...)" places - Organ, the producer/product/link in Samples, Author - are completed by hand, plus
  the required version if a fix matters (Stralsund: 0.8.92). Never retype the rest. Its form:
- A configuration README has exactly this, in this order, and nothing more (people do not read):
  title; one opening paragraph (what it is, the perspectives - they make the recording unique -, divisions, the
  number of Stop List rows) + "No samples are published here"; "The configuration" bullets Organ, Samples
  (producer, product, link, packages, WAV, perspectives, required OrganSampler version), Expects (trees), Audio
  layout (routes, unused ones marked), Saved with, Author + date; "Where to put the file" (the packages are now
  known: one sentence + the tree diagram, never a made-up parent folder like `haupwerk`); last the "Stop List"
  table of the row names by division. Never mention what OrganSampler does natively (couplers, voicing,
  crescendo, combinations). No "What you need", no "Using it", no prose about the divisions.
- A `.json` in the repository opens as text in the browser, so every configuration is also attached to the
  permanent release `configurations` (workflow `configurations.yml`, job `publish`, on every push to main;
  asset names get hyphens for spaces). Name configuration files WITHOUT spaces, and put the line
  `**Download:** https://github.com/zurek-jiri/OrganSampler/releases/download/configurations/<file>.json`
  right after the opening paragraph of its README.
- Stop List counting: every row counts - stops, stop folders, transmissions and extensions (transmissions
  are rows of their receiving division, `transmissions[]` with `manual`) - except the rows nested in a stop
  folder (`groupMaster` set). The table is NEVER typed by hand: it comes from the program, in grid order (row by
  row, left to right - the organ builder's order the user arranged) with the exact button captions (footages,
  "(Ext.)"/"(Trsm.)", a stop folder's "[III]"):
  `ORGAN_DUMP_STOPLIST=<out.md> ORGAN_LOAD_CONFIGURATION=<cfg.json> OrganSampler.exe` writes it once the load is
  over (use a non-portable configuration of this machine, a portable one would ask for the root); quit the program
  afterwards. No legend above the table. Every README looks the same. Stralsund: 53 + 18 = 71.

## Testing
- Windows delivery: after every release build copy the exe to `C:\OrganSampler\OrganSampler_<version>.exe`
  (the local MSBuild Release build or the CI exe) and point the desktop shortcut
  `OrganSampler_<version>.exe – zástupce.lnk` at it (rename the previous version's shortcut; WScript.Shell
  CreateShortcut, TargetPath + IconLocation). The user starts the program from that shortcut.
- Never build or launch the program while the user runs it. Linux test: `~/tools/linux_test.sh <exe> <seconds>`
  with `CFG=`, `ORGAN_OPEN_STOP_LIST=`, `ORGAN_JACK_BUFFER=`, `ORGAN_SAVE_PORTABLE=` hooks; the activity log is
  `~/.config/OrganSampler/CrashLogs/activity.log` (Windows: `%APPDATA%\OrganSampler\CrashLogs\activity.log`).
