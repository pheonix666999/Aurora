#include "HarmonicEnhancer.h"

namespace aurora
{
void HarmonicEnhancer::prepare(double sr){rate=sr;for(auto* s:{&drive,&mix,&moduleMix,&tone})s->reset(rate,0.03);reset();}
void HarmonicEnhancer::reset(){dcIn.fill(0);dcOut.fill(0);toneState.fill(0);}
void HarmonicEnhancer::process(juce::AudioBuffer<float>& b,const ParameterSnapshot& p) noexcept
{
    drive.setTargetValue(juce::Decibels::decibelsToGain(p.enhancerDrive));mix.setTargetValue(p.enhancerMix);moduleMix.setTargetValue(p.enhancer?1.0f:0.0f);tone.setTargetValue(p.enhancerTone);const auto channels=juce::jmin(2,b.getNumChannels());
    for(int i=0;i<b.getNumSamples();++i){const auto d=drive.getNextValue(),m=mix.getNextValue()*moduleMix.getNextValue(),t=tone.getNextValue();for(int ch=0;ch<channels;++ch){const auto x=b.getSample(ch,i);toneState[ch]+=0.08f*(x-toneState[ch]);const auto shapedInput=x+t*(x-toneState[ch]);auto wet=std::tanh(shapedInput*d)/juce::jmax(1.0f,d);const auto dc=wet-dcIn[ch]+0.995f*dcOut[ch];dcIn[ch]=wet;dcOut[ch]=dc;b.setSample(ch,i,juce::jmap(m,x,dc));}}
}
}
