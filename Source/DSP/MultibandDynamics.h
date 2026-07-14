#pragma once
#include "Parameters/ParameterSnapshot.h"
#include <array>

namespace aurora
{
class MultibandDynamics
{
public:
    void prepare(double); void reset(); std::array<float,4> process(std::array<juce::AudioBuffer<float>,4>&,juce::AudioBuffer<float>&,const ParameterSnapshot&) noexcept;
private:
    double rate{48000};std::array<float,4> envelopes{},reductions{};std::array<juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear>,4> soloMix,makeupGain,parallelMix;juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear> moduleMix;
};
}
