#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Config/ProductConfig.h"

namespace aurora
{
AuroraBroadcastProcessor::AuroraBroadcastProcessor():AudioProcessor(BusesProperties().withInput("Input",juce::AudioChannelSet::stereo(),true).withOutput("Output",juce::AudioChannelSet::stereo(),true)),parameters(*this,nullptr,"AuroraState",createParameterLayout()),presets(parameters){presets.loadFactory(0);}
const juce::String AuroraBroadcastProcessor::getName()const{return config::productName;}
bool AuroraBroadcastProcessor::isBusesLayoutSupported(const BusesLayout& l)const{const auto in=l.getMainInputChannelSet(),out=l.getMainOutputChannelSet();return in==out&&(out==juce::AudioChannelSet::mono()||out==juce::AudioChannelSet::stereo());}
void AuroraBroadcastProcessor::prepareToPlay(double sr,int maximumBlockSize){blockSize.store(maximumBlockSize);processorEngine.prepare(sr,maximumBlockSize,getTotalNumOutputChannels());setLatencySamples(processorEngine.latencySamples());}
void AuroraBroadcastProcessor::releaseResources(){processorEngine.reset();}
void AuroraBroadcastProcessor::processBlock(juce::AudioBuffer<float>& b,juce::MidiBuffer& midi){juce::ignoreUnused(midi);for(int ch=getTotalNumInputChannels();ch<getTotalNumOutputChannels();++ch)b.clear(ch,0,b.getNumSamples());ParameterSnapshot snapshot(parameters);processorEngine.process(b,snapshot);}
juce::AudioProcessorEditor* AuroraBroadcastProcessor::createEditor(){return new AuroraBroadcastProcessorEditor(*this);}
int AuroraBroadcastProcessor::getNumPrograms(){return static_cast<int>(factoryPresets().size());}int AuroraBroadcastProcessor::getCurrentProgram(){return presets.currentFactoryIndex();}void AuroraBroadcastProcessor::setCurrentProgram(int i){presets.loadFactory(i);}const juce::String AuroraBroadcastProcessor::getProgramName(int i){return juce::isPositiveAndBelow(i,getNumPrograms())?factoryPresets()[static_cast<size_t>(i)].name:juce::String{};}
void AuroraBroadcastProcessor::getStateInformation(juce::MemoryBlock& data){auto state=parameters.copyState();state.setProperty("schemaVersion",1,nullptr);state.setProperty("productVersion",config::version,nullptr);state.addChild(presets.metadata(),-1,nullptr);if(auto xml=state.createXml())copyXmlToBinary(*xml,data);}
void AuroraBroadcastProcessor::setStateInformation(const void* data,int size){stateRestoreValid.store(false);auto xml=getXmlFromBinary(data,size);if(!xml)return;auto tree=juce::ValueTree::fromXml(*xml);if(!tree.hasType(parameters.state.getType()))return;auto metadata=tree.getChildWithName("PresetMetadata");if(metadata.isValid()){presets.restoreMetadata(metadata);tree.removeChild(metadata,nullptr);}parameters.replaceState(tree);stateRestoreValid.store(true);}
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new aurora::AuroraBroadcastProcessor();}
