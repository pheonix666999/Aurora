# Presets

Factory presets are embedded C++ data, reset every parameter to its declared safe default, then apply moderate overrides. In order: Safe Start (conservative); European Contemporary (tight/clear/assertive); Clean Digital (low coloration); Dance Energy (strong low control); Warm Adult Contemporary (soft top/warm mids); Voice Forward (less masking); Bass Controlled (less limiter pumping); Bright Pop (presence with upper-band control); Low Bitrate Stream (narrower and codec-conscious); Gentle Leveling (slow/restrained).

Save writes a timestamped user preset. Save As exports a portable `.aurorapreset`. Import/load, rename, and delete operations are available through `PresetManager` and validate the Aurora state root before applying. User files live only under `%APPDATA%\Ammar Audio Labs\Aurora Broadcast Processor\Presets`. Factory data stays in the binary. A/B captures complete sound parameter trees; switching captures the side being left and restores the selected side.

The presets are initial engineering settings, not copied station/commercial presets. Tune them with level-matched listening, diverse legal programme material, and the intended downstream encoder before broadcast use.

