#include "InputStage.h"
#include "DSPUtilities.h"

namespace aurora
{
void InputStage::prepare(double sampleRate, int channels)
{
    juce::ignoreUnused(channels); rate=sampleRate; trim.reset(rate,0.03); dcMix.reset(rate,0.02); hpMix.reset(rate,0.02); highPassCutoff.reset(rate,0.05);highPassCutoff.setCurrentAndTargetValue(25.0f);reset();
}
void InputStage::reset()
{
    previousInput.fill(0); previousOutput.fill(0); for(auto& f:highPassFilters) f.reset(); lastHighPass=-1;
}
void InputStage::process(juce::AudioBuffer<float>& buffer, const ParameterSnapshot& p) noexcept
{
    trim.setTargetValue(juce::Decibels::decibelsToGain(p.inputTrim)); dcMix.setTargetValue(p.dcBlock?1.0f:0.0f); hpMix.setTargetValue(p.highPass?1.0f:0.0f);highPassCutoff.setTargetValue(p.highPassFrequency);const auto smoothedCutoff=highPassCutoff.skip(buffer.getNumSamples());
    if (std::abs(lastHighPass-smoothedCutoff)>0.01f)
    {
        const auto c=juce::IIRCoefficients::makeHighPass(rate,smoothedCutoff);
        for(auto& f:highPassFilters) f.setCoefficients(c); lastHighPass=smoothedCutoff;
    }
    const auto channels=juce::jmin(2,buffer.getNumChannels());
    for(int i=0;i<buffer.getNumSamples();++i)
    {
        const auto gain=trim.getNextValue(), dm=dcMix.getNextValue(), hm=hpMix.getNextValue();
        for(int ch=0;ch<channels;++ch)
        {
            auto x=dsp::sanitise(buffer.getSample(ch,i))*gain;
            const auto dc=x-previousInput[static_cast<size_t>(ch)]+0.995f*previousOutput[static_cast<size_t>(ch)];
            previousInput[static_cast<size_t>(ch)]=x; previousOutput[static_cast<size_t>(ch)]=dc;
            x=juce::jmap(dm,x,dc);
            const auto hp=highPassFilters[static_cast<size_t>(ch)].processSingleSampleRaw(x);
            buffer.setSample(ch,i,juce::jmap(hm,x,hp));
        }
    }
}
}
