# Installation and removal

Run `AuroraBroadcastProcessor-Setup-vX.Y.Z.exe` as an administrator. This is the normal Windows installer application: it places the complete VST3 bundle in `%CommonProgramFiles%\VST3`, installs the standalone application/documentation under `%ProgramFiles%\Ammar Audio Labs\Aurora Broadcast Processor`, and registers an uninstaller. Start-menu and desktop shortcuts are optional.

For manual VST3 installation, copy the complete `Aurora Broadcast Processor.vst3` directory—not only its inner binary—to `%CommonProgramFiles%\VST3`, then rescan VST3 plug-ins in the x64 host. For portable standalone use, extract the standalone ZIP and run the executable; choose valid input/output devices, sample rate, buffer size, and channels in JUCE audio settings.

Use Windows Installed Apps or the supplied uninstaller to remove installed binaries. Manual users can remove only the matching `.vst3` bundle and application folder. Normal uninstall intentionally preserves `%APPDATA%\Ammar Audio Labs\Aurora Broadcast Processor\Presets`; delete that folder manually only when its user presets are no longer wanted.

## macOS

Open `AuroraBroadcastProcessor-Setup-macOS-Universal-vX.Y.Z.dmg`. The DMG is the macOS installation package; it contains the application and both plug-in formats. Copy:

- `Aurora Broadcast Processor.vst3` to `/Library/Audio/Plug-Ins/VST3` or `~/Library/Audio/Plug-Ins/VST3`.
- `Aurora Broadcast Processor.component` to `/Library/Audio/Plug-Ins/Components` or `~/Library/Audio/Plug-Ins/Components`.
- `Aurora Broadcast Processor.app` to `/Applications` or `~/Applications`.

Restart or rescan the host after installation. The initial CI artifacts are ad-hoc signed and not notarized, so macOS may warn or block them. Verify the published SHA-256 checksum and use only artifacts from a trusted repository release. Production public distribution should use an Apple Developer ID certificate and notarization.

To uninstall, remove only those three Aurora bundles. User presets are stored below `~/Library/Application Support/Ammar Audio Labs/Aurora Broadcast Processor/Presets` and are preserved unless removed manually.
