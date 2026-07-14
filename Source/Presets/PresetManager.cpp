#include "PresetManager.h"
#include "Config/ProductConfig.h"

namespace aurora
{
PresetManager::PresetManager(juce::AudioProcessorValueTreeState& s):state(s){captureA();copyAToB();}
juce::File PresetManager::directory()const{return juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile(config::companyName).getChildFile(config::productName).getChildFile("Presets");}
void PresetManager::resetToDefaults(){for(int i=0;i<state.state.getNumChildren();++i){auto child=state.state.getChild(i);if(auto* ranged=state.getParameter(child.getProperty("id").toString()))ranged->setValueNotifyingHost(ranged->getDefaultValue());}}
bool PresetManager::loadFactory(int index)
{
    if(!juce::isPositiveAndBelow(index,static_cast<int>(factoryPresets().size())))return false;resetToDefaults();for(const auto& [name,value]:factoryPresets()[static_cast<size_t>(index)].values)if(auto* p=state.getParameter(name))p->setValueNotifyingHost(p->convertTo0to1(value));factoryIndex=index;captureA();captureB();return true;
}
bool PresetManager::saveUser(const juce::String& name,juce::String& error)
{
    auto dir=directory();if(!dir.createDirectory()){error="Could not create the user preset directory.";return false;}auto safe=juce::File::createLegalFileName(name.trim());if(safe.isEmpty()){error="Enter a valid preset name.";return false;}auto xml=state.copyState().createXml();if(!xml){error="Could not serialize the preset.";return false;}if(!xml->writeTo(dir.getChildFile(safe+".aurorapreset"),{})){error="Could not write the preset file.";return false;}return true;
}
void PresetManager::apply(const juce::ValueTree& tree){if(tree.isValid()&&tree.hasType(state.state.getType()))state.replaceState(tree.createCopy());}
bool PresetManager::loadUser(const juce::File& file,juce::String& error){auto xml=juce::XmlDocument::parse(file);if(!xml){error="The preset is not valid XML.";return false;}auto tree=juce::ValueTree::fromXml(*xml);if(!tree.hasType(state.state.getType())){error="This is not an Aurora preset.";return false;}apply(tree);return true;}
bool PresetManager::deleteUser(const juce::File& f,juce::String& error){if(!f.isAChildOf(directory())||!f.deleteFile()){error="The user preset could not be deleted.";return false;}return true;}
bool PresetManager::renameUser(const juce::File& f,const juce::String& n,juce::String& error){auto target=directory().getChildFile(juce::File::createLegalFileName(n)+".aurorapreset");if(!f.isAChildOf(directory())||!f.moveFileTo(target)){error="The user preset could not be renamed.";return false;}return true;}
bool PresetManager::exportPreset(const juce::File& f,juce::String& error){auto xml=state.copyState().createXml();if(!xml||!xml->writeTo(f,{})){error="The preset could not be exported.";return false;}return true;}
bool PresetManager::importPreset(const juce::File& f,juce::String& error){if(!loadUser(f,error))return false;return saveUser(f.getFileNameWithoutExtension(),error);}
juce::Array<juce::File> PresetManager::userPresets()const{return directory().findChildFiles(juce::File::findFiles,false,"*.aurorapreset");}
void PresetManager::captureA(){snapshotA=juce::ValueTree{"SnapshotA"};snapshotA.addChild(state.copyState(),-1,nullptr);}void PresetManager::captureB(){snapshotB=juce::ValueTree{"SnapshotB"};snapshotB.addChild(state.copyState(),-1,nullptr);}void PresetManager::selectA(){if(!selectedA)captureB();apply(snapshotA.getChild(0));selectedA=true;}void PresetManager::selectB(){if(selectedA)captureA();apply(snapshotB.getChild(0));selectedA=false;}void PresetManager::copyAToB(){snapshotB=juce::ValueTree{"SnapshotB"};if(snapshotA.getNumChildren()>0)snapshotB.addChild(snapshotA.getChild(0).createCopy(),-1,nullptr);}void PresetManager::copyBToA(){snapshotA=juce::ValueTree{"SnapshotA"};if(snapshotB.getNumChildren()>0)snapshotA.addChild(snapshotB.getChild(0).createCopy(),-1,nullptr);}
juce::ValueTree PresetManager::metadata()const{juce::ValueTree v{"PresetMetadata"};v.setProperty("selectedA",selectedA,nullptr);v.setProperty("factoryIndex",factoryIndex,nullptr);v.addChild(snapshotA.createCopy(),-1,nullptr);v.addChild(snapshotB.createCopy(),-1,nullptr);return v;}
void PresetManager::restoreMetadata(const juce::ValueTree& v){if(!v.isValid())return;selectedA=v.getProperty("selectedA",true);factoryIndex=v.getProperty("factoryIndex",0);auto a=v.getChildWithName("SnapshotA"),b=v.getChildWithName("SnapshotB");if(a.isValid())snapshotA=a.createCopy();if(b.isValid())snapshotB=b.createCopy();}
}
