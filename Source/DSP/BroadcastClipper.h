#pragma once
#include "Parameters/ParameterSnapshot.h"
#include <memory>
#include <juce_dsp/juce_dsp.h>

namespace aurora
{
class BroadcastClipper
{
public:
    void prepare(double,int,int);void reset();void process(juce::AudioBuffer<float>&,const ParameterSnapshot&) noexcept;int latencySamples() const noexcept{return latency;}bool hadOversampledPeak()const noexcept{return peakWarning;}
private:
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampling;juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear> drive,mix,modeMix;double rate{48000};int latency{};bool peakWarning{};
};
}
