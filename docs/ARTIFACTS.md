# Artifacts

All names use version `X.Y.Z` and SHA-256. Windows is x64/unsigned; macOS is Universal `arm64` + `x86_64`, ad-hoc signed, and not notarized.

| Filename | Purpose/audience | Contents | Public |
|---|---|---|---|
| `AuroraBroadcastProcessor-Windows-x64-vX.Y.Z.zip` | Complete manual install | VST3, standalone, selected docs/notices, version/build info | Yes |
| `AuroraBroadcastProcessor-VST3-vX.Y.Z.zip` | Host users | Complete VST3, install notes/notices/version | Yes |
| `AuroraBroadcastProcessor-Standalone-vX.Y.Z.zip` | No-host users | Standalone, install notes/notices/version | Yes |
| `AuroraBroadcastProcessor-Setup-vX.Y.Z.exe` | Normal Windows install | VST3, standalone, docs, uninstaller | Yes |
| `AuroraBroadcastProcessor-Checksums-vX.Y.Z.txt` | Integrity verification | Sorted SHA-256 lines for versioned assets | Yes |
| `AuroraBroadcastProcessor-ReleaseManifest-vX.Y.Z.json` | Build provenance | Versions, commit/run, test/validator/installer/signing status, sizes/hashes | Yes |
| `AuroraBroadcastProcessor-TestResults-vX.Y.Z.zip` | Engineering review | Test/build/validator diagnostics | Workflow only |
| `AuroraBroadcastProcessor-macOS-Universal-vX.Y.Z.zip` | Complete macOS manual install | Universal VST3, AU, standalone, selected docs/notices/build info | Yes |
| `AuroraBroadcastProcessor-VST3-macOS-Universal-vX.Y.Z.zip` | macOS VST3 hosts | Universal VST3 and installation/licence/version files | Yes |
| `AuroraBroadcastProcessor-AU-macOS-Universal-vX.Y.Z.zip` | macOS Audio Unit hosts | Universal AU component and installation/licence/version files | Yes |
| `AuroraBroadcastProcessor-Standalone-macOS-Universal-vX.Y.Z.zip` | macOS standalone users | Universal application and installation/licence/version files | Yes |
| `AuroraBroadcastProcessor-Setup-macOS-Universal-vX.Y.Z.dmg` | macOS installation image | VST3, AU, app, standard-location links, installation guide | Yes |
| `AuroraBroadcastProcessor-Checksums-macOS-Universal-vX.Y.Z.txt` | macOS integrity | Sorted SHA-256 values for public macOS assets | Yes |
| `AuroraBroadcastProcessor-ReleaseManifest-macOS-Universal-vX.Y.Z.json` | macOS provenance | Toolchain, architectures, tests, signing/notarization status, hashes | Yes |
| `AuroraBroadcastProcessor-TestResults-macOS-Universal-vX.Y.Z.zip` | macOS engineering review | CTest and build diagnostics | Workflow only |

Packages exclude `prompt.txt`, source/workflow files, `.git`, caches, compiler intermediates, Debug output, test audio, local paths, user presets, keys, and certificates.
