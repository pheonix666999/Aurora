#pragma once
#include "Parameters/ParameterSnapshot.h"
#include <array>

namespace aurora
{
class StereoProcessor
{
public:
    void prepare(double);void reset();float process(juce::AudioBuffer<float>&,const ParameterSnapshot&) noexcept;
private:
    double rate{48000};std::array<float,2> bassState{};float correlationSmoothed{1},protection{1};bool parametersInitialised{};juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear> width,balance,moduleMix,bassCutoff;
};
}
