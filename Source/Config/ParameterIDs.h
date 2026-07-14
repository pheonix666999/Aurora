#pragma once
#include <array>

namespace aurora::params
{
inline constexpr auto inputTrimDb = "inputTrimDb";
inline constexpr auto dcBlockEnabled = "dcBlockEnabled";
inline constexpr auto highPassEnabled = "highPassEnabled";
inline constexpr auto highPassFrequencyHz = "highPassFrequencyHz";
inline constexpr auto agcEnabled = "agcEnabled";
inline constexpr auto agcTargetDb = "agcTargetDb";
inline constexpr auto agcMaxGainDb = "agcMaxGainDb";
inline constexpr auto agcMaxAttenuationDb = "agcMaxAttenuationDb";
inline constexpr auto agcAttackMs = "agcAttackMs";
inline constexpr auto agcReleaseMs = "agcReleaseMs";
inline constexpr auto agcGateThresholdDb = "agcGateThresholdDb";
inline constexpr auto agcStereoLinkPercent = "agcStereoLinkPercent";
inline constexpr auto eqEnabled = "eqEnabled";
inline constexpr std::array eqBandEnabled { "eqBand1Enabled", "eqBand2Enabled", "eqBand3Enabled", "eqBand4Enabled", "eqBand5Enabled", "eqBand6Enabled" };
inline constexpr std::array eqBandFrequency { "eqBand1FrequencyHz", "eqBand2FrequencyHz", "eqBand3FrequencyHz", "eqBand4FrequencyHz", "eqBand5FrequencyHz", "eqBand6FrequencyHz" };
inline constexpr std::array eqBandGain { "eqBand1GainDb", "eqBand2GainDb", "eqBand3GainDb", "eqBand4GainDb", "eqBand5GainDb", "eqBand6GainDb" };
inline constexpr std::array eqBandQ { "eqBand1Q", "eqBand2Q", "eqBand3Q", "eqBand4Q", "eqBand5Q", "eqBand6Q" };
inline constexpr auto crossover1Hz = "crossover1Hz";
inline constexpr auto crossover2Hz = "crossover2Hz";
inline constexpr auto crossover3Hz = "crossover3Hz";
inline constexpr auto multibandEnabled = "multibandEnabled";
inline constexpr auto multibandAmountPercent = "multibandAmountPercent";
inline constexpr std::array bandEnabled { "band1Enabled", "band2Enabled", "band3Enabled", "band4Enabled" };
inline constexpr std::array bandThreshold { "band1ThresholdDb", "band2ThresholdDb", "band3ThresholdDb", "band4ThresholdDb" };
inline constexpr std::array bandRatio { "band1Ratio", "band2Ratio", "band3Ratio", "band4Ratio" };
inline constexpr std::array bandAttack { "band1AttackMs", "band2AttackMs", "band3AttackMs", "band4AttackMs" };
inline constexpr std::array bandRelease { "band1ReleaseMs", "band2ReleaseMs", "band3ReleaseMs", "band4ReleaseMs" };
inline constexpr std::array bandKnee { "band1KneeDb", "band2KneeDb", "band3KneeDb", "band4KneeDb" };
inline constexpr std::array bandMakeup { "band1MakeupDb", "band2MakeupDb", "band3MakeupDb", "band4MakeupDb" };
inline constexpr std::array bandMix { "band1MixPercent", "band2MixPercent", "band3MixPercent", "band4MixPercent" };
inline constexpr std::array bandSolo { "band1Solo", "band2Solo", "band3Solo", "band4Solo" };
inline constexpr auto enhancerEnabled = "enhancerEnabled";
inline constexpr auto enhancerDriveDb = "enhancerDriveDb";
inline constexpr auto enhancerMixPercent = "enhancerMixPercent";
inline constexpr auto enhancerTonePercent = "enhancerTonePercent";
inline constexpr auto stereoEnabled = "stereoEnabled";
inline constexpr auto stereoWidthPercent = "stereoWidthPercent";
inline constexpr auto stereoBalancePercent = "stereoBalancePercent";
inline constexpr auto monoBassEnabled = "monoBassEnabled";
inline constexpr auto monoBassFrequencyHz = "monoBassFrequencyHz";
inline constexpr auto correlationProtectionEnabled = "correlationProtectionEnabled";
inline constexpr auto clipperEnabled = "clipperEnabled";
inline constexpr auto clipperDriveDb = "clipperDriveDb";
inline constexpr auto clipperMode = "clipperMode";
inline constexpr auto limiterEnabled = "limiterEnabled";
inline constexpr auto limiterDriveDb = "limiterDriveDb";
inline constexpr auto limiterCeilingDb = "limiterCeilingDb";
inline constexpr auto limiterLookaheadMs = "limiterLookaheadMs";
inline constexpr auto limiterReleaseMs = "limiterReleaseMs";
inline constexpr auto limiterStyle = "limiterStyle";
inline constexpr auto outputTrimDb = "outputTrimDb";
inline constexpr auto invertLeft = "invertLeft";
inline constexpr auto invertRight = "invertRight";
inline constexpr auto globalBypass = "globalBypass";
inline constexpr auto analyzerEnabled = "analyzerEnabled";
inline constexpr auto analyzerMode = "analyzerMode";
inline constexpr auto analyzerSmoothing = "analyzerSmoothing";
inline constexpr auto analyzerFreeze = "analyzerFreeze";
}

