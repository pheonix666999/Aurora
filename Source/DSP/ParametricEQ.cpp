#include "ParametricEQ.h"

namespace aurora
{
void ParametricEQ::prepare(double sr,int maxBlock,int channels){rate=sr;dry.setSize(juce::jmin(2,channels),maxBlock,false,false,true);mix.reset(rate,0.03);for(size_t i=0;i<6;++i){last[i][0]=-1;initialised[i]=false;frequencies[i].reset(rate,0.03);gains[i].reset(rate,0.03);qualities[i].reset(rate,0.03);}reset();}
void ParametricEQ::reset(){for(auto& band:filters)for(auto& f:band)f.reset();}
void ParametricEQ::update(const ParameterSnapshot& p,int samples)
{
    for(size_t i=0;i<filters.size();++i)
    {
        const auto& b=p.eqBands[i];if(!initialised[i]){frequencies[i].setCurrentAndTargetValue(b.frequency);gains[i].setCurrentAndTargetValue(b.gain);qualities[i].setCurrentAndTargetValue(b.q);initialised[i]=true;}frequencies[i].setTargetValue(b.frequency);gains[i].setTargetValue(b.gain);qualities[i].setTargetValue(b.q);const auto frequency=frequencies[i].skip(samples),gain=gains[i].skip(samples),q=qualities[i].skip(samples);if(last[i][0]==frequency&&last[i][1]==gain&&last[i][2]==q&&lastEnabled[i]==b.enabled)continue;
        if (! b.enabled) { last[i][0]=frequency;last[i][1]=gain;last[i][2]=q;lastEnabled[i]=false;continue; }
        juce::IIRCoefficients c;
        if(i==0)c=juce::IIRCoefficients::makeLowShelf(rate,frequency,0.707f,juce::Decibels::decibelsToGain(gain));
        else if(i==5)c=juce::IIRCoefficients::makeHighShelf(rate,frequency,0.707f,juce::Decibels::decibelsToGain(gain));
        else c=juce::IIRCoefficients::makePeakFilter(rate,frequency,q,juce::Decibels::decibelsToGain(gain));
        for(auto& f:filters[i])f.setCoefficients(c); last[i][0]=frequency;last[i][1]=gain;last[i][2]=q;lastEnabled[i]=b.enabled;
    }
}
void ParametricEQ::process(juce::AudioBuffer<float>& b,const ParameterSnapshot& p) noexcept
{
    update(p,b.getNumSamples());mix.setTargetValue(p.eq?1.0f:0.0f);const int channels=juce::jmin(2,b.getNumChannels());dry.setSize(channels,b.getNumSamples(),false,false,true);
    for(int ch=0;ch<channels;++ch)dry.copyFrom(ch,0,b,ch,0,b.getNumSamples());
    for(size_t band=0;band<filters.size();++band)if(p.eqBands[band].enabled)for(int ch=0;ch<channels;++ch)filters[band][ch].processSamples(b.getWritePointer(ch),b.getNumSamples());
    for(int i=0;i<b.getNumSamples();++i){const auto m=mix.getNextValue();for(int ch=0;ch<channels;++ch)b.setSample(ch,i,juce::jmap(m,dry.getSample(ch,i),b.getSample(ch,i)));}
}
}
