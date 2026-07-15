#include "FactoryPresets.h"
#include "Config/ParameterIDs.h"

namespace aurora
{
const std::vector<FactoryPreset>& factoryPresets()
{
    static const std::vector<FactoryPreset> presets{
        {"Safe Start","Conservative processing and safe output.",{{params::agcTargetDb,-16.0f},{params::multibandAmountPercent,50.0f},{params::limiterDriveDb,1.5f},{params::limiterCeilingDb,-1.0f}}},
        {"European Contemporary","Tight bass, clear vocals, controlled high mids, mild widening, and moderately assertive limiting.",{{params::eqBandGain[0],1.8f},{params::eqBandGain[3],1.2f},{params::stereoWidthPercent,112.0f},{params::limiterDriveDb,4.0f},{params::multibandAmountPercent,78.0f}}},
        {"Clean Digital","Minimal coloration, light AGC, gentle compression, and transparent limiting.",{{params::agcTargetDb,-17.0f},{params::multibandAmountPercent,38.0f},{params::enhancerMixPercent,5.0f},{params::stereoWidthPercent,100.0f},{params::limiterDriveDb,1.5f}}},
        {"Dance Energy","Stronger low-band control, energetic presence, and firmer output processing.",{{params::bandThreshold[0],-24.0f},{params::bandRatio[0],3.2f},{params::eqBandGain[0],2.2f},{params::eqBandGain[3],1.5f},{params::limiterDriveDb,5.0f}}},
        {"Warm Adult Contemporary","Warmer low mids, softer upper frequencies, and relaxed limiting.",{{params::eqBandGain[1],1.5f},{params::eqBandGain[5],-1.2f},{params::enhancerTonePercent,-22.0f},{params::limiterDriveDb,2.0f}}},
        {"Voice Forward","Reduced bass masking and increased vocal clarity.",{{params::eqBandGain[1],-2.0f},{params::eqBandGain[2],1.4f},{params::eqBandGain[3],1.8f},{params::monoBassFrequencyHz,140.0f}}},
        {"Bass Controlled","Stronger low-band control intended to reduce limiter pumping.",{{params::bandThreshold[0],-26.0f},{params::bandRatio[0],3.8f},{params::bandAttack[0],45.0f},{params::bandRelease[0],340.0f},{params::eqBandGain[0],-0.8f}}},
        {"Bright Pop","Clear top end and presence with harshness safeguards.",{{params::eqBandGain[3],1.4f},{params::eqBandGain[5],1.7f},{params::bandThreshold[3],-20.0f},{params::bandRatio[3],2.0f}}},
        {"Low Bitrate Stream","Controlled high frequencies, reduced stereo width, and safer codec-friendly output.",{{params::eqBandGain[5],-2.0f},{params::stereoWidthPercent,88.0f},{params::monoBassFrequencyHz,155.0f},{params::limiterCeilingDb,-1.2f},{params::clipperDriveDb,0.8f}}},
        {"Gentle Leveling","Slow AGC and restrained multiband compression.",{{params::agcAttackMs,1000.0f},{params::agcReleaseMs,8000.0f},{params::multibandAmountPercent,28.0f},{params::limiterDriveDb,1.0f}}}
    };return presets;
}
}
