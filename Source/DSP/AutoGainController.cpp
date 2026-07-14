#include "AutoGainController.h"

namespace aurora
{
void AutoGainController::prepare(double sampleRate){rate=sampleRate;bypassMix.reset(rate,0.05);reset();}
void AutoGainController::reset(){rmsSquared.fill(0);gainDb.fill(0);}
float AutoGainController::process(juce::AudioBuffer<float>& b,const ParameterSnapshot& p) noexcept
{
    bypassMix.setTargetValue(p.agc?1.0f:0.0f);
    const auto detectorCoeff=std::exp(-1.0f/static_cast<float>(rate*0.35));
    const auto attackCoeff=std::exp(-1.0f/static_cast<float>(rate*p.agcAttack*0.001));
    const auto releaseCoeff=std::exp(-1.0f/static_cast<float>(rate*p.agcRelease*0.001));
    const int channels=juce::jmin(2,b.getNumChannels());
    for(int i=0;i<b.getNumSamples();++i)
    {
        float levelDb[2] {-100,-100};
        for(int ch=0;ch<channels;++ch){const auto x=b.getSample(ch,i);rmsSquared[ch]=detectorCoeff*rmsSquared[ch]+(1-detectorCoeff)*x*x;levelDb[ch]=juce::Decibels::gainToDecibels(std::sqrt(rmsSquared[ch]),-100.0f);}
        const auto linked=juce::jmax(levelDb[0],channels>1?levelDb[1]:levelDb[0]);
        const auto mix=bypassMix.getNextValue();
        for(int ch=0;ch<channels;++ch)
        {
            const auto detector=juce::jmap(p.agcLink*0.01f,levelDb[ch],linked);
            auto target=detector>p.agcGate?juce::jlimit(p.agcMaxAttenuation,p.agcMaxGain,p.agcTarget-detector):gainDb[ch];
            const auto coeff=target<gainDb[ch]?attackCoeff:releaseCoeff; gainDb[ch]=coeff*gainDb[ch]+(1-coeff)*target;
            b.setSample(ch,i,b.getSample(ch,i)*juce::Decibels::decibelsToGain(gainDb[ch]*mix));
        }
    }
    return (gainDb[0]+(channels>1?gainDb[1]:gainDb[0]))*0.5f;
}
}

