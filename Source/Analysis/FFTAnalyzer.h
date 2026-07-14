#pragma once
#include "LockFreeAudioFIFO.h"
#include <array>
#include <juce_dsp/juce_dsp.h>

namespace aurora
{
class FFTAnalyzer
{
public:
    static constexpr int order=12,fftSize=1<<order,bins=fftSize/2;
    explicit FFTAnalyzer(AnalyzerFIFO& source);
    bool update(float smoothing);const std::array<float,bins>& magnitudes()const noexcept{return smoothed;}
private:
    AnalyzerFIFO& fifo;juce::dsp::FFT fft{order};juce::dsp::WindowingFunction<float> window{fftSize,juce::dsp::WindowingFunction<float>::hann,true};std::array<float,fftSize*2> fftData{};std::array<float,fftSize> history{},incoming{};std::array<float,bins> smoothed{};int historyCount{};
};
}

