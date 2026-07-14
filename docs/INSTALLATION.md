# Installation and removal

Run `AuroraBroadcastProcessor-Setup-vX.Y.Z.exe` as an administrator. The installer places the complete VST3 bundle in `%CommonProgramFiles%\VST3` and the standalone application/documentation under `%ProgramFiles%\Ammar Audio Labs\Aurora Broadcast Processor`. Start-menu and desktop shortcuts are optional.

For manual VST3 installation, copy the complete `Aurora Broadcast Processor.vst3` directory—not only its inner binary—to `%CommonProgramFiles%\VST3`, then rescan VST3 plug-ins in the x64 host. For portable standalone use, extract the standalone ZIP and run the executable; choose valid input/output devices, sample rate, buffer size, and channels in JUCE audio settings.

Use Windows Installed Apps or the supplied uninstaller to remove installed binaries. Manual users can remove only the matching `.vst3` bundle and application folder. Normal uninstall intentionally preserves `%APPDATA%\Ammar Audio Labs\Aurora Broadcast Processor\Presets`; delete that folder manually only when its user presets are no longer wanted.

