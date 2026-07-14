# Installation prerequisites

Install on Windows x64:

1. Visual Studio 2022 Community or Build Tools with **Desktop development with C++**, MSVC v143, a current Windows 10/11 SDK, and C++ CMake tools.
2. CMake 3.22 or newer and Git 2.30 or newer on `PATH`.
3. Inno Setup 6.7.1 for local installer builds (release CI provisions the pinned package through Chocolatey).
4. Optional: GitHub CLI, Steinberg VST3 validator, JUCE AudioPluginHost, and target broadcast hosts.

After installation, open a fresh x64 Native Tools PowerShell and run:

```powershell
./scripts/check-environment.ps1
./scripts/configure.ps1
./scripts/build-release.ps1
./scripts/run-tests.ps1
```

JUCE is fetched from its official repository at the exact 8.0.14 commit. Network access is needed only during dependency bootstrap/configuration, not at product runtime.

## macOS

Install Xcode with the macOS SDK and command-line tools. CMake 3.22 or newer, Git, Python 3, `codesign`, `lipo`, `hdiutil`, `shasum`, and `unzip` are required; these Apple/system tools are available on GitHub-hosted `macos-15` runners. Both `arm64` and `x86_64` slices are built in one Xcode Universal configuration.
