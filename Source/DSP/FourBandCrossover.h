#pragma once
#include <array>
#include <juce_dsp/juce_dsp.h>

namespace aurora
{
class FourBandCrossover
{
public:
    void prepare(double,int,int); void reset(); void process(const juce::AudioBuffer<float>&,std::array<float,3>);
    std::array<juce::AudioBuffer<float>,4>& getBands() noexcept{return bands;}
private:
    double rate{48000}; int maxChannels{2}; std::array<juce::dsp::LinkwitzRileyFilter<float>,3> filters;std::array<juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear>,3> cutoffs; std::array<juce::AudioBuffer<float>,4> bands;
};
}
