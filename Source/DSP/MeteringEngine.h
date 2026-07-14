#pragma once
#include <array>
#include <atomic>
#include <juce_audio_basics/juce_audio_basics.h>

namespace aurora
{
struct MeteringEngine
{
    void reset() noexcept;
    void measureInput(const juce::AudioBuffer<float>&) noexcept;
    void measureOutput(const juce::AudioBuffer<float>&) noexcept;
    std::array<std::atomic<float>, 2> inputPeak {}, inputRms {}, outputPeak {}, outputRms {};
    std::atomic<float> agcGainDb { 0 }, limiterReductionDb { 0 }, correlation { 1 };
    std::array<std::atomic<float>, 4> bandReductionDb {};
    std::atomic<bool> clipWarning { false }, oversampledPeakWarning { false };
private:
    static void measure(const juce::AudioBuffer<float>&, std::array<std::atomic<float>, 2>&, std::array<std::atomic<float>, 2>&) noexcept;
};
}

