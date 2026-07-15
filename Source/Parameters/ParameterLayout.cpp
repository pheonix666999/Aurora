#include "ParameterLayout.h"
#include "Config/ParameterIDs.h"

namespace aurora
{
namespace
{
using Layout = juce::AudioProcessorValueTreeState::ParameterLayout;
auto id(const char* value) { return juce::ParameterID { value, 1 }; }
void addFloat(Layout& layout, const char* parameterId, const char* name, float min, float max, float step, float skew, float initial, const char* unit)
{
    juce::NormalisableRange<float> range { min, max, step };
    if (skew != 1.0f) range.setSkewForCentre(skew);
    layout.add(std::make_unique<juce::AudioParameterFloat>(id(parameterId), name, range, initial, juce::AudioParameterFloatAttributes().withLabel(unit)));
}
void addBool(Layout& layout, const char* parameterId, const char* name, bool initial)
{
    layout.add(std::make_unique<juce::AudioParameterBool>(id(parameterId), name, initial));
}
void addChoice(Layout& layout, const char* parameterId, const char* name, juce::StringArray choices, int initial)
{
    layout.add(std::make_unique<juce::AudioParameterChoice>(id(parameterId), name, choices, initial));
}
}

Layout createParameterLayout()
{
    Layout p;
    addFloat(p, params::inputTrimDb, "Input Trim", -18, 18, 0.1f, 1, 0, "dB");
    addBool(p, params::dcBlockEnabled, "DC Block", true);
    addBool(p, params::highPassEnabled, "High Pass", true);
    addFloat(p, params::highPassFrequencyHz, "High Pass Frequency", 15, 60, 0.1f, 30, 25, "Hz");

    addBool(p, params::agcEnabled, "AGC", true);
    addFloat(p, params::agcTargetDb, "AGC Target", -24, -8, 0.1f, 1, -15, "dBFS");
    addFloat(p, params::agcMaxGainDb, "AGC Maximum Gain", 0, 15, 0.1f, 1, 9, "dB");
    addFloat(p, params::agcMaxAttenuationDb, "AGC Maximum Attenuation", -15, 0, 0.1f, 1, -9, "dB");
    addFloat(p, params::agcAttackMs, "AGC Attack", 100, 2000, 1, 500, 500, "ms");
    addFloat(p, params::agcReleaseMs, "AGC Release", 1000, 12000, 1, 4500, 4500, "ms");
    addFloat(p, params::agcGateThresholdDb, "AGC Gate", -80, -30, 0.1f, 1, -52, "dBFS");
    addFloat(p, params::agcStereoLinkPercent, "AGC Stereo Link", 0, 100, 0.1f, 1, 100, "%");

    addBool(p, params::eqEnabled, "Equalizer", true);
    constexpr float eqMin[] { 30, 80, 250, 1000, 2500, 5000 };
    constexpr float eqMax[] { 250, 1000, 3500, 8000, 14000, 18000 };
    constexpr float eqDefault[] { 90, 280, 1000, 3000, 7000, 11000 };
    for (size_t i = 0; i < params::eqBandEnabled.size(); ++i)
    {
        const auto number = juce::String(static_cast<int>(i + 1));
        addBool(p, params::eqBandEnabled[i], ("EQ Band " + number).toRawUTF8(), true);
        addFloat(p, params::eqBandFrequency[i], ("EQ Band " + number + " Frequency").toRawUTF8(), eqMin[i], eqMax[i], 0.1f, eqDefault[i], eqDefault[i], "Hz");
        addFloat(p, params::eqBandGain[i], ("EQ Band " + number + " Gain").toRawUTF8(), -12, 12, 0.1f, 1, 0, "dB");
        addFloat(p, params::eqBandQ[i], ("EQ Band " + number + " Q").toRawUTF8(), 0.25f, 6.0f, 0.01f, 1, 0.8f, "Q");
    }

    addFloat(p, params::crossover1Hz, "Low Crossover", 60, 300, 1, 120, 120, "Hz");
    addFloat(p, params::crossover2Hz, "Mid Crossover", 400, 3000, 1, 1100, 1100, "Hz");
    addFloat(p, params::crossover3Hz, "High Crossover", 3000, 14000, 1, 6000, 6000, "Hz");
    addBool(p, params::multibandEnabled, "Multiband", true);
    addFloat(p, params::multibandAmountPercent, "Multiband Amount", 0, 100, 0.1f, 1, 70, "%");
    constexpr float thresholds[] { -20, -18, -17, -16 };
    constexpr float ratios[] { 2.4f, 2.0f, 1.8f, 1.6f };
    constexpr float attacks[] { 35, 20, 10, 4 };
    constexpr float releases[] { 280, 220, 160, 110 };
    for (size_t i = 0; i < params::bandEnabled.size(); ++i)
    {
        const auto number = juce::String(static_cast<int>(i + 1));
        addBool(p, params::bandEnabled[i], ("Band " + number).toRawUTF8(), true);
        addFloat(p, params::bandThreshold[i], ("Band " + number + " Threshold").toRawUTF8(), -50, 0, 0.1f, 1, thresholds[i], "dB");
        addFloat(p, params::bandRatio[i], ("Band " + number + " Ratio").toRawUTF8(), 1, 10, 0.01f, 1, ratios[i], ":1");
        addFloat(p, params::bandAttack[i], ("Band " + number + " Attack").toRawUTF8(), 0.5f, 250, 0.1f, 20, attacks[i], "ms");
        addFloat(p, params::bandRelease[i], ("Band " + number + " Release").toRawUTF8(), 20, 2000, 0.1f, 200, releases[i], "ms");
        addFloat(p, params::bandKnee[i], ("Band " + number + " Knee").toRawUTF8(), 0, 18, 0.1f, 1, 6, "dB");
        addFloat(p, params::bandMakeup[i], ("Band " + number + " Makeup").toRawUTF8(), -6, 12, 0.1f, 1, 0, "dB");
        addFloat(p, params::bandMix[i], ("Band " + number + " Mix").toRawUTF8(), 0, 100, 0.1f, 1, 100, "%");
        addBool(p, params::bandSolo[i], ("Band " + number + " Solo").toRawUTF8(), false);
    }

    addBool(p, params::enhancerEnabled, "Enhancer", true);
    addFloat(p, params::enhancerDriveDb, "Enhancer Drive", 0, 12, 0.1f, 1, 2, "dB");
    addFloat(p, params::enhancerMixPercent, "Enhancer Mix", 0, 100, 0.1f, 1, 12, "%");
    addFloat(p, params::enhancerTonePercent, "Enhancer Tone", -100, 100, 0.1f, 1, 0, "%");
    addBool(p, params::stereoEnabled, "Stereo Processing", true);
    addFloat(p, params::stereoWidthPercent, "Stereo Width", 0, 150, 0.1f, 1, 108, "%");
    addFloat(p, params::stereoBalancePercent, "Stereo Balance", -100, 100, 0.1f, 1, 0, "%");
    addBool(p, params::monoBassEnabled, "Mono Bass", true);
    addFloat(p, params::monoBassFrequencyHz, "Mono Bass Frequency", 40, 250, 0.1f, 120, 120, "Hz");
    addBool(p, params::correlationProtectionEnabled, "Correlation Protection", true);

    addBool(p, params::clipperEnabled, "Oversampled Peak", true);
    addFloat(p, params::clipperDriveDb, "Clipper Drive", 0, 9, 0.1f, 1, 1.5f, "dB");
    addChoice(p, params::clipperMode, "Clipper Mode", { "Soft", "Firm" }, 0);
    addBool(p, params::limiterEnabled, "Limiter", true);
    addFloat(p, params::limiterDriveDb, "Limiter Drive", 0, 12, 0.1f, 1, 3, "dB");
    addFloat(p, params::limiterCeilingDb, "Limiter Ceiling", -3, -0.1f, 0.01f, 1, -0.8f, "dBFS");
    addFloat(p, params::limiterLookaheadMs, "Limiter Lookahead", 1, 10, 0.1f, 1, 5, "ms");
    addFloat(p, params::limiterReleaseMs, "Limiter Release", 20, 500, 0.1f, 120, 120, "ms");
    addChoice(p, params::limiterStyle, "Limiter Style", { "Transparent", "Punch" }, 0);
    addFloat(p, params::outputTrimDb, "Output Trim", -18, 6, 0.1f, 1, 0, "dB");
    addBool(p, params::invertLeft, "Invert Left", false);
    addBool(p, params::invertRight, "Invert Right", false);
    addBool(p, params::globalBypass, "Global Bypass", false);
    addBool(p, params::analyzerEnabled, "Analyzer", true);
    addChoice(p, params::analyzerMode, "Analyzer Mode", { "Input", "Output", "Both" }, 2);
    addFloat(p, params::analyzerSmoothing, "Analyzer Smoothing", 0, 100, 1, 1, 65, "%");
    addBool(p, params::analyzerFreeze, "Analyzer Freeze", false);
    return p;
}
}
