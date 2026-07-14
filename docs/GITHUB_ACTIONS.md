# GitHub Actions

The repository has four workflows. All third-party actions are pinned to exact commits, pull-request jobs have read-only repository permission, generated binaries are never cached, and a cache miss remains safe.

## Continuous integration

**Windows CI** runs for pull requests to `main`, pushes to `main`, and manual dispatch on `windows-2022`. It configures a Release x64 build, builds VST3, standalone, and tests, executes CTest, verifies the binaries, and uploads a compact CI package plus diagnostics.

**macOS CI** runs for the same events on the supported Apple Silicon `macos-15` image. It uses the Xcode Universal preset to build `arm64` and `x86_64` slices for VST3, Audio Unit, standalone, and tests. It runs CTest, ad-hoc signs the bundles, verifies signatures, property lists, architectures, ZIP contents, checksums, manifest fields, and the DMG, then uploads the platform package set plus diagnostics.

## Release build

**Build Cross-Platform Release** runs when a `vX.Y.Z` tag is pushed or when manually dispatched with `X.Y.Z`. The requested version must match `VERSION.txt`.

- The Windows job builds and tests x64 VST3 and standalone, records VST3 validator status, provisions pinned Inno Setup 6.7.1, creates the installer and deterministic ZIPs, and verifies all outputs and SHA-256 values.
- The macOS job builds and tests Universal VST3, Audio Unit, and standalone bundles, ad-hoc signs them, creates deterministic ZIPs and a compressed DMG, and verifies signatures, both architecture slices, archive contents, manifests, and SHA-256 values.
- The final gate succeeds only if both platform jobs succeed. Each platform artifact includes its internal test-results archive; those diagnostic archives are deliberately excluded from public checksum lists and releases.

No signing secrets are required. Windows artifacts are unsigned. macOS bundles are ad-hoc signed and the DMG is not notarized; production public distribution should configure Developer ID signing and Apple notarization separately.

## Publishing a release

**Publish GitHub Release** is a separate, manual workflow and is the only workflow with `contents: write`. Supply an existing `vX.Y.Z` tag and the run ID of a successful **Build Cross-Platform Release** run. It checks that the run name, conclusion, and commit match the exact tag, refuses to overwrite an existing release, downloads both platform artifacts, requires every named public file exactly once, verifies both SHA-256 files, uploads a draft release, and publishes only after every upload succeeds. It never rebuilds binaries.

## Typical release sequence

1. Update `VERSION.txt` and `CHANGELOG.md`, then merge only after both CI workflows pass.
2. Create and push the matching tag, for example `git tag v0.1.0` followed by `git push origin v0.1.0`.
3. Wait for **Build Cross-Platform Release** to succeed and note its run ID from the Actions page or run URL.
4. Manually run **Publish GitHub Release** with the tag and run ID.
5. Download the release assets and independently verify the platform checksum files before distribution.

The current local workspace has no configured Git remote, so these workflows have not been executed here. Static parsing can confirm workflow and script structure, but the first pushed run remains the authoritative compiler, test, packaging, and runner-image validation.
