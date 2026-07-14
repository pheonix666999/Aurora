#include "MultibandDynamics.h"
#include "DSPUtilities.h"

namespace aurora
{
void MultibandDynamics::prepare(double sr){rate=sr;moduleMix.reset(rate,0.04);for(int i=0;i<4;++i){soloMix[i].reset(rate,0.02);soloMix[i].setCurrentAndTargetValue(1);makeupGain[i].reset(rate,0.02);makeupGain[i].setCurrentAndTargetValue(1);parallelMix[i].reset(rate,0.02);parallelMix[i].setCurrentAndTargetValue(1);}reset();}
void MultibandDynamics::reset(){envelopes.fill(0);reductions.fill(0);}
std::array<float,4> MultibandDynamics::process(std::array<juce::AudioBuffer<float>,4>& bands,juce::AudioBuffer<float>& out,const ParameterSnapshot& p) noexcept
{
    out.clear();moduleMix.setTargetValue(p.multiband?p.multibandAmount:0.0f);int solo=-1;for(int b=0;b<4;++b)if(p.bands[b].solo){solo=b;break;}for(int b=0;b<4;++b){soloMix[b].setTargetValue(solo<0||solo==b?1.0f:0.0f);makeupGain[b].setTargetValue(juce::Decibels::decibelsToGain(p.bands[b].makeup));parallelMix[b].setTargetValue(p.bands[b].mix);}
    const int channels=out.getNumChannels(),n=out.getNumSamples();
    for(int i=0;i<n;++i)
    {
        const auto amount=moduleMix.getNextValue();
        for(int band=0;band<4;++band)
        {
            const auto& bp=p.bands[band];const auto audibleGain=soloMix[band].getNextValue(),makeup=makeupGain[band].getNextValue(),bandMix=parallelMix[band].getNextValue();float detector=0;for(int ch=0;ch<channels;++ch)detector=juce::jmax(detector,std::abs(bands[band].getSample(ch,i)));
            const auto inputDb=juce::Decibels::gainToDecibels(detector,-100.0f);const auto target=bp.enabled?dsp::softKneeGainDb(inputDb,bp.threshold,bp.ratio,bp.knee):0.0f;
            const auto a=std::exp(-1.0f/static_cast<float>(rate*bp.attack*0.001)),r=std::exp(-1.0f/static_cast<float>(rate*bp.release*0.001));const auto c=target<envelopes[band]?a:r;envelopes[band]=c*envelopes[band]+(1-c)*target;reductions[band]=-envelopes[band];
            const auto compressedGain=juce::Decibels::decibelsToGain(envelopes[band])*makeup;const auto wetGain=juce::jmap(bandMix,1.0f,compressedGain);const auto gain=juce::jmap(amount,1.0f,wetGain);
            if(audibleGain>0.0f)for(int ch=0;ch<channels;++ch)out.addSample(ch,i,bands[band].getSample(ch,i)*gain*audibleGain);
        }
    }
    return reductions;
}
}
