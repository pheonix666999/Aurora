#pragma once
#include "FactoryPresets.h"
#include <juce_audio_processors/juce_audio_processors.h>

namespace aurora
{
class PresetManager
{
public:
    explicit PresetManager(juce::AudioProcessorValueTreeState&);
    bool loadFactory(int);bool saveUser(const juce::String&,juce::String&);bool loadUser(const juce::File&,juce::String&);bool deleteUser(const juce::File&,juce::String&);bool renameUser(const juce::File&,const juce::String&,juce::String&);bool exportPreset(const juce::File&,juce::String&);bool importPreset(const juce::File&,juce::String&);
    juce::Array<juce::File> userPresets()const;void captureA();void captureB();void selectA();void selectB();void copyAToB();void copyBToA();juce::ValueTree metadata()const;void restoreMetadata(const juce::ValueTree&);int currentFactoryIndex()const noexcept{return factoryIndex;}
private:
    void resetToDefaults();void apply(const juce::ValueTree&);juce::File directory()const;
    juce::AudioProcessorValueTreeState& state;juce::ValueTree snapshotA{"SnapshotA"},snapshotB{"SnapshotB"};bool selectedA{true};int factoryIndex{};
};
}

