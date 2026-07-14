#pragma once
#include <map>
#include <vector>
#include <juce_core/juce_core.h>

namespace aurora
{
struct FactoryPreset{juce::String name,description;std::map<juce::String,float> values;};
const std::vector<FactoryPreset>& factoryPresets();
}

