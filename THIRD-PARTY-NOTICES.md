# Third-party software in OrganSampler

OrganSampler itself is free software under the GNU General Public License v3
(see `LICENSE`). It contains or links the following software; the same notices
and the full licence texts are shown in the program under Configuration Editor >
About & licences.

| Component | Version | Licence | Location |
|---|---|---|---|
| JUCE framework (Raw Material Software Limited) | 9.0.3 | GNU AGPLv3 (dual-licensed with the commercial JUCE licence; used here under the AGPLv3) | `JUCE/` (git submodule, tag 9.0.3) |
| Steinberg ASIO SDK (Steinberg Media Technologies GmbH) | 2.3.4 | GNU GPLv3 option of the dual licence (proprietary ASIO licence or GPLv3) | `ASIOSDK/` |
| Signalsmith Stretch (Signalsmith Audio Ltd) | main, 2026 | MIT | `ThirdParty/signalsmith/signalsmith-stretch.h` |
| Signalsmith Linear (Signalsmith Audio Ltd) | main, 2026 | MIT | `ThirdParty/signalsmith/signalsmith-linear/` |

ASIO is a trademark and software of Steinberg Media Technologies GmbH. ASIO
support is compiled into the Windows build only; the macOS and Linux builds use
CoreAudio and ALSA/JACK through JUCE.

No sample set is part of this repository or of any release; sample sets remain
the property of their authors.

The licence texts: `LICENSE` (GPLv3), `ThirdParty/licence-texts/AGPL-3.0.txt`,
`ASIOSDK/LICENSE.txt`, `ThirdParty/signalsmith/LICENSE-signalsmith-stretch.txt`,
`ThirdParty/signalsmith/LICENSE-signalsmith-linear.txt`, and the notices in
`ThirdParty/notices/`.
