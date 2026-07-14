# Artifacts

All names use version `X.Y.Z`, Windows x64, SHA-256, and UNSIGNED status until signing is configured.

| Filename | Purpose/audience | Contents | Public |
|---|---|---|---|
| `AuroraBroadcastProcessor-Windows-x64-vX.Y.Z.zip` | Complete manual install | VST3, standalone, selected docs/notices, version/build info | Yes |
| `AuroraBroadcastProcessor-VST3-vX.Y.Z.zip` | Host users | Complete VST3, install notes/notices/version | Yes |
| `AuroraBroadcastProcessor-Standalone-vX.Y.Z.zip` | No-host users | Standalone, install notes/notices/version | Yes |
| `AuroraBroadcastProcessor-Setup-vX.Y.Z.exe` | Normal Windows install | VST3, standalone, docs, uninstaller | Yes |
| `AuroraBroadcastProcessor-Checksums-vX.Y.Z.txt` | Integrity verification | Sorted SHA-256 lines for versioned assets | Yes |
| `AuroraBroadcastProcessor-ReleaseManifest-vX.Y.Z.json` | Build provenance | Versions, commit/run, test/validator/installer/signing status, sizes/hashes | Yes |
| `AuroraBroadcastProcessor-TestResults-vX.Y.Z.zip` | Engineering review | Test/build/validator diagnostics | Workflow only |

Packages exclude `prompt.txt`, source/workflow files, `.git`, caches, compiler intermediates, Debug output, test audio, local paths, user presets, keys, and certificates.

