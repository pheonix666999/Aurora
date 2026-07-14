#pragma once
#include "InputStage.h"
#include "AutoGainController.h"
#include "ParametricEQ.h"
#include "FourBandCrossover.h"
#include "MultibandDynamics.h"
#include "HarmonicEnhancer.h"
#include "StereoProcessor.h"
#include "BroadcastClipper.h"
#include "BroadcastLimiter.h"
#include "MeteringEngine.h"
#include "Analysis/LockFreeAudioFIFO.h"
#include <vector>

namespace aurora
{
class BroadcastProcessorEngine
{
public:
    void prepare(double,int,int);void reset();void process(juce::AudioBuffer<float>&,const ParameterSnapshot&) noexcept;
    int latencySamples()const noexcept{return totalLatency;} MeteringEngine& meters()noexcept{return metering;} AnalyzerFIFO& inputAnalyzer()noexcept{return inputFifo;} AnalyzerFIFO& outputAnalyzer()noexcept{return outputFifo;} void setAnalyzerConsumerActive(bool active)noexcept{analyzerConsumerActive.store(active);}
private:
    void delayBypass(juce::AudioBuffer<float>&) noexcept;
    InputStage input;AutoGainController agc;ParametricEQ eq;FourBandCrossover crossover;MultibandDynamics multiband;HarmonicEnhancer enhancer;StereoProcessor stereo;BroadcastClipper clipper;BroadcastLimiter limiter;MeteringEngine metering;AnalyzerFIFO inputFifo,outputFifo;
    juce::AudioBuffer<float> bypass;std::array<std::vector<float>,2>bypassDelay;int bypassIndex{},totalLatency{};double rate{48000};std::atomic<bool> analyzerConsumerActive{false};juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear> outputGain,bypassMix,polarityL,polarityR;
};
}
