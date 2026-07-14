#include "PluginProcessor.h"

class LifecycleTests final:public juce::UnitTest
{
public:LifecycleTests():UnitTest("Processor lifecycle","Lifecycle"){}
void runTest()override{beginTest("Repeated prepare, process, state, and editor");for(int n=0;n<12;++n){auto p=std::make_unique<aurora::AuroraBroadcastProcessor>();p->prepareToPlay(n%2?44100:96000,64+n*16);juce::AudioBuffer<float>b(2,64+n*16);juce::MidiBuffer m;b.clear();p->processBlock(b,m);std::unique_ptr<juce::AudioProcessorEditor> editor(p->createEditor());expect(editor!=nullptr);editor.reset();juce::MemoryBlock state;p->getStateInformation(state);p->setStateInformation(state.getData(),static_cast<int>(state.getSize()));expect(p->lastRestoreWasValid());p->releaseResources();}}
};static LifecycleTests lifecycleTests;

