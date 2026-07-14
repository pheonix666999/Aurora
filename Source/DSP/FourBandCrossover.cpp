#include "FourBandCrossover.h"

namespace aurora
{
void FourBandCrossover::prepare(double sr,int maxBlock,int channels){rate=sr;maxChannels=juce::jmin(2,channels);juce::dsp::ProcessSpec spec{sr,static_cast<juce::uint32>(maxBlock),static_cast<juce::uint32>(maxChannels)};for(auto& f:filters)f.prepare(spec);for(auto& b:bands)b.setSize(maxChannels,maxBlock,false,false,true);for(auto& c:cutoffs)c.reset(rate,0.05);cutoffs[0].setCurrentAndTargetValue(120);cutoffs[1].setCurrentAndTargetValue(1100);cutoffs[2].setCurrentAndTargetValue(6000);reset();}
void FourBandCrossover::reset(){for(auto& f:filters)f.reset();for(auto& b:bands)b.clear();}
void FourBandCrossover::process(const juce::AudioBuffer<float>& input,std::array<float,3> x)
{
    x[0]=juce::jlimit(60.0f,juce::jmin(300.0f,x[1]-100.0f),x[0]); x[1]=juce::jlimit(x[0]+100.0f,juce::jmin(3000.0f,x[2]-500.0f),x[1]); x[2]=juce::jlimit(x[1]+500.0f,static_cast<float>(juce::jmin(14000.0,rate*0.45)),x[2]);
    for(size_t i=0;i<3;++i){cutoffs[i].setTargetValue(x[i]);x[i]=cutoffs[i].skip(input.getNumSamples());}filters[0].setCutoffFrequency(x[0]);filters[1].setCutoffFrequency(x[1]);filters[2].setCutoffFrequency(x[2]);
    const int channels=juce::jmin(maxChannels,input.getNumChannels()), n=input.getNumSamples();for(auto& b:bands){b.setSize(channels,n,false,false,true);b.clear();}
    for(int ch=0;ch<channels;++ch)for(int i=0;i<n;++i)
    {
        float lower{},upper{},low{},lowMid{},highMid{},high{};filters[1].processSample(ch,input.getSample(ch,i),lower,upper);filters[0].processSample(ch,lower,low,lowMid);filters[2].processSample(ch,upper,highMid,high);
        bands[0].setSample(ch,i,low);bands[1].setSample(ch,i,lowMid);bands[2].setSample(ch,i,highMid);bands[3].setSample(ch,i,high);
    }
}
}
