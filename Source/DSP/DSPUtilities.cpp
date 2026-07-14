#include "DSPUtilities.h"
#include <cmath>

namespace aurora::dsp
{
float sanitise(float sample) noexcept { return std::isfinite(sample) ? juce::jlimit(-32.0f, 32.0f, sample) : 0.0f; }

float softKneeGainDb(float inputDb, float thresholdDb, float ratio, float kneeDb) noexcept
{
    const auto over = inputDb - thresholdDb;
    if (ratio <= 1.0f || over <= -kneeDb * 0.5f) return 0.0f;
    const auto slope = 1.0f / ratio - 1.0f;
    if (kneeDb > 0.0f && over < kneeDb * 0.5f)
    {
        const auto x = over + kneeDb * 0.5f;
        return slope * x * x / (2.0f * kneeDb);
    }
    return slope * over;
}

void sanitiseBuffer(juce::AudioBuffer<float>& buffer) noexcept
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            buffer.setSample(channel, sample, sanitise(buffer.getSample(channel, sample)));
}
}

