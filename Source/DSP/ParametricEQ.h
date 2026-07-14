#pragma once
#include "Parameters/ParameterSnapshot.h"
#include <array>
#include <juce_dsp/juce_dsp.h>

namespace aurora
{
class ParametricEQ
{
public:
    void prepare(double sampleRate,int maxBlockSize,int channels); void reset();
    void process(juce::AudioBuffer<float>&,const ParameterSnapshot&) noexcept;
private:
    void update(const ParameterSnapshot&,int);
    double rate{48000}; float last[6][3]{}; bool lastEnabled[6]{},initialised[6]{};std::array<juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear>,6> frequencies,gains,qualities;
    std::array<std::array<juce::IIRFilter,2>,6> filters;
    juce::AudioBuffer<float> dry; juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear> mix;
};
}
