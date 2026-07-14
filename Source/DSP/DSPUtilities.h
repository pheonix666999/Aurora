#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

namespace aurora::dsp
{
float sanitise(float sample) noexcept;
float softKneeGainDb(float inputDb, float thresholdDb, float ratio, float kneeDb) noexcept;
void sanitiseBuffer(juce::AudioBuffer<float>&) noexcept;
}

