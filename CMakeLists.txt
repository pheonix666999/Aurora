cmake_minimum_required(VERSION 3.22)
include(cmake/Versioning.cmake)
project(AuroraBroadcastProcessor VERSION ${PROJECT_VERSION} LANGUAGES CXX)

include(cmake/Dependencies.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/ProjectOptions.cmake)

juce_add_plugin(AuroraBroadcastProcessor
    COMPANY_NAME "Ammar Audio Labs"
    BUNDLE_ID "com.ammaraudio.aurorabroadcastprocessor"
    PLUGIN_MANUFACTURER_CODE AALB
    PLUGIN_CODE AUBP
    FORMATS VST3 Standalone
    PRODUCT_NAME "Aurora Broadcast Processor"
    DESCRIPTION "Broadcast audio processor for digital and internet radio"
    VST3_CATEGORIES Fx Dynamics EQ
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE)

target_sources(AuroraBroadcastProcessor PRIVATE
    Source/PluginProcessor.cpp Source/PluginProcessor.h
    Source/PluginEditor.cpp Source/PluginEditor.h
    Source/Config/ProductConfig.h Source/Config/ColourPalette.h Source/Config/ParameterIDs.h
    Source/Parameters/ParameterLayout.cpp Source/Parameters/ParameterLayout.h
    Source/Parameters/ParameterSnapshot.cpp Source/Parameters/ParameterSnapshot.h
    Source/DSP/DSPUtilities.cpp Source/DSP/DSPUtilities.h
    Source/DSP/InputStage.cpp Source/DSP/InputStage.h
    Source/DSP/AutoGainController.cpp Source/DSP/AutoGainController.h
    Source/DSP/ParametricEQ.cpp Source/DSP/ParametricEQ.h
    Source/DSP/FourBandCrossover.cpp Source/DSP/FourBandCrossover.h
    Source/DSP/MultibandDynamics.cpp Source/DSP/MultibandDynamics.h
    Source/DSP/HarmonicEnhancer.cpp Source/DSP/HarmonicEnhancer.h
    Source/DSP/StereoProcessor.cpp Source/DSP/StereoProcessor.h
    Source/DSP/BroadcastClipper.cpp Source/DSP/BroadcastClipper.h
    Source/DSP/BroadcastLimiter.cpp Source/DSP/BroadcastLimiter.h
    Source/DSP/MeteringEngine.cpp Source/DSP/MeteringEngine.h
    Source/DSP/BroadcastProcessorEngine.cpp Source/DSP/BroadcastProcessorEngine.h
    Source/Analysis/LockFreeAudioFIFO.h Source/Analysis/FFTAnalyzer.cpp Source/Analysis/FFTAnalyzer.h
    Source/Presets/FactoryPresets.cpp Source/Presets/FactoryPresets.h
    Source/Presets/PresetManager.cpp Source/Presets/PresetManager.h
    Source/UI/AuroraLookAndFeel.cpp Source/UI/AuroraLookAndFeel.h
    Source/UI/SpectrumEQComponent.cpp Source/UI/SpectrumEQComponent.h)

target_include_directories(AuroraBroadcastProcessor PRIVATE Source)
target_link_libraries(AuroraBroadcastProcessor PRIVATE
    juce::juce_audio_utils juce::juce_dsp
    PUBLIC juce::juce_recommended_config_flags juce::juce_recommended_lto_flags)
aurora_apply_options(AuroraBroadcastProcessor)
aurora_enable_warnings(AuroraBroadcastProcessor)
juce_generate_juce_header(AuroraBroadcastProcessor)

enable_testing()
add_subdirectory(Tests)
include(cmake/Packaging.cmake)

