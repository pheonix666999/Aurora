#include "FFTAnalyzer.h"
#include <cstring>

namespace aurora
{
FFTAnalyzer::FFTAnalyzer(AnalyzerFIFO& f):fifo(f){smoothed.fill(-100.0f);}
bool FFTAnalyzer::update(float smoothing)
{
    auto got=fifo.pop(incoming.data(),juce::jmin(fftSize,fifo.ready()));if(got<=0)return false;
    if(got>=fftSize){std::copy_n(incoming.begin(),fftSize,history.begin());historyCount=fftSize;}else{std::memmove(history.data(),history.data()+got,static_cast<size_t>(fftSize-got)*sizeof(float));std::copy_n(incoming.begin(),got,history.end()-got);historyCount=juce::jmin(fftSize,historyCount+got);}if(historyCount<fftSize)return false;
    std::copy(history.begin(),history.end(),fftData.begin());std::fill(fftData.begin()+fftSize,fftData.end(),0.0f);window.multiplyWithWindowingTable(fftData.data(),fftSize);fft.performFrequencyOnlyForwardTransform(fftData.data());const auto a=juce::jlimit(0.0f,0.98f,smoothing);
    for(int i=0;i<bins;++i){const auto db=juce::Decibels::gainToDecibels(fftData[static_cast<size_t>(i)]/static_cast<float>(fftSize),-100.0f);smoothed[static_cast<size_t>(i)]=a*smoothed[static_cast<size_t>(i)]+(1-a)*db;}return true;
}
}
