# Aurora Broadcast Processor

Aurora Broadcast Processor is an original Windows x64 audio effect for continuous digital and internet-radio processing. It combines slow source leveling, tonal shaping, four-band dynamics, stereo management, oversampled peak control, and look-ahead limiting in one resizable JUCE interface. It is not RF equipment, an FM stereo/RDS generator, or a certified loudness or true-peak compliance system.

## Current development status

Version **0.1.0** has a complete initial implementation candidate ready for its first engineering build. The current development machine lacks CMake, Visual Studio/MSVC, the Windows SDK, Inno Setup, validators, and test hosts, so no local compile, test, installer, validator, or host-compatibility success is claimed. Windows CI is ready to provide the authoritative clean build after this repository is pushed to GitHub.

## Features

- Mono and stereo audio-effect buses at 44.1, 48, 88.2, and 96 kHz
- Sanitizing input stage, DC block, 12 dB/octave high-pass filter, and slow gated RMS AGC
- Six interactive EQ bands over a real 4096-point input/output FFT display
- Four Linkwitz-Riley crossover bands with stereo-linked soft-knee compression
- Harmonic density, width/balance, mono bass, and gradual correlation protection
- 4× oversampled soft peak stage and fixed-latency look-ahead limiter
- Real peak/RMS, AGC, band reduction, limiter reduction, and correlation metering
- Versioned complete state, ten factory presets, user files, and A/B snapshots
- Windows x64 VST3 and standalone targets, automated tests, installer, packaging, and release workflows

## Formats and operating system

- VST3 x64 audio effect
- Standalone x64 application using JUCE's audio-device selector and the same processor/editor
- Windows 10/11 x64; Visual Studio 2022 is the supported build toolchain

Designed for compatible Windows x64 VST3 hosts. Compatibility must be verified with the specific host and host version.

## Build and test

Install the items in [docs/INSTALL_PREREQUISITES.md](docs/INSTALL_PREREQUISITES.md), then use an x64 Native Tools PowerShell:

```powershell
./scripts/configure.ps1
./scripts/build-release.ps1
./scripts/run-tests.ps1
```

JUCE 8.0.14 is fetched at exact commit `2cdfca8feb300fb424002ba2c2751569e5bacb64`. See [docs/BUILDING.md](docs/BUILDING.md) for targets and troubleshooting.

## Installation

Use the versioned installer when available, or copy the complete `Aurora Broadcast Processor.vst3` bundle to `%CommonProgramFiles%\VST3`. The standalone executable can run from a writable local folder. Detailed steps and uninstallation are in [docs/INSTALLATION.md](docs/INSTALLATION.md).

User presets are stored in `%APPDATA%\Ammar Audio Labs\Aurora Broadcast Processor\Presets`; packages and normal uninstall do not delete them.

## Known limitations and licensing

The initial release has no VST2 or WinAmp DSP adapter, is unsigned, and has not yet been validated or tested in RadioBOSS, NextKast, REAPER, JUCE AudioPluginHost, or the standalone application. Presets require listening tests against legally supplied reference material and downstream codecs. See [docs/KNOWN_LIMITATIONS.md](docs/KNOWN_LIMITATIONS.md).

JUCE 8 is dual-licensed under the JUCE licence and AGPLv3. The owner must select and comply with an applicable licence before distributing binaries. See [LICENSE_NOTICE.md](LICENSE_NOTICE.md) and [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## GitHub Actions artifacts

Open the repository's **Actions** tab, choose **Windows CI** or **Build Windows Release**, open a successful run, and download its artifact. CI produces `AuroraBroadcastProcessor-Windows-x64-CI`; a release build produces the versioned verified ZIPs, installer, checksums, manifest, and test-result archive documented in [docs/ARTIFACTS.md](docs/ARTIFACTS.md).

## Troubleshooting

- “CMake not found”: install CMake 3.22+ and reopen the terminal.
- “Generator Visual Studio 17 2022 not found”: install VS 2022 Desktop development with C++ and a Windows SDK.
- Plug-in not found: copy the entire `.vst3` directory, rescan VST3 paths, and confirm the host is x64.
- No standalone audio: open audio settings, choose valid input/output devices and matching enabled channels.
- Preset write failure: verify `%APPDATA%` is writable and the preset name is legal.
