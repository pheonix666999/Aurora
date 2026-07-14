#pragma once
#include "Parameters/ParameterSnapshot.h"
#include <array>

namespace aurora
{
class AutoGainController
{
public:
    void prepare(double sampleRate); void reset();
    float process(juce::AudioBuffer<float>&, const ParameterSnapshot&) noexcept;
private:
    double rate { 48000 }; std::array<float,2> rmsSquared {}, gainDb {};
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> bypassMix;
};
}

