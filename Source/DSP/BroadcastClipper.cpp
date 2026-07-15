#include "BroadcastClipper.h"

namespace aurora
{
void BroadcastClipper::prepare(double sr,int maxBlock,int channels)
{
    rate=sr;oversampling=std::make_unique<juce::dsp::Oversampling<float>>(static_cast<size_t>(channels),2,juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR,true,true);oversampling->setUsingIntegerLatency(true);oversampling->initProcessing(static_cast<size_t>(maxBlock));latency=static_cast<int>(std::round(oversampling->getLatencyInSamples()));drive.reset(rate*4,0.02);mix.reset(rate*4,0.02);modeMix.reset(rate*4,0.02);reset();
}
void BroadcastClipper::reset(){if(oversampling)oversampling->reset();peakWarning=false;}
void BroadcastClipper::process(juce::AudioBuffer<float>& b,const ParameterSnapshot& p) noexcept
{
    if(!oversampling)return;peakWarning=false;drive.setTargetValue(juce::Decibels::decibelsToGain(p.clipperDrive));mix.setTargetValue(p.clipper?1.0f:0.0f);modeMix.setTargetValue(p.clipperMode==0?0.0f:1.0f);juce::dsp::AudioBlock<float> block(b);auto up=oversampling->processSamplesUp(block);
    const auto numSamples=static_cast<int>(up.getNumSamples()),numChannels=static_cast<int>(up.getNumChannels());for(int i=0;i<numSamples;++i){const auto d=drive.getNextValue(),m=mix.getNextValue(),style=modeMix.getNextValue();for(int ch=0;ch<numChannels;++ch){const auto x=up.getSample(ch,i);const auto driven=x*d;peakWarning=peakWarning||std::abs(driven)>1.0f;const auto soft=std::tanh(driven),firm=driven/std::sqrt(1.0f+driven*driven),wet=juce::jmap(style,soft,firm);up.setSample(ch,i,juce::jmap(m,x,wet));}}
    oversampling->processSamplesDown(block);
}
}
