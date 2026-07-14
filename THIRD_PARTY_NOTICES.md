# Third-party notices

| Component | Pinned version | Purpose | Licence/notes |
|---|---:|---|---|
| JUCE | 8.0.14 (`2cdfca8…`) | Application, VST3, DSP, GUI, test primitives | JUCE commercial licence or AGPLv3; product owner must select an applicable licence |
| VST3 SDK portions supplied through JUCE | JUCE-pinned | VST3 target support | Steinberg VST3 SDK licence terms in the JUCE dependency apply |
| CMake | 3.22 minimum | Configure/build | Build-time only |
| Visual Studio/MSVC | 2022/v143 | Windows compiler | Build-time Microsoft terms |
| Inno Setup | 6.2.2 | Installer | Build-time; see Inno Setup licence |
| GitHub Actions official actions | Exact commits shown in workflows | CI artifacts/cache | Workflow-only |

No external fonts, icons, audio, test framework, or runtime downloader is bundled. Tests use `juce::UnitTest`.

