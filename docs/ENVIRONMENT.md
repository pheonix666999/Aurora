# Development environment inspection

Inspection date: 2026-07-14 (Asia/Karachi)

| Item | Actual result |
|---|---|
| Operating system | Windows 11, build 26100 (x64 host) |
| PowerShell | Windows PowerShell 5.1.26100.8737 |
| Git | 2.50.0.windows.1 |
| Repository | Newly initialized in the workspace; no prior history |
| Git remotes | None configured |
| CMake | Not installed / not on `PATH` |
| Visual Studio 2022 | Not installed (no installation directory or `vswhere`) |
| MSVC x64 | Not installed / not on `PATH` |
| Windows SDK | No installed SDK was detected |
| Ninja | Not installed / not on `PATH` |
| Inno Setup | Not installed / not on `PATH` |
| GitHub CLI | Not installed / not on `PATH` |
| JUCE source | Not initially present. An ignored temporary API-audit clone was fetched successfully at JUCE 8.0.14 commit `2cdfca8feb300fb424002ba2c2751569e5bacb64`; CMake FetchContent is pinned to the same commit. |
| VST3 validator | Not found |
| JUCE AudioPluginHost | Not found |
| RadioBOSS | Not found |
| NextKast | Not found |
| REAPER | Not found |
| Other VST3 test host | None found |

## Current blocker

Local CMake configuration, compilation, tests, VST3 validation, installer compilation, and performance measurement cannot run until Visual Studio 2022 Build Tools with the Desktop development with C++ workload, a Windows 10/11 SDK, and CMake are installed. No successful build or test result is claimed.

GitHub Actions also cannot be executed from this workspace because no Git remote is configured and GitHub CLI is unavailable.
