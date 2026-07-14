# GitHub Actions

**Windows CI** runs on pull requests to `main`, pushes to `main`, and manual dispatch with read-only contents permission, branch-ref concurrency cancellation, `windows-2022`, a dependency-source cache keyed by OS/x64/MSVC/JUCE/CMake inputs, Release configuration, all targets/tests, binary verification, and official artifact uploads. It produces `AuroraBroadcastProcessor-Windows-x64-CI` and a separate diagnostics artifact.

**Build Windows Release** runs on `v*.*.*` tags or manual semantic version input. It requires version agreement, configures a clean Release build, runs extended tests, records validator availability/result, provisions pinned Inno Setup 6.2.2 from the trusted Chocolatey package source, builds the installer, packages deterministic ZIP entries, emits metadata/checksums, verifies everything, and uploads `AuroraBroadcastProcessor-Windows-x64-Release-vX.Y.Z`. A cache miss is safe and final binaries are never cached.

**Publish GitHub Release** is manual with write permission only to repository contents. It verifies an existing tag, checks that the supplied run is a successful release build for the exact tag commit, refuses an existing release, downloads that exact artifact, verifies SHA-256, creates a draft, uploads all public assets, and publishes only after successful upload. It does not rebuild.

Official actions are pinned by full commit. PR code never receives write permission, `pull_request_target` is not used, and user-controlled titles are not executed. No signing secret is configured; manifests state UNSIGNED.

Use the Actions tab to dispatch workflows and download artifacts. Diagnose failures from the job log and diagnostics artifact; required build/test/package failures remain failures. The current workspace has no remote, so no workflow has been executed here.

