# Aurora Broadcast Processor

Aurora Broadcast Processor is an original Windows and macOS audio effect for continuous digital and internet-radio processing. It combines slow source leveling, tonal shaping, four-band dynamics, stereo management, oversampled peak control, and look-ahead limiting in one resizable JUCE interface. It is not RF equipment, an FM stereo/RDS generator, or a certified loudness or true-peak compliance system.

## Current development status

Version **0.1.0** is built and tested by clean GitHub Actions jobs on Windows x64 and macOS Universal. CI verifies the processor, responsive editor layouts, plug-in/standalone targets, packages, checksums, and manifests. Host-specific listening and compatibility testing is still required before a production release.

## Features

- Mono and stereo audio-effect buses at 44.1, 48, 88.2, and 96 kHz
- Sanitizing input stage, DC block, 12 dB/octave high-pass filter, and slow gated RMS AGC
- Six interactive EQ bands over a real 4096-point input/output FFT display
- Four Linkwitz-Riley crossover bands with stereo-linked soft-knee compression
- Harmonic density, width/balance, enabled-by-default low-end preservation, and gradual correlation protection
- Live control-strip activity display for EQ, band thresholds/reduction, harmonics, stereo width, and bass protection
- 4× oversampled soft peak stage and fixed-latency look-ahead limiter
- Real peak/RMS, AGC, band reduction, limiter reduction, and correlation metering
- Versioned complete state, ten factory presets, user files, and A/B snapshots
- Windows x64 VST3/standalone targets and macOS Universal VST3/AU/standalone targets
- Automated tests, Windows installer, macOS DMG, checksums, manifests, and coordinated release workflows

## Formats and operating systems

- Windows 10/11 x64: VST3 and standalone, built with Visual Studio 2022
- macOS 11 or newer, Universal `arm64` + `x86_64`: VST3, Audio Unit, and standalone, built with Xcode
- The standalone applications use JUCE's audio-device selector and the same processor/editor as the plug-ins

Designed for compatible Windows x64 VST3 hosts and macOS VST3/Audio Unit hosts. Compatibility must be verified with the specific host, host version, operating system, and architecture.

## Build and test

Install the items in [docs/INSTALL_PREREQUISITES.md](docs/INSTALL_PREREQUISITES.md), then use an x64 Native Tools PowerShell:

```powershell
./scripts/configure.ps1
./scripts/build-release.ps1
./scripts/run-tests.ps1
```

JUCE 8.0.14 is fetched at exact commit `2cdfca8feb300fb424002ba2c2751569e5bacb64`. See [docs/BUILDING.md](docs/BUILDING.md) for targets and troubleshooting.

On macOS with Xcode installed:

```bash
./scripts/github-configure-macos.sh
./scripts/github-build-macos.sh
./scripts/github-test-macos.sh
```

## Installation

The release is delivered as installers plus separate portable/manual packages. On Windows, `AuroraBroadcastProcessor-Setup-vX.Y.Z.exe` installs the VST3, standalone application, documentation, shortcuts, and uninstaller. On macOS, the versioned DMG contains the VST3, AU, and standalone application with links to their standard install locations. Detailed steps and uninstallation are in [docs/INSTALLATION.md](docs/INSTALLATION.md).

User presets are stored in `%APPDATA%\Ammar Audio Labs\Aurora Broadcast Processor\Presets` on Windows and under the user's application-data directory on macOS; packages and normal uninstall do not delete them.

## Known limitations and licensing

The initial release has no VST2 or WinAmp DSP adapter. Windows artifacts are unsigned; macOS artifacts are ad-hoc signed and not notarized. No host has yet been validated. Presets require listening tests against legally supplied reference material and downstream codecs. See [docs/KNOWN_LIMITATIONS.md](docs/KNOWN_LIMITATIONS.md).

JUCE 8 is dual-licensed under the JUCE licence and AGPLv3. The owner must select and comply with an applicable licence before distributing binaries. See [LICENSE_NOTICE.md](LICENSE_NOTICE.md) and [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## GitHub Actions artifacts

Open the repository's **Actions** tab and choose **Windows CI**, **macOS CI**, or **Build Cross-Platform Release**. CI produces platform-specific artifacts. A release run produces verified Windows and macOS ZIPs, installers, checksums, manifests, and internal test-result archives documented in [docs/ARTIFACTS.md](docs/ARTIFACTS.md).

## Troubleshooting

- “CMake not found”: install CMake 3.22+ and reopen the terminal.
- “Generator Visual Studio 17 2022 not found”: install VS 2022 Desktop development with C++ and a Windows SDK.
- Plug-in not found: copy the entire `.vst3` directory, rescan VST3 paths, and confirm the host is x64.
- No standalone audio: open audio settings, choose valid input/output devices and matching enabled channels.
- Preset write failure: verify `%APPDATA%` is writable and the preset name is legal.
- macOS blocks an artifact: these initial bundles are ad-hoc signed, not Developer ID signed or notarized; verify the checksum and follow the trusted-release instructions in the installation guide.
