#include "MeteringEngine.h"

namespace aurora
{
void MeteringEngine::reset() noexcept
{
    for (auto* values : { &inputPeak, &inputRms, &outputPeak, &outputRms }) for (auto& v : *values) v.store(0);
    for (auto& v : bandReductionDb) v.store(0);
    agcGainDb.store(0); limiterReductionDb.store(0); correlation.store(1); clipWarning.store(false); oversampledPeakWarning.store(false);
}

void MeteringEngine::measure(const juce::AudioBuffer<float>& b, std::array<std::atomic<float>, 2>& peaks, std::array<std::atomic<float>, 2>& rms) noexcept
{
    for (int ch = 0; ch < 2; ++ch)
    {
        if (ch < b.getNumChannels() && b.getNumSamples() > 0)
        {
            const auto peak = b.getMagnitude(ch, 0, b.getNumSamples());
            const auto level = b.getRMSLevel(ch, 0, b.getNumSamples());
            peaks[static_cast<size_t>(ch)].store(peak, std::memory_order_relaxed);
            rms[static_cast<size_t>(ch)].store(level, std::memory_order_relaxed);
        }
        else { peaks[static_cast<size_t>(ch)].store(0); rms[static_cast<size_t>(ch)].store(0); }
    }
}
void MeteringEngine::measureInput(const juce::AudioBuffer<float>& b) noexcept { measure(b, inputPeak, inputRms); }
void MeteringEngine::measureOutput(const juce::AudioBuffer<float>& b) noexcept
{
    measure(b, outputPeak, outputRms);
    if (outputPeak[0].load() >= 1.0f || outputPeak[1].load() >= 1.0f) clipWarning.store(true);
}
}
