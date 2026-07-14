#include "ParameterSnapshot.h"
#include "Config/ParameterIDs.h"

namespace aurora
{
ParameterSnapshot::ParameterSnapshot(const juce::AudioProcessorValueTreeState& s)
{
    const auto v = [&s](const char* name) { return s.getRawParameterValue(name)->load(); };
    inputTrim=v(params::inputTrimDb); dcBlock=v(params::dcBlockEnabled)>0.5f; highPass=v(params::highPassEnabled)>0.5f; highPassFrequency=v(params::highPassFrequencyHz);
    agc=v(params::agcEnabled)>0.5f; agcTarget=v(params::agcTargetDb); agcMaxGain=v(params::agcMaxGainDb); agcMaxAttenuation=v(params::agcMaxAttenuationDb); agcAttack=v(params::agcAttackMs); agcRelease=v(params::agcReleaseMs); agcGate=v(params::agcGateThresholdDb); agcLink=v(params::agcStereoLinkPercent);
    eq=v(params::eqEnabled)>0.5f;
    for (size_t i=0;i<eqBands.size();++i) eqBands[i]={v(params::eqBandEnabled[i])>0.5f,v(params::eqBandFrequency[i]),v(params::eqBandGain[i]),v(params::eqBandQ[i])};
    crossovers={v(params::crossover1Hz),v(params::crossover2Hz),v(params::crossover3Hz)};
    multiband=v(params::multibandEnabled)>0.5f; multibandAmount=v(params::multibandAmountPercent)*0.01f;
    for(size_t i=0;i<bands.size();++i) bands[i]={v(params::bandEnabled[i])>0.5f,v(params::bandSolo[i])>0.5f,v(params::bandThreshold[i]),v(params::bandRatio[i]),v(params::bandAttack[i]),v(params::bandRelease[i]),v(params::bandKnee[i]),v(params::bandMakeup[i]),v(params::bandMix[i])*0.01f};
    enhancer=v(params::enhancerEnabled)>0.5f; enhancerDrive=v(params::enhancerDriveDb); enhancerMix=v(params::enhancerMixPercent)*0.01f; enhancerTone=v(params::enhancerTonePercent)*0.01f;
    stereo=v(params::stereoEnabled)>0.5f; width=v(params::stereoWidthPercent)*0.01f; balance=v(params::stereoBalancePercent)*0.01f; monoBass=v(params::monoBassEnabled)>0.5f; monoBassFrequency=v(params::monoBassFrequencyHz); correlationProtection=v(params::correlationProtectionEnabled)>0.5f;
    clipper=v(params::clipperEnabled)>0.5f; clipperDrive=v(params::clipperDriveDb); clipperMode=static_cast<int>(v(params::clipperMode));
    limiter=v(params::limiterEnabled)>0.5f; limiterDrive=v(params::limiterDriveDb); limiterCeiling=v(params::limiterCeilingDb); limiterLookahead=v(params::limiterLookaheadMs); limiterRelease=v(params::limiterReleaseMs); limiterStyle=static_cast<int>(v(params::limiterStyle));
    outputTrim=v(params::outputTrimDb); invertL=v(params::invertLeft)>0.5f; invertR=v(params::invertRight)>0.5f; bypass=v(params::globalBypass)>0.5f; analyzerEnabled=v(params::analyzerEnabled)>0.5f; analyzerFreeze=v(params::analyzerFreeze)>0.5f;
}
}
