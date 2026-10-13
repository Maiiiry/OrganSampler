# Contributing to OrganSampler

## Reporting a problem

In the program: *Configuration Editor > Report a problem...* Describe what
happened and how to reproduce it. The program bundles its activity log, the
crash reports, `settings.json`, the last self-test report and a description of
the computer and the audio devices into one zip (no samples, no voicing, no
combinations), takes screenshots of its own windows - keep the dialog in
question open - and lets you add screenshot files of your own. It then opens
the [bug report form](https://github.com/zurek-jiri/OrganSampler/issues/new/choose)
in your browser with the fields filled in, and the report folder on your
Desktop: drag the zip into "Logs", the pictures into "Screenshots", submit. A
free GitHub account is needed to submit.

The activity log is what makes a report fixable: it records every device,
every load and the last minutes before a crash.

The maintainer reads the reports with `OrganSampler/Tools/fetch_issues.ps1`,
which downloads every issue with its comments and attachments into a local
folder for analysis.

## Sharing an organ configuration

See `configurations/README.md`: what a published configuration must contain
and how to submit it (pull request or the "Organ configuration" issue form).
Configurations only - never samples; the `Configurations check` workflow
refuses them.

## Changing the code

- Build with CMake (`README.md`) or the Projucer project under
  `OrganSampler/`; both compile the same sources.
- C++17, JUCE 9. Windows-only code (ASIO, WASAPI aggregate, the crash
  handler's dbghelp, window enumeration, message pumps) stays under
  `#if JUCE_WINDOWS` / `JUCE_MSVC` with a fallback, so macOS and Linux keep
  building. GCC and clang are stricter than MSVC: `alignas` before `static`,
  explicit `juce::int64`/`juce::uint64` for `jmax` and `countNumberOfBits`,
  `std::max<size_t>` instead of `jmax<size_t>`, `juce::File()` instead of `{}`.
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
