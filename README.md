# OrganSampler

**Pipe organ sampler for creative minds.** Design your console, build your stop
list, perform.

OrganSampler plays sampled pipe organs from plain folders of WAV files - one
folder per stop, its pipes as WAV files - and turns any number of such folders into
an organ with up to seven manuals and a pedal: massive polyphony, per-key
voicing, divisions and microphone perspectives with independent audio routes on
any number of sound cards, IR reverb per route, wind and tremulant models,
temperaments, transmissions with transposition (extensions), velocity layers,
general crescendo and combinations. No organ definition file is needed: you load
the folders, the program proposes the organ, you shape it and save it as a
configuration. Free software under the GNU GPL v3. Windows, macOS and Linux.

## Try it in five minutes

1. **The program:** the latest build on the
   [releases page](https://github.com/zurek-jiri/OrganSampler/releases)
   (pre-release builds for testers; one executable for Windows 10/11, macOS
   - 11 Big Sur or newer on Apple silicon, 10.11 El Capitan or newer on
   Intel Macs - or Linux, no installation).
2. **An organ to play:** download
   [**RuralOrgan.zip**](https://github.com/zurek-jiri/OrganSampler/releases/download/rural-organ-0.8/RuralOrgan.zip)
   (about 950 MB) - the **Rural Organ** demo: a small village organ, three
   divisions, 24 stops, **dry recordings with IR reverb**, with its
   sample folders and a portable configuration. Unpack it, start
   OrganSampler, *Load Configuration*, choose `Rural Organ.json`. It plays
   through your default sound device at once; a message says what to set
   up next. Details: [`configurations/rural-organ`](configurations/rural-organ/README.md).
3. **More organs:** OrganSampler ships no other samples. Sample folders in
   wave format from [**Sonus Paradisi**](https://www.sonusparadisi.cz/en/organs.html)
   work with OrganSampler as they are - and their
   [Free stuff](https://www.sonusparadisi.cz/en/free-stuff.html) page has
   free instruments and impulse responses. Ready-made configurations for
   them: [`configurations/INDEX.md`](configurations/INDEX.md) (how to use and
   publish one: [`configurations/`](configurations/README.md)).

## Links

- Website: https://zurek-jiri.github.io/OrganSampler/
- Downloads: https://github.com/zurek-jiri/OrganSampler/releases
- User manual: [`OrganSampler/USER_MANUAL.md`](OrganSampler/USER_MANUAL.md)
  (also inside the program: Configuration Editor > About & licences)
- Changes: [`OrganSampler/CHANGELOG.md`](OrganSampler/CHANGELOG.md)
- Organ configurations to share and download: [`configurations/`](configurations/README.md),
  the list of the published organs: [`configurations/INDEX.md`](configurations/INDEX.md)
- Problems and ideas: *Configuration Editor > Report a problem...* in the
  program bundles the logs and opens the
  [bug report](https://github.com/zurek-jiri/OrganSampler/issues/new/choose);
  see [`CONTRIBUTING.md`](CONTRIBUTING.md)
- Third-party software and licences: [`THIRD-PARTY-NOTICES.md`](THIRD-PARTY-NOTICES.md)

The executable is portable: every data table and every licence text is embedded
at build time, nothing is read beside the executable. Settings live in the
user's application-data folder (`%APPDATA%\OrganSampler` on Windows,
`~/Library/Application Support/OrganSampler` on macOS, `~/.config/OrganSampler`
on Linux).

## Building

Everything the build needs is in the repository (JUCE as a git submodule, the
Steinberg ASIO SDK under its GPLv3 option, Signalsmith Stretch); nothing refers
to a path outside it.

```
git clone --recurse-submodules https://github.com/zurek-jiri/OrganSampler.git
cd OrganSampler
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The JUCE submodule is pinned to JUCE 9.0.3. JUCE 9 needs CMake 3.22 or newer,
and on Linux the XInput2 and EGL development packages in addition to the usual
ones.

The result is `build/OrganSampler_artefacts/Release/OrganSampler.exe`
(Windows), `OrganSampler.app` (macOS) or `OrganSampler` (Linux).

Linux needs the usual JUCE development packages (ALSA, JACK, X11, FreeType,
fontconfig, Mesa; the list is in `.github/workflows/build.yml`). Audio on macOS
is CoreAudio, on Linux ALSA or JACK; ASIO, Windows Audio and the aggregated
Windows Audio device exist in the Windows build only.

Windows developers may also use the Projucer project
`OrganSampler/OrganSampler.jucer` (Visual Studio 2026 exporter under
`OrganSampler/Builds`). Its paths are relative to the repository as well.

### Continuous integration

`.github/workflows/build.yml` builds Release on Windows, macOS and Linux for
every push and pull request, runs the engine self-test on synthetic sample
folders (Windows and Linux) and uploads the executables as artifacts. A tag `v*` also
attaches them to a GitHub release. The macOS and Linux builds are not signed:
macOS asks for confirmation the first time (right-click > Open), Linux needs
`chmod +x OrganSampler`.

## Self-test

```
OrganSampler --selftest "<division sample folder>"
```

Loads the folder without the GUI, exercises the engine (kernels, transmissions,
filled pipes, velocity layers, pitch-shifted copies, tutti stress, recording)
and writes `selftest.txt` into the application-data folder.

## Samples

No samples are part of this repository or of any release: they are bought or
downloaded elsewhere - from [Sonus Paradisi](https://www.sonusparadisi.cz/)
above all - and remain the property of their authors. OrganSampler plays
plain folders of WAV files: one folder per stop, named after the stop, with
the WAVs of its pipes named after their keys (`060-C4.wav`; several files for
one key play in turn; velocity layers `060-C4_070.wav`; releases in
`rel`, `rel500` ... subfolders); loop points and pitch belong ideally in the
WAV's `smpl` chunk, but the program copes without them. See "How the sample
folders must be organised" in the user
manual, and the website for where the samples come from.
