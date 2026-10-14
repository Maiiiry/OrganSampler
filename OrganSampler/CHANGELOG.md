# Changelog

All notable changes to OrganSampler are recorded here. The format follows
[Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and the project uses
[Semantic Versioning](https://semver.org/): MAJOR.MINOR.PATCH, where a new
feature raises MINOR, a fix raises PATCH, and an incompatible change of saved
files or of the way the program is used raises MAJOR.

The version lives in one place per artefact and must be changed together:
`OrganSampler.jucer` (`version`), `JuceLibraryCode/JuceHeader.h`
(`ProjectInfo::versionString`, `versionNumber`), `Builds/VisualStudio2026/resources.rc`
and the `JUCE_APP_VERSION` defines in `OrganSampler_App.vcxproj`. The window
title shows the version. Released executables are named `OrganSampler_<version>.exe`.

## [0.8.71] - 2026-10-14

### Changed
- Pictures: the inside of the mouth cut-out is dark throughout, up to the
  rim's inner line on both sides and up to the apex - no reflections inside
  the mouth. All six pictures and the two largest icons.

## [0.8.70] - 2026-10-14

### Changed
- Pictures: the inside of the mouth cut-out gets no light - the highlight on
  its left inner wall is gone (the wall mirrors the dark right one); the lower
  lip is a flat sheet with only a thin soldered seam on its rounded edge, no
  inward bevel. The flame-shaped lips are built from this base too.

## [0.8.69] - 2026-10-14

### Changed
- Pictures: the upper lip's bevelled rim ends at the top edge of the mouth
  slit; beside the slit the cut cylinder continues bare, with a sharp edge and
  no soldered rim (nothing is soldered around the mouth). The flame-shaped
  lips have their rim drawn in one piece down to the slit, without the seam
  at the shoulders of 0.8.68.

## [0.8.68] - 2026-10-14

### Changed
- Pictures, made anew once more: the upper lip's bevelled rim ends exactly at
  the bottom edge of the mouth slit; below it the lower lip is a raised plate
  with its own edge, laid over the smooth cone of the foot. Six pictures at
  random on the website: the three upper lips as before and three
  flame-shaped ones with the rim's own bevel and the recess's shading, the pipe
  body straight behind them. The two largest icons follow the main picture.

## [0.8.67] - 2026-10-14

### Changed
- Pictures: the three upper lips as they were (the pointed arch and the two
  variants), every foot one smooth cone from the mouth slit to the soldered
  tip, and the raised lower lip with its rounded rim back over the cone, with
  its shadow. The flame-shaped lips of 0.8.65 and 0.8.66 are withdrawn: their
  rims and shading were not photorealistic. The two largest icons follow the
  main picture.

## [0.8.66] - 2026-10-14

### Changed
- Pictures and the two largest icons, made anew: the foot of every pipe is one
  smooth cone from the bottom edge of the mouth slit down to the soldered tip,
  without any edge on it (0.8.65 had narrowed the foot but left the old rim
  and its shadow on the cone, on the middle pipe only). The flame-shaped lips
  no longer show the old pointed lip through them; the pipe body behind every
  lip is straight.

## [0.8.65] - 2026-10-14

### Changed
- Pictures: the middle pipe's foot now starts at the bottom edge of the mouth
  cut-out (the top edge of the lower lip), as a pipe is built; the cylinder
  no longer runs on below the mouth. Done in every website picture and in the
  two largest icons (1024 and 256), which are now the website picture on its
  dark background with rounded corners; the small icons are untouched.
- Website: six pictures at random - three upper lips as before and three
  flame-shaped ones (leaning left, tall and slender, leaning right), the pipe
  body straight in all of them.
- Linux audio tab: the button in the place of "Driver Control Panel" is one
  latching button, "List unusable devices too" / "Unusable devices listed -
  hide them", pressed while the devices that cannot be opened are listed. It
  shows the one shared state on every tab and device type, and rescans at
  once; the checkbox drawn over it in 0.8.64 is gone. Pressed, JACK lists
  every client too.

## [0.8.64] - 2026-10-14

### Changed
- Linux, ALSA: a card that PipeWire holds for one of this program's own JACK
  routes (the probe says "busy" because of us) stays in the list - the
  Komplete Audio 6 vanished from ALSA while a JACK route played through it.
- Linux: the audio tab has the switch "List unusable devices too" where
  Windows has the driver control panel button; it rescans at once. The
  switch in the settings editor stays.
- Website: the pipes picture is one of four at random on every visit - the
  middle pipe's upper lip differs, the fourth is a flame-shaped lip in the
  manner of Tangermuende.

## [0.8.63] - 2026-10-14

### Fixed
- Linux, JACK: a buffer size or rate chosen in the audio tab took five
  seconds (a probe that never saw the new clock) and the client was opened
  twice too early. The client is now opened and its buffer size awaited (a
  fresh client learns it a moment after activation); one that got the old
  size is opened again. Measured: 512 frames on two JACK routes, the other
  route reopened by itself, no stall.

## [0.8.62] - 2026-10-14

### Added
- The settings of a feature by right-click where the feature sits: the swell
  bar's menu has "Swell settings..." (maximum attenuation, shutter inertia,
  harmonic filter), the Tremulant button's menu "Tremulant settings..."
  (rate, depth, drift, volume per cent, harmonic filter and phase), and a
  right-click on a division header's background offers the swell, tremulant
  and wind settings of the division, the Divisions dialog and the
  Configuration Editor. Small dialogs with the same rows as the editor, in
  effect at once; the editor is rebuilt when such a dialog closes.

## [0.8.61] - 2026-10-14

### Fixed
- Linux, JACK through PipeWire: the buffer size and sample rate are chosen in
  the usual boxes of the audio tab (the extra "PipeWire buffer / rate" row of
  0.8.59 is gone). The choice is handed to PipeWire, the program waits until
  the server runs the new clock and opens the route only then; the other JACK
  routes reopen at the new size by themselves. In 0.8.59 the route was
  reopened while PipeWire was still changing its quantum: the client got a
  buffer of 1 frame and millions of empty callbacks a second, which stalled
  the whole computer. A client that reports such a buffer is now thrown away
  and tried again, and an empty callback returns at once.
- Linux, ALSA: instead of hiding every card device while a sound server
  runs, each device is tried when the list is made and only those that
  cannot be opened (held by PipeWire or another program) are left out. A
  card's S/PDIF or HDMI entry that opens with all its channels stays listed.

## [0.8.60] - 2026-10-14

### Fixed
- The Stop List took many seconds to open with the samples on an NTFS disk
  under Linux: the folder scan asked the size and times of every sample file
  (30 000 files, a millisecond each on the FUSE mount), three times over. The
  scan now lists directories and names only; the same scan runs when an organ
  loads.

## [0.8.59] - 2026-10-14

### Added
- Linux, JACK through PipeWire: the audio tab of a JACK route has the row
  "PipeWire buffer / rate": PipeWire's buffer size (quantum) and sample rate,
  the same for every JACK route and every other program; a choice sets them
  with pw-metadata and reopens the JACK devices of all routes.
- Linux: the settings editor's "Audio devices: list also the ALSA devices of
  the cards the sound server holds" (off by default, see Fixed).
- The activity log notes how long the Stop List took to prepare and to show;
  ORGAN_OPEN_STOP_LIST=<seconds> opens it that long after the start.

### Fixed
- Linux, ALSA: the cards' own devices ("Direct hardware device", "IEC958",
  HDMI), which PipeWire holds and which therefore gave no channels or 'none',
  are no longer listed (the settings editor can list them). "PipeWire Sound
  Server" and "Default ALSA Output" offer their two real channels, not 64.
- A device chosen for a route no longer jumps to 44.1 kHz: the route keeps
  the rate it ran at when the new device offers it. "Set for all" copies the
  rate the device really runs at (the manual says so now).
- Configurations saved on Linux or macOS: the library root was computed per
  "drive" with the Windows rule, so every tree was a drive of its own and the
  root became the parent of one tree; the other trees were written relative
  to that wrong root and their stops were gone after a reload (pedal stops of
  the Stop List not on the grid, Apply dropping more). One root for all trees.
- Folder names from a configuration written on Windows are found on Linux
  and macOS regardless of case ("Ped_octave4" against "Ped_Octave4").
- Website: the ring on each pipe foot sits near the tip.

## [0.8.58] - 2026-10-13

### Fixed
- Linux: the menu entry written by "Install menu entry and icon" had Windows
  line endings, which makes a .desktop file invalid - KDE and GNOME ignored
  it. Written with Unix line endings now, the KDE menu cache rebuilt, and the
  message says where the entry is (the application menu, type "Organ";
  Multimedia group).

## [0.8.57] - 2026-10-13

### Changed
- Manual, "Audio on Linux": the "Pro Audio" profile of a sound card (system
  settings) hands every channel of the card to the program as a port of its
  own, unmixed, under the card's name - the setting to make for the interface
  the organ plays through.

## [0.8.56] - 2026-10-13

### Fixed
- Linux: the JACK device list showed PipeWire's MIDI bridges and MIDI devices
  ("Midi-Bridge", "BLE MIDI 1") as if they were sound devices - a route moved
  there ran with a buffer of one frame. Only clients with audio ports are
  listed.
- Linux and macOS: the icon in every size is the hand-tuned image of the
  Windows icon (16-256 px from the .ico, 512 from the original), not a
  downscale; the menu entry, the dock and the macOS bundle use them.
- The activity log lists every audio device type with its devices at start.
- A route with every output channel unticked went on playing through
  channels 1-2 (the engine fell back to them). No channel is none: the route
  is silent.

## [0.8.55] - 2026-10-13

### Fixed
- No configuration carries an absolute path where a relative one exists any
  more - portable or not. The pool of folder trees is stored relative to the
  library root ("Samply", "003136/pipe/HW"), the IRs by their relative paths,
  and both are resolved against the configuration file on load (a tree
  outside the library root, on another disk, keeps its absolute path and is
  tried last). Before, the pool and the IRs kept the paths of the computer
  that saved the file; on Linux and macOS those are no paths at all, so a
  configuration from Windows loaded with an empty pool - the Rural Organ
  package found no stop. The package and the published configurations are
  rebuilt.

## [0.8.53] - 2026-10-13

### Fixed
- Relative paths inside a configuration (the trees below the library root,
  the stop folders of the tables, the IRs) were written with "\" and did not
  resolve on Linux and macOS, where "\" is an ordinary character - a
  configuration from Windows, the Rural Organ package included, found its
  tree but none of the folders in it. Paths are now stored with "/" on every
  platform and normalised when read. The Rural Organ package is rebuilt (its
  zip, made on Windows, had "\" in the entry names as well and unpacked flat
  on Linux).
- Linux: the ALSA device list was drowned in the templates ALSA advertises
  for every card (surround 4.0 ... 7.1, front, rear, dmix), none of which can
  be opened while PipeWire holds the card; they are hidden now.

### Added
- "Replace ... with a folder on this computer..." on the missing-tree notice
  of a division header, and "Replace ... with another folder..." in the Stop
  List's Unload folder menu: a tree is taken from another folder whatever
  its name; its rows keep their folders and the divisions reload.
- Linux: JACK through PipeWire. The program loads PipeWire's JACK library at
  start, so the JACK device type lists every output of every card with no
  separate server; the manual's "Audio on Linux" explains the setup.
- The window carries the program's icon on every platform (Linux title bar
  and task bar, macOS dock), and the Configuration Editor's "Install menu
  entry and icon" (Linux) writes a .desktop entry and the icon in every size
  under ~/.local/share.

## [0.8.52] - 2026-10-13

### Fixed
- A portable configuration dropped the auxiliary routes (and their rear
  reverbs) - wrong: every route that has a device, the auxiliary ones
  included, now plays through the default sound device of the computer that
  loads it, with its reverb; only a route that was none stays none. The
  route states say `deviceType="default"`, which stands for the computer's
  own driver type (Windows Audio, CoreAudio, ALSA), so the same file works
  on every platform. The Rural Organ package is rebuilt with it.

## [0.8.51] - 2026-10-13

### Added
- "For another computer (sharing)" in the Save Configuration box: the
  configuration carries the organ only - no MIDI devices, no audio devices
  or channels (every route on channels 1-2, the auxiliary routes none), no
  render-thread, interface-size, window, cache or measured-polyphony
  settings, no unused folder trees. Loaded elsewhere it plays through the
  default sound device, keeps that computer's own interface and thread
  settings, and a message says what to set up (MIDI, audio tabs, measure,
  save). Published configurations are saved this way.
- Rural Organ: a small demo organ (dry recordings of three divisions with IR
  reverb) with its sample folders and a shared configuration, downloadable
  from the website - the only package that ever carries samples.
- An IR inside the sample library is also stored relative to the library
  root, so it is found wherever the library is (a shared package).

### Changed
- "Aliases from folders" writes the alias with a capital first letter
  ("Fletna 8'"): the labels of a grid look even that way.
- Every build on GitHub is marked pre-release (for testers) until the first
  release the project is satisfied with.

## [0.8.50] - 2026-10-13

### Added
- "Report a problem..." in the Configuration Editor: a dialog for what
  happened and how to reproduce it; the program bundles its activity log,
  crash reports, settings, the last self-test and a description of the
  computer and the audio routes into logs.zip, takes screenshots of its own
  windows (and takes screenshot files of yours), puts everything into a
  folder on the Desktop and opens the GitHub bug report with the fields
  filled in. The report scripts of 0.8.49 are gone.
