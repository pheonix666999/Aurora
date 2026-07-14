#pragma once
#include "Parameters/ParameterSnapshot.h"
#include <array>
#include <juce_dsp/juce_dsp.h>

namespace aurora
{
class InputStage
{
public:
    void prepare(double sampleRate, int channels);
    void reset();
    void process(juce::AudioBuffer<float>&, const ParameterSnapshot&) noexcept;
private:
    double rate { 48000 };
    std::array<float, 2> previousInput {}, previousOutput {};
    std::array<juce::IIRFilter, 2> highPassFilters;
    float lastHighPass { -1 };
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> trim, dcMix, hpMix, highPassCutoff;
};
}
