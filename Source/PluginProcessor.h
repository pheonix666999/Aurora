#pragma once
#include "DSP/BroadcastProcessorEngine.h"
#include "Parameters/ParameterLayout.h"
#include "Presets/PresetManager.h"
#include <juce_audio_processors/juce_audio_processors.h>

namespace aurora
{
class AuroraBroadcastProcessor final:public juce::AudioProcessor
{
public:
    AuroraBroadcastProcessor();~AuroraBroadcastProcessor() override=default;
    void prepareToPlay(double,int)override;void releaseResources()override;void processBlock(juce::AudioBuffer<float>&,juce::MidiBuffer&)override;
    bool isBusesLayoutSupported(const BusesLayout&)const override;juce::AudioProcessorEditor* createEditor()override;bool hasEditor()const override{return true;}
    const juce::String getName()const override;bool acceptsMidi()const override{return false;}bool producesMidi()const override{return false;}bool isMidiEffect()const override{return false;}double getTailLengthSeconds()const override{return 0;}
    int getNumPrograms()override;int getCurrentProgram()override;void setCurrentProgram(int)override;const juce::String getProgramName(int)override;void changeProgramName(int,const juce::String&)override{}
    void getStateInformation(juce::MemoryBlock&)override;void setStateInformation(const void*,int)override;
    juce::AudioProcessorValueTreeState parameters;PresetManager presets;BroadcastProcessorEngine& engine()noexcept{return processorEngine;}int currentBlockSize()const noexcept{return blockSize.load();}bool lastRestoreWasValid()const noexcept{return stateRestoreValid.load();}
private:
    BroadcastProcessorEngine processorEngine;std::atomic<int> blockSize{0};std::atomic<bool> stateRestoreValid{true};
};
}