- "Aliases from folders" in the Stop List: every stop without an alias gets
  the bare name of its sample folder - no parent folders ("TA - fletna"
  becomes "fletna 8'"), no perspective prefix, the footage with its foot
  mark (a mixture's first sounding harmonic as the program measures it).
  Aliases typed by hand stay.

### Changed
- Wording everywhere (manual, program, website, repository): "sample
  folders" and "folder trees" instead of "sample set"; no samples are ever
  part of the program or the repository, they are bought or downloaded
  elsewhere. The configurations folder of the repository takes
  configurations only - a check refuses samples.

## [0.8.49] - 2026-10-13

### Fixed
- A configuration whose sample folder trees are not on this computer (saved
  elsewhere, a drive not plugged in) is never refused any more. Up to 0.8.48
  a folder dialog asked for "the sample-library root" without saying which
  folders were missing, and Cancel dropped the whole configuration. Now the
  configuration loads with the trees that are here; every division that uses
  a missing tree shows an orange notice under its header ("Sample folder not
  found: 003136", the paths and the way out in its tooltip; a click opens the
  Stop List). In the Stop List, "Load folder..." with a folder of the same
  name puts it in the missing tree's place - the stops keep their samples -
  and "Unload folder..." removes the tree with its stops. The notice goes
  when no tree of the division is missing; the activity log lists the
  missing trees at load.

## [0.8.48] - 2026-10-13

### Fixed
- Aggregated WASAPI played through the first two channels of the list
  whatever was ticked: the device opened its first endpoint on its own, and
  only some paths replaced that with the configured channels (the auxiliary
  routes never did when a configuration loaded, so they were silent and the
  organ sounded dry - their IRs never played). The device now opens no
  endpoint by itself; the route's configured channels open exactly their
  endpoints the moment the device starts, on every path (loading, "Set for
  all", a switch of the type, a reopen).
- Unticking every channel of an Aggregated WASAPI route re-ticked the first
  two a second later (the list was rebuilt after each change and an empty
  selection was filled with the device's first pair). An empty selection the
  user made now stays empty: no channel is none, on any device.
- An endpoint the aggregate cannot open (an unplugged interface, a digital
  output without a cable) was silently accepted, so the result depended on
  which endpoint failed. It is refused now: its channels are set to none,
  the list shows them unticked with the reason after their names, and the
  activity log names the refused endpoints and what each aggregated route
  plays through - the same outcome as the Windows Audio type, which refuses
  such a device.
- The summary at the top of the "Measuring the maximum polyphony" window
  overflowed with many routes on many devices. The routes are grouped by
  driver type ("Windows Audio: 2 on "A", 1 on "B"") and the window is sized
  to the text.
- The measurement counted the bursts of a shared-mode stream opened below
  the Windows audio engine's 10 ms packet (an aggregate at 256 gets two
  blocks, then a 20 ms wait, even idle) as late driver callbacks. The late
  rule and the health log's "max gap" count from the packet for non-ASIO
  devices.

### Changed
- Aggregated WASAPI defaults to a buffer of 480 samples (the engine's
  packet; 480 is in the list now). The dry Tábor set, ten aggregated routes
  with 4-channel IRs, measured 300 at 256 (an IR's tail FFT alone exceeds a
  5 ms block) and 34 700 at 512 on the same machine. The report says so
  when aggregated routes run below the packet.
- The report's advice names the aggregated routes in one paragraph and the
  auxiliary routes in another, with their loads, instead of one paragraph
  per route.
- Manual: why an organ spread over several devices can measure more than
  the same organ on one ASIO driver (every device has a driver thread; the
  unshared part of each callback runs on it), that channels on one device
  make no difference, the spread of the measurement between runs, and when
  to raise "Render threads for Aggregated WASAPI" (one or two aggregated
  routes carrying the whole organ; with many such routes 1 is right).

## [0.8.47] - 2026-10-13

### Changed
- Windows Audio like ASIO: all routes on one Windows Audio device share one
  stream and one callback thread, rendered by the whole pool of render
  threads. Before, every route opened its own stream with its own thread: a
  division on one route was bound to a single core (a dry set with 108 stops
  on one division broke up at 2 000 voices whatever the thread setting), and
  with more threads per route ten device threads starved against the
  workers. Routes without a saved device share the system's default output
  instead of opening it once each. The setting "Render threads for Windows
  Audio / WASAPI" is now "Render threads for Aggregated WASAPI" and applies
  to that virtual device only. The divisions' private device managers start
  without a device; a division's own route joins its shared host when the
  settings load.
- The IR reverb convolutions run on the render workers, one block behind the
  dry signal: the callback hands a block's dry output to a worker and mixes
  the wet signal of the previous block. The convolutions of all routes thus
  run in parallel instead of one after another on the callback thread (ten
  routes with a 4-channel 7 s IR cost 3 ms of a 5.3 ms period that way, with
  tail-FFT bursts far beyond it). The block of delay is part of the pre-delay
  (the set pre-delay is reduced by it). Measured on the dry Tábor set, ten
  routes with IRs on Windows Audio: clean polyphony 300 before (one stream
  and one thread per route, serial convolutions) and 10 000 after.
- The measurement gives a level one more chance after a single late driver
  callback while every callback stayed within its period (such gaps come from
  the system as often as not); a second one counts. The progress window and
  the report start with what is under test: the stops, the routes by device
  (type and name) and how many routes carry an IR.

## [0.8.46] - 2026-10-13

### Changed
- The polyphony ceiling is 300 000 voices (was 100 000): it only sizes the
  voice pool and the routes' rings at start-up (about 1.4 KB per voice,
  allocated once), nothing per block grows with it, so a far faster computer
  may use all of it. The maximum polyphony slider goes up to it; the
  measurement puts the limit there for the run.
- The measurement adds keys in proportion to the shortfall (at least one, at
  most half as many again) instead of half as many again at every step, so a
  target is no longer overshot by thousands of voices; the report says what
  ended the climb (a late driver callback, a callback over its period, or the
  four-second hold over 85 %) and why the DSP field may read lower at that
  moment. The activity log notes the voice pool's size at start.

## [0.8.45] - 2026-10-12

### Changed
- Polyphony: the note bookkeeping (MIDI commands, attacks and releases,
  reclaiming finished voices) runs on a control thread of its own instead of
  inside the audio callback, where it took up to a third of the period and
  made single callbacks run over the period while the render was light. The
  audio callback now only renders. Measured on the user's Stralsund (12 ASIO
  routes, buffer 256): clean up to 27 000 voices instead of 20 600, the
  callback at 19 000 voices at 55 % of the period instead of 74 %. The
  bookkeeping holds the control lock for at most 2 ms at a time and wakes
  within a millisecond of a MIDI command or a finished voice.
  `ORGAN_CONTROL_ON_AUDIO=1` keeps the old way for comparison; the self-test
  never starts the thread (its renders expect synchronous bookkeeping).
- The polyphony measurement engages the octave couplers (16' and 4') of every
  division onto itself after all 61 keys, before raising the rate - whether
  they are on the grid or not - and names the heaviest single route with its
  IR in the report (on a dry set the convolutions are the main cost).

## [0.8.44] - 2026-10-12

### Added
- "Measure the maximum polyphony" (Configuration Editor, below *Maximum
  polyphony*): with the loaded organ and its configured audio outputs, every
  stop engaged and the keys of every division struck repeatedly, the limit is
  raised step by step (5 000 ... 100 000 voices, six seconds each) until a
  block exceeds 85 % of its time or an audio callback comes late; the
  maximum polyphony is then set to three quarters of the highest clean limit
  (or to the highest clean limit when the organ never reached it), the
  registration is restored and a report lists every step with advice about
  auxiliary routes over their budget and the ASIO buffer. Nothing is heard
  during the run (rendered, not sent). Greyed out without a sample set: the
  reachable polyphony depends on the number of audio routes, the auxiliary
  routes, the IRs, the drivers and buffers, so it is a value of the organ,
  saved with its configuration. Manual: "Getting the most polyphony",
  "Maximum polyphony", "Measure the maximum polyphony".
- The measurement is a regulator, not a list of limits: the limit is out of
  the way (100 000) and the stimulus grows - one key struck twice a second
  on every division first, then more keys whenever the voices stop rising
  below the target (up to all 61), then a faster rate (up to 16 a second) -
  so a wet set reaches its ceiling with a few keys and a dry set with every
  key hammered. Targets grow by 2 000 voices (a tenth above 20 000), each
  held four seconds and judged by the whole driver callback of the busiest
  device (a device clock: the first route of each driver callback marks when
  the driver really called; render, note bookkeeping and copies together
  within 85 % of the period; a callback more than half a period behind is a
  late one), by every single route's block, and by late driver callbacks.
  The first breakup ends the climb; the run narrows down between the clean
  and the failing number of keys twice, letting the tails die away in
  between. The maximum polyphony is set to the most voices that stayed
  clean; the slider is for adjusting it afterwards. The attack minimum time
  and delay are bypassed for the run, the sound plays at -42 dB from the
  first strike and glides back over two seconds after the report's OK.
- The *DSP* field shows the whole driver callback of the busiest device
  (render, note bookkeeping, copies), not the render alone: with a dozen
  routes on one ASIO driver the render alone said 30 % while the callback
  was at 75 %. The note bookkeeping's budget per period is smaller (65 % of
  the period minus the render, at most 35 %) and its deadline is checked
  after every MIDI command, so a callback no longer runs over the period
  while the render is light.
- Development: `ORGAN_MEASURE_POLYPHONY=1` runs the measurement once the
  instrument is loaded; the audio-health statistics count late callbacks
  (gaps beyond three periods, the rule of the "max gap" anomaly);
  `ORGAN_STRESS_NO_AUX=2` sets the auxiliary routes to none the device-list
  way (`stress_test.ps1 -NoAuxDirect`).

