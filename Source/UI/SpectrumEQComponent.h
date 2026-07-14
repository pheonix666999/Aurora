#pragma once
#include "Analysis/FFTAnalyzer.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace aurora
{
class SpectrumEQComponent final:public juce::Component,private juce::Timer
{
public:
    SpectrumEQComponent(juce::AudioProcessorValueTreeState&,AnalyzerFIFO&,AnalyzerFIFO&,std::function<double()>);void paint(juce::Graphics&)override;void mouseDown(const juce::MouseEvent&)override;void mouseDrag(const juce::MouseEvent&)override;void mouseUp(const juce::MouseEvent&)override;void mouseDoubleClick(const juce::MouseEvent&)override;void mouseWheelMove(const juce::MouseEvent&,const juce::MouseWheelDetails&)override;
private:
    void timerCallback()override;float frequencyToX(float)const;float xToFrequency(float)const;float gainToY(float)const;float yToGain(float)const;juce::Point<float> nodePosition(int)const;void setBandValue(int,const char*,float);
    juce::AudioProcessorValueTreeState& state;FFTAnalyzer inputAnalyzer,outputAnalyzer;std::function<double()> sampleRate;int selected{-1};
};
}
