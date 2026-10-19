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
  run `C:\JUCE9\Projucer.exe --resave OrganSampler\OrganSampler.jucer` and copy the .ico (above)
  (the first `--resave` after a change sometimes prints nothing and writes nothing - check `versionString` in
  `JuceLibraryCode/JuceHeader.h` and run it again until it prints "Finished saving"), commit the regenerated
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
  0.8.129 crescendo: `Combination` (a id, b name) named "cresc..." = one stage each, the stage = the number at the end
  of the name (`getTrailingIntValue`); `CombinationElement` (b CombinationID, c ControlledSwitchID, e Y = on); the
  switches reach the stops' `Stop.switchId` (d, ControllingSwitchID) through `SwitchLinkage` both ways within 8 hops
  (Doesburg 1 hop; Schwerin 4: CrescendoCombSwitch > CrescendoIfOnSwitch > ... > "C.<stop>" > the stop - a limit of
  4 iterations found nothing there, every stage came out empty); couplers unresolved -> left out.
  `HauptwerkOdf::crescendo` / `Plan.crescendo` -> the import writes the configuration key "crescendo"
  (`crescendoFromVar` format: stepCount, cancelsHandStops, steps[{step, stops[{name=row name, perspective "",
  enabled, primaryManual}], couplers[]}]); more than 60 stages spread over 60 steps. Doesburg Surround: 64 stages,
  stage 2 = Aeoline 8 + Echobass 16 + Echo-Gamba 8.
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

## Pending for the next Linux session (delete when done)
- `git pull` in `~/OrganSamplerC-linux` (the NTFS checkout `F:` is current, 0.8.109; git cannot run there from
  Linux: "expected submodule path 'JUCE' not to be a symbolic link"); copy the Linux binary of the newest release
  to `~/Downloads/OrganSampler`.
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
