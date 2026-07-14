#pragma once
#include "Parameters/ParameterSnapshot.h"
#include <array>

namespace aurora
{
class HarmonicEnhancer
{
public:
    void prepare(double); void reset(); void process(juce::AudioBuffer<float>&,const ParameterSnapshot&) noexcept;
private:
    double rate{48000};std::array<float,2> dcIn{},dcOut{},toneState{};juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear> drive,mix,moduleMix,tone;
};
}