### Changed
- Nothing stale in the configuration, no hidden routes. A division's own
  ("default") route that no stop plays through has no audio tab - it used to
  keep a device open anyway (the Windows default device: six WASAPI streams
  in the user's configuration), load its IR and render in silence. Now its
  device is closed and remembered as none, and its settings are dropped; the
  same for every route of a division beyond the division count. A route that
  no longer exists loses its settings, an auxiliary route without a device
  keeps none (no IR of an old set waiting), and the saved configuration
  carries only the divisions in use and the routes that exist. A loaded file
  resets the divisions it does not carry. The measurement's report is short:
  the result, and advice naming every route on Windows Audio or auxiliary
  route with a device and the share of its period it needed.

### Fixed
- A route set to *none* directly in the device list was saved with its old
  device again since 0.8.43 (that fix only covered "set for all"): the
  remembered state is now written at once whichever way none is chosen, and
  a save without an open device takes the device manager's state only when
  it says "no device" itself. None is none, by any route.

## [0.8.43] - 2026-10-12

### Fixed
- An auxiliary route set to *none* (directly or by "Set for the auxiliary
  routes of all divisions") got its previous device back within seconds. The
  save that follows the change took the route's state from the JUCE device
  manager, which still remembered the closed device, so the remembered state
  said "that device" again and the missing-device watcher reopened it
  ("is available again: reopening ..."). A route without an open device now
  keeps its remembered state (*none*, or the saved device that is not
  available yet) when the settings are saved.

## [0.8.42] - 2026-10-12

### Fixed
- Dropouts while many keys are struck at once ("bashing the keys" on a
  tutti) although the render load was far below 100 %: the note bookkeeping
  (attacks and releases of a strike, thousands of voices) ran once in every
  route callback of the ASIO device, and with a dozen routes (every division
  and perspective) a dozen bookkeeping slices of 1.5 ms each landed in one
  5.3 ms period - the device thread fell 18-30 ms behind. The bookkeeping
  now runs once per device period, with a budget that adapts to the render
  load (what the period leaves free, 20 % kept in reserve). Measured on a
  16-core machine at buffer 256: a bashed tutti of 25 000 voices now plays
  without a single late callback (before: late callbacks every second from
  about 12 000 voices).
- At the polyphony limit, finding a voice to steal scanned the voice pool
  (every step a cache miss) for every new voice; a bashed tutti at the limit
  cost up to 5 ms per key. The scan now reads a compact flag array and stops
  after 1024 steps (nothing left to steal: the start goes ahead, the stolen
  voices free their slots within a few blocks anyway).

### Added
- The stress hook logs the longest rendered block against the block's
  budget (what decides about dropouts) and the route it belonged to, the
  control work per second (ticks, total time, voices started, the longest
  reclaim / MIDI / attack / stale-route phase) and the load of every device
  thread; the audio-health log says how long the longest callback took and
  how much of the longest gap the device thread actually ran.
  `ORGAN_STRESS_BASH=1` strikes every key again every 250 ms instead of
  holding it, `ORGAN_STRESS_THREADS=<n>` sets the render thread count for the
  run, `ORGAN_STRESS_NO_AUX=1` detaches the auxiliary routes for the run.
- Manual: the auxiliary route on a Windows Audio device needs more than one
  render thread at a tutti ("Render threads for Windows Audio / WASAPI").

## [0.8.41] - 2026-10-12

### Changed
- Polyphony: the harmonic filter (the high shelf that follows the swell,
  the wind sag and the tremulant) is no longer computed for every voice.
  Every voice only splits its signal into the share to filter and the rest;
  the route low-passes the sum once per channel. The filter is linear, so
  the sound is the same; the cost of a tutti with the filter active fell by
  a third (measured: 1.14 to 0.78 ms per block for 6832 voices), close to
  the cost with the filter off.
- The level and treble compensation of a filled pipe transposed upwards
  (-0.1 dB per semitone) now applies to plain resampling only; the copies
  of the pitch-shift methods have their loudness and timbre settled when
  they are built.
- Folder-name patterns (ignored folders, one-shot stops) understand a star:
  "*_tr" matches names ending in "_tr", "dir-*" names starting with "dir-";
  without a star the pattern matches anywhere, as before. Underscores now
  count. "*_tr" joins the default ignored patterns (a saved list that was
  still the old default "trem" takes the new default).
- The LIMIT box of the polyphony display gives the caret away on Enter.
- Manual: the first-start steps are sections ("Step 1: Size" ... "Step 7:
  Save") so that references to them are links; the hint about the empty
  window moved to the introduction; output channels are ticked in the list
  of the audio tab (no channel numbers in the text); the upward-fill
  compensation explained with the resampling method; the pattern stars;
  MIDI learn from the keyboard drawing; the measured cost of hyper-threads
  under "Render threads".

### Added
- `ORGAN_SELFTEST_BENCH=1` with `--selftest`: a polyphony benchmark only
  (every stop, every key, ms per 1000 voices, with and without the harmonic
  filter), for comparing builds and settings.
- `ORGAN_STRESS=<seconds>` (development): once the instrument is loaded,
  every stop is engaged and every key of every division held; the polyphony
  and the render load go to the activity log every second - the real
  instrument on the real audio devices. `ORGAN_STRESS_LIMIT` lifts the
  polyphony limit for the run, `ORGAN_STRESS_NO_IR=1` removes the IRs.

## [0.8.40] - 2026-10-12

### Fixed
- The empty-instrument hint sits on a panel of the window's background, so
  the divider between two division columns no longer runs through its text.

## [0.8.39] - 2026-10-12

### Added
- An empty instrument (no folder tree, no stop) shows a hint in the stop
  workspace, "Need help designing your pipe organ?", with a button that
  opens the user manual at "Getting started"; it disappears as soon as a
  folder or a configuration is loaded.

### Changed
- Manual, "What you need": a pre-saved organ configuration saves all the
  building work - load it and the organ is ready, improve it as you like.

## [0.8.38] - 2026-10-12

### Changed
- Manual: do not chase the smallest audio buffer - about 10 ms (512 samples
  at 48 kHz) is absolutely fine for organ playing, even 20 ms is still
  comfortable, and small buffers cut the reachable polyphony dramatically
  (in "Getting the most polyphony", "Maximum polyphony" and the first start).

## [0.8.37] - 2026-10-12

### Fixed
- Crescendo with "may switch off hand-registered stops" on: every step
  switched all stops off and the step's stops on again, so a held chord
  restarted at every step. Now only the difference between the steps is
  switched; the stops, transmissions and couplers the steps have in common
  keep sounding.

## [0.8.36] - 2026-10-12

### Fixed
- A stop folder without pipes of its own made its division open a "default"
  audio route (and show a "default" audio tab) although no folder without a
  perspective prefix existed. A stop folder now adds no perspective; the
  routes of a division come from its sounding stops only.

## [0.8.35] - 2026-10-12

### Fixed
- "New stop here" (a free cell of the grid) and *Add stop...* (Stop List)
  offered every ready-made stop the division did not have, including stops
  already used by another division - adding one made a second copy of the
  same folders. Now only stops that no division uses (none of their folders
  is in any row of the table) are offered.

## [0.8.34] - 2026-10-12

### Fixed
- A copy dragged in arrange mode (right button or Shift: a new extension or
  transmission) could not be dropped onto a stop folder's orange bar: the
  bar lit up, but the drop ignored it and the copy was left where it fell.
  Now the new transmission is created straight under that stop folder, in
  the folder's division, with the transposition of the dragged item.

## [0.8.33] - 2026-10-12

### Changed
- Transmission captions: "(Ext.)" for a transmission in the stop's own
  division (an extension), "(Trsm.)" for one in another division, at any
  footage. An alias now carries the mark too ("Gedekt 8' (Trsm.)"); before,
  an aliased transmission showed no mark at all. The Divisions dialog's
  stop list says "(<stop> ext.)" / "(<stop> trsm.)" accordingly.

## [0.8.32] - 2026-10-12

### Changed
- Voicing: *Attack Decay* is gone; *Attack Time* (0-500 ms, default 150)
  now covers the whole attack envelope - the attack gain holds for the
  first half and fades back over the second. Saved voicings keep their
  values (hold + decay become one time).
- *Filled pipes: transposition method* defaults to *Pitch shift + time
  stretch*; the phase vocoder stays as an option.
- Manual: a full language review; the sample folder chapter as rewritten by
  the author (nested stops and perspectives, the stop label from the folder
  path, the smpl chunk fields with the rounding-down rule of the unity
  note); what the velocity does on a stop without layers (the attack
  softened in volume and colour); a pipe held through several routes takes
  the loudest volume and the highest treble setting and keeps the tuning of
  the key that started it; "engage/disengage" throughout.

## [0.8.31] - 2026-10-11

### Changed
- Manual, *Maximum polyphony*: set it so that DSP never exceeds 100 %, and
  the procedure to find the value (many stops with long releases, bashing
  the keys - palm smearing - for the most releases at once, watch DSP and
  PEAK, enter a little less than the PEAK at which the audio broke up,
  repeat).

## [0.8.30] - 2026-10-11

### Added
- Manual window: a *Back* button returns to the place read before the last
  jump (a link, a chapter, a "?"); the chapter box and the contents show
  the outline with the sections indented under their parts.

### Changed
- Manual: the sections numbered as an outline (1.1, 1.2 ... 8.6) and the
  contents list the whole outline, every entry a link; every reference in
  the text points to the exact section (no more "see part N"); a chapter
  "Sample channels and output channels" (mono and stereo samples spread
  over the route's channels by key number, pairs with an even count,
  circulating pairs with an odd one); a full sample folder tree with
  perspectives, release folders and a one-shot stop; the smpl chunk fields
  the program reads (MIDI unity note, pitch fraction, loops); "Velocity
  layers" as a section of its own; the first-start steps reordered (samples
  before audio, as the perspectives exist only then), the divisions box in
  the main window's corner, resizing the window; "Audio devices not ready
  when OrganSampler starts"; "How the sample folders must be organised".

## [0.8.29] - 2026-10-11

### Added
- Hyperlinks in the manual: a chapter named in the text (in quotes or in
  emphasis), "part N" and the contents list are links - a click jumps to the
  chapter; the mouse shows a hand over them.

### Changed
- Manual: the phase vocoder often breaks the loops (the loop crossfade is
  the remedy), and the other methods can worsen a loop's seam too.

## [0.8.28] - 2026-10-11

### Added
- *MIDI transposition* per division (Divisions dialog, last line of the MIDI
  column; Configuration Editor division section; -24 to +24 semitones,
  default 0):
  the keys of the division's MIDI keyboard play that many semitones away.
- Search in the About dialog: a *Find* box above the text (Enter or *Next*
  for the next occurrence, *Previous* for the one before), for the manual
  and the licences alike.

### Changed
- Manual: the auxiliary route explained (a mix of everything, for
  monitoring, irreplaceable as a fake Rear with a different IR on dry sets,
  halves the polyphony - *none* for normal use); a chapter "Audio drivers:
  many sound cards at once" (ASIO, WASAPI, Aggregated WASAPI mixed in one
  organ, and the price in polyphony and latency); the polyphony chapter in
  plain words; folder names without footage, files without smpl, missing
  pipes and loops, a release folder tree and stops without releases; MIDI
  transposition; temperaments (how the base and the target are chosen, the
  automatic base, what a1 = 440 Hz is for); plain resampling plays at once
  while the copies are built; the cache and its three buttons in detail;
  "engage/disengage" for stop buttons; "Audio devices that are not there
  yet" moved to the audio chapter.

## [0.8.27] - 2026-10-11

### Fixed
- Choosing a chapter in the manual's chapter box did not scroll to it (the
  view was moved before the text had its new height).

### Changed
- The manual in the About dialog is laid out for reading: a proportional
  font, headings bold and larger, paragraphs and list items wrapped at the
  window's width with a blank line after each; the licences keep their
  monospaced text.
- Manual text: Sonus Paradisi sets work as they are, other sets follow the
  conventions; round-robin variants (any further name part) and velocity
  layers (the last "_NNN") combine; "rel" counts as "rel99999"; perspective
  prefixes of one perspective on one line, the first entry naming it; the
  audio routes and drivers explained (ASIO for everything on Windows, the
  costs of mixing ASIO and WASAPI); a configuration is the counterpart of
  an organ definition file and none is needed to start - the sample trees
  should live on one disk for relative paths; MIDI learn of a keyboard
  from the keyboard icon in the division header; Shut down PC never asks.
- The tooltip of *Shut down PC* said "after a confirmation": there is none.

## [0.8.26] - 2026-10-11

### Fixed
- The "?" buttons opened the manual at its start: the window was scrolled
  before it had a size. Now the chapter is at the top of the text and its
  heading is painted orange.

### Changed
- The user manual is the first entry of *About & licences...* (now "About
  OrganSampler, user manual & licences"), with a chapter box above the text
  that jumps to any chapter. The "?" buttons open that dialog.

## [0.8.25] - 2026-10-11

### Changed
- New defaults: *Filled pipes: release played from* 140 ms (was 50),
  *Filled pipes: time stretch of the release* 55 % (was 70). Saved values
  are kept.
- Every option of the Configuration Editor has a "?" button, and each
  opens the user manual directly at the chapter of that option (never at
  the start of the manual or at a general section).
- The user manual rewritten as a manual: getting started, the main window,
  the stops and their voicing, the Stop List, the Divisions dialog, the
  playing aids, filled pipes and the transposition methods (with the
  weaknesses of each: the phase vocoder slightly wobbles the pitch
  upwards, the time stretch frays the attack downwards), and a reference
  chapter per Configuration Editor option: what it is for, what the values
  do, notes. Developer notes and measurements left out.

## [0.8.24] - 2026-10-11

### Changed
- Formants beyond an octave: every further octave in either direction
  adds 15 points with the resampling methods and 25 with the phase
  vocoder (0.8.23 had taken points away downwards - the wrong way round).

## [0.8.23] - 2026-10-11

### Changed
- Formants: the stop's value holds for shifts up to an octave; beyond it
  every further octave adds points (the phase vocoder is excellent within
  an octave and lost level two octaves away).
- The attack fade-out and release fade-in grow 1.5 times per octave below
  the sounding c1 (was 1.3); a filled pipe counts as the pipe it stands
  for.

## [0.8.22] - 2026-10-11

### Changed
- The copies of a stop's filled pipes are built on every core at once (one
  pipe per worker); before, the whole organ was built one pipe after the
  other on a single core.
- *Filled pipes: release played from* (was "release trimmed at the
  start") and the new *release level matched to the source* (off by
  default) act at playback: changing them rebuilds nothing. The level
  gain is only measured when a copy is built and kept with it.
- The methods renamed: *Resampling*, *Pitch shift (resampling + timbre
  correction)*, *Pitch shift + time stretch (attack and release)*, *Phase
  vocoder (Signalsmith Stretch)*; the sliders *time stretch of the
  attack* / *of the release*.

### Removed
- The Rubber Band method and library: minutes per stop for a result no
  better than the others.

## [0.8.21] - 2026-10-11

### Added
- *Filled pipes: release trimmed at the start* (0-300 ms, default 50):
  every pitch-shift method cuts the first milliseconds of a release copy,
  where the methods leave their worst (a stop transient that no longer
  belongs, a level that does not follow the sustain).
- "?" buttons beside the filled-pipe settings open the user manual -
  embedded in the executable, the program stays one file - at the new
  section "Filled pipes: the transposition methods", which describes every
  method with its weakness.
