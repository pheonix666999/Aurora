#include "StereoProcessor.h"

namespace aurora
{
void StereoProcessor::prepare(double sr){rate=sr;width.reset(rate,0.04);balance.reset(rate,0.04);moduleMix.reset(rate,0.04);bassCutoff.reset(rate,0.05);bassCutoff.setCurrentAndTargetValue(120);reset();}
void StereoProcessor::reset(){bassState.fill(0);correlationSmoothed=1;protection=1;}
float StereoProcessor::process(juce::AudioBuffer<float>& b,const ParameterSnapshot& p) noexcept
{
    if(b.getNumChannels()<2)return 1; width.setTargetValue(p.width);balance.setTargetValue(p.balance);moduleMix.setTargetValue(p.stereo?1.0f:0.0f);bassCutoff.setTargetValue(p.monoBassFrequency);const auto alpha=1.0f-std::exp(-juce::MathConstants<float>::twoPi*bassCutoff.skip(b.getNumSamples())/static_cast<float>(rate));double sumLR=0,sumL2=0,sumR2=0;
    for(int i=0;i<b.getNumSamples();++i)
    {
        const auto inL=b.getSample(0,i),inR=b.getSample(1,i);auto mid=(inL+inR)*0.5f,side=(inL-inR)*0.5f;bassState[0]+=alpha*(mid-bassState[0]);bassState[1]+=alpha*(side-bassState[1]);if(p.monoBass)side-=bassState[1];
        const auto m=moduleMix.getNextValue(),w=juce::jmap(m,1.0f,width.getNextValue()*protection),bal=balance.getNextValue()*m;side*=w;auto l=mid+side,r=mid-side;l*=bal>0?1-bal:1;r*=bal<0?1+bal:1;b.setSample(0,i,juce::jmap(m,inL,l));b.setSample(1,i,juce::jmap(m,inR,r));sumLR+=l*r;sumL2+=l*l;sumR2+=r*r;
    }
    const auto corr=static_cast<float>(sumLR/std::sqrt(juce::jmax(1.0e-12,sumL2*sumR2)));correlationSmoothed=0.95f*correlationSmoothed+0.05f*juce::jlimit(-1.0f,1.0f,corr);const auto target=p.correlationProtection?juce::jmap(juce::jlimit(-0.5f,0.2f,correlationSmoothed),-0.5f,0.2f,0.55f,1.0f):1.0f;protection+=0.002f*(target-protection);return correlationSmoothed;
}
}
