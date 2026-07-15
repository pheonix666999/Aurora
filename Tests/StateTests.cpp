#include "PluginProcessor.h"

class StateTests final:public juce::UnitTest
{
public:StateTests():UnitTest("Versioned state","State"){}
void runTest()override
{
    beginTest("Round trip");aurora::AuroraBroadcastProcessor a,b;auto* input=a.parameters.getParameter("inputTrimDb");input->setValueNotifyingHost(input->convertTo0to1(7.3f));juce::MemoryBlock state;a.getStateInformation(state);expect(state.getSize()>0);b.setStateInformation(state.getData(),static_cast<int>(state.getSize()));expect(b.lastRestoreWasValid());expectWithinAbsoluteError(b.parameters.getRawParameterValue("inputTrimDb")->load(),7.3f,0.001f);
    beginTest("Corrupt state is rejected safely");const char invalid[]="not a JUCE state";b.setStateInformation(invalid,sizeof(invalid));expect(!b.lastRestoreWasValid());
    beginTest("Restore while prepared");b.prepareToPlay(48000,256);b.setStateInformation(state.getData(),static_cast<int>(state.getSize()));expect(b.lastRestoreWasValid());juce::AudioBuffer<float> audio(2,256);juce::MidiBuffer midi;audio.clear();b.processBlock(audio,midi);expect(audio.getMagnitude(0,256)==0.0f);b.releaseResources();
    beginTest("Unknown properties survive safely");a.parameters.state.setProperty("futureProperty",42,nullptr);juce::MemoryBlock future;a.getStateInformation(future);b.setStateInformation(future.getData(),static_cast<int>(future.getSize()));expect(b.lastRestoreWasValid());
}
};static StateTests stateTests;
