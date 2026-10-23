# OrganSampler - rules for every session (Windows and Linux)

The user writes Czech; answer in Czech; append a closing report to `OrganSampler/CLAUDE_REPORTS.md`
(the copy on the NTFS disk: `F:\JUCE_PROJEKTY\OrganSamplerC` = `/mnt/pracovni/JUCE_PROJEKTY/OrganSamplerC`).

## Never change what you need not change
- The user's rule (23 Oct 2026, "Nikdy neměň něco, co nemusíš!"): a fix touches the lines of the fault and nothing
  else - no renaming, no "while I am here" rewrites, no new defaults, no changed texts or separators. When a change
  outside the fault seems needed, say so first. The automatic stop name "A / B" (two perspectives with differently
  named folders) dates from the first commit, not from a session - before blaming a session, `git blame` the line.

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
- Commit (plain commit messages: no Co-Authored-By trailer, no assistant name anywhere in the repository - the
  user is the only contributor), push, tag `v<version>`; CI builds Windows,
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
  run `C:\JUCE9\Projucer.exe --resave "F:\JUCE_PROJEKTY\OrganSamplerC\OrganSampler\OrganSampler.jucer"` with the
  ABSOLUTE path and copy the .ico (above) (with a relative path the resave prints nothing and writes nothing - four
  tries in 0.8.145; check `versionString` in `JuceLibraryCode/JuceHeader.h`, it must print "Finished saving"), commit the regenerated
  `Builds/VisualStudio2026` and `JuceLibraryCode`. Linux clone: `~/OrganSamplerC-linux` (git cannot run on the NTFS
  checkout from Linux); Linux builds come from CI only: `OrganSampler-linux-x86_64.tar.gz`, unpacked to
  `~/Downloads/OrganSampler`. Releases carry the native archive of each system, never a bare executable (0.8.110):
  `OrganSampler-windows.zip` (OrganSampler.exe inside), `OrganSampler-macos.zip` / `OrganSampler-macos-10.11-intel.zip`
  (the app), `OrganSampler-linux-x86_64.tar.gz` (keeps the executable bit); names without a version. On Linux `gh` is NOT on
  PATH: use `~/tools/gh/bin/gh` (export PATH="$HOME/tools/gh/bin:$PATH"); never run `~/Downloads/OrganSampler` with
  made-up flags such as `--version` - it starts the program.

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
- Publishing a configuration needs no Git. GitHub's web upload is for collaborators only ("Uploads are
  disabled"), so the way is the issue form `organ_configuration.yml` (label `configuration`): the workflow
  `configuration-issue.yml` takes the attached `<Name>.zip` + `<Name>.README.md` from the issue body
  (`.github/scripts/configuration_from_issue.py`), runs `.github/scripts/configurations_check.sh`, pushes
  the flat pair into `configurations/` on branch `configuration/issue-<n>` and opens the pull request
  (closes the issue on merge), answering in the issue on success or failure. The check accepts that flat
  pair; after the merge the `index` job moves it into `configurations/<slug>/` (README.md + zip) and
  regenerates INDEX. `.github/PULL_REQUEST_TEMPLATE.md` is the checklist for hand-made pull requests.
  Repository setting needed (set 15 Oct 2026 via the API, `actions/permissions/workflow`): "Allow GitHub
  Actions to create and approve pull requests" ON - without it `gh pr create` fails with "GitHub Actions is
  not permitted to create or approve pull requests". The label `configuration` must exist (it does).
- `configurations/<organ>/` holds exactly two files: the zipped `.json` and `README.md`. No IR files, no pictures,
  no bare `.json`, nothing else (impulse responses belong to the sample folders); the CI check refuses anything else.
- Transmissions grouped under a stop folder are NOT table rows: they live in the source stop's voicing entry
  (`voicingSettings.stops[].transmissions[]` with `groupMaster`). Search there before claiming they are gone.
- The transmission list of a stop belongs to the Stop List / voicing state only. A combination, crescendo step
  or registration sets on/off states and must never replace, add to or remove from the list (0.8.92).
- The voicing state keeps entries of stops the engine could not build (missing folders) and writes them back;
  never let a save drop voicing of a stop that merely failed to load.
