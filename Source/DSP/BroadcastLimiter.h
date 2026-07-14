#pragma once
#include "Parameters/ParameterSnapshot.h"
#include <array>
#include <vector>

namespace aurora
{
class BroadcastLimiter
{
public:
    void prepare(double,int);void reset();float process(juce::AudioBuffer<float>&,const ParameterSnapshot&) noexcept;int latencySamples()const noexcept{return maximumDelay;}
private:
    double rate{48000};int maximumDelay{},writeIndex{};std::array<std::vector<float>,2> audioDelay;std::vector<float> detectorDelay;float gain{1};juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear> moduleMix,drive,ceiling,lookaheadSamples;
};
}
