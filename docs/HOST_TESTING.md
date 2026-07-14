# Host testing

Designed for compatible Windows x64 VST3 hosts and macOS VST3/Audio Unit hosts. Compatibility must be verified with the specific host and host version.

| Host | Version/arch/format | Result |
|---|---|---|
| RadioBOSS | unavailable | NOT TESTED — HOST UNAVAILABLE |
| NextKast | unavailable | NOT TESTED — HOST UNAVAILABLE |
| REAPER | unavailable | NOT TESTED — HOST UNAVAILABLE |
| JUCE AudioPluginHost | unavailable | NOT TESTED — HOST UNAVAILABLE |
| Standalone application | build unavailable | NOT TESTED — BUILD TOOLCHAIN UNAVAILABLE |
| Other VST3 host | unavailable | NOT TESTED — HOST UNAVAILABLE |
| Logic Pro (Audio Unit) | unavailable | NOT TESTED — macOS HOST UNAVAILABLE |
| GarageBand (Audio Unit) | unavailable | NOT TESTED — macOS HOST UNAVAILABLE |
| REAPER macOS (VST3/AU) | unavailable | NOT TESTED — macOS HOST UNAVAILABLE |
| macOS standalone | build unavailable | NOT TESTED — macOS BUILD ENVIRONMENT UNAVAILABLE |

For every available host record its exact version, architecture, format, scan result, editor/audio/bypass/preset/state behavior, closed-editor processing, CPU stability, sample-rate changes, unload/reload, and an extended playback result. Test mono and stereo at all supported rates and include downstream encoder monitoring. A row may be changed to PASS only after all checks actually run.