- The self-test judges every method on a pipe above and on one an octave
  below the compass (a transmission shifted -12, the 32' extension of a
  16' stop) and measures the level step at the key release - the
  sustain's last 50 ms against the release's first 50 ms - which must be
  what the real source pipe shows (within 4 dB).

### Fixed
- A filled pipe's release started at its own level - up to 6 dB away from
  what the source pipe's release has against its sustain, and differently
  in every method (measured by the new self-test) - so the key release
  jumped (HW Tromp 32', D# 039). Every release copy is now set to the
  level relation the source has: the source's release start against its
  sustain, measured after the trimmed part, against the copy's.

### Changed
- The global attack fade-out and release fade-in are set for the sounding
  c1 (MIDI 60 of an 8' rank, the rank's footage counted in); lower pipes
  get 1.3 times as much per octave below it (a 32' C 2.2 times), higher
  pipes the value as set.
- *Release timing kept* applies to the phase vocoders too, upwards only
  (their kept length is too long for a smaller pipe); default 70 % (a
  value saved before is kept). *Attack timing kept* is greyed out unless
  the timing kept method is chosen, *release length* unless the pitch
  shift method is; the labels and tooltips follow the method.
- Tooltips: more room between the lines, a wider box, left-aligned text.

## [0.8.20] - 2026-10-11

### Added
- A fourth transposition method, *Phase vocoder (Signalsmith Stretch, block
  from the pitch; length kept)*: the library's block is sized to eight
  periods of the lower of source and copy pitch (0.1-2 s), so a 32' or 64'
  pipe's harmonics are resolved - with the default 120 ms block it dropped
  harmonics and tremulated on deep tones. The library only keeps the
  envelope in place; the formant move is the same static EQ as in the
  resampling methods (the stop's "Formants" value). Lengths and loop
  points are kept; releases follow *Filled pipes: release length*.
- A fifth method, *Phase vocoder (Rubber Band R3, formants preserved;
  length kept)*: the Rubber Band Library (GPL 2+, bundled as its
  single-file unit in ThirdParty/rubberband) with its finer engine, offline
  mode, high-quality pitch, formants preserved - the stop's "Formants"
  move again by our EQ. Its windows are fixed by the sample rate (the
  longest about 85 ms), so it resolves the harmonics of 8' and 4' ranges
  but not of 32' extensions; the choice is a matter of listening. It is by
  far the slowest method: about seven times real time, minutes for a stop
  (Ped Trompete 8, 124 copies: three minutes), hours for an organ - the
  cache keeps the result. The library's notice and the GPLv2 text are in
  About & licences.

### Changed
- *Pitch shift, timing kept*: the attack timing is given back downwards
  only (where the resampled attack is too slow). Upwards the attack stays
  as resampled - stretching it by repeating its windows made the speaking
  of a pipe waver and detune.
- The join of a timed attack to the sustain lands where the last window
  stands alone at full weight and the material goes on from that window's
  own frames: no jump any more. The first window of a timed part starts at
  full weight (no fade-in), so a release begins as loud as the sustain it
  replaces and a chiff keeps its first milliseconds.
- Pipes already filled keep their source when a stop's compass grows (a new
  transmission): before, the fill rule regrouped them for the wider range
  and every copy already built for them was built again. Only pipes new to
  the map get the rule; a change of the fill group size still starts afresh.
- The tooltip of a filled pipe's key says what sounds there in one line:
  the parent stop, the sample's file name, the shift, and - when this very
  row plays that sample on one of its keys - that key ("HW Tromp 16',
  sample 039-D#.wav, -12 st (from key 51)").

## [0.8.19] - 2026-10-10

### Added
- A third transposition method for the filled pipes, *Pitch shift, timing
  kept*: the same resampling with the timbre correction, but the attack
  (everything before the first loop) and the release get a share of their
  length change back - *attack timing kept* (default 50 %: two octaves down
  the attack is twice as slow instead of four times) and *release timing
  kept* (default 100 %: the source's duration). Done by overlap-add of
  windows three periods of the copy's pitch long, each taken where the
  waveform matches the previous one best (WSOLA): no periodicity is assumed,
  the drifting pitch, the modulations and the noise of a real pipe only move
  the cutting points. The loops are never touched; they shift with the
  attack and are settled to whole periods as before.
- The filled pipes cache folder can be chosen (Advanced global audio,
  *Choose...*); the files already built move along. A button removes the
  cache files of stops that are not loaded now (other configurations, gone
  folders).

### Fixed
- The filled pipes cache doubled on every change of a setting (10 GB for
  a 4.7 GB organ): since 0.8.17 the build parameters were part of the file
  names, so every setting made a new set of files beside the old. One file
  per rank and sample set again, whatever the settings - a set built with
  other settings replaces its file. Files of older cache formats are removed
  when the cache is switched on.

## [0.8.18] - 2026-10-10

### Changed
- Pitch shift, releases: the grains of 0.8.17 are gone (they chopped the
  decaying harmonics, and the join after the first milliseconds was
  audible). The copy keeps the resampled release whole; its measured decay
  rate is turned into the rate of the source's duration by one smooth
  exponential gain - downwards an extra damping and a cut, upwards a slowed
  decay continued, where the material ends, with long crossfaded pieces of
  the release's own late part. See the manual's pitch shift section.
- Pitch shift, formants: shifts upwards follow 30 points more than the
  stop's value (was 40); default 30 % = 60 % upwards. The tooltip and the
  manual say which value a pipe takes when several transmissions ask for it
  (the first transmission in the stop's list).
- The self-test's pitch shift line prints the decay profile of a release
  copy against its source (dB at 25/50/75/95 % of the length).

## [0.8.17] - 2026-10-10

### Changed
- Pitch shift, releases: a filled pipe's release keeps its source's duration
  in both directions. The resampling alone made a release shifted two
  octaves down four times as long (a 32' extension decayed for ten seconds)
  and one shifted up a quarter, and the old "shortened per octave" rule cut
  the upward ones further. Now the decay rate of the resampled release is
  measured and one smooth exponential gain brings it to the rate of the
  source's duration: downwards an extra damping and a cut (the harmonics
  fade sooner, like a release cut), upwards a slowed decay continued, where
  the material ends, with long crossfaded pieces of the release's own late
  part - nothing foreign, no grains. The new slider *Filled pipes: release
  length* (25-200 %, default 100 %) scales the duration.
- Pitch shift, formants: the setting is per stop and per transmission, a
  slider *Formants (filled pipes)* in every stop's voicing row (and in every
  transmission's, for the pipes only it asks for). The global slider is
  gone. The value applies to shifts downwards; shifts upwards follow 30
  points more (default 30 %, so 60 % upwards). A pipe beyond the stop's
  compass takes the value of the first transmission asking for it. Changing
  it rebuilds only the copies of that stop's pipes concerned.
- The filled pipes cache is never emptied by a change of a setting any
  more: the build parameters are part of a cache file's name and a copy's
  formant share is in its record, so copies made with other settings stay
  on disk and are used again when the settings come back. A long build
  interrupted by an operation in the voicing writes the pipes it finished.
  Only *Empty the cache* deletes the files.
- Adding a stop offers the ready-made stops of the loaded folders: the
  free-cell menu of the arrange mode ("New stop here" lists every stop the
  trees hold but the division does not, added whole with all its
  perspectives) and the Stop List's "Add stop..." (per division: an empty
  stop folder or one of those stops).
- Adding a stop folder on the grid, indenting, un-indenting and merging no
  longer leave the arrange mode.

### Fixed
- Crash (bad allocation while painting the grid) when the Stop List was
  applied with the arrange mode on: the reload retired the old stops while
  the grid's rows still pointed at them. The rows are rebuilt right after
  the commit now, before anything paints.
- A Load Configuration or a slider move emptied the filled pipes cache and
  interrupted the running build at every change ("Filled pipes cache
  emptied" dozens of times in the log), so big stops were computed again
  and again.

## [0.8.16] - 2026-10-10

### Fixed
- The loading dialog no longer stands still while the Stop List is compiled
  after Load Configuration (or a division reload). No copy build of filled
  pipes is launched before the last step: the voicing state used to launch
  dozens, and every later step that needs the build lock (a pipe switched
  off in the voicing, a transmission) waited for the running pipe - the
  "voicing state" step took up to seven seconds in one piece. The builds
  start, all at once, when the stop list is complete.
- A division loaded again (Stop List applied, table reset) opens its audio
  routes one per loading step as the startup loading does, so the dialog
  names the route being opened instead of standing at "audio routes".
- Every step of "Compiling the Stop List" that takes half a second or more
  is noted in the activity log with its duration.

## [0.8.15] - 2026-10-10

### Changed
- Nothing expensive is thrown away any more:
  - Indenting a stop under a stop folder, taking it out, making a stop
    folder on the grid, merging stop folders and dragging a stop into
    another division are done in place: the engine keeps the stops with
    their samples and copies (a stop that changes division takes them
    along; only the copies its new compass still misses are built), the
    grouping is metadata. Before, the divisions concerned were loaded
    again, every stop's filled pipes with them.
  - When a division must be loaded again (the Stop List applied with
    changed folders, a reset of the table), stops whose folders the
    division already holds are not decoded again: the new stops adopt the
    samples, velocity and release layers, filled-pipe copies and tuning
    analysis of the old ones. Only folders new to the division are loaded,
    only copies that are missing are built. The activity log says how
    many stops kept their data.

## [0.8.14] - 2026-10-10

### Changed
- Removing a stop on the grid (the red cross of the arrange mode) no
  longer reloads the division: the engine drops that one stop alone - its
  samples, copies and transmissions - and nothing else is reloaded,
  refilled or rebuilt. The arrange mode goes on, so the next stop can be
  removed at once. (Before, the whole division was loaded again and every
  stop's filled pipes were built again.)
- The long press that opens the arrange mode is 2 s (was 3 s).
- The pitch-shift copies are built about three times faster (the sinc
  kernels are tabulated).

## [0.8.13] - 2026-10-10

### Changed
- Pitch shift of filled pipes: a new method replaces the phase vocoder
  (Signalsmith Stretch, 0.7.27 - 0.8.12), which on organ pipes dropped
  harmonics when shifting down an octave or two, modulated the level like
  an LFO and, with the envelope processing, laid a noise floor over the
  tone. The copy is now made the way a pipe is re-pitched: the sample
  resampled with a windowed sinc (every harmonic kept, nothing added, an
  anti-alias filter for upward shifts), its spectral envelope - the
  formants - corrected by one static, zero-phase equaliser according to
  *Formants follow the pitch shift* (0 % keeps the source's envelope in
  place, 100 % leaves it where the resampling put it), the loop points
  scaled with the ratio and refined to whole periods, the loudness matched
  to the source. Nothing in it varies with time, so nothing can modulate
  or hiss. The copy's length changes with the pitch (an octave down lasts
  twice as long); a release is cut to the *release shortened per octave
  up* rule and never longer than the source. The combo box reads *Pitch
  shift (resampling with timbre correction)*. The cache files carry a new
  parameter version, every copy is rebuilt. The vocoder code stays in the
  sources for reference, unused.
- The tooltip of a filled pipe's per-key sliders names the sample it
  sounds: "Sample of <stop> key C2 (36), transposed by -12 semitones",
  with the key's own pipe mentioned in brackets.

## [0.8.12] - 2026-10-10

### Added
- Stop-Merging merges enabled transmissions too. Their samples come from
  the engine: real pipes as loaded, filled pipes from their generated
  copies (pitch-shifted at unity; resampling copies are resampled to the
  pipe's pitch), written as 24-bit WAV with a smpl chunk (the pitch and
  the loop points) and the release layers in "rel" folders, with the key's
  voicing (stop and transmission level, per-key level and tone, attack
  envelope). A transmission alone yields a complete, loadable stop of the
  transposed rank; with stops enabled, its keys mix into theirs. Keys
  still building their copies stop the merge with a message.

### Changed
- Defaults: *Formants follow the pitch shift* 60 % (40 % upwards), *Filled
  pipes: loop crossfade* 40 %.
- The tooltip of a filled pipe's per-key sliders names the key, the pipe it
  plays (on a transmission they differ by the shift) and the pipe whose
  sample it is made from.
- The Stop-Merging progress window is titled "Merging Stops".

### Fixed
- Stop List: an alias typed in the voicing (shown on the stop button) did
  not appear in the alias column when the stop building table had been
  saved before; the rows now take the aliases from the loaded stops.

## [0.8.11] - 2026-10-10

### Fixed
- *Filled pipes: loop crossfade*: a long splice blended the loop end with
  the frames before the loop start, and with a share of the loop's length
  that head reaches into the attack - every pass of the loop mixed the
  chiff in, which is what "changes are still sudden" was. A splice longer
  than the ordinary 50 ms now runs after the jump: from the loop start, the
  continuation past the loop end fades into the loop (the file must have
  that much sustain after the loop; otherwise the share is cut to what is
  there). Self-test: the filled pipe is rendered across two passes of its
  loop and the largest sample-to-sample jump after the attack is compared
  with the source pipe's ("click factor"), at 0 % and 50 % crossfade.

## [0.8.10] - 2026-10-10

### Fixed
- Loops of pitch-shifted copies: the copy kept the source's loop points, but
  its period is another one, so the loop no longer spanned whole periods
  and every pass jumped in phase (the sustain of a filled pipe buzzed or
  clicked). Each loop of a copy now ends at a whole number of the copy's
  periods, refined to the position where the frames before the end match
  the frames before the start best. Self-test: the seam match of the
  copy's first loop is measured against the source's.

### Added
- *Filled pipes: loop crossfade* (Advanced global audio, 0-100 % of the
  loop length, default 0 % = the ordinary 50 ms splice): a longer blend
  for loops that are not quite periodic. Applied to the existing copies at
  once, nothing is rebuilt.

### Changed
- *Formants follow the pitch shift*: shifts upwards follow 20 points less
  than the setting (70 % set = 50 % up).

## [0.8.9] - 2026-10-10

### Fixed
- Transmission captions: the footage in the automatic caption was counted
  from the footage written in the stop's name while the *Transposition*
  box counts from the measured smpl footage, so a "Quinte 3" (pipes at
  2 2/3') shifted +5 read "2 2/7'" on the button and "2'" in the box.
  Both count from the same footage now.

### Changed
- *Filled pipes: formants follow the pitch shift* defaults to 70 %.

## [0.8.8] - 2026-10-10

### Changed
- Pitch shift: the formant setting goes the other way. *Filled pipes:
  formants follow the pitch shift* (Advanced global audio, default 50 %)
  replaces the counter-shift of 0.8.7, which the user found far too sharp
  at anything but 0 %. The harmonics of a shifted copy move to the new
  pitch; the source pipe's spectral envelope stays in place at 0 % (the
  brightest result) and moves with the pitch by this share: at 50 % a pipe
  shifted two octaves down gets its envelope one octave lower, at 100 %
  fully with the pitch (plain transposition, veiled). New settings key
  `transposeFormantFollowPct`; the cache files carry a new parameter
  version, older cached copies are rebuilt.

## [0.8.7] - 2026-10-09

### Changed
- Pitch shift: *Filled pipes: formant counter-shift when pitch shifting*
  (Advanced global audio, default 70 %) replaces *timbre kept*. The
  harmonics of a shifted copy move to the new pitch; the source pipe's
  spectral envelope (the formants) is kept in place and then moved
  against the shift by this much of it: at 70 % a pipe shifted up an
  octave gets its envelope 8.4 semitones lower, one shifted two octaves
  down gets it 16.8 semitones higher. A repitched pipe would otherwise
  sound shrill upwards and veiled downwards. 0 % keeps the envelope where
  the recording had it (the previous behaviour). New settings key
  `transposeFormantCounterPct`; the filled pipes cache files carry the new
  parameter version, so older cached copies are rebuilt. Self-test: the
  same copy is built at 0 % and at 100 % and the spectral centroids must
  differ by at least half of the expected shift.
- Found by that check: the formant settings had never had any effect. The
  base frequency of the formant analysis was handed to the pitch shifter
  in Hz where it expects a fraction of the sample rate, so the spectral
  envelope it worked with was a flat line. "Timbre kept" (0.8.2, 0.8.3)
  therefore sounded exactly like plain transposition. Fixed; the copies
  are rebuilt. The envelope is analysed at twice the pipe's fundamental
  (a coarser envelope): at the fundamental itself it followed single
  partials and a mixture shifted a few semitones lost 8 dB. Measured on
  the Stralsund set (self-test, 0 % against 100 % counter-shift): Pedal
  Trompete 8 shifted an octave moves its spectral centroid by 52 % of the
  envelope shift, Vox humana 50 %, Mixtur 32 %, all at the source's
  loudness.

### Fixed
- Stop grid: a division whose stops had all been placed in one column
  (placed while a voicing was open in another division, which narrows the
  other columns to one grid column, or in a narrow window) stayed in that
  column for ever. Stops are now placed over the division's natural width,
  and a division found with every stop in the first column while two or
  more fit is laid out afresh across the columns.

## [0.8.6] - 2026-10-09

### Fixed
- *Driver Control Panel* (RME): the buffer size chosen in the panel was
  never picked up. The watch that reopens the device after the panel
  closes also took RME's TotalMix window, raised together with the panel,
  for a panel window, and TotalMix stays open for the whole session, so
  the reopen waited for ever. Mixer windows are ignored now; the device
  is reopened 300 ms after the settings dialog closes (activity log:
  "Control panel window detected", "ignoring the window", "reopened").

## [0.8.5] - 2026-10-09

### Fixed
- Polyphony with an ASIO route next to Windows Audio or Aggregated WASAPI
  routes: the render workers of the ASIO route kept spinning across the
  gaps between its short periods on every core, so the device threads of
  the Windows Audio routes (one per route) could not get a CPU in time -
  gaps of 30-40 ms on every route, "DSP" at 400-700 % and crackling at a
  fraction of the usual polyphony. When other device threads render, the
  workers now sleep between blocks and leave those threads their CPUs.
- Choosing "none" as the device of a route that played through a shared
  ASIO driver closed the driver for every route of the division (Diffuse,
  Direct, Rear went silent with it). Only that route is detached now; the
  others keep playing.
- *Set for the auxiliary routes of all divisions* (and *Set for all
  perspectives and divisions*) did nothing when the chosen device was
  "none"; "none" is copied like any device now.
- Aggregated WASAPI: *Set for all ...* copies the selected output channels
  (endpoints) as well, so every route of the scope plays through the same
  endpoints. Other device types keep their channel masks individual.
- A route remembered without a device stays without one at the next start
  (it used to join the division's ASIO driver or open the default device).

### Changed
- The tooltips of the *Set for all ...* boxes say what is copied; the
  manual has a section on getting the most polyphony (auxiliary routes
  double the voices, impulse responses cost per route, mixed ASIO and
  Windows Audio setups share the CPUs).

## [0.8.4] - 2026-10-09

### Fixed
- Filled pipes cache: the cache was emptied at every start ("Filled pipes
  cache emptied" a few seconds after the program opened), so every load
  generated the copies again although the option was on. The settings
  switched the cache on before the transposition method and the other
  copy settings were applied, and each of those setters empties the cache
  when its value changes. The cache is now switched on last. The stop list
  of a large organ (49 stops, about 3500 copies) now loads its copies from
  the cache in about a second.
- The loader's own build of the copies (before the stops were committed)
  had been silently skipped since the background builds came; the copies of
  a newly loaded division are now launched explicitly after the commit,
  and a build that ended before its last pipe (interrupted by a voicing
  change, the pool, a reload) and was not launched again is resumed by the
  stop list's timer within five seconds. The activity log says why a build
  ended early ("interrupted", "the program is closing").

### Added
- A watchdog of the window thread: when the program answers nothing for
  eight seconds, the activity log (`CrashLogs/activity.log`) gets a note
  with the thread's call stack, again every minute while it lasts, and a
  note when the program answers again. Nothing else changes; the note
  shows afterwards where a frozen window was waiting.

## [0.8.3] - 2026-10-09

### Added
- *Filled pipes: keep the generated copies on disk (cache)* (Advanced
  global audio, off by default): the anti-alias or pitch-shifted copies of
  the filled pipes are written to files in the application-data folder
  (`FilledPipesCache`) as they are built, one file per rank and sample
  set, and read from there the next time the same stops load with the
  same settings - the build after loading takes seconds instead of
  minutes. A copy is taken from the cache only when the build parameters
  and the source sample match; changing any setting of the copies
  (transposition method, resampling quality, timbre, loudness, release
  shortening) empties the cache. Switching the cache on when the copies
  are already built writes them all to the cache at once (in the
  background), so the next load finds them. The Configuration Editor
  shows the cache size and has *Empty the cache*. Self-test: a second
  build of the same stop reads every pipe from the cache and yields
  identical samples.
- The loading dialog names the ASIO driver it opens ("opening the ASIO
  driver ... (this takes a few seconds)") as its own step before the audio
  routes; that is the step that used to sit silently at the first route.

### Fixed
- Divisions dialog (JUCE 9): the device's own channel list was visible
  again below the output selector and pushed *Driver Control Panel* and
  *Reopen device* under the window's edge. JUCE 9 replaced the list's
  class; it is now found through its label and hidden as before, the
  dialog looks exactly like the JUCE 8 one (checked side by side, as were
  the Configuration Editor and the Stop List).

## [0.8.2] - 2026-10-08

### Added
- Pitch shift method: *Filled pipes: timbre kept when pitch shifting*
  (Advanced global audio, 0-100 %, default 100 %). The harmonics move to
  the new pitch but the spectral envelope (the formants) stays where the
  recording had it, so an 8' pipe shifted two octaves down for a 32' keeps
  the brightness and body of its source instead of the dull, veiled sound
  of a slowed-down recording. 0 % is the previous behaviour (the envelope
  moves with the pitch, like resampling).
- *Filled pipes: loudness kept when pitch shifting* (default on): every
  shifted copy is brought to the loudness (RMS) of its source sample, so a
  filled pipe is as strong as the pipe it was made from.
- Self-test: the pitch-shift check reports the spectral centroid of the
  copy against its source (timbre kept) and the loudness difference.
- The loading dialog stays for *Compiling the Stop List*: after the
  samples are loaded it reports every step with a percentage (committing
  the divisions, every audio route one by one - "audio route I. / Diffuse
  (3/16)" - from 10 to 35 %, stop folders and grid, voicing state, the
  configuration's registration, temperament and settings) and its bar
  moves, instead of closing and a separate notice appearing in the stop
  area. The same for a division reload.
- Filled-pipe copies being built show their progress: a stop whose copies
  are queued shows a dark track along the bottom of its button (grid and
  voicing alike), the stop being built fills it in orange with the pipes
  done, a finished stop shows nothing. Until then the pipes without a copy
  play resampled.
- Hang fixed: switching a pipe off or on in the voicing (or unloading,
  reloading, swapping a stop) while the copies were being built waited
  for the whole running build - up to a minute of a frozen program. The
  running build now stops at its next pipe (a second at most), the
  operation runs, and the pending builds continue. Self-test: a pipe
  toggle with 11 stops building took 64 ms, the builds finished complete.
- The loading dialog opens each ASIO driver as its own step ("opening the
  ASIO driver RME ... (this takes a few seconds)") before the routes, so
  the percentage no longer sits at the first route while the driver opens.
- Icon: the two smallest sizes (16 and 24 px, the title bar and the task
  bar) are made from the photographic pipes for legibility - pipes twice
  as wide and further apart, a taller black mouth cut, more contrast and
  clearer twist bands (`Tools/make_small_icons.ps1`; the other sizes are
  unchanged). Projucer rewrites `icon.ico` with plain downscales at every
  resave, so the script runs after each resave.

### Fixed
- Quitting took up to a minute and looked like a crash: the engine waited
  for the pitch-shifted copies still being built in the background (a
  whole stop's set at a time). The builds now stop at the next pipe, and
  every window disappears before the audio devices close and the state is
  saved, so nothing greys out.
- Every dialog (Divisions, Temperaments, General Crescendo, Stop List,
  Couplers, IR settings, patterns, licences, ...) sits in a scrolling
  viewport: when the window is smaller than the dialog's design size (a
  large interface size on a small screen, a window remembered at another
  size) scrollbars appear instead of controls vanishing under the window's
  edge. A remembered dialog window is also brought back to its design size
  when the screen allows it.

## [0.8.1] - 2026-10-08

### Added
- *Interface size* (Configuration Editor > Console, 50-300 %): one factor
  for every window, button, slider and text. The first start estimates it
  from the screen so that a Full HD, QHD, 4K or Retina screen shows the
  console as a 2560 x 1440 screen does at the system's 100 % (4K at the
  system's 100 % gets 150 %, Full HD 75 %, never below 70 %); double-click
  returns to the estimate. The slider applies when it is released (or
  shortly after a wheel step or a typed value), never while dragging: every
  open window then keeps its place and size on the screen and only its
  content changes size, so the mouse stays where it was. Settings key
  `uiScalePercent`; the chosen value is kept even when the screen changes.
- The scale is applied per window (the program's windows, their dialogs,
  menus, tooltips, combo-box lists and alert boxes), not through JUCE's
  global desktop scale: with the global one, JUCE 9 converts window
  positions twice on a secondary screen, so a dialog moved to the second
  screen could not be resized. Text and shapes are drawn as vectors at the
  final size, exactly as with the system's own display scaling.

## [0.8.0] - 2026-10-08

### Changed
- Framework: JUCE 9.0.3 (from 8.0.12) on every platform. What it brings:
  Windows painting driven by the system's paint messages and faster
  rendering, fixed window placement and focus on Windows, fixed GUI scaling,
  a new CoreAudio implementation and correct default rate and buffer on
  macOS, ALSA sample-rate and channel fixes and responsive GUIs on Linux,
  MIDI fixes. The audio back-ends are unchanged: ASIO, Windows Audio and the
  aggregated Windows Audio device on Windows, CoreAudio on macOS, ALSA and
  JACK (PipeWire through pipewire-jack) on Linux.
- Window placement uses the framework's fractional display bounds (the
  integer ones are deprecated in JUCE 9).
- Smaller executable: the MP3, Opus and WebP codecs that JUCE 9 enables by
  default are left out; sample sets are WAV (FLAC and Ogg readers stay).
- The Projucer project is saved by Projucer 9; the JUCE submodule and the
  continuous build are pinned to 9.0.3 (Linux adds the XInput2 and EGL
  development packages).
- Not enabled on purpose: Windows MIDI Services (MIDI 2.0). In JUCE 9.0.3
  it still needs a NuGet package at build time and an SDK package installed
  on the user's computer, which would break the single portable executable;
  JUCE falls back to the classic Windows MIDI API, which is what we use.

### Fixed
- *About & licences* dialog in the program's palette: the dark grey dialog
  of every other dialog, the list and the text in rounded boxes with the
  ground and outline of the Configuration Editor's boxes, the chosen entry in the
  orange "on" colour (it had the framework's slate background, light
  outlines and a brown selection).
- Voicing row: the *Velocity layers* boxes wrap onto a third line when the
  column is too narrow for them after the sliders (the row grows by one
  line); before, they were cut off at the row's right edge.

### Notes
- The licence texts in *About & licences* name JUCE 9.0.3. The JUCE licence
  is unchanged: AGPLv3 or the commercial JUCE licence; OrganSampler uses the
  AGPLv3.

## [0.7.30] - 2026-10-08

### Changed
- Clean build: the last calls of JUCE interfaces deprecated in JUCE 8 are
  gone (label fonts through `FontOptions`, WAV writers for recording and
  Stop-Merging through `AudioFormatWriterOptions`). Shadowed variable names
  in the engine, the voicing table and the console code are renamed, an
  unused variable removed. The sources now compile without a single warning
  of their own; the two remaining warnings come from the Steinberg ASIO SDK
  and are left untouched. This prepares the move to JUCE 9.

## [0.7.29] - 2026-10-08

### Added
- Free software: OrganSampler is released under the GNU GPL v3. *About &
  licences...* at the top of the Configuration Editor shows the program's
  licence and every third-party licence in full (JUCE under the AGPLv3, the
  Steinberg ASIO SDK under its GPLv3 option, Signalsmith Stretch and
  Signalsmith Linear under the MIT licence). The texts are embedded in the
  executable at build time; nothing is read beside it.
- Portable, platform-neutral build: a `CMakeLists.txt` in the repository root
  builds the same sources on Windows, macOS and Linux (JUCE is a git
  submodule, the ASIO SDK and Signalsmith are in the repository). A GitHub
  Actions workflow builds and self-tests every push on all three systems and
  attaches the executables to tagged releases. ASIO, Windows Audio and the
  aggregated Windows Audio device exist in the Windows build only; macOS uses
  CoreAudio, Linux ALSA/JACK. *Shut down the computer* works on every
  platform.
- The Projucer project refers to JUCE, the ASIO SDK and Signalsmith by paths
  relative to the repository; no path on the developer's disk is baked in.
- Stop grid: a right click on a free cell offers *New stop folder here* as
  well as the left click.

### Fixed
- Filled-pipe copies built in the background are counted and awaited when
  the engine closes; a failure in one copy no longer takes the others down.

## [0.7.28] - 2026-10-07

### Added
- Pitch shift method: the release of a filled pipe shifted upwards is
  shortened as well - a small pipe's room decay is shorter. *Filled pipes:
  release shortened per octave up* (Advanced global audio, default 25 %):
  a pipe two octaves above its source decays for half the time. Downward
  fills keep the full release, attacks are never shortened. Signalsmith
  Stretch shifts the pitch and shortens in one pass. Changing the value
  rebuilds the release copies in the background.
- Self-test: the pitch-shift check also verifies the release length of an
  upward fill against the expected shortening.

## [0.7.27] - 2026-10-07

### Added
- Filled pipes (beyond the compass, gaps, extensions): a second transposition
  method, *Pitch shift, length kept* (Configuration Editor > Advanced global
  audio > *Filled pipes: transposition method*). Resampling, the old and
  still default way, plays a neighbour's sample faster or slower, so an
  octave down lasts twice as long, release included. The new method shifts
  the pitch spectrally with Signalsmith Stretch (MIT licence, header-only,
  ThirdParty/signalsmith) and keeps the duration, the loop points and the
  release lengths exactly as recorded: a copy of every filled pipe's attack
  samples (every velocity layer) and release samples is built in the
  background when a stop loads or its compass, group size or a transmission
  changes, like the anti-alias copies, and is played at unity; until a copy
  is ready the pipe plays resampled. No tonality limit, no formant
  correction: a pipe an octave away is a different pipe, its whole spectrum
  moves. Switching the method rebuilds the copies of every stop.
- Self-test: a "Pitch shift, length kept" check (copies built, same length
  as the source, the filled pipe sounds, build time reported).

## [0.7.26] - 2026-10-07

### Added
- Key velocity, second means besides the fade-in: a gently struck key of a
  stop without velocity layers starts with a treble cut on the attack - a
  high shelf above the tone start frequency (the same filter as "Treble per
  key") of *Key velocity: treble cut on the attack at the lowest velocity*
  (default 9 dB) at velocity 1, proportionally less for firmer strokes, none
  at 127 - that opens to nothing over *Key velocity: treble cut opens over*
  (default 80 ms). The chiff is blunted, the level is not. Both sliders sit
  in Configuration Editor > Advanced global audio next to the fade-in.

## [0.7.25] - 2026-10-07

### Changed
- Division Settings: the velocity badge on the keyboard is readable now - a
  black box three keys wide with the number in bold over the key pressed
  last; earlier keys lose theirs (the keyboard keeps its size).
- Voicing: the velocity-layer tick boxes carry the suffix only ("_070",
  "_128"); what they do is in the tooltip and the manual.

## [0.7.24] - 2026-10-07

### Added
- Velocity layers: the plain files (the top layer, "_128") have a tick box
  too ("_128 (plain, 71-127)"). Unticked, the velocities above the highest
  file layer play the highest layer that is ticked (its release too); when
  nothing is ticked the plain files play anyway, a key never falls silent.
- Division Settings: a sounding key of the on-screen keyboard shows the MIDI
  velocity it arrived with as a tiny number at its bottom (127 when "Ignore
  MIDI velocity" is on), so you see what the organ receives.

### Changed
- Self-test: the velocity-layer check judges "every path sounds" and only
  reports whether the layers' levels differ (real sets may keep the same
  level in every layer, the fine-tuned case).

## [0.7.23] - 2026-10-07

### Changed
- Key velocity works like an organ, not a piano: a gentle key never plays
  softer. With velocity layers the velocity picks the layer (at its own,
  normal level); a stop without layers softens a gentle stroke with a short
  fade-in instead - Configuration Editor > Advanced global audio > *Key
  velocity: fade-in at the lowest velocity* (default 40 ms), shrinking
  linearly to nothing at velocity 127. "Ignore MIDI velocity" sends 127, so
  then there is no fade-in at all. Until now the volume scaled with the velocity.
- Stop names with a spaceless mutation footage ("Larigot 113", "Terz 135",
  "Nasard 223", "Quinte 513", "1023") are read as 1 1/3', 1 3/5', 2 2/3',
  5 1/3', 10 2/3' - the last digit is the denominator (3, 5, 7 or 9), the one
  before the numerator - and shown with the real footage; the odd number no
  longer stays in the caption, and no measured footage is appended behind it.
  Only harmonic footages are accepted ("Cymbel 114" is left alone), and a
  bare "113" without a name before it is no footage either.
- Self-test: a "Key velocity" check (same loudness at velocity 1 and 127,
  fade-in at the start) on stops without layers.

## [0.7.22] - 2026-10-07

### Changed
- Velocity layers reach the releases and the filled pipes. Release files
  carry the same "_NNN" suffix: the release of a "_070" attack is the "_070"
  release from the rel folder; a velocity without a release of its own takes
  the nearest one that exists (the one just above, else the one below, the
  plain release counting as the top); a layer unticked in the voicing is
  skipped here too. Filled pipes (beyond the compass, transposed from a
  neighbour) follow the layers as well: every layer gets its own anti-alias
  copies, so the right layer plays at every velocity. (0.7.21 ignored the
  suffix in rel folders and built copies of the plain samples only.)
- Self-test: the velocity-layer check also compares the releases of a soft
  and a loud key and the layers of a filled pipe.

## [0.7.21] - 2026-10-07

### Added
- Velocity layers from the file names: a sample named with "_NNN" after the
  key (and the optional note name), e.g. "060-C_070.wav", plays for key
  velocities up to NNN; above that the plain "060-C.wav" plays, or the next
  layer up ("_025", "_070", "_100" ...). Two or three digits, 1 to 127; a
  single digit ("036_1") stays a round-robin variant. A key that has layers
  but no plain file plays its highest layer for every velocity. The voicing of such a stop shows one tick box
  per layer ("_070 (to 70)"); unticked, those velocities use the next layer
  up or the plain samples. The tooltip explains the special use: plain
  samples as fine-tuned versions (harmonics tuned inside the audio, for
  detuned mixtures) and _070 as the original recordings - untick _070 to hear
  only the fine-tuned ones. Saved in the voicing state (disabledVelocityLayers).
- Stop List > Unload folder...: "Unload all folder trees (N)" - after a
  question every tree leaves the organ, the rows stay as empty stops, every
  division is unloaded.
- Self-test: a "Velocity layers" check (soft key = layer, loud key = plain,
  layer off = plain for the soft key too) on sets that have layers.

## [0.7.20] - 2026-09-24

### Added
- A stop folder can be indented under another stop on the grid (the
  rest-and-pull-down gesture or the open voicing of a folder). Folders have
  one level only, so the folder dissolves: its stops and transmissions become
  children of the target, the folder itself too when it has pipes of its
  own. A question names every item first and can be refused ("Cancel" leaves
  everything as it was). A folder without pipes would be left with nothing,
  so the question says that its button (name and alias) is removed - and it
  is, silently, on "Merge". Until now the grid refused such a drop without a
  word.
- Stop List: dragging a stop folder's row right (under the folder above)
  dissolves it the same way, without a question - the table shows the result
  and the info line says so; an empty folder row is removed by it.

## [0.7.19] - 2026-09-24

### Added
- Stop folders are made on the grid: in arrange mode a click on a free
  (dashed) cell offers "New stop folder here" - an empty stop appears in that
  cell and in the Stop List - and the division's pooled stops and
  transmissions ("Place ... here (from the pool)").
- Any stop becomes a stop folder from the grid: while dragging, let the item
  rest over another stop's button for a moment - a dim orange bar appears
  under that button - then pull it a little down onto the bar, which lights
  up: the drop indents the item under that stop (the stop keeps its pipes and
  sounds as one of the folder's [N]). Pulled back up, the bar dims and the
  drop is the ordinary move or swap. A quick drop never indents, so slow
  dragging cannot group stops by accident. Dropping on the open voicing of an
  existing folder indents at once, as before.
- Stop List: "Remove empty stops" removes every row without folders that has
  no stops or transmissions indented under it (empty folders, stops whose
  tree was unloaded).

### Changed
- Rows without folders are never dropped any more - not on Apply (the "will
  be dropped" question is gone), not on load, not when a folder tree is
  unloaded (its stops stay as empty rows). Empty stop folders therefore stay
  as long as you want them; "Remove empty stops" clears them on purpose.

## [0.7.18] - 2026-09-24

### Fixed
- Loading: after the samples are decoded the progress window no longer sat
  silently at 100 % for seconds. It now reports the remaining phases
  ("Compiling the Stop List: tuning, temperament and footage analysis ...",
  "... pipe fills and anti-alias copies ...") and, once it closes, the main
  window shows "Compiling the Stop List and building the stop grid ..." over
  the stop area until the stop buttons appear. (The box existed since 0.7.17
  but never reached the screen: the Direct2D renderer paints on the next
  vertical blank only, and the commit blocked the message thread at once. The
  commit now waits a few frames.)
- The two notices in the main window ("Waiting for audio devices ...", "Compiling
  the Stop List ...") are drawn like the message boxes - their background, outline
  and text colours, rounded corners - instead of the old yellow-on-olive boxes with
  sharp corners, and both lie in the stop area, one above the other, while the
  stop buttons are not there yet.
- Arrange mode: a stop indented under a stop folder can be dragged out of the
  folder again - dropped on a grid cell of its division it leaves the folder,
  takes that cell and its Stop List row follows the folder's block unindented
  (the division reloads). A transmission indented under a folder leaves it the
  same way. Dropping the child into another division moves it there as before.
- Removing a stop folder that still has indented stops or transmissions (the
  red x on the grid, or the x in the Stop List) is refused with a message that
  lists them: take them out of the folder or remove them first. Until now the
  whole folder with everything under it was destroyed.

## [0.7.17] - 2026-09-24

### Added
- Stop List: "Show Trsm." lists every transmission as a grey row of the
  division it plays in (its caption, "Trsm. of <stop> (<division>)", the source
  stop's folders greyed). Transmission rows are reordered, moved to another
  division and indented under stop folders like stops; their x removes the
  transmission. Apply carries these changes into the engine at once, without a
  reload, and "arrange" lays out stops and transmissions in the table's order.
- Stop folders take transmissions as children: an indented transmission
  switches with the master, sits under its open voicing and counts in "[N]".
- Arrange mode: every button carries a red x (the red of PANIC and GLOBAL
  RESET) that removes the stop or transmission from the organ; a stop with
  transmissions is listed with them in a warning first. A stop or transmission
  dropped on the open voicing of a stop folder is indented under it - an orange
  bar under the master's button shows where, while dragging.
- "Generating the Stop List and the stop grid ..." is shown while loaded
  divisions are committed and the console is built.
- The manual's motto: PIPE ORGAN SAMPLER FOR CREATIVE MINDS. Design your
  console, Build your stop list, Perform ... !

### Changed
- "Shut down PC" asks nothing any more (headless consoles): it saves and shuts
  down at once; without an open configuration the instrument goes into
  "Shutdown rescue configuration.json" beside the settings.
- Any change of one route's effects (volume, channel separation, IR, dry/wet)
  unticks "Set effects for all": the routes no longer agree, tick it again to
  copy the new state to all.
- The Divisions dialog lost its "Stops on the grid" list: the Stop List and
  arrange mode do that now.
- Voicing: the line of a stop folder without pipes sits right beside its
  button; the pitch information is as wide as its text, so the alias follows
  it closely; a stop folder with pipes of its own has its voicing stepped
  right of the tree lines.

## [0.7.16] - 2026-09-19

### Fixed
- A transmission's alias without a footage ("Gedekt" on a transmission of
  Subbass 16' shifted up an octave, "Aeoline" on one of a Salicional) showed
  no footage: the caption now appends the transmission's footage ("Gedekt 8'",
  "Aeoline 8'"). The footage of a transposed transmission counts from the
  footage its stop's caption shows (a "Mixtura 2 2/3'" shifted down an octave
  reads 5 1/3').

### Changed
- Voicing: the fold button in the bottom-left corner is a little bigger; the
  tree lines of a stop folder start right under the master's button.

## [0.7.15] - 2026-09-19

### Added
- Advanced global audio: "Tremulant: harmonic filter phase" (0..360 degrees,
  default 90) - the phase of the tremulant's tone oscillation behind its volume
  oscillation (0 = the tone dips with the level, 180 = against it).

### Changed
- A transmission switches its keys off on its own: the pipe boxes of a
  transmission's voicing row act on that transmission only, and pipes switched
  off on the stop's own row keep sounding through its transmissions (a stop
  can be cut to half its compass while its transmission plays all of it). A
  stop with transmissions keeps its fill map and copies for that reason.
- Arrange mode: an item can be dropped anywhere in a division's column, also
  far below the last row - the cell is computed from the grid, empty rows
  above it are kept, and the item stays there. The copy travelling with a
  duplicating drag already reads "... (Trsm.)".
- Voicing: the fold button of every row (stop, transmission, stop folder) sits
  in the bottom-left corner of its button; the tree lines of a stop folder's
  children reach their buttons; note names of loop-less samples are orange
  (the emphasis colour) instead of green.

## [0.7.14] - 2026-09-19

### Changed
- Arrange mode: every division arranges at once, so stops can be dragged
  from any division to any other. A duplicating drag (right button or Shift)
  leaves the original in place and carries a translucent copy of the button.
- Stop folders: the children under an open master are indented and joined to
  it by tree lines (a trunk with a branch to every child); the master's line
  reads "Stop folder (no pipes of its own). Each subordinate stop in its branch
  is voiced separately below ↓".
- Resampling to 48 kHz is polyphase now (one fixed tap vector per phase,
  about three times faster) and can be switched off in Configuration Editor >
  Advanced global audio > "Sample rate on loading" (files then keep their rate
  and play by interpolation, as before 0.7.12).
- Icon: the pipes are clearly brighter (a tone curve lifts the dark shades
  most), the mouth openings stay black. The multi-size icon is also saved as
  `OrganSampler_icon.ico` (and a 1024 px PNG) in the project folder.

## [0.7.13] - 2026-09-19

### Fixed
- An empty stop folder (a container without pipes of its own) was dropped
  whenever the settings or a configuration were read, so it vanished after a
  save and restart. Rows without folders now stay whenever children follow them,
  and a container is written into the table derived from a loaded division too.

### Changed
- Arrange mode works across divisions: a stop dragged into another division's
  grid moves there (its Stop List row, its children and its voicing follow;
  both divisions reload), a transmission dragged there is moved as well. A drag
  with the right mouse button (or with Shift) makes a copy instead: a new
  transmission of the stop in the division it lands in, its own included.
- Stop folders: the children's voicings open and close on their own (and their
  key rows too). A child with a folded voicing stays stacked under its master
  as a compact row; the children return to the grid only when the master folds.
- Icon: the pipes of the large and medium sizes are a little brighter, for more
  contrast against the grey background.

## [0.7.12] - 2026-09-19

### Changed
- Every sample is kept at the base rate of 48 kHz. Files recorded at another
  rate (96, 88.2, 44.1 kHz ...) are resampled once, when they are loaded, with
  a Kaiser-windowed sinc kernel (32 zero crossings each side, about 90 dB of
  stop-band rejection, flat to 20 kHz); downsampling lowers the cut-off to the
  new Nyquist limit so nothing folds back, loop points follow the new frame
  grid. Playback therefore never has to decimate a 96 kHz sample or stretch a
  44.1 kHz one; sets of mixed rates sound alike, and a 96 kHz set needs half
  the RAM it did. The self-test reports how many samples were resampled and
  from which rate, and for every rank the level of the spectrum between the
  harmonics (noise, hum, aliasing) of its lowest pipe.

## [0.7.11] - 2026-09-19

### Fixed
- Quitting could drop a whole session silently: after a clean start the name of
  an earlier configuration was still remembered, the program took it for an
  open configuration whose settings had not changed, and quit without a
  question. A configuration counts as open only when it was loaded or saved in
  this session; otherwise an instrument with folders or stops is offered for a
  new configuration file. The same question guards "Load Configuration" and
  "Shut down PC". Opening a voicing or unfolding its key rows no longer counts
  as a change.

### Changed
- Voicing: "Volume per key" and "Treble per key" fold per stop (and per
  transmission), both folded by default, remembered in the voicing state; the
  "Reset to 0 dB" button shows only with its unfolded row.
- A stop folder without pipes of its own shows "Stop folder only ..." and its
  alias instead of the voicing controls (one line).

## [0.7.10] - 2026-09-19

### Changed
- Stop names without a footage show the footage the lowest pipes really sound
  at. The smpl pitch of a mixture names a fundamental its ranks do not produce;
  the sustain of the lowest pipes of every footage segment is now measured
  (FFT, harmonics 1..16 of the smpl pitch) for the lowest harmonic within
  34 dB of the strongest one and 12 dB above the spectrum around it. "Mixtura"
  reads "Mixtura 2 2/3'" when its lowest sounding rank is the fifth. A missing
  fundamental under a complete harmonic series (low reeds) keeps its footage:
  only a series with gaps, as compound stops have, moves it up. The footage is
  the offset above the key, wherever the rank begins.
- Voicing "Orig. Pitch" keeps the smpl footage and the breaks and adds the
  sounding footage in brackets ("8' (2 2/3')"). The self-test reports the
  harmonic levels behind the decision for every rank.

## [0.7.9] - 2026-09-19

### Added
- Top bar: "Shut down PC" (hidden by default, Configuration Editor > Console).
  After a confirmation it saves the open configuration file and the settings,
  quits and shuts Windows down.
- Every top-bar button can be fired from MIDI: right-click it to learn (the
  buttons answer the left mouse button only). A hidden button still fires.
- Global Reset: "Unload everything" empties the instrument completely - stops,
  sample folders, the tree pool and every setting; the other options grey out
  because nothing is kept. No option is ticked beforehand any more.

### Changed
- Starting without a configuration (no "Load the last configuration at
  startup") begins with an empty instrument: no folder trees in the pool, no
  stop tables, an empty Stop List. A development reload keeps the trees.
- Quitting with folders or stops loaded but no configuration file open asks
  whether to save them as a new configuration file (Save to a file... / Quit
  without saving / Cancel).

### Fixed
- Configuration Editor: a division showed separate perspective entries for a
  prefix and the perspective it belongs to ("difMk2H" next to "Diffuse"). Only
  the perspectives the division has now are listed, and the leftovers are
  dropped when the division is rebuilt.

## [0.7.8] - 2026-09-19

### Changed
- Stop names: a stop whose name (or alias) carries no footage shows the footage
  measured from the smpl pitch of its lowest pipe ("Bourdon" -> "Bourdon 16'").
- Stop folders: the caption counts the ranks in Roman numerals ("Plenum [III]"),
  the master's own pipes included; an empty stop folder named after its
  children ends with the same numeral instead of "[+N]".
- Voicing: the children of an opened stop folder are no longer indented.
  Folding the folder's voicing up puts every child into the master's on/off
  state, so no child keeps sounding behind a master that is off.
- Stop List: dragging a stop folder (or choosing another division for it)
  takes its indented children along. An empty table shows a big hint with an
  arrow to "Load folder...".
- Icon: the lower lip is drawn deeper; the 128 and 64 px sizes draw the pipes
  wider so they stay visible, the smaller sizes keep the drawn outline pipes.

## [0.7.7] - 2026-09-19

### Changed
- Application icon: the large sizes (256, 128, 64) show three tin pipes in
  photographic quality - two twisted outer pipes and a taller middle pipe with
  a cross of five bosses right above the labium. The feet join the bodies at
  the mouth cut-out: the lower lip is the top of the foot, the cone continues
  from it and carries its rim just above the tip. The small sizes
  (48 and below) keep the drawn pipes, which are relatively wider so they stay
  readable. The grey rounded background is unchanged.

## [0.7.6] - 2026-09-19

### Changed
- Tree pool: every sample folder tree is loaded once for the whole organ; its
  folders are assigned to any division in the Stop List. A tree used by two
  divisions is scanned and loaded once (one loader job per tree, the folders go
  to their divisions). Divisions no longer own trees; their tree lists are
  derived from the tables for the older code paths. Settings written before
  the pool are migrated (table references remapped); a moved library is
  followed by folder name.
- "Unload folder..." lists the pool's trees with the number of stops built from
  each and, after a confirmation that names the count, drops the tree from the
  whole organ (the rows concerned turn orange while the question is open).
- "Load folder..." asks which division the stops go to, adds the tree to the
  pool once and loads only that tree.

## [0.7.5] - 2026-09-19

### Fixed
- Stop List: a folder cell dropped back where it was turns grey again; indented
  rows have no orange outline (the indent alone marks a child).
- Automatic names follow the folders again: every action used to mark an
  untouched "New stop" as a typed name. A new stop is named after its first
  folder at once; an empty stop folder takes its children's names (all of them
  when short, otherwise the first one and "[+N]").
- Orange text marks only a row just added (until the next action) and, while
  Apply asks about them, the rows that would be dropped - which the question
  now names. Typed names are white.
- Indenting a row under a row of another division is refused with a message
  that closes itself after three seconds: move the stop first, then indent it.
- "Apply and reload" looks like the other buttons until the table needs
  applying, then it turns orange.

### Changed
- Top bar: Stop List sits right after Record, before Couplers.
- Icon: each pipe 2 px thicker, the gaps unchanged.

## [0.7.4] - 2026-09-18

### Fixed
- Stop List: "Add stop..." asks which division the new row goes to (it used to
  land in the pedal). Dragging a row to the right under a row of another
  division moves the stop into that division as its child, so an empty new stop
  can collect stops from anywhere. A division whose table changed is applied
  even when it had no folder tree before (the children bring their trees along);
  an empty stop folder is created with the division's first tree. Rows that
  would be dropped are named before applying.

## [0.7.3] - 2026-09-18

### Fixed
- Divisions dialog: with one program window the "Window" combo always shows
  Window 1; closing the second window (or switching to one window) puts every
  division back into window 1, so nothing stays assigned to a window that does
  not exist. The combo follows later changes while the dialog is open.

## [0.7.2] - 2026-09-18

### Fixed
- Six and seven manuals: the console really shows eight columns now (the
  voicer clamped the count at six).
- The division header shrinks its swell display, Tremulant and Settings
  proportionally in a narrow column instead of losing the Settings button.

### Added
- Stop List: a dragged folder cell turns orange, a translucent copy follows the
  mouse, the vacated cell and the cell it would land in carry a marching white
  border. "Apply and reload" is grey while nothing needs applying (transmissions
  act at once and need no Apply).
- Stop folders are transmitted as a whole: a child sounds through every
  transmission of its master (transposition and volume included). A row without
  folders is kept as an empty container stop when indented rows follow it, so
  a folder can be transmitted whole while its members are transmitted alone.
- Divisions dialog: "Window 1 / Window 2" combo per division (which program
  window shows it) instead of the stop table button. Choosing window 2 while
  the program runs with one window opens the second window.
- Icon: all three pipes a little thicker.

## [0.7.1] - 2026-09-18

### Added
- Stop List: one table for the whole organ ("Stop List" in the top bar, also
  from the Divisions dialog and the Configuration Editor). Each row shows the
  stop's division (a combo box; changing it moves the stop, dragging a row into
  another division's block does the same), transmissions (click: add one into
  any division or remove one; the transmission boxes left the voicing rows),
  name, alias and one folder per perspective. Folder cells are dragged onto
  other cells (empty: move, occupied: swap). "Load folder..." asks which
  division the stops go to; "Unload folder...", "Swap divisions..." (pairs) and
  "Reset division..." are here too. Apply reloads only the changed divisions.
- Stop folders: a row dragged a little to the right is indented under the row
  above and becomes its child - the master's button switches its children too
  and shows "[+N]"; children have no cell of their own and open, each with its
  own button and voicing, under the master's opened voicing. Dragging left makes
  the row a stop of its own again. One level.
- Up to seven manuals plus pedal (was five).
- Every top bar button can be hidden (Configuration Editor > Console); Stop-
  Merging, Load Voicing and Save Voicing start hidden.

### Changed
- The division header is one line: keyboard indicator, name, swell display,
  Tremulant, Settings; the couplers follow right below. The per-division stop
  table and "Load Stops" are gone (the Stop List replaces them).
- Palette: the Stop List uses the program's colours only (orange on/dark grey,
  dark orange Apply); the icon background is dark grey, both side pipes have
  the same width.
- "default" is offered nowhere as a new perspective (it is the route of folders
  without a perspective prefix).

## [0.7.0] - 2026-09-18

### Changed
- One button per division: "Load Stops" opens the Stop building table, which
  now also loads and unloads the division's folder trees ("Load folder...",
  "Unload folder..."), moves a folder tree with its stops into another division
  ("Move folder to...") and swaps the whole stop lists of two divisions ("Swap
  stops with..."). The Configuration Editor's division section shows a "Stop
  building..." button instead of the folder list and the swap combo.
- Stops never placed on the grid by hand are laid out row by row (the first row
  across all columns, then the next) instead of column by column.
- Stop building: rows are reordered by dragging a stop name (double-click edits
  it); on Apply the program asks whether the grid should follow the new order
  (otherwise every stop keeps its cell). The default width of the dialog grew
  and it widens itself to fit all perspective columns. "default" (folders
  without a perspective prefix) is no longer offered as a new perspective; it
  appears only when such folders exist. The downmix route is "auxiliary".

## [0.6.19] - 2026-09-18

### Changed
- Stop building table: larger text (16 px in the name, alias and folder
  cells); a folder cell shows only the last 30 characters of the path after
  "...", the whole path appears in a large tip while the mouse is over the cell.
- Icon: both side pipes are twisted and all three pipes differ in length (the
  right one is a little shorter and narrower than the left).

### Added
- Removing a stop that has transmissions (the x of its row) asks first and names
  them; applying a table that removes or renames such stops warns once more.
  The reload after Apply loads only the folders in the table, so folders and
  stops left out are released from RAM; a folder used again is loaded again.

## [0.6.18] - 2026-09-18

### Added
- Stop building: a table per division ("Stops..." in the division header,
  "Stop building..." in the Divisions dialog) with one row per stop - name,
  alias - and one column per perspective holding the sample folder that plays
  it. The table is filled from the automatic detection; cells pick from the
  free folders of all loaded trees (a folder serves one cell only, the x beside
  a cell frees it), perspectives and stops can be added or removed, names come
  from the folders until typed. "Apply and reload division" builds the division
  from the table (the perspectives of one stop may lie in different folder
  trees); "Reset to automatic" returns to the folder names. The table is saved
  with the settings and the configuration and used at every later load; a tree
  added to a built division joins the table by the automatic rules.
- Development aid: ORGAN_OPEN_STOP_BUILD=<division> opens the table after start-up.

### Changed
- Application icon: three slender tin pipes of realistic proportions (the
  tallest in the middle with the cross of five bosses, a twisted one on the
  left) instead of one wide pipe.
- The loader derives "which folder is which stop" in one place
  (scanTreeAssignments); without a table the result is unchanged.

## [0.6.17] - 2026-09-18

### Added
- Start-up wait for audio devices: a device that does not open (an RME
  interface switches its sample rate first and registers late) is awaited for
  up to six seconds with a "Waiting for audio devices: ..." box in the window,
  checked every second. A device that appears in that time is opened silently;
  the "Audio devices not available" message names only devices still absent.
- New application icon: a single tin pipe with its mouth and a cross of five
  bosses above it on a dark case, drawn for every size from 16 to 256 px.

### Changed
- Default pattern lists: perspectives Direct, Diffuse, Rear; one-shot "Chiff";
  ignored "trem". They apply on first start, whenever a saved list is empty and
  after a Global Reset with the pattern box checked (which no longer empties
  the lists). Perspective prefixes found in folder names join these lines
  ("Direct;01-dirMk2H-").

### Fixed
- The "Audio devices not available" message appeared for a device that was
  merely late; it is now shown only after the wait and only for devices that
  are still missing.
- A substitute device opened at start-up because the saved device was missing
  is no longer adopted as the user's choice; the saved device stays wanted until
  a selector is touched.

## [0.6.16] - 2026-09-18

### Fixed
- Perspective detection handles prefixes with several separators
  ("01-dirMk2H-OW_Flote8"): every cut after a separator run is tried, a prefix
  qualifies when most of its stops (at least two) also appear under another
  prefix, and the shortest qualifying form is used. New perspective lines get a
  readable name ("DirMk2H;01-dirMk2H-").

## [0.6.15] - 2026-09-18

### Changed
- Perspective prefixes are detected only when every stop of the folder tree
  appears under the prefix (the stops under "dir-" are exactly all the stops);
  a prefix on only some stops is part of their names. At least two such
  prefixes are needed, one prefix on everything is a naming style.

## [0.6.14] - 2026-09-13

### Changed
- Extensions transposed upwards lose 0.1 dB of level and 0.1 dB of treble per
  semitone automatically (+12 semitones: -1.2 dB and -1.2 dB); downward
  transpositions and real pipes are unchanged. Release samples follow.
- Voicing: the volume and treble sliders of a pipe switched off are greyed out
  for real pipes too, not only for extensions.

## [0.6.13] - 2026-09-13

### Fixed
- Choosing an audio device: the missing-device check no longer runs while the
  Divisions dialog or any menu is open, so the device list stays put while the
  user browses it. It also does nothing when no device is missing.
- "Audio devices not available" lists every missing device: Windows Audio
  devices that fail to open are reported too (they were not), and the message
  waits until 2.5 s after the last missing device instead of 1.5 s after the
  first.
- The main window returns to its last position (or maximised) and is never
  placed with its title bar off the screen.
- Quitting asks to save the open configuration whenever its settings changed,
  also for a configuration loaded at start-up. "Changed" is now decided by
  comparing the settings (without window and dialog geometry) with their state
  at the configuration's load or save, so every setting the configuration
  holds (it holds all of them) counts and nothing else does.

## [0.6.12] - 2026-09-13

### Fixed
- Removing a division's audio routes (fewer divisions, unload, swap, loading a
  configuration) closes the Divisions dialog of that division first, detaches
  the other dialogs' selectors meanwhile and forgets the routes' managers in
  the test-tone player, the driver-panel watch and the pending Windows Audio
  resumes: no dangling pointers to freed device managers.

### Changed
- Activity log: a persisting audio callback anomaly is written once when it
  appears, then once a minute, and once when it ends (was every second); the
  "Audio levels" line at most every 10 s.

## [0.6.11] - 2026-09-13

### Fixed
- Crash two seconds after start (and on every retry) while a saved ASIO
  device was switched off: 0.6.10 deleted idle ASIO hosts while a device
  selector still listened to them. Hosts are again released only after their
  selectors are gone. The missing-device check now probes an ASIO driver with
  a throw-away manager first; only a driver that really opens makes the route
  reopen, so a switched-off interface causes no muting, no route rebuilds and
  no crash.

## [0.6.10] - 2026-09-13

### Changed
- The Configuration Editor refreshes its values by itself after actions that
  change them (swapping divisions, the second window, loading or unloading
  folders, the number of divisions).
- A missing audio device is checked every 2 seconds (was 5, with a 30 s pause
  after a failed attempt); a device switched on plays again within seconds.
  Attempts still wait while the organ sounds. ASIO hosts left over from failed
  attempts are dropped instead of accumulating.

## [0.6.9] - 2026-09-13

### Fixed
- An unavailable audio device (ASIO hardware switched off) produced the
  "not available" message every five seconds: each device is now reported once
  per session; afterwards the program waits silently, retries a failed device
  at most every 30 s and never while the organ sounds.

### Added
- Configuration Editor > division section: "Swap all stops with" exchanges the
  whole stop lists of two divisions (folders, voicing, grid positions, pool,
  transmissions). The divisions keep their own tremulant, swell, wind model,
  couplers, MIDI and audio; combinations, the crescendo and MIDI bindings
  follow; transmission captions show the stop's new home division. Saved
  combination files can be converted afterwards (rewritten in place).
- Two program windows (Console > Program windows): every division chooses the
  window it is shown in; the divisions of each window share the width by their
  weights (dividers). The second window's position is remembered; closing it
  returns to one window.
- Folder load: perspective prefixes are detected from the folder names (the
  same stop names under several prefixes). A prefix sharing its stem with a
  known perspective (dir ~ Direct, dif ~ Diffuse, rea ~ Rear) joins that line
  of the perspective patterns, an unknown one gets a new line.

## [0.6.8] - 2026-09-11

### Added
- Self-test: the rendered pitch of a detected celeste is measured (key 60):
  with undulation 0 it must sound at equal-tempered pitch, with the detected
  undulation as recorded. Unda 8 of the Tabor set: -0.4 c and -19.3 c
  (recorded -20.4 c). The tuning report also lists the raw recorded pitches,
  the rank folder and the applied correction/undulation.

## [0.6.7] - 2026-09-11

### Fixed
- Switching a pipe off no longer frees the samples of a real pipe (a pipe at
  the edge of the compass came back as an extension instead of its own
  sample). Only an extension's anti-alias copies are freed; switching a pipe on
  plays its own samples again at once.

## [0.6.6] - 2026-09-11

### Fixed
- A device chosen by the user was swapped back to the previously saved one a
  few seconds later: the missing-device watcher compared against the state
  loaded at start-up. The remembered state now follows every device the user
  picks (and is saved right after the change); the watcher only restores a
  device when a route has none open or when nothing was chosen in this session.
- Celeste detection failed on flat (and sharp) ranks with one mistuned pipe
  (a wrong unity note at the top of the compass destroyed the trend
  correlation): such pipes are left out of the trend test. Unda 8 of the Tabor
  set is now recognised (-4.5 beats/s) and its diapason reported correctly.
- A saved undulation of 0 from older states no longer overrides the detected
  celeste; only a value the user set is saved and applied.
- Reducing the number of divisions forgets everything about the removed
  divisions (folders, devices, MIDI, grid, pool); a division added later starts
  empty instead of loading folders remembered from long ago.
- Voicing: the pipe on/off checkbox is centred under the note name.

## [0.6.5] - 2026-09-11

### Added
- Voicing: a checkbox under every note name of "Volume per key" switches the
  pipe off: it stays silent, is never filled from a neighbour (no extension for
  a cornet), and its samples, release samples and anti-alias copies are freed.
  Ticking it again reads the pipe's own samples from disk. Saved with the
  voicing and with Save/Load Voicing.

### Changed
- The mouse wheel scrolls the stop workspace again, except over a division
  column whose voicing is open (there it belongs to the sliders).
- Per-key sliders are 30 px wide (were 32).

## [0.6.4] - 2026-09-11

### Changed
- Stop-Merging: the folder is named after the merged stops
  (`StopMerged_Principal_8_Octava_4`: underscores, no spaces or forbidden
  characters, long lists cut with `_etc`); the rank folders inside carry the
  same name (with the perspective prefix), so the merged stop loads under it.
  No text file is written.

## [0.6.3] - 2026-09-11

### Changed
- Harmonic filter (swell, wind, tremulant): the high shelf now has a defined
  transition band from 1.8 kHz to about 3.6 kHz (fourth-order lowpass at 3 kHz
  behind the level-following mix) instead of the gentle first-order slope
  around 2.2 kHz.
- Stop-Merging writes into the loaded folder tree the enabled stops come from
  (with several trees, the one holding most of them). The completion message
  names the folder and the merged stops.

## [0.6.2] - 2026-09-11

### Changed
- Audio tab: "Set IR for all" became "Set effects for all": impulse response,
  dry/wet, IR settings, perspective volume and channel separation are copied
  and stay linked while the box is checked (a change on one tab changes all).
- Audio tab: both "set for all" boxes have two scopes with clear captions. On
  an ordinary perspective they act on all perspectives and divisions except
  the auxiliary routes; on the auxiliary tab they act on the auxiliary routes
  of all divisions.
- Voicing: the "Treble per key" sliders light up while the key sounds and mark
  filled pipes and loop-less samples like the volume row.
- "Treble per key: shelf start frequency" defaults to 1800 Hz.

## [0.6.1] - 2026-09-11

### Added
- Voicing: a second row of per-key sliders, "Treble per key" (-9..9 dB): a
  high shelf per key whose transition band starts at the tone start frequency
  (Configuration Editor > Advanced global audio, default 700 Hz, 120..5000 Hz)
  or at the pipe's recorded fundamental when that is higher, and ends about an
  octave above. A pipe held through several routes takes the highest setting.
  Saved with the voicing, Save/Load Voicing, transmissions, and rendered into
  Stop-Merging files.
- Voicing: "Volume per key" and "Treble per key" fold under orange section
  headers (folding remembered; volume open, treble folded by default), each
  with a small "Reset to 0 dB".
- Configuration Editor: "Wind: maximum wind model effect" (0..100 %, default
  100). Every key stroke uses a random 40..100 % of it; 0 switches the wind
  model off.

### Changed
- Per-key volume sliders of stops and transmissions now range -9..9 dB and sit
  closer together (32 px).
- Division dialog: no banner line at the top; the settings, wind model and
  "Stops on the grid" columns moved up and gained that space. Learn messages
  appear small beside the division label.

## [0.6.0] - 2026-09-11

### Added
- Wind model: every key stroke (press and release) shakes the wind with a
  random strength between 40 and 100 % of the computed impulse.
- Tremulant: the strength wanders between 60 and 100 % of the set depth, a new
  value every cycle, reached over about half a second; all perspectives of a
  division follow the same wander.
- Division dialog: a small note "Shared with <division>" under the MIDI channel
  when another division listens to the same device and channel.

### Changed
- Audio tab: Perspective volume, Channel separation and the IR reverb section
  share one column of equal width; the device box (type, device, sample rate,
  buffer size) takes only the height its rows need, and Driver Control Panel,
  Reopen device and Set for all sit directly under the buffer size.

### Removed
- The large yellow "NOTICE" about a shared MIDI channel at the top of the
  Division dialog (replaced by the note under the channel).

## [0.5.0] - 2026-09-11

First version with formal numbering. State of the program at this point:

### Added
- Transmissions with transposition (extensions -24..+24 semitones, pure
  intervals following the harmonic series), per-transmission volume and
  per-key voicing, full voicing rows for transmissions, aliases for stops and
  transmissions.
- Missing pipes filled from neighbouring pipes in groups (Configuration
  Editor > Advanced global audio), the smallest possible transposition for the
  last group, cubic interpolation and anti-alias copies for filled pipes.
- Division compass (lowest/highest key), Unison Off per division, celeste beat
  ratio up to 20x.
- Per-route channel separation, an always-present auxiliary (downmix) route per
  division with its own device, channels, volume and IR.
- IR reverb per route: true stereo IRs, dry/wet, high/low cut, pre-delay, wet
  volume, decay, fade-out; caption with a full tooltip.
- General crescendo, combinations saved in configurations, portable
  configurations with relative paths, ASIO host pool (different drivers per
  route), embedded temperaments, harmonic filter (swell/wind/tremulant), wind
  and tremulant advanced settings, release cutting, per-slider main-panel
  visibility.
- Single instance: a second start brings the running one to the front.
- Missing audio devices reported in one message; routes reopen automatically
  when their device appears.
- Option to load the last configuration at startup (off by default).
- Sustained samples without loop points play as a whole-file loop with a 50 ms
  end fade.

### Changed
- Switched-on buttons use a darker orange; hovering darkens one more shade.
- Stop names ending in a bare footage number are shown with the foot mark.
- Division dialog: tremulant and wind model details fold under section
  headers; folding is remembered, as is the folding of Configuration Editor
  sections.
- MIDI Learn of keyboards and swell sliders uses the common menu and dialog.
- Recorded temperament matching equal temperament is reported as "Equal".

### Fixed
- Second folder tree replaced the first (missing append flag).
- Same ASIO driver opened twice; convolution partitioning by device block size;
  IR tails trimmed silently (now never); configuration save refused on mixed
  drive roots; audio tab rows placed relative to the device selector's layout.
