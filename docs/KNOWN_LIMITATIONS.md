# Known limitations

- No host or standalone binary has been tested yet; the build toolchain is unavailable.
- VST3 validation is **VALIDATION NOT RUN — VALIDATOR UNAVAILABLE**.
- RadioBOSS, NextKast, REAPER, and JUCE AudioPluginHost are not installed and are not claimed compatible.
- VST2 and WinAmp DSP are not included.
- The product is digital audio DSP, not RF equipment, a transmitter, FM stereo/RDS generator, or certified loudness/true-peak compliance equipment.
- The oversampled peak stage reduces inter-sample risk but has not received formal true-peak validation.
- Factory presets require later tuning against legally supplied reference stations/programme and intended codecs.
- Results depend on source material, gain staging, sample rate, the host, and downstream encoding; excessive drive can sound dense or distorted.
- Host compatibility varies. Only Windows x64 VST3 and standalone are scoped.
- Initial binaries and installer are UNSIGNED; Windows reputation warnings are expected.
- JUCE licensing must be selected and verified before binary distribution.

