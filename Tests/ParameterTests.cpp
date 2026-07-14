#include "PluginProcessor.h"
#include <set>

class ParameterTests final:public juce::UnitTest
{
public:ParameterTests():UnitTest("Parameter contract","Parameters"){}
void runTest()override
{
    beginTest("IDs are unique and defaults are valid");aurora::AuroraBroadcastProcessor p;std::set<juce::String> ids;expect(p.getParameters().size()>=80);
    for(auto* parameter:p.getParameters()){auto* ranged=dynamic_cast<juce::RangedAudioParameter*>(parameter);expect(ranged!=nullptr);if(!ranged)continue;expect(ids.insert(ranged->getParameterID()).second,"Duplicate "+ranged->getParameterID());expect(ranged->getDefaultValue()>=0.0f&&ranged->getDefaultValue()<=1.0f);const auto text=ranged->getText(ranged->getDefaultValue(),64);expect(text.length()>=0);}
    beginTest("Core parameters exist");for(auto id:{"inputTrimDb","agcTargetDb","eqBand6GainDb","band4ThresholdDb","limiterCeilingDb","globalBypass"})expect(p.parameters.getParameter(id)!=nullptr,id);
}
};static ParameterTests parameterTests;
