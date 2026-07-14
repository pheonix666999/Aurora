#pragma once
#include <array>
#include <juce_audio_processors/juce_audio_processors.h>

namespace aurora
{
struct EQBandParameters { bool enabled {}; float frequency {}, gain {}, q {}; };
struct CompressorParameters { bool enabled {}, solo {}; float threshold {}, ratio {}, attack {}, release {}, knee {}, makeup {}, mix {}; };

struct ParameterSnapshot
{
    explicit ParameterSnapshot(const juce::AudioProcessorValueTreeState&);
    float inputTrim {}, highPassFrequency {}, agcTarget {}, agcMaxGain {}, agcMaxAttenuation {}, agcAttack {}, agcRelease {}, agcGate {}, agcLink {};
    bool dcBlock {}, highPass {}, agc {}, eq {};
    std::array<EQBandParameters, 6> eqBands {};
    std::array<float, 3> crossovers {};
    bool multiband {};
    float multibandAmount {};
    std::array<CompressorParameters, 4> bands {};
    bool enhancer {}, stereo {}, monoBass {}, correlationProtection {}, clipper {}, limiter {}, invertL {}, invertR {}, bypass {}, analyzerEnabled {}, analyzerFreeze {};
    float enhancerDrive {}, enhancerMix {}, enhancerTone {}, width {}, balance {}, monoBassFrequency {};
    float clipperDrive {}, limiterDrive {}, limiterCeiling {}, limiterLookahead {}, limiterRelease {}, outputTrim {};
    int clipperMode {}, limiterStyle {};
};
}
