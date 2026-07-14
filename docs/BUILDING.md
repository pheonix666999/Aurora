# Building

From an x64 Native Tools PowerShell at the repository root:

```powershell
./scripts/check-environment.ps1
./scripts/configure.ps1
./scripts/build-debug.ps1
./scripts/build-release.ps1
./scripts/run-tests.ps1
./scripts/run-tests.ps1 -Extended
```

Equivalent direct commands are `cmake --preset windows-x64`, `cmake --build build/windows-x64 --config Release --parallel`, and `ctest --test-dir build/windows-x64 -C Release --output-on-failure`.

Targets are `AuroraBroadcastProcessor_VST3`, `AuroraBroadcastProcessor_Standalone`, `AuroraBroadcastProcessorTests`, and `aurora-package`. FetchContent clones only pinned JUCE commit `2cdfca8…`. Delete only `build/windows-x64` for a clean configure. Release uses MSVC optimization and JUCE LTO flags; project sources use C++20 and `/W4`.

Outputs normally appear below `build/windows-x64/AuroraBroadcastProcessor_artefacts/Release`. If configuration fails before downloading JUCE, verify Git/network access. If MSVC is absent, install the Desktop development with C++ workload rather than changing generators.

## macOS Universal

Install Xcode and its command-line tools, then run on macOS:

```bash
./scripts/github-configure-macos.sh
./scripts/github-build-macos.sh
./scripts/github-test-macos.sh
```

The `macos-universal` preset uses the Xcode generator, deployment target macOS 11.0, and `CMAKE_OSX_ARCHITECTURES=arm64;x86_64`. It builds `AuroraBroadcastProcessor_VST3`, `AuroraBroadcastProcessor_AU`, `AuroraBroadcastProcessor_Standalone`, and `AuroraBroadcastProcessorTests`. Outputs are under `build/macos-universal/AuroraBroadcastProcessor_artefacts/Release`.

For a local macOS package after successful tests:

```bash
./scripts/package-release-macos.sh 0.1.0 PASSED
```

The packaging script verifies both architecture slices, applies an ad-hoc signature, validates bundles and archives, builds a DMG, and checks SHA-256 values. It does not perform Apple Developer ID signing or notarization.