- Hauptwerk ODF import (0.8.116, `Source/HauptwerkOdf.{h,cpp}`, `MainComponent::importHauptwerkOdf`): Load Configuration
  takes `*.Organ_Hauptwerk_xml` too. Only divisions (keyboard order, pedal first, compass from KeyboardKey), stop folders
  (the common ancestor of a stop's rank folders; their parents are the division trees), perspectives (rank names
  "Front A (direct): ..." -> patterns "Direct;01-dirMk2H-") and per-pipe levels (`Pipe_SoundEngine01_Layer`
  AmpLvl_LevelAdjustDecibels, first layer only, averaged over perspectives -> stop volume + per-key dB) are taken.
  Never couplers/combinations/tremulants/wind/displays. Tremmed ranks (AlternateRankID, "(tremmed)", `trem` folders) and
  noise ranks are skipped; `.hbw`/`.hbx` (encrypted) refuse the import. Hauptwerk layout:
  `<root>/OrganDefinitions/<odf>` + `<root>/OrganInstallationPackages/<6-digit id>/...`; the configuration is written
  into the main package folder (`<organ>.json`, library root = OrganInstallationPackages), no zip/README
  (`savePortableConfiguration(..., publishFiles=false)`). Compacted ODFs use letter tags (Stop a/b/c, StopRank a/d/p,
  Rank a/b, Pipe a/b/d, Layer a/b/h, AttackSample b/c, Sample a/b/c, Keyboard a/b/x, KeyboardKey a/c, Division a/b).
  Merge (0.8.117, `MainComponentMerge.cpp`, right-click Load Configuration -> `mergeConfigurationDialog`; dev hook
  `ORGAN_MERGE_CONFIGURATION=<file>` after the load): base = settings.json, added = the file (or the ODF plan var);
  divisions by number; shared folders -> stop not added; same name -> "(2)"; new perspectives bring their routes;
  empty division takes the added keys/MIDI; couplers/combinations/crescendo/temperament/global stay; free RAM
  (`availablePhysicalMemoryBytes`) vs 0.7 x WAV bytes of the new trees, refused when short; result as a message.
  Dev check without the GUI: `OrganSampler.exe --hauptwerk "<odf>"` prints the parse and the plan (also to
  `%APPDATA%\OrganSampler\hauptwerk.txt`); the in-app path is `ORGAN_LOAD_CONFIGURATION=<odf>` + `ORGAN_DUMP_STOPLIST=`.
  Stralsund (3135+3136) must give trees `00313x/pipe/{PE,HW,OW,UW}`, 13+14+12+12 stops, patterns Direct/Diffuse/Rear.
  0.8.118: the way is stop > StopRank rows (h/i/j = first key, count, increment) > rank > folder. A rank belongs to the
  stop that plays it with increment 0; a stop whose rows borrow one other stop's ranks with one increment is an
  EXTENSION/transmission (`PlanStop.extensionOfStopId`, `shift`; `hauptwerkExtensions` -> `engine.addTransmission`
  after the load, alias = HW name) - before, the common folder of the two stops was the whole division and the tree
  the whole package (Stellwagen "III. G. Plockfloett 4": pool full of every folder, stops twice). A stop folder never
  holds another stop's folder (fallback: rank folders). Noise ranks by whole words ("I. Zimbell" is no bell).
  Rows: `name` = the folder's automatic name (`scanTreeAssignments`), `customName=false`, `alias` = HW stop name.
  Ignored patterns on import: `trem`, `*_tr` (the `II_TR` sibling folders). The import writes several prefixes into
  one perspective line when one organ spells a perspective two ways ("Direct;01-dirc-;03-dif2S-") - the engine has
  read `Label;prefix;prefix2` lines since the first commit (0.8.50, `stripPerspectivePrefix`); never call that new.
  Stellwagen (2723+2724, non-encrypted ODF in OrganDefinitions) must give 14+14+12+12 stops, 1 extension, trees
  `00272x/pipes/{PE,I,II,III}` only.
  0.8.125: a division of OURS is a keyboard. `Keyboard::divisionIds` = every ODF division the key switches reach
  through `SwitchLinkage` (KeyboardKey.SwitchID -> source/dest chains, condition switch = a ventil: still the
  keyboard's division) to `DivisionInput.SwitchID`; a division reached from several keyboards goes to the one with
  the most unconditional keys; the ODF hint (`Hint_PrimaryAssociatedDivisionID`) stays primary when among them;
  no linkage at all -> the hint. The plan takes the stops of ALL those divisions (primary first). Caen Surround
  (ODF by the user, packages 1318-1320, 1691-1695, 45 GB) must give 11+15+13+16 = 55 stops from 8 ODF divisions
  (Pedal+PedalAnches ...), never 4 divisions / 32 stops. Several ranks in ONE perspective ("(L component)" /
  "(R component)" ranks, folders `quintaton16` + `quintaton16l`) -> `PlanStop.children` (grouped by the rank name
  without its perspective head, else by folder name; sorted by name): the import writes a container row (HW name,
  customName, no folders) + child rows (`child: true`, automatic names, alias = rank name) - the 16 Recit stops of
  Caen are stop folders "[II]". Before, those stops took `pipe/rec` (the whole division) and dropped the second
  rank. The configuration goes to `<root>/OrganConfigurations/<organ>.json` (folder created by the import; library
  root OrganInstallationPackages via `sampleLibraryRootRelative` "../OrganInstallationPackages"), no longer into
  the numbered package. Dev: `--hauptwerk` prints "keyboard N: ... -> division X (all: X Y)" and "STOP FOLDER with
  N children".
  Voicing (0.8.126, `applyHauptwerkVoicing`): Hauptwerk has NO volume per perspective, so the pipe levels are split
  in three: median of all pipe levels of (division, perspective) -> route volume (`perspectiveAudio[key].volumeDb` +
  `setPerspectiveVolumeDb`, clamped -31..6), median of the residual (averaged over the stop's perspectives) -> stop dB,
  the rest -> per-key dB. Never put a perspective's offset into the stops or pipes again.
  0.8.132 perspectives: `perspectiveOfRankName` returns the perspective WORD itself capitalised (Direct, Diffuse,
  Distant, Rear, Dry, Close ... ; "Front A"/"Front B" without a parenthesis get the letter) - never a synonym class
  (distant -> Rear put "Front C (distant)" and "Rear" of Ashton-under-Lyne into one label and the 0.8.125 rule made a
  stop folder of a plain four-perspective set). Safety net at the end of `parse`: ranks of one stop with the same
  label but different folder prefixes (longest common ending of the folder names = the stop part, the rest = the
  prefix; core = letters/digits "dir22", "dis2h", "reaSen") are relabelled "<label> <core>" - four prefixes are four
  perspectives, whatever the names say; the same prefix twice (L/R split) stays a stop folder, and so do names that
  differ at their END (common ending < 3: Caen "quintaton16" / "quintaton16l" are pieces, not perspectives). The
  user's rule: the ODF's StopRanks (rank names) decide, prefixes support them where they exist - Caen has none (its
  perspectives are packages). Ashton Surround must give Direct / Diffuse / Distant / Rear, patterns "Direct;02-dir22-"
  etc., no stop folders; Caen Recit keeps its 16 stop folders.
  0.8.128: stop compass from StopRank rows (h = first key, default the division's first; i = count, default to the
  division's last; union over the rows with increment 0; `HauptwerkOdf::compassForFolders`) -> keys outside switched
  off with `engine.setPipeEnabled(stop, pipe, false)` (voicing `disabledKeys`, no fills) in `applyHauptwerkVoicing`.
  Tremulant: `Tremulant` (a id, b name, d/e = speed min/max Hz, the middle taken) > `TremulantWaveform` (a id, c
  TremulantID) > `TremulantWaveformPipe` (a PipeID, b waveform) -> pipe -> rank -> owner stop -> division
  (`HauptwerkOdf::tremulants[].divisionIds`); `PlanDivision.tremulantRateHz` -> manualKeyboards "tremulantRateHz"
  (Caen: Recit 5.61 Hz; Stellwagen: Recit + Positif 4.79 Hz). Master level: `MainComponent::kDefaultMasterDb` = -18
  (slider, settings default, reset, MIDI-learn default); `registrationMasterDb` absent -> the default, never 0 dB.
  A stop folder's children: `Stop::groupChildOrdinal` (set in `applyStopGroups`) adds `childOrdinal * rankDrift`
  to the tremulant phase. Polyphony display: fifth column "RAM GB" (`processMemoryBytes`, once a second, WHOLE GB
  rounded up, no unit in the value - "1.2 GB" did not fit the cell on a 1280 px display; orange when free < 10 % of
  installed). `processMemoryBytes` (0.8.129): macOS `task_vm_info.phys_footprint` (resident + compressed + swapped, what
  Activity Monitor shows; `resident_size` alone said 995 MB for a multi-GB organ on the 8 GB Mac), Linux VmRSS + VmSwap,
  Windows max(WorkingSet, PrivateUsage). Never report the resident size alone as "what the program holds".
  0.8.130: `warnIfMemoryShortForTrees` (MainComponentMerge.cpp, called in `applyPortableConfiguration` before
  `beginStartupFolderLoading`): WAV bytes of the trees (nested trees once) x 0.7 (16-bit) / 1.0 (float) vs the free
  memory measured before the load; counted on a juce::Thread::launch, verdict by callAsync: log "Memory for ..." and
  an AlertWindow when needed > free - 512 MB. The load is never refused (#17: a 7 GB organ on the 8 GB Mac thrashed,
  the window thread stalled 10-16 s at a time, "the application cannot be closed").
  0.8.131 (the user's rule): the loading STOPS when the memory reserve is reached - `freePhysicalMemoryBytes()` /
  `memoryReserveBytes()` (768 MB or installed/16) in SamplerEngine.cpp, checked in `loadDivisionTree` before every
  non-container stop and every 32 files inside `loadStopFolder`'s parallelFor; `loadStoppedForMemory` + `loadOutOfMemory`
  set, the remaining stops of the load go to `loadMemorySkipped` (cleared by `takeLastLoadError`), the instrument is
  built from the stops loaded so far, `lastLoadError` names the stops left out (first 12). Never let the system page
  samples: a stalled window thread looks like a hang. 0.8.133: the figures live in `Source/MemoryInfo.h`
  (`MemoryInfo::freeBytes/heldBytes/swappedBytes/installedBytes/systemShareBytes/reserveBytes/shortNow`), used by the
  engine (`memoryShortNow`), `MainComponent::availablePhysicalMemoryBytes/processMemoryBytes`, the RAM column and the
  report. `shortNow`: free < reserve (768 MB / installed/16) OR held > installed - systemShare (1.5 GB / installed/5)
  OR swapped > 256 MB (Linux VmSwap of the process, macOS compressor pages). The free figure alone let a 7 GB organ
  load whole on the 8 GB Mac (#17) and on the 7.7 GB Linux laptop (#19: held 6.9 GB, "2 GB free", swapping) - the
  footprint rule is the one that bites. 0.8.134: `MemoryInfo::pressureLevel()` = macOS `kern.memorystatus_vm_pressure_level`
  (1 normal, 2 warning, 4 critical; the kernel's own figure, counting what freeing the inactive pages would give - a
  user process cannot purge them: `purge` needs root and drops only the disk cache, madvise touches our pages only);
  level >= 2 is the fourth sign in `shortNow`. The pre-load estimate reads bits/rate from the first 48 WAV headers (16/bits x
  48000/rate when resampling, x 1.15 for copies), never a flat two thirds. Hauptwerk rows: name = the STOP FOLDER's
  name as on disk (the user's rule: "ve jmenech ma byt, jak se jmenuji adresare"), alias = the HW stop name; a child
  of a stop folder = its rank folder's name. The Hauptwerk import sets `suppressPortableNotice` so the
  "Configuration from another computer" notice is not shown (two message boxes over each other, the user refused).
  0.8.129 crescendo: `Combination` (a id, b name) named "cresc..." = one stage each, the stage = the number at the end
  of the name (`getTrailingIntValue`); `CombinationElement` (b CombinationID, c ControlledSwitchID, e Y = on); the
  switches reach the stops' `Stop.switchId` (d, ControllingSwitchID) through `SwitchLinkage` both ways within 8 hops
  (Doesburg 1 hop; Schwerin 4: CrescendoCombSwitch > CrescendoIfOnSwitch > ... > "C.<stop>" > the stop - a limit of
  4 iterations found nothing there, every stage came out empty); couplers unresolved -> left out.
  `HauptwerkOdf::crescendo` / `Plan.crescendo` -> the import writes the configuration key "crescendo"
  (`crescendoFromVar` format: stepCount, cancelsHandStops, steps[{step, stops[{name=row name, perspective "",
  enabled, primaryManual}], couplers[]}]); more than 60 stages spread over 60 steps. Doesburg Surround: 64 stages,
  stage 2 = Aeoline 8 + Echobass 16 + Echo-Gamba 8.
  0.8.135 ownership (`HauptwerkOdf::plan`, the claims block): a rank's owner among the stops playing it with increment 0
  = name match (rank name after the perspective head == stop name) > count 0 (the whole division) > more keys > file
  order; a rank nobody plays straight -> its first user. A stop with ANY rank of its own is a stop, loaded from them
  (its borrowed rows are covered by the filled pipes, noted); a stop with none -> extension of the owner it borrows
  the MOST keys from (count 0 = division size). Never the first-in-file rule again (Bellevue: the pedal came first in
  the file and "owned" GO. Bombarde 16; Billerbeck Tuba 16/4 own their ranks, Tuba 8 and "5. Tuba Ped. 2" and
  "P. Flute 16" are extensions). Floating ODF division = no keyboard's `unconditionalDivisionIds` holds it (Bellevue
  "Chamade", reached through the Bom-Ped/Gt/Pos/Rec 8 couplers only; Billerbeck "Tuba" has a keyboard, not floating):
  its stops go stop by stop where the name's prefix says (`divisionIndexByPrefix`: P./Ped. -> pedal, digits and roman
  -> manual N, go/gt/hw/sw/rec/pos/rp/ch/so/ech synonyms against the keyboard and ODF division names), noted.
  One-shot ranks: zero `Pipe_SoundEngine01_ReleaseSample` rows (b = LayerID) -> `Rank::oneShot`; a stop with only such
  ranks -> `Plan.oneShotFolders` -> the configuration's "oneShotPatterns" (defaults + those folder names). Tremmed
  siblings: `isTremmedVariantFolder` (SamplerEngine.cpp) - a folder named like a sibling or its parent + "tr"/"trem"
  (separators trimmed) is skipped by every tree scan (listSampleFolders, scanTreeAssignments: the pool showed every
  Bellevue/Billerbeck stop twice), never by a row that names it (Billerbeck "P. Grosse flute 32" lives in
  Ped_flute32/Rea_Ped_flute32TREM only - loaded as Hauptwerk has it, noted). Defaults: ignored {"trem","*_tr","output"}
  ("outputsum"/"outputharm" under a rank), one-shot {"Chiff","Chimes","Tubular","Celesta","Harp","Glockenspiel"} -
  "Celesta" the instrument, never "Celeste". `StopNames::capitaliseAfterDivisionPrefix` (Source/StopNames.h) on the
  scan's row names, the folder aliases and the HW names/aliases ("P. Trombone 16", "3 Geigenpr 8") - never a custom
  name. Memory 0.8.135: `gSwapBaseline` (SamplerEngine.cpp) = swapped/compressed figure at load start, `shortNow(reason,
  baseline)` fires on a growth > 256 MB (the mac compressor held 0.43 GB of other programs at idle, #21); reserve
  512 MB or installed/16; system share 1 GB on <= 4.5 GB installed; a stop cut in the middle of its files
  (`loadMemoryCutStops`, set in `loadSamplesIntoStop`) is removed whole from the division at the end of the tree and
  named first in `loadMemorySkipped` - nothing half-loaded on the grid. The "Configuration from another computer"
  message box is gone (a log line); `applyPortableConfiguration` closes an open Stop List window (it was shown again
  stale after the load). Dev tools (Python, run with -I): `Tools/odf_stops.py <odf> <name filters>` (stop > StopRank
  rows > ranks > folders, users), `Tools/odf_links.py <odf> <division filter>` (keyboard -> division switch paths),
  `Tools/pool_folders.py <cfg.json> <OrganInstallationPackages>` (the scan folders no row uses = what the pool shows;
  Bellevue and Billerbeck must list nothing but Ped_ContreBomb32's output folders).
  0.8.136: the pool matches folders by their PLACE ON DISK (`placeOf` in `readyStopsNotIn`) and a tree's scan skips
  folders of a nested tree (Testing organ: tree "Samply" holds the trees "Samply/testy/P|I|II|III" - every stop was in
  the pool twice, #23). Memory stop: `loadMemorySkipped` once per stop (`addIfNotAlreadyThere`; #22 said "35 stops" for
  15 rows x perspectives), `loadMemorySkippedKeys` drops a stop with a perspective loaded and another skipped;
  `lastLoadMemoryLimited` (engine, `lastLoadWasMemoryLimited()`) makes the polyphony report say "the loading stopped
  on purpose" - never "the system pages" after our own stop (#25, #26). macOS compressor rule only with free < 2 x
  reserve (#22/#25: 0.3 GB growth stopped a 2 GB load with 3.4 GB free). Report CPU = one figure (render cores busy on
  average; #24), a second with callbackShare 0 while voices sound is never clean (#24: 93 700 after the stream died).
  `EdgeViewport` (UIControls.h) for viewports inside ScrollableContent dialogs: the wheel passes to the window at the
  edge (Stop List buttons unreachable by wheel). `showLongMessage(title, body)` = `LongMessageComponent` (read-only
  editor + OK, `placeDialog` key "longMessage") for the Hauptwerk summary; the polyphony report uses the same component.
  Save prompt = Save / Don't Save / Cancel ("Save As..." without a file). Linux window icon = icon48 (X11 takes one
  image); the desktop-entry installer also writes ~/.local/share/pixmaps/organsampler.png (48) for antiX-like WMs.
  0.8.137 device-thread profile (while `gVoiceProfiling`): `renderRoute` laps `statRouteTake/Loop/Compact/Separation/
  Reverb/Clip/Record/Stats` (profiling clock, ten reads per route callback), the pool fills `tlPoolOwnTicks` (the
  poster's own chunks) and `tlPoolWaitTicks` (the rest of pool->render: waiting for lanes, merging their scratch),
  `waitIrTask` fills `tlReverbWaitTicks`; `RenderPoolStats::routeCallbacks` + `route*Ns` per route callback
  (`routeMixNs` = loop - own - wait), `routeTotalNs()`. The report paragraph "The device thread, per route callback
  (a period is P us): ..." and the per-second log line ("device thread per route callback: ... N % of the time") are the
  measurement the batch rendering of all routes of a device in one pool job (the next step, not coded yet) must be
  judged against: routes of one device are ALREADY one stream (one AudioDeviceManager host, route callbacks called in
  sequence - `route->managerInUse->addAudioCallback`); the IR convolution already runs as a pool task posted by the last
  route of the group and joined by the first route of the next block. Never claim "one stream per device" is missing.
  0.8.138 couplers: `KeyAction` rows (a source keyboard, c dest keyboard / d dest division, f condition switch = the
  drawstop, n semitones, h = 3 melody; e name "4 To 3,121") -> `HauptwerkOdf::couplers` (+ `switchName` from the
  `Switch` table, b = name); `plan()` maps them to OUR ids "target-source-type" (type 8/16/4/mel; source = our index of
  keyboard a, target = our index of keyboard c or of the division d) within the catalog (target > source any type,
  self 16/4, 1 from 2) -> `Plan.couplers` (`PlanCoupler{id,label,switchId}`), the rest `couplersUnmapped` + notes
  (Bellevue "Bom-Ped 8" to the floating Chamade division, Billerbeck 24'). The import writes the top-level "couplers"
  list ({id, visible}: the HW ones visible, the catalog's default unison ones hidden) and the crescendo steps'
  "couplers" [{id, enabled}] (`CrescendoStep::couplerSwitchIds`: elements reaching a coupler drawstop through the
  linkage BFS, like the stops). Billerbeck must give iv-i, iv-ped, super iii-ped ... as 5-1-8, 5-0-8, 4-0-4 ...;
  Caen 4 To 1 / 3 To 1 / 2 To 1 (keyboards) + the -12 division couplers. `--hauptwerk` prints "coupler <id> = ...".
  Report link 0.8.138: GitHub returned 500 after the sign-in for a long link (the whole system block with 20+ routes
  folded into the sign-in's return URL): `makeUrl` keeps the link under 6000 chars (what/steps clipped to 1500, the
  routes replaced by their count, then the block dropped; everything is in system.txt of logs.zip), logged "Problem
  report link: N characters". OS string with version (Windows DisplayVersion + build from the registry, Linux
  PRETTY_NAME + kernel). The report folder opens with `startAsProcess()` (not `revealToUser`: macOS showed the
  parent), the after-report box sits at the bottom of the screen.
  First profiles (#27-#31, 0.8.137): per route callback the AVERAGE is 100-500 us (own render 70-80 %, wait for lanes
  10-15 %, mix, IR hand-over ~0, bookkeeping up to 66 us at 44 000 voices on the Ryzen) = 13-71 % of the time, while
  "longest driver callback" (DeviceClock: first route entry -> last route end of one driver callback, a gap > 0.2 ms
  between routes starts a new one) was 80-87 % - the break is a PEAK of one period, invisible in averages (a single
  9 ms stretch per second adds 6 us to a 440 us average). 0.8.138 adds `statRouteMaxTotal/Own/Wait` (RenderPoolStats
  `routeMax*Ns`, MeasureState `devMax*`, log "longest T us with own render A, wait B", report "The longest route
  callback of the run took ..."). Next: find what the peak is (a lane preempted? the control thread? the worker asleep
  at the period start on <= 2 cores?) before touching the 85 % rule or batching. `lastLoadError` is logged as "Load
  message: ..." (the Mac reports of #27 stopped by kernel pressure level 2 at 5.0 GB of 8 GB).
  0.8.139: `measurementShareLimit` (MainComponent, machine-specific key "measurementShareLimit", 0.6..1.0, editor
  slider "Measurement limit" before Measure...; `PolyphonyMeasurement::shareLimit` fixed at the start of a run; every
  former 0.85f of the measurement reads it, the "safe" scaling uses limit - 0.05). `cancelLoading()` /
  `loadCancelRequested` + `loadCancelled` (engine; reset at the first tree of a load with gSwapBaseline): the
  loaders' ThreadWithProgressWindow have a Cancel button (30 s), their progress lambdas call cancelLoading() when
  threadShouldExit(); a cancelled StartupFolderLoader compiles the phases like a complete one (finished(false)).
  Adoptable stops (`isPooledStop` or `hasLoadedRankForFolder` on a replacing reload) bypass the memory stop (#33).
  `lowerCurrentThreadPriority()` (SamplerEngine.cpp) on parallelFor workers and copy-build threads (Windows
  BELOW_NORMAL, Linux nice 10 per thread, macOS SCHED_OTHER priority - 8). `startPolyphonyMeasurement` refuses while
  `pendingBackgroundWorkers() > 0` (#32). `arrangeReportWindows(folder)` (static; Windows EnumWindows "CabinetWClass"
  by the folder name -> right half, the foreground window of another process -> left half, SPI_GETWORKAREA physical
  pixels; macOS osascript Finder bounds + System Events frontmost; Linux wmctrl) called 1.5 s and 4.5 s after the
  launch. Coupler catalog: downward couplers for every (target < source, target >= 1) in all five types after the
  pedal bass couplers; default visible only unison with target > source and 1-2-8; HW mapping allows target < source.
  0.8.140: KeyAction h = 2 -> `Coupler::bass` -> type "bass", target 0 allowed for bass (Doesburg bass_1..4 = 0-N-bass);
  `Source/CpuInfo.h` `CpuInfo::hybrid()` (Windows GetLogicalProcessorInformationEx EfficiencyClass; P = the max class,
  `performanceMask`), preset "performance" (menu item only on a hybrid CPU) -> `workersForCurrentPreset` = P cores,
  `applyRenderThreadSetting` sets `engine.setRenderWorkerAffinityMask` (gRenderWorkerAffinity, applied in `workerLoop`
  with SetThreadAffinityMask) and rebuilds the pool (`setRenderThreadCount(..., force)`); the report names a hybrid CPU.
  Report windows: showAfterReport FIRST, then startAsProcess + launchInDefaultBrowser; `placeNativeWindow` HWND_TOP +
  SWP_SHOWWINDOW, BringWindowToTop both, SetForegroundWindow the browser; macOS: frontmost app name via System Events,
  then `tell application "<app>" to set bounds of front window` (no permission needed for Safari/Chrome/Edge/Brave),
  System Events position/size as the fallback. "ASIO shows not ready" (#34 log: `ASIO host opened for 'default
  device': no device`, then "Audio devices still missing after the wait: ASIO default") = a route on the ASIO TYPE
  without a driver name; 0.8.141 `sharedHostFor`: ASIO + empty name -> the driver running in another host of ours,
  else `defaultDeviceNameOfType` (a scan, no open), logged "ASIO without a driver name: X taken".
  0.8.142: `DisplayPage` (a id, b name) + `ImageSetInstance` (b name "2. clairon 4 (StopPage)", e page, g x, h y) ->
  `HauptwerkOdf::stopPositions` (the page with the most stop-named images, name = the stop name minus trailing
  parentheses; Ashton "Simple Jamb" 47/47) -> `PlanStop.gridColumn/gridRow` = ranks of x / y within the division of
  ours (skipped when > 6 columns or > 24 rows, noted "Grid: N stop(s) placed...") -> the import writes
  voicingSettings "stopGrid" {manual: {rowName: "c,r"}} (the voicer's own format) and `HauptwerkExtension.column/row`
  -> `voicer->setGridCell(manual, Voicer::transmissionKey(owner, index), ...)` after addTransmission. "couplersExclusive":
  true in the import -> the settings loader hides every coupler first. `transmissionCaption` = `t.alias` when set
  (never shown before!), else `transposedStopName`, which now skips a division prefix ("4. Tuba 8": the "4." was taken
  for the footage -> "2'. Tuba 8"). Pool (#44): `readyStopsNotIn` counts a row's folders as used only when the engine
  holds the stop (`stopLoaded`); `addReadyStopToDivision` with a row of the same folders present -> no new row,
  `reloadDivisionTrees(manual)`. Measurements 0.8.138-0.8.141 (#40-#45): Ultra 7 P-cores-only 37 600 < all 16
  cores 42 200 (#45: both together better - keep the default; the E-core wait peak 2.2 ms stays, #44), Ryzen
  Windows longest callback 2.1 ms of which OWN render 1.9 ms (#40), Core 2 Duo longest 3-5 ms own render 3 ms
  (#41/#42: the poster is half the lanes), Ryzen Linux 0.4 ms (#43, fine). Verbal recommendation given: cap the
  poster's chunk share at the tail of a job + smaller tail chunks for slow lanes; nothing large left.
  0.8.143 (the user: "tohle zapracuj"): RenderPool wake-ahead - a sleeping worker's `sleepCv.wait_for` is cut to
  `lastJobTicks + periodTicks - 1.5 ms` (min 100 us, max 20 ms) and on a timeout it sets lastWorkTicks so it spins the
  hot window into the period (gFewCores outside Windows: no spin, it just polls); `takeChunkAsPoster` = the poster takes
  nothing more when remaining <= 2 x (lanesJoined - lanesLeft) and some lane still works (never when none works: a
  stall); `Job::openedTicks` + `statJoinDelayMax` -> `RenderPoolStats::laneJoinDelayMaxNs` -> report "The latest a
  lane joined a job after it opened: N us" + log "lanes joined within N us". Storage: `storageSampleRate` (engine +
  MainComponent, settings key, NOT machine-specific; 48000/36000/32000 in the "Sample rate on loading" combo, index 3
  = keep the files' rate) replaces kBaseSampleRate in `decodeSampleFile(..., baseRate)`; `releaseHalfRate` (third item
  of "Sample storage in RAM"): releases decoded at baseRate/2 (`loadSamplesIntoStop` isRelease). The merge estimate
  uses storageRate. `dropNestedTrees()` (called first in `syncDivisionTreeLists`): an inner tree's refs -> the outer
  tree with the path prefixed, indices shifted, division paths replaced and deduped, note + an AlertWindow via
  runWhenLoaded ("Nested folder trees"). Caen like-for-like (#46 Ryzen 54 400 / #47 Ultra 7 54 800): same peak on
  both = the poster rendering alone (2.3-2.5 ms) while the workers wake - this build's wake-ahead + poster tail is
  the fix to be measured; the review is posted on #46/#47.
  0.8.145 packed samples (`Sample::compress16/decode16/read16`, SamplerEngine.cpp after `finaliseLoops`): FLAC-like,
  blocks of 256 frames, header byte (order 0/1/2 in bits 0-1, residual width in bits 2-6), `order` warm-up int16,
  residuals of w bits LSB-first as r + 2^(w-1), w = 0 = all zero; `blockOffsets` for random access; verified frame by
  frame at pack time (a mismatch keeps the raw data). `pcm16` is FREED when packed: every reader outside the render
  path goes through `read16(ch, scratch)` (copy builds, the cache file, rmsOf, the spectrum analysis, MainComponent's
  sample export, the self-test) - never `channel16` on a sample that may be packed. The render (`renderVoiceSpans
  <int16_t>`) decodes a window per span into `tlPackedWindow` (2 channels x main/head x 4096 frames; kept while the next
  spans of the voice read inside it; the span is capped to `kPackedSpanFrames`), positions made relative to the window
  (`posBase`), the splice head decoded a loop length away into the second region (`headOffset = kPackedWindow << 32`).
  Float storage is never packed. Packing happens where a sample is finished: `packIfEnabled` after `decodeSampleFile`
  in loadSamplesIntoStop (on the loader threads, before the memory count), after the cache hit and after the copies
  in the copy build; `notePackStats` logs "Packed samples: ..." per tree / per copy build. Setting `compressSamples`
  (machine-specific key), engine `setCompressSamples`, editor toggle "Compress samples in RAM (lossless)" with the
  price in its text; the voice profile has `unpack` (report "unpacking the packed samples N"). `--kerneltest` prints
  "Packed samples: ... -> OK/MISMATCH" (windows + a packed-vs-raw render over splices and cubic; MISMATCH fails CI).
  Linux: `LinuxWindowPlacement.{h,cpp}` is JUCE-FREE (std::string, `Box`) - Xlib's Font/Time/Drawable typedefs are
  ambiguous under JuceHeader's `using namespace juce` (the 0.8.144 CI Linux build failed on it); never include
  JuceHeader.h and Xlib.h in one translation unit. CMakeLists links `${X11_LIBRARIES}` on Linux (find_package(X11)):
  JUCE itself dlopens libX11, so nothing linked it and the first v0.8.145 tag failed with "undefined reference to
  XOpenDisplay" - the tag was moved onto the CMake fix (the Windows exe is the same build).
  0.8.146 (Linux session): the storage choices (`sampleStorageFloat`, `releaseHalfRate`, `compressSamples`,
  `resampleOnLoad`, `storageSampleRate`) are NO LONGER machine-specific keys: they travel with the organ and
  `MainComponent::askLoadOptions` (an AlertWindow with three combos: storage / packing / rate on loading, Load /
  Cancel) shows them before every Load Configuration and Hauptwerk import with the file's values (else this
  computer's last choice) - the user's choice goes into the configuration var and the members/engine, and is
  saved with the configuration; the dev hooks ORGAN_LOAD_CONFIGURATION / ORGAN_MERGE_ON_START skip the box. The
  startup auto-load and the merge take the file's values silently. Never apply a file's storage choices without
  the box again, and never silently override them from settings.json either (the 0.8.144 rule is replaced by the
  box). Measurement end (`finishPolyphonyMeasurement`): keys up, NO noteOffAll (the tails ring out), the level
  glides up over 3 s at once (callback glide 3 s), the limit (new or restored) is applied 8 s later by
  `setLimitLater` (`pendingMaxPolyphony` is what saveSettings writes meanwhile) - a limit of a few thousand set
  while thousands of tails sounded stole them at once ("the audio stops dead"). `SamplerEngine::setPerspectiveDeviceKey`
  flushes the route's voices (`Impl::flushRouteVoices` = the body of cleanStaleRoutes, `finishRouteVoicesLocked`)
  when the key changes or empties (JACK -> ALSA after a measurement played every tail and kill fade at once).
  Half-rate releases: `decodeSampleFile(..., fullRate, &keptFull)` keeps the full rate when `energyAboveDb`
  (4096-frame windows over the first second, the loudest) above 0.42 x the half rate is > -30 dB; measured on Caen
  Clairon 4' key 80 with numpy (/tmp/halfrate.py): linear interpolation x2 of a 24 kHz file leaves images in
  12-16 kHz at -20 dB and the file loses -9 dB of its energy above 16 kHz - a reed must stay at the full rate.
  Log "Releases at half the rate in the tree ...: N stored at half, M kept at the full rate". The "Not enough memory
  for this organ" box is gone for good (the log line stays). `maxVoices` 500000 (the pool is built at start,
  ~160 MB; the user asked). `Stop::showRankCount` + StopRow `rankCountToggle` ("Rank count [II]", stop folders
  only; saved as "showRankCount": false only when off). `displayOfMainWindow()` for the report windows and the
  after-report box (two screens: everything went to the primary); Linux rects get 36 px for the frame on top;
  macOS box lifted by height/7. `fitWindowOnScreen(..., wanted)`: a dialog that came up < 70 % of the bounds
  placeDialog gave it is set again (the KDE "miniature Configuration Editor" - not reproduced here: the editor
  opened right with and without a remembered position; the cure is generic). Measurement reviews #53-#56: the
  spread is buffer size (256/480/512/1024), the device path (PipeWire default output vs direct hardware vs JACK),
  the packing (16 packed trees: Ultra 7 60 000 -> 16-26 000, Ryzen 64 000 -> 46-52 000) and, on two cores,
  single callbacks at 107-130 % (the OS scheduled something else) plus lane join delays up to 4.6 ms (the worker
  preempted) - no wake-ahead bug (inactive on 2 cores). The Projucer resave and the VS2026 files are NOT updated
  for 0.8.146 (Linux session): do it on Windows next.
  0.8.150 WAYLAND (the AMD KDE box runs Plasma 6.6 on Wayland, XDG_SESSION_TYPE=wayland): the X11 `_NET_CLIENT_LIST`
  holds XWayland windows only (Dolphin/Firefox invisible, "windows seen:" empty) and an XWayland client cannot place
  itself - KWin's placement put the report box in the top-left corner (and that is the user's "miniature Configuration
  Editor in the corner" too, most likely). `LinuxWindowPlacement::placeThroughKWin`: a JS file in ~/.config/OrganSampler
  loaded over D-Bus (`gdbus call --session --dest org.kde.KWin --object-path /Scripting --method
  org.kde.kwin.Scripting.loadScript <file> <name>` -> "(N,)" - gdbus prints the int32 WITHOUT a type word; 0.8.150
  looked for "int32 N", took every answer for a refusal and fell back to X11 (fixed 0.8.151: the number after "(" is
  parsed, a script of ours still loaded from an earlier run is unloaded first - loadScript answers -1 while the name is
  loaded -, the X11 fallback keeps the KWin note), then `/Scripting/ScriptN org.kde.kwin.Script.run`,
  then unloadScript); the script finds windows by caption/resourceClass (Plasma 6 `workspace.windowList()`, 5
  `clientList()`), un-maximises, sets `frameGeometry`, places our own "Report a problem" box (`wantedScreen`) too.
  Test: `ORGAN_TEST_REPORT_WINDOWS=<folder>` with the folder open in Dolphin and a browser open (/tmp/report_test.sh).
  A KWin script can close a window too (`w.closeWindow()`): /tmp/kwin-close-box.js dismisses the "Audio devices not
  available" box of the user's Doesburg configuration (its routes are Windows Audio) for a screenshot on Wayland, where
  nothing else can click (no xdotool). Website 0.8.151: `site/assets/main.png` = the console with Doesburg Surround
  (the user: "jen na první okno", the Stop List / Divisions / Editor pictures stay; README.md keeps console-stralsund.png).
  0.8.156: the engine's `startNote` no longer checks `keyInCompass` and `keyHeldInCompass` ignores the compass - the
  compass is applied in `MainComponent::routeKeyboardNote` to the physical key only; coupled notes beyond the compass
  play the folder's pipes (001313: files 024-096 on a 36-96 manual). The voicing rows of a stop span the compass
  AND its own sample range (`makeStopRow`, `widestCompass`). `reloadStopSamples(stop, true)` kills every voice first
  (crash 23 Oct 03:08: renderVoiceSpans on freed samples after a "Files vs keys" change while a key was held).
  The key tooltips name the file for pipes of their own too. Tremulant rank drift of a stop folder: the master's
  logical stop has `groupChildOrdinal` 0 and its children 1, 2 ... (applyStopGroups) - the phase code at the voice
  start uses the LOGICAL stop's ordinal plus the perspective rank's ordinal, so a master with pipes of its own and
  its child differ by one rank drift; nothing was changed there. The sample folder chooser is the OS's own dialog
  (`FileChooser(..., true)`): its behaviour on "up" is Windows', not ours. Test configuration: D:\Hauptwerk\...\
  OrganConfigurations\Pokusny.json (tree 001313: stop folders with own pipes, "Files vs keys" -12 on quintaton).
  0.8.155 FILES VS KEYS (#76): the number in a sample's file name is NOT always the key. Doesburg (Sonus Paradisi)
  names the file of key 36 "024" on 232 of 244 ranks (Caen, Beziers, Ashton: 0) - the ODF's `Pipe_SoundEngine01`
  NormalMIDINoteNumber (d) is the key, the Sample row's file name carries the number; our loader took the number as
  the key, so Doesburg sounded an octave high with its lowest octave off the keyboard and the top octave filled by
  copies. Now `Stop::sampleOffset` / `FolderAssignment::sampleOffset` / `StopBuild::Entry::sampleOffset` (json
  "sampleOffset", only when != 0) = file number - key; `loadSamplesIntoStop` puts a file on key number - offset (files
  whose key falls off 0..127 are not decoded; all others are - six-octave folders load whole for the couplers).
  Set in TWO places: the Stop List column "Files vs keys" (after Alias, applied with Apply and reload) and the voicing's
  box "Files vs keys" on every stop with pipes (`StopRow::sampleOffsetBox` -> `Voicer::sampleOffsetRequest` ->
  `MainComponent::setStopSampleOffset` -> entry + `engine.setStopSampleOffset` = `reloadStopSamples(stop, true)` with
  the copies cleared + `finishInPlaceStopEdit`). Adoption on a reload compares the offset too (`hasLoadedRankForFolder
  (manual, folder, offset)`, the commit's source match). The Hauptwerk import: `Rank::sampleOffset` = the most common
  (file number - pipe note) over the rank's pipes, `PlanStop::sampleOffset` the pipe-weighted mode over the stop's ranks
  (children per child), written into the rows; the plan notes "Files vs keys: N stop(s) ..." (Doesburg Surround: 64).
  The per-pipe levels (`levelsForFolders`, keyed by pipe note) are keys already - they were misaligned before on Doesburg.
  Never verify this by the sound's pitch (the user: "je to podle jména souboru ... zřejmé z ODF"). Organs imported
  before 0.8.155 keep offset 0: load the definition again or type the column. Dev hook `ORGAN_TEST_OFFSET=
  "<manual>|<stop prefix>|<offset>"` (the voicing path); the grid dump prints "notes a..b files N" per stop.
  0.8.158 TRANSPOSITION (the user's rule, three messages): "Files vs keys" IS the transposition - ONE number,
  `Stop::sampleOffset` (= file number - key, the engine's `keyShift()`), named Transposition everywhere (voicing box
  `shiftBox` shared with the transmission rows, Stop List column "Transp."), counted against the REAL footage the files
  have on their own numbers (smpl pitch - file number), never against the name: Doesburg quintaton "16" = files 024..
  sounding 8' on their numbers -> "-12 -> 16'" (the ODF says -12, the auto-detection says -12, the box shows -12).
  `Stop::footageSemitones` (computeTuning440, `footageSounding`, `footageBreaks`, `lowestFootageSemitones`) is the REAL
  footage of the files on their own numbers (pitch - file number = pitch - (key + sampleOffset)) - the Orig. Pitch
  line shows it (quintaton "8'"); what sounds on the keys is real + sampleOffset (`stopFootageSemitones`, `fadeScale`,
  `displayName`). `untransposedFootageSemitones` = stopFootageSemitones - sampleOffset is the box's base ("0 -> 8' (as
  the original)"); `cleanIntervals()` = {-24,-17,-12,-8,-5,-2,0,2,4,7,10,12,14,16,19,22,24} (+ the
  current odd value) are the ONLY steps offered in both boxes (the user: "14.25' at -10" is nonsense). Auto-detection
  in `loadRankSamples` when the row carries no value (`FolderAssignment/Entry/Stop::sampleOffsetSet` false = the json
  key "sampleOffset" absent; the Hauptwerk import writes it ALWAYS, also 0 - the ODF has precedence): mode of
  round(smplPitch - key) over >= 3 samples with >= 70 % agreement vs `footageFromName(displayFolderName(folder))` or the
  stop name -> offset += named - sounding, samples re-placed, `sampleOffsetDetected` -> `takeDetectedSampleOffsets()`
  (StartupFolderLoader afterCommit) writes it into the Stop List rows (set). No footage in the name -> nothing. Verified
  on Pokusny (001313): "NachtHorn 8" (files 024.. sounding 4' on their numbers) -> -12 = Doesburg's ODF value; 001313 smpl
  unity notes are the real pitch (echoBurdon 024 -> 35.98, quintaton16 024 -> 23.98). Caption footage: untransposed
  stops keep the name's footage as ever; a transposed stop shows the MEASURED footage (`Stop::displayName`,
  `nameWithFootage`), else name + offset. The Hauptwerk import strips the footage from aliases (`nameWithoutFootage`:
  stops, children, extensions) so the caption's footage is the sounding one. ONE-SHOT: `finishNoteOff` skips
  `isOneShot` stops (a key up never releases or fades them - the user's rule; the engine never had it, the first commit
  released them too); `Stop::oneShotManual` (-1 auto by the patterns / 0 / 1; voicing state "oneShot", `setStopOneShot`,
  `updateOneShotFlags` honours it), voicing row toggle "One-shot" after the alias (`oneShotToggle`, every stop with
  pipes). "Rank count [II]" only on stop folders (visibility set in `setChildrenGetter` - the getter comes after
  construction), after the alias. CONFIGURATION PLACEMENT: `HauptwerkOdf::configurationFolderFor(root)` =
  OrganConfigurations beside the folder ABOVE HauptwerkSampleSetsAndComponents (`D:\OrganConfigurations` for
  D:\Hauptwerk\...), else beside the root (rar set E:\sonus\HW2\caen\dEMO -> E:\sonus\HW2\caen\OrganConfigurations);
  never inside Hauptwerk's tree (the user's rule). ARCHIVES ELSEWHERE: `SampleFiles::setSearchFolders` +
  `locate()` suffix matching (>= 2 path parts, longest first) over the archives of: the configuration's folder, the
  lines of its `ArchiveFolders.txt`, `OrganRarPackages` beside, the settings folder's ArchiveFolders.txt
  (`MainComponent::refreshArchiveSearchFolders` at start/apply/load, `registerArchiveFolder` on every archive chosen
  in Load folder / Load Configuration, the rar import writes the list beside the new configuration). Load Configuration
  takes `*.rar` (`loadConfigurationFromArchive`: index job, .json (not .bak) first, then ODFs, several -> PopupMenu;
  a .json inside is read via SampleFiles::openInput). Dev hook `ORGAN_TEST_TRANSPOSITION="<manual>|<prefix>|<st>"`
  (was ORGAN_TEST_OFFSET). Manual audit of 0.8.158 fixed: master default -18, melody/bass new-key default ON, "Hide
  all voicing" only folds, Stop volume slider, combination row (Bank, Tutti, X, Pistons), Measurement limit + ALSA
  switch in 8.1, Show settings.json / Refresh values, the Divisions dialog's own captions, "_128"; code tooltips
  -52 dB and "Fletna", master label "-18.0". The user's settings `configurationFile` pointed at my Adlington test
  configuration (E:\sonus\HW2\Adlington\10\OrganConfigurations) - restored to F:\Stralsund_Jakobi_Wegscheider.portable.json;
  every test run must back up AND restore settings.json (a load retargets it).
  0.8.157 RAR (`Source/SampleFiles.{h,cpp}`, jucer ids SmFls1/SmFls2 + CMakeLists): a configuration never names an
  archive; every path is a disk path, `locate()` walks up to the nearest existing ancestor folder and looks the rest up
  in the indexes of the first-volume `.rar` files of that folder (the disk wins; `archivesIn`, `volumesOf` = partN /
  .r00 sets). The INDEX is OUR OWN header walk (`Walker::walk4/walk5`: RAR 4 blocks CRC/TYPE/FLAGS/SIZE[/ADD_SIZE],
  RAR 5 CRC32/vint size/type/flags/extra/data; one 1 KB read per block, seeks over the data; FILE/NEWSUB/SERVICE/
  ENDARC; split before/after -> `Entry::pieces` {volume, headerOffset/Length, dataOffset/Length}; per volume the
  main block and the end block offsets; `solid`, `stored` (RAR4 method 0x30 / RAR5 method 0), `encrypted` (RAR4
  FHD_PASSWORD, RAR5 extra record 1, CRYPT block -> refused), RAR4 unicode names ported from libarchive's
  decoder). Cache `%APPDATA%\OrganSampler\ArchiveIndex\<hash>.idx` v2 ("V main|end" per volume, "D path",
  "F order|size|stored|encrypted|pieces|path"), keyed by the first volume's size+mtime; written through a
  MemoryOutputStream (a juce::String appended 18 000 times took MINUTES - O(n^2)). READING: `readEntryAlone` - a
  stored file straight from the volumes; a packed one through libarchive fed by `VirtualStream` with the file's
  bytes alone ([sig+main][file header][data] and between the pieces [end block][next sig+main][continuation
  header][data], 4 KB of zeros at the end for the RAR 5 reader's 512-byte look-ahead) - one handle per file, so the
  loader's parallelFor reads files in parallel (`FileData::archived` + `SampleFiles::readFile` in
  `loadSamplesIntoStop`; a solid archive is read in sequence by the old cursor, `readEntriesInSequence`). Measured
  (Re_glockenton 189 MB in 8 variants: RAR4/5, volumes, recovery record, stored): byte-exact against the disk,
  100-120 MB/s unpacked per thread packed, 1 GB/s stored; Caen DEMO import from its rar 7 s (was 47 s with the
  sequential cursor). WHY OUR OWN WALK: libarchive 3.8.9's RAR 5 `scan_for_signature` checks 504 of every 512-byte
  chunk, so the next volume's signature on a chunk edge is missed (Ashton 14 x 4.48 GB, all volumes a multiple of
  512: the index ended after 27 GB and 34 GB were read through) and its RAR 4 reader fails ("Bad RAR file") on the
  18 zero bytes WinRAR leaves after the end block of a volume with a recovery record (-rr); libarchive is a git
  submodule with a prebuilt archive.lib - never patch it, feed it. `indexArchivesThen` (MainComponent) builds the
  indexes on a ThreadWithProgressWindow ("Reading the archives", `SampleFiles::indexingProgress`) before
  `loadIndexedIntoDivision`; E: on this machine is a 16 TB Toshiba HDD (3 ms per seek): a walk of 18 000 headers
  is a minute there. Dev: `ORGAN_TEST_ARCHIVE=<rar>` (volumes, index time, bytes read, top folders, definitions),
  `ORGAN_TEST_ARCHIVE_CHECK=<disk root>` (every 50th wav + the first 80 compared byte by byte - Rar.exe stores
  the path from the drive root, so the root is `D:\` for the rartest sets in %TEMP%\rartest made by
  `C:\Program Files\WinRAR\Rar.exe a -ma5 -m3 -v40m -rr3`), `ORGAN_TEST_LOAD_ARCHIVES="<division>|<rar>"` (the
  Load folder path, the definition box skipped), `ORGAN_OPEN_LOAD_FOLDER=<division>` + ORGAN_TEST_SNAPSHOT. The
  Unpack archive top-bar button is hidden by default (the 0.8.112 one-time forcing is gone); Load folder's switch
  "Unpack the archives into a folder first" + destination does the unpacking (`startUnpack` with a continuation).
  0.8.154: merge grid - `baseShown` (MainComponentMerge.cpp, the base's non-child table rows per division) decides the
  "last row"; the added organ's first row lands at lastRow + 1 (Beziers stood 11 empty rows under Caen: stale cells
  of stops gone/pooled/indented counted). The move between divisions keeps the arrange mode (`dropStopReferences()`
  instead of exitArrangeMode in the move-house branch); "Grid drop: ..." log line in `dropItem`. `freezeGrid()/thawGrid()`
  (MainComponent): `applyPortableConfiguration` takes a snapshot of `stopsViewport` before `engine.unloadManual`, shows
  it dimmed (0.55) over the workspace, the loader's completion thaws it - the grid stayed blank for the minutes a
  merge took. The user drags in MY test instance when one is open (he found the minimized window): every test run
  shows up in his hands - say so, or close it first. Disk facts (this Ryzen): the organs live on D: = Samsung 870 QVO
  8 TB SATA; Caen (45 GB) 24 s from the page cache, 119 s cold (~400 MB/s = the disk) - a merge of a second 45 GB organ
  evicts the first; "loading was super fast before" = warm cache. Never call a cold load a regression without the
  warm/cold pair. Filled pipes: the copy build (SincTable, makeAntiAliasCopy, applyStaticEq, `formantFollowForShift`: the
  stop's setting downward up to an octave, +30 points for any upward shift, beyond an octave +15 points per further
  octave for the resampling methods / +25 for the phase vocoder, keepLoudness, copyParametersKey v5) is unchanged since 0.8.49; the cache record
  carries the source frame counts, so copies of a 32 kHz run are not taken for a 48 kHz load.
  0.8.153 CI LESSON: the two Windows pushes of 22 Oct evening (main + tag v0.8.153) updated the refs but GitHub created
  NO workflow run and no check suite for them (Actions enabled, status page green, the Linux pushes of the afternoon
  ran) - `POST /repos/.../actions/workflows/build.yml/dispatches {"ref":"v0.8.153"}` (build.yml and pages.yml have
  `workflow_dispatch`) started the build; a dispatched run on a tag ref still publishes the release (github.ref =
  refs/tags/...). When `actions/runs?head_sha=<sha>` says 0 and `commits/<sha>/check-suites` says 0 five minutes after
  a push, dispatch instead of re-pushing the tag. Packing measured (Doesburg demo, 8 GB raw, warm cache): 0.8.145 packer
  15 ns per frame and channel = 63 s of thread time (~4 s wall on 16 threads), 0.8.153 one-pass packer 6.7 ns; the
  "Packed samples" line prints the thread time. A cold load of the same demo took 38 s (disk), warm 7-8 s.
  0.8.153 REGRESSIONS (the user: "soupis všech poškození a regresí"): (1) 0.8.144's rebuildUI rule "a stop folder with
  no children is left off the grid" hid every NEW stop folder (empty by nature) - now only after a load stopped for
  memory (`engine.lastLoadWasMemoryLimited()`). (2) The 0.8.146 "Rank count [II]" switch was NEVER shown: its
  visibility was decided in StopRow's constructor with `childrenGetter` still null (the getters come after construction
  through `setChildrenGetter`) - now visible on every expanded stop row right after the button, enabled in
  `setChildrenGetter`. Never decide a row widget's state from the getters inside the constructor. (3) The 0.8.146 size
  cap of the Report a problem dialog (60 % of 800 = 480 px) left the buttons a negative height on 1280x800 (#69/#72/#74) -
  fixed parts first, editors flexible. (4) Loading: the user's own files carry "packing on" (Caen 96-138 s vs 47-82 s
  off; Doesburg cached 14 s off / 36 s on / 58 s at 32 kHz without packing) - the load box shows it; the pack costs
  ~2.5x (three predictor passes + a full unpack verify), the 32 kHz sinc (99 taps) 4x. Not yet made cheaper.
  Grid dev hooks (voicer.cpp, end of rebuildUI, 1.5 s after a layout): `ORGAN_DUMP_GRID=1` (buttons + free cells in
  SCREEN coordinates, expanded rows with `describeLineOne()`), `ORGAN_TEST_DROP="<manual>|<stop>|<target>|<col>|<row>"`
  (dropItem as the mouse would; verified: Rural "TA - violon" 0 -> 1), `ORGAN_TEST_NEW_FOLDER="<manual>|<col>|<row>"`,
  `ORGAN_TEST_EXPAND="<manual>|<stop prefix>"`, `ORGAN_TEST_SNAPSHOT=<png>` (+ "<png>.row.png" of the expanded row on a
  dark background - the texts are white, a bare snapshot shows nothing). ORGAN_LOAD_CONFIGURATION needs
  ORGAN_DUMP_STOPLIST or ORGAN_SAVE_PORTABLE set as well. Start test runs `-WindowStyle Minimized` and NEVER drive the
  mouse or take screen captures while the user works at the machine (he was writing mail). The drag between divisions
  by mouse (StopRow::mouseDown/Drag/Up) is unchanged since 0.8.111 and the drop path works - #77's "nefunguje
  přetahování" still needs the user's description of what happens. Small test organs on Windows:
  F:\Suchy_rejstrik\Kobrle\Rural Organ.json and TestingOrgan.json (portable, trees beside them).
  0.8.151/0.8.152 RAM min: `SamplerEngine::estimateMinimumRam()` (every k-th of all loaded samples INCLUDING the
  perspective ranks in `Stop::ranks` - 0.8.151 took the first rank alone, Doesburg 2.3 of 7.2 GB -, ~160, first 3 s each:
  resampled to 32 kHz with `SincResampler`, a release halved to 16 kHz when `energyAboveDb(0.46 x 16 kHz) <= -18`,
  int16, `compress16`; ratio of their bytes to `frameBytes()` x all sample bytes) -> the portable save writes
  `estimatedMinimumRamBytes` = (measured - sample bytes) + sample bytes x ratio and `estimatedMinimumRamStorage`
  (`minimumRamStorageText`), log "RAM with every saving option (...)"; the README RAM line gets "; about X GB at the
  least, with every saving option of the load box (...)" and `configurations_index.py` the sortable "RAM min" column
  (absent -> "-"; >= measured -> the measured figure: the organ was saved with the saving options in force).
  `measuredRamStorage` = the whole `storageSignature()` since 0.8.152. Never type or estimate it by hand. Doesburg
  0.8.152 test: /tmp/ram_test.sh (auto-load + ORGAN_SAVE_PORTABLE into /tmp/ramtest, the device box closed by the KWin
  script) - Doesburg (already 16-bit/32 kHz/halved/packed): samples 6.76 of 7.23 GB, ratio 1.03 -> min = measured;
  Rural Organ loaded as float 48 kHz: 1.76 GB measured, 0.90 GB at the least (ratio 0.232, 79 of 86 releases halvable).
  A portable save (the hook too) RETARGETS settings' `configurationFile` to the saved file: a test script must back up
  and restore settings.json, else the next start says "Started without a configuration" when the file is gone.
  The copies of filled pipes are counted because they are in RAM and in the measured figure (a few per cent); the
  disk cache of filled pipes plays no role anywhere in the RAM figures - never say a fast load is "thanks to the cache"
  (Doesburg loaded in 12 s from the OS page cache, 53 GB of WAV still cached from the previous load). `juce::jmax<juce::int64>(0, x)` does not compile with
  juce_dsp (the SIMDRegister overload takes the explicit argument): use std::max or a ternary.
  0.8.149 (Linux): `AfterReportWindow` (MainComponent.cpp, modeless DialogWindow, always on top, `gAfterReportBox`)
  replaces the modal AlertWindow after a report (#58 keyboard, #67 invisible blocker behind the main window on KDE,
  #64 macOS buttons shrunk: the System Events fallback resized OUR front window - now skipped when the frontmost app
  is OrganSampler/Finder/empty; `AfterReportWindow::resized` puts its size back). `ORGAN_TEST_REPORT_WINDOWS=<folder>`
  shows the box + placement 4 s after the start (open the folder and a browser by hand first). LinuxWindowPlacement:
  `wmSupports("_NET_MOVERESIZE_WINDOW")` -> EWMH message alone, else XMoveResizeWindow (never both); file manager by
  class when the title lacks the folder; the note lists the windows seen. `HauptwerkOdf::spelledAsOnDisk` (rank
  folders spelt as the disk, #57). `Sample::sincHead` (set in decodeSampleFile: halved AND energy above 0.25 x rate
  > -40 dB; copies inherit it): the sinc runs only for those (Core 2 Duo Mac 4100 -> 2200 with every release through
  it, #64 14:18). Levels log: "Audio levels (master X dB[, measurement level -52 dB on top]): ...". #62/#63 "aggregate
  20 dB louder": every aggregated run 13:19-13:54 shows -15 dB at 10 k voices, ASIO/Windows Audio from 14:01 -38 dB -
  the user lowered the master to -40.6 as his reaction; the per-route renders are the same chain for every device
  type (renderRoute: masterGain x route volume), so the loudness comes after our buffer: 15 shared-mode streams of one
  route each on the endpoint "ADAT (7+8)" - most likely the endpoint's audio enhancements (loudness equalisation
  boosts each quiet stream) - told the user to disable them on the endpoint; JUCE's shared WASAPI client does not set
  AUDCLNT_STREAMOPTIONS_RAW (would need a JUCE patch). Measurement table of every issue log: /tmp/measure_all.py
  (dedupes by start time + CPU, version by the release times, options from the "Load Configuration" line).
  `Stop::storageSignature` (= `SamplerEngine::storageSignature()` at decode, copied on adoption): a reload adopts a
  division's loaded samples ONLY with the same storage choices (`hasLoadedRankForFolder` and the adopt lambda check
  it) - before, a reload after the Load box kept the packed/halved samples of the previous load (#62: the "16-bit,
  32 kHz" runs at 7 000-11 000 were the packed organ; "packedTrees 0" after a reload = adoption). The measurement
  summary names "samples <signature>".
  0.8.148: LESSONS. (1) "Audio callback anomaly: <old device> manual N perspective default: no callbacks" every minute
  = a route WITHOUT a device (closed by closeHiddenRoutes, "remembered as none") whose RouteCallback kept its old
  deviceLabel/expectedBlock - fixed (audioDeviceStopped zeroes expectedBlock/Rate); never read such lines as a dead
  device on the endpoint (the #59 first analysis did, and sent the user to set a "default" route to none that does
  not exist in the dialog - closeHiddenRoutes works). (2) The sinc kernel of 0.8.147 applied to every sample below 0.8 x
  the route rate - with the 32 kHz storage that is EVERY attack, scalar 13 ns/frame vs 1.1 AVX2: Doesburg 45 400 ->
  15 900 (#61) and 7 700 with 1 lane (#60); now < 0.6 x the rate = halved releases only. When a report shows "kernel
  6684" instead of ~1500, suspect a kernel path change before anything else. (3) Aggregated WASAPI: `aggregatedRoutesRunning`
  (counted in RouteCallback::audioDeviceAboutToStart/Stopped) caps the lanes at physical cores / routes in the callback
  (15 routes x 16 lanes: 16 highest-priority workers spinning for 15 posters starved the endpoint threads - breakups at
  one pipe, clean with 1 lane; #59). (4) The load box takes the FILE's storage choices: the user loaded Doesburg with
  "packing on, 32 kHz, halved releases" from his Linux experiments without noticing - say so in every polyphony
  analysis (the log line "Load Configuration "<name>": sample storage ..., packing ..., rate ...").
  0.8.147 (Windows): the level after a measurement is held at -52 dB for 4 s (`outputGainHoldUntilMs`) and then
  glides up IN DECIBELS, 4.5 dB/s (~12 s to 0 dB) - the callback's `factor = 10^(4.5 * blockSeconds / 20)`; a linear
  glide (0.8.146 "3 s") reached -20 dB in a third of a second and the user heard a jump into the tails. NEVER a
  linear gain ramp upward. `SincTable` + `sincAt/kernelMonoSinc/kernelStereoSinc` (8 taps, 128 phases, Kaiser 6,
  cutoff 0.45): `renderVoiceSpans` uses them when `s.sampleRate < 0.8 x routeRate` (halved releases, 36/32 kHz
  storage) for the first 1.3 s of the sample (`sincEndF`), not for splices/cubic; the images of linear interpolation
  (7-11 dB under the signal for a 2x upsample) go to -40..-50 dB; the full-rate rule is now -18 dB above 0.46 x the
  halved rate. `--kerneltest` renders at 24 kHz too (the sinc path packed vs raw). The import's crescendo writes
  "transmissions" [{manual, enabled}] on the owner row for its extensions (`extensionsOfOwner`, the order of
  `hauptwerkExtensions` = the engine's transmission order; `storedTransmissionsOn` counts ordinals the same way).
  `resolveCaseInsensitive` in `firstExistingDirectory` (#57: Windows "ped" vs disk "Ped" on Linux). `gAfterReportBox`
  closed by `showProblemReportDialog` (#58: the modal box kept the keyboard). Manual 5.8 "What moves the measured
  figure" (buffer, device path, packing, two cores, what the measurement cannot see). #59 (Aggregated WASAPI, Doesburg,
  breakups at one pipe, the measurement clean): the breakups happen after our buffer (15 shared streams on ONE endpoint
  "ADAT (7+8)" + five "default" routes with a dead Windows Audio device on the same endpoint: "Audio callback anomaly:
  no callbacks") - asked the user to try default routes = none, Aggregated lanes = 1, ASIO; the measurement cannot see
  a mixer dropout (documented). Linux #58 multi-monitor placement still open (file manager "not found" by title).
  0.8.144 - LESSON: the 0.8.143 wake-ahead used `periodTicks` (the smoothed gap between CONSECUTIVE JOBS = the gap
  between the routes of one callback, ~0.3 ms with 8 routes) as the period -> workers woke every few hundred us and
  spun: #48 Core 2 Duo Linux 2900 (was 2500-2600 ok), #49 Mac 5000 but kernel 9.7 us/voice (3x), #50 Ultra 7 8 routes
  10 700 (was 54 800) with callbacks at 131-145 %. Now `periodStartTicks`/`devicePeriodTicks` (a job after a gap >
  0.3 ms starts a period), wake-ahead only with >= 2 workers, min wait 0.2 ms. NEVER derive the period from the job
  cadence. Unison off: `conditionOfKeyboardDivision` (keyboard -> division -> the condition switch of the conditional
  key links) -> a `Coupler{unisonOff}` for a keyboard whose primary division has no unconditional keys -> id
  "N-N-uoff". Empty stop folders (children skipped) are left off the grid in `Voicer::rebuildUI` items. Every
  configuration file keeps this machine's keys (`applyPortableConfiguration`: the machineSpecificKeys override runs for
  non-portable files too; "resampleOnLoad", "storageSampleRate", "releaseHalfRate" added to the list). ParseJob plans
  on its thread (status text). `nestedTreesNoticed` dedupes the notice per session. `Source/LinuxWindowPlacement.{h,cpp}`
  (X11: _NET_CLIENT_LIST, _NET_WM_NAME/WM_NAME, WM_CLASS; _NET_WM_STATE remove maximised + _NET_MOVERESIZE_WINDOW +
  XMoveResizeWindow + XRaiseWindow; in the jucer as LxWpl1/LxWpl2 and in CMakeLists). #52: Aggregated WASAPI 20 routes
  x 1 lane per endpoint thread = 62 900 > the shared pool - route-level parallelism beats lane-level; keep default 1.
  Ashton: the user's ODF ("Ashton-under-Lyne, Albion Church, Lewis Organ, Surround", 34 KeyActions incl. "Choir to pedal
  sub/oct") - the couplers are right; only the unison off was missing (now a coupler). The "Save, then load reloaded
  the organ" report: the log shows the chooser opened and the same ODF was imported again at 15:36 - the chooser starts
  in lastSettingsDirectory with no file preselected; nothing of ours reloads. 0.8.145 = the packed samples above
  (measured: Rural Organ 25-42 % saved per stop, the self-test tutti +40 % per block, kerneltest unpack 1.5 ns/frame).
- A route without a device ("none") gets NO voices (0.8.126, `Route::hasDevice` set by `setPerspectiveDeviceKey`,
  checked in `publishVoice`: the voice is finished at once). Before, every pipe was published to its perspective's
  route whatever the device and sat there unrendered until `cleanStaleRoutes` flushed it after ~1 s, counted as
  polyphony: the "8000 voices" of the Core 2 Duo Mac (#14, default routes none + auxiliary on the device) were half
  ghosts - the same stimulus (61 keys x 7-8 strikes/s) on the default routes gives 2500. The self-test sets a device
  key on every route (`"selftest|<perspective>|<manual>"`) - never clear it to {} there, that silences the route.
- Engine kernels (0.8.119): AVX2 (8 frames; `ENGINE_AVX2_FN` = `__attribute__((target("avx2")))` on GCC/clang, plain on
  MSVC - before 0.8.119 the AVX2 code was `#if _MSC_VER` only, so Linux and both Macs rendered scalar), SSE2 and NEON
  four-frame kernels (`Lane4`, one code for both), scalar. `bestKernel()` picks at run time; `--kerneltest` (CI on every
  platform, Apple silicon included) compares each kernel with the scalar one (end positions identical, samples within
  1e-4 of the peak) and prints ns per voice and frame from cache and from RAM (`benchmarkKernel`). ENGINE_CPU_PAUSE is
  `_mm_pause`/`yield`, never `std::this_thread::yield()` (a system call in a spin). The render pool keeps lane
  utilisation statistics (`takeRenderPoolStats`); the polyphony report shows the CPU of the RENDER CORES (16 threads
  busy on a 32-thread CPU = 100 %, not 50 %), the lane utilisation, the kernel benchmark with the ceiling it gives, and
  the fixed-per-block / per-voice split of the callback time with what other buffers would give - never a general
  "doubling the buffer doubles the polyphony" claim (false, #8). Measurement level -52 dB.
  Voice profiling (`gVoiceProfiling`, on during the measurement): ns per voice and block for header / spans / kernel /
  post, folded from thread-locals into the pool stats; the report prints it. Stellwagen 0.8.119: 135 / 170 / 1050 /
  880 ns - the 880 was the dynamic tone shelf (private buffer + split pass, on for every voice as soon as the wind sags),
  folded into the kernels in 0.8.120 (`kernel*Shelf`, `dispatch*Shelf`: out += y(1-k), wet += y k; the route low-passes
  the wet sum as before). Splice/cubic spans and per-key tone voices still go through a private buffer.
- "All physical cores" means ALL of them on every computer, two-core laptops included - never silently one (the user
  refused that: nobody picks threads by hand). What broke the Core 2 Duo laptops in 0.8.119-0.8.122 (1700 -> 1400
  voices, #9/#10) was not the SSE2 kernel (their own benchmark: 4.6-4.9 vs scalar 6.7-7.1 ns) but that on Linux and
  macOS the render workers and the control thread had NO priority (Windows had THREAD_PRIORITY_HIGHEST + MMCSS from
  the start): a worker preempted by PipeWire/CoreAudio/the GUI held every block up, and the poster waited with
  `_mm_pause` instead of yielding. 0.8.123: `raiseAudioThreadPriority()` (Linux SCHED_FIFO/SCHED_RR, logged when
  refused; macOS time-constraint thread), `gFewCores` (<= 2 physical): poster waits with `std::this_thread::yield()`,
  chunks <= 12 voices, hot spin window 1 ms. Never blame a kernel without the benchmark numbers from the report.
  ENGINE_CPU_PAUSE outside Windows is `std::this_thread::yield()` again (do not bring the pause spin back there
  without a measurement). 0.8.127: a real-time thread's wait for another thread (`ENGINE_RT_WAIT`: the poster for
  its lanes, a worker for its task) is `_mm_pause` on Windows and a 30 us SLEEP everywhere else - NEVER a yield:
  under SCHED_FIFO `sched_yield` hands the core only to FIFO threads, the SCHED_OTHER window thread starved on the
  two-core Linux laptop, and as it drives the measurement's strokes the keys stayed down and the hang never resolved
  (#15, "Message thread unresponsive for 70 s"). On <= 2 cores outside Windows the worker does not spin after work.
  `CrashHandler::StallWatchdog::setStallAction` (called from the watchdog thread after 3 s of silence): the
  measurement sets it to `engine.noteOffAll()` + abandon the run; cleared in `finishPolyphonyMeasurement`. x86 without AVX2 renders SSE2 by default again since 0.8.126 (`defaultKernel`): the user's
  own A/B with the editor's switch on the same routes - Core 2 Duo Mac scalar 1700 vs SSE2 2500 voices (#14, 0.8.125),
  Linux bench 5.07 vs 7.70 ns (#13) - and he asked for it ("bude stary CPU defaultne loadovat SSE2?"). 0.8.123-0.8.125
  had scalar there at his request from before the priority fix.
  The kernel is a VISIBLE switch (0.8.124, the user asked three times where SSE2 is): Config Editor > Engine >
  "Interpolation kernel" = Automatic / scalar / SSE2 / AVX2 / NEON (only what this build+CPU can run,
  `SamplerEngine::availableKernelNames()`), setting `renderKernel` (machine-specific key), applied at once through the
  atomic `gKernelOverride` read by `bestKernel()` every block (`setKernelOverride`, -1 = automatic). `ORGAN_KERNEL`
  (env, `envKernel()`) only sets the Automatic choice for one start; the editor's choice wins. The log line
  "Render kernel: X" and system.txt ("kernel X") say which runs; the measurement report names it.
  Profiling clock (0.8.124): `profileTicks()` = `__rdtsc` / `cntvct_el0`, calibrated once in `setVoiceProfiling(true)`
  (`calibrateProfileClock`, 100 ms, logged "Profiling clock: N MHz"), deltas through `profileDelta` (negative or > 1 s
  dropped). Never use `juce::Time::getHighResolutionTicks()` per voice: clock_gettime was ~1 us on the HPET Core 2 Duo
  Linux laptop and inflated bookkeeping/spans threefold (#11).
- Filled-pipes cache `copyCacheEnabled` is ON by default (0.8.124; the user's 16-core rebuilt every Stralsund copy at
  each load with it off); the "Copy build of ..." log line says "(the filled pipes cache is OFF...)" when it is not.
- Measurement ramp: a level held with `holdShare` < 0.5 steps half the way to the proportional 85 % point, at most
  doubling (16-core: 40 000 in five steps instead of twenty); below that the old 2 000 / tenth steps.
- The polyphony report makes NO forecast for other buffer sizes (128/256/512 runs on the Ryzen: 24 900 / 38 600 ->
  no model fits); the small-buffer paragraph is a recommendation (256/512), never "breaks at a few hundred voices".
- Render threads default = all physical cores, always. `renderThreadPresetChosen` (settings, machine-specific) marks a
  choice made in the Configuration Editor; without it a non-default preset is reset at load (the old portable Rural file
  carried `allMinusOne` and left two-core laptops on ONE render thread, issues #6-#8).
  A polling lambda stored in a shared std::function (`*poll = [...]{...}`) must never do `*poll = nullptr` while it
  runs - that destroys its own captures (a copy `self = *poll` does NOT help: the executing object is the original) -
  it clears itself afterwards: `juce::MessageManager::callAsync([keep = poll] { *keep = nullptr; });`. Two crashes
  of 0.8.116 (read of 0x24/0x28 in a SafePointer); a crashed process is then held by WerFault.exe, which keeps the
  exe locked for the linker - `taskkill /F /PID <WerFault>` frees it.
- "A stop" is every button of the grid: a stop with samples, every transmission, every extension, and a stop
  folder that has at least one sound under it (its own or a child's); rows nested in a stop folder are not
  stops (the folder is). NEVER count or switch only the stops with samples of their own (`engine.getStops()`
  with `enabled`): use `MainComponent::gridStopItems()` / `Voicer::gridItems()` (grid order) and
  `setGridItemEnabled()` - the polyphony measurement, its retreat and every "N stops" text do (0.8.111).

## Dialogs and layout
- Every window and dialog must be wholly visible: title bar (draggable) and bottom edge included, with 2 % of
  the screen height free above and below AFTER the taskbar/dock/panel is taken off (`workAreaWithMargins`,
  `fitWindowOnScreen` + `fitWindowOnScreenSoon` retries at 150/700/2000 ms - window managers place a window
  after it shows). A dialog taller than that is shrunk, never centred with its edges off the screen.
- Use the space of a dialog: a button is 100-150 px wide, never stretched over the row; a switch and its
  button (or any setting and its action) go on ONE row (`addToggleButton`), never the button under the switch.
  A dialog that asks for a file and a folder shows both as path fields with Browse... buttons in one window,
  the thing the user looks for first at the top (Unpack archive), never a chain of bare file dialogs.
- GitHub's repository tabs (Code, Issues, Pull requests, Actions...) are fixed; a "Releases" or
  "Configurations" tab cannot be added. README.md therefore starts with a heading of links: RELEASES (the
  program), the Rural Organ demo, "Configurations, Organs" (always that name - people want organs), plus a
  release badge. Every release carries two "Source code" assets added by GitHub itself - they cannot be
  removed; the release bodies say they are not needed.
- Release bodies and READMEs are for humans: one bullet per system/platform, the one sentence that matters
  (how to unpack, what to download) in bold on its own line, links as links - never one long paragraph.

- A configuration README is WRITTEN BY THE PROGRAM: saving a portable configuration puts `<name>.README.md`
  beside the `.json` (`MainComponent::configurationReadme`; dev: `ORGAN_SAVE_PORTABLE=<file>` with
  `ORGAN_LOAD_CONFIGURATION=<non-portable cfg>` loads and saves without the dialogs). Only the three
  "(fill in: ...)" places - Organ, the producer/product/link in Samples, Author - are completed by hand, plus
  the required version if a fix matters (Stralsund: 0.8.92). Never retype the rest. Its form:
- A configuration README has exactly this, in this order, and nothing more (people do not read):
  title; one opening paragraph (what it is, the perspectives - they make the recording unique -, divisions, the
  number of Stop List rows) + "No samples are published here"; "The configuration" bullets Organ, Samples
  (producer, product, link, packages, WAV, perspectives, required OrganSampler version), Expects (trees), Audio
  layout (routes, unused ones marked), RAM (measured by the program at the portable save: `measuredRamBytes` +
  `measuredRamStorage` in the json, `processMemoryBytes()`; INDEX.md shows it as a sortable column - never typed,
  never estimated), Saved with, Author + date; "Where to put the file" (the packages are now
  known: one sentence + the tree diagram, never a made-up parent folder like `haupwerk`); last the "Stop List"
  table of the row names by division. Never mention what OrganSampler does natively (couplers, voicing,
  crescendo, combinations). No "What you need", no "Using it", no prose about the divisions.
- Configurations live ONLY in `configurations/<organ>/` on GitHub as `README.md` + `<Name>.zip` (the one `.json`
  inside; a zip downloads from GitHub, a bare `.json` opens as text - never "Download raw file" instructions).
  The portable save writes `<name>.zip` and `<name>.README.md` beside the `.json`; the README's Download line
  is `[<Name>.zip](<Name>.zip?raw=true)`. The list of all organs is `configurations/INDEX.md` (README link,
  divisions, Stop List rows, routes: the perspectives as named - `default` stays `default`, never "-" - plus
  `auxiliary` when a division uses it; what they carry is the organ README's job, e.g. Rural Organ: default =
  front, auxiliary = rear; a Markdown table cannot be sorted by a click, so the header's ▲▼ are links to the
  pre-sorted copies `INDEX-<column>-<asc|desc>.md`, all generated), written by `.github/scripts/configurations_index.py` in the
  `Configurations check` workflow (job `index`) and committed back - never a list on the website, never a
  release for configurations, never a list in `configurations/README.md` (there will be thousands). The
  website section and the release body only link to INDEX.md. Name configuration files WITHOUT spaces;
  the README's Download line is `https://zurek-jiri.github.io/OrganSampler/configurations/<name>.zip`.
- Stop List counting: every row counts - stops, stop folders, transmissions and extensions (transmissions
  are rows of their receiving division, `transmissions[]` with `manual`) - except the rows nested in a stop
  folder (`groupMaster` set). The table is NEVER typed by hand: it comes from the program, in grid order (row by
  row, left to right - the organ builder's order the user arranged) with the exact button captions (footages,
  "(Ext.)"/"(Trsm.)", a stop folder's "[III]"):
  `ORGAN_DUMP_STOPLIST=<out.md> ORGAN_LOAD_CONFIGURATION=<cfg.json> OrganSampler.exe` writes it once the load is
  over (use a non-portable configuration of this machine, a portable one would ask for the root); quit the program
  afterwards. No legend above the table. Every README looks the same. Stralsund: 53 + 18 = 71.

## Pending for the next session (delete when done)
- Measurements 0.8.141 (#44/#45, Ultra 7 155H, Caen, 7 routes, WASAPI 480 = 10 ms): #44 (16 threads, IR on 6 routes,
  78 buttons) 42 200 = the stimulus cap (61 keys x 16 strikes, callback 53 %, "first failing level 0") - the log says
  so even though the user disputes it; #45 (6 P cores, 32 buttons, no IR) broke at 39 239 with callback 95 %.
  Ryzen 5950X (#35 Doesburg, 15 routes, WASAPI 480) 53 000 at 85 %; (#40 ASIO 256) 32 200. Per voice and block in
  the run: notebook 1382 ns (kernel 1043) vs Ryzen 1922 ns (kernel 1484) at 480 frames - at 40-50 k voices both
  stream from RAM (bench 1.57 vs 1.54 ns/frame) and the notebook's DDR5 + 2 MB L2 per P core keep up with Zen 3's
  DDR4; the Ryzen also carried 15 routes (2x the fixed cost per period). A like-for-like run (same organ, routes,
  buffer) is still missing.
- Linux window icon at start-up "rozsypana" (scrambled) since 0.8.136 (`Main.cpp` sends `icon48_png` to `setIcon`).
  Before: upside down for a moment while loading but crisp. Find why the 48 px image is scrambled through JUCE's X11
  `_NET_WM_ICON` path (format/stride?) - NEVER a rescaled icon.
- Device-thread peaks (0.8.138 reports): #34 Ultra 7 155H (P+E cores): longest route callback 2616 us of which WAITING
  FOR THE LANES 2412 us - a worker on an E core (or preempted) holds the poster; try lanes on P cores only / smaller
  chunks at the end of a job. #35 Ryzen Windows: longest 1270 us of which OWN RENDER 1071 - the poster takes chunks
  while the workers are busy; cap the poster's share or batch all routes of a device into one pool job (#30: 21 routes
  x fixed cost). #29: bookkeeping 66 us per route at 44 000 voices (compaction of the active list) - speed up.
- Issue form: the "Operating system" field is a text field filled by the program (now with the version); fine.
- GitHub's issue list hides the number in its new UI (the user asked why "#27" is not in the row): nothing of ours.

## Pending for the next Linux session (delete when done)
- LESSON 0.8.147: the two
  checkouts are two git repositories on the SAME sources - after a Linux session the Windows checkout `F:` is behind
  origin and holds the Linux commits' content as uncommitted changes; `git pull` there fails ("local changes would be
  overwritten") and a commit lands on the old base (the push is rejected, a tag push is not: v0.8.147 was first built
  from such a dangling commit). The way: `git fetch`, `git reset --mixed origin/main` (HEAD moves, the files stay),
  then commit the tree anew, push, re-point the tag. Always `git fetch` + `git status -sb` BEFORE committing on the
  other system. #58 (Linux, two monitors): the KDE AMD box runs WAYLAND - the X11 way finds nothing there; the KWin
  scripting way (0.8.150, working since 0.8.151) is to be confirmed by the user's own report. The NTFS checkout is the user's (git cannot
  run there from Linux: "expected submodule path 'JUCE' not to be a symbolic link"). After each release copy the Linux
  binary to `~/Downloads/OrganSampler`.
- Windows builds: never run two MSBuilds of the solution at once - the second fails with C1041 (PDB) and can
  leave orphaned `cl.exe` processes (0 s CPU, parent gone) holding the PDB; stop them before the next build.
- The dual boot clock is settled: Windows and Linux both keep the hardware clock in local time
  (`timedatectl` says "RTC in local TZ: yes"); never set `RealTimeIsUniversal` on Windows. Windows fast startup
  is off (`powercfg /h off`), so the NTFS disks are writable from Linux.

## Compile checks on Linux without a toolchain for the whole program
- `g++ -std=c++17 -fsyntax-only -w -DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1 -DJUCE_STANDALONE_APPLICATION=1
  -DJUCE_USE_CURL=0 -DJUCE_WEB_BROWSER=0 -DJUCE_MODULE_AVAILABLE_juce_dsp=1 -DJUCE_LINUX=1 -DNDEBUG
  -I JuceLibraryCode -I ../JUCE/modules -I ../ThirdParty/signalsmith -I /tmp/stubinc Source/<file>.cpp` (run in
  `~/OrganSamplerC-linux/OrganSampler`) checks a source in seconds (SamplerEngine.cpp 4 s, MainComponent.cpp
  longer). `/tmp/stubinc/alsa/asoundlib.h` is a stub of the seven ALSA symbols PlatformAudio.h uses
  (snd_pcm_t, snd_pcm_open/close, snd_strerror, snd_device_name_hint/get_hint/free_hint, SND_PCM_STREAM_PLAYBACK,
  SND_PCM_NONBLOCK; libasound2-dev is not installed); recreate it when /tmp was cleared. Real builds come from CI.
- Never start `~/Downloads/OrganSampler` with a made-up flag such as `--version`: it opens the program (kill it).
  `--selftest "<folder>;<folder>"` is the offline test (folders separated by ';'), with `ORGAN_SELFTEST_IR=<wav>`,
  `ORGAN_IR_LIGHT=1`. `gh` is not installed on Linux: the GitHub API with the token from `git credential fill`.

## Testing
- Windows delivery: after every release build copy the exe to `C:\OrganSampler\OrganSampler_<version>.exe`
  (the local MSBuild Release build or the CI exe) and retarget the EXISTING desktop shortcut in place
  (today `OrganSampler_0.8.98.exe – zástupce.lnk`; find it by its TargetPath under `C:\OrganSampler\`; WScript.Shell
  CreateShortcut, set TargetPath + IconLocation, Save). NEVER delete, rename or recreate the shortcut: Explorer
  keeps icon positions by file name and a new file lands anywhere on the desktop. The user starts the program
  from that shortcut.
- Never build or launch the program while the user runs it. Linux test: `~/tools/linux_test.sh <exe> <seconds>`
  with `CFG=`, `ORGAN_OPEN_STOP_LIST=`, `ORGAN_JACK_BUFFER=`, `ORGAN_SAVE_PORTABLE=` hooks; the activity log is
  `~/.config/OrganSampler/CrashLogs/activity.log` (Windows: `%APPDATA%\OrganSampler\CrashLogs\activity.log`).
