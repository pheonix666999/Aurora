#include "BroadcastLimiter.h"

namespace aurora
{
void BroadcastLimiter::prepare(double sr,int maxBlock){rate=sr;maximumDelay=static_cast<int>(std::ceil(sr*0.010))+2;const auto size=static_cast<size_t>(maximumDelay+maxBlock+2);for(auto& d:audioDelay)d.assign(size,0.0f);detectorDelay.assign(size,0.0f);for(auto* s:{&moduleMix,&drive,&ceiling,&lookaheadSamples})s->reset(rate,0.02);lookaheadSamples.setCurrentAndTargetValue(static_cast<float>(sr*0.005));reset();}
void BroadcastLimiter::reset(){for(auto& d:audioDelay)std::fill(d.begin(),d.end(),0.0f);std::fill(detectorDelay.begin(),detectorDelay.end(),0.0f);writeIndex=0;gain=1.0f;}
float BroadcastLimiter::process(juce::AudioBuffer<float>& b,const ParameterSnapshot& p) noexcept
{
    moduleMix.setTargetValue(p.limiter?1.0f:0.0f);drive.setTargetValue(juce::Decibels::decibelsToGain(p.limiterDrive));ceiling.setTargetValue(juce::Decibels::decibelsToGain(p.limiterCeiling));lookaheadSamples.setTargetValue(static_cast<float>(rate*p.limiterLookahead*0.001));const int channels=juce::jmin(2,b.getNumChannels()),size=static_cast<int>(detectorDelay.size());float maxReduction=0;
    for(int i=0;i<b.getNumSamples();++i)
    {
        float peak=0;for(int ch=0;ch<channels;++ch){const auto x=b.getSample(ch,i);audioDelay[ch][writeIndex]=x;peak=juce::jmax(peak,std::abs(x));}detectorDelay[writeIndex]=peak;
        const auto lookahead=juce::jlimit(1,maximumDelay-1,static_cast<int>(lookaheadSamples.getNextValue()));const auto audioRead=(writeIndex-maximumDelay+size)%size,detectorRead=(writeIndex-(maximumDelay-lookahead)+size)%size;const auto d=drive.getNextValue(),c=ceiling.getNextValue();const auto target=juce::jmin(1.0f,c/juce::jmax(1.0e-9f,detectorDelay[detectorRead]*d));const auto release=std::exp(-1.0f/static_cast<float>(rate*p.limiterRelease*0.001*(p.limiterStyle==0?1.0f:0.65f)));gain=target<gain?target:release*gain+(1-release)*target;const auto m=moduleMix.getNextValue();
        for(int ch=0;ch<channels;++ch){const auto dry=audioDelay[ch][audioRead];const auto wet=juce::jlimit(-c,c,dry*d*gain);b.setSample(ch,i,juce::jmap(m,dry,wet));}maxReduction=juce::jmax(maxReduction,-juce::Decibels::gainToDecibels(gain));writeIndex=(writeIndex+1)%size;
    }
    return maxReduction;
}
}
