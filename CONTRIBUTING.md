# Contributing to OrganSampler

## Reporting a problem

Open a [bug report](https://github.com/zurek-jiri/OrganSampler/issues/new/choose)
and attach the log bundle: run `OrganSampler/Tools/collect_logs.ps1` (Windows:
right-click > Run with PowerShell) or `OrganSampler/Tools/collect_logs.sh`
(macOS, Linux). It zips the program's `CrashLogs` folder (the activity log and
the crash reports), `settings.json`, the last self-test report and a short
description of the computer - no sample data - and places the zip on your
Desktop (home folder on macOS and Linux). Drag it into the "Logs" field of the
report, add screenshots where they help, and describe what you did step by
step. The activity log is what makes a report fixable: it records every
device, every load and the last minutes before a crash.

The maintainer reads the reports with `OrganSampler/Tools/fetch_issues.ps1`,
which downloads every issue with its comments and attachments into a local
folder for analysis.

## Sharing an organ configuration

See `configurations/README.md`: what a published configuration must contain
and how to submit it (pull request or the "Organ configuration" issue form).

## Changing the code

- Build with CMake (`README.md`) or the Projucer project under
  `OrganSampler/`; both compile the same sources.
- C++17, JUCE 9. Windows-only code (ASIO, WASAPI aggregate, the crash
  handler's dbghelp, window enumeration) stays under `#if JUCE_WINDOWS` /
  `JUCE_MSVC` with a fallback, so macOS and Linux keep building.
- Nothing may refer to a path outside the repository; nothing is read from
  beside the executable at run time (data tables and licence texts are
  embedded at build time).
- The audio is never altered silently: no hidden normalisation, trimming or
  fading of samples or impulse responses.
- Every user-visible change gets a line in `OrganSampler/CHANGELOG.md` and,
  where the user sees it, a paragraph in `OrganSampler/USER_MANUAL.md` (the
  manual is embedded in the program; `Tools/number_manual.ps1` renumbers its
  chapters after an edit).
- Before a pull request: the CI build on all three platforms passes and
  `OrganSampler --selftest "<folder>"` completes without crash.

## Licence

OrganSampler is free software under the GNU GPL v3; contributions are
accepted under the same licence. Third-party components and their licences are
listed in `THIRD-PARTY-NOTICES.md`.
