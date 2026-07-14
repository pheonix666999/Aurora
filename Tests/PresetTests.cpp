#include "PluginProcessor.h"

class PresetTests final:public juce::UnitTest
{
public:PresetTests():UnitTest("Presets","State"){}
void runTest()override
{
    aurora::AuroraBroadcastProcessor p;beginTest("All factory presets load");for(int i=0;i<p.getNumPrograms();++i){expect(p.presets.loadFactory(i),p.getProgramName(i));for(auto* parameter:p.getParameters())expect(parameter->getValue()>=0.0f&&parameter->getValue()<=1.0f);}
    beginTest("User preset round trip");juce::String error;const auto name="Aurora-Automated-Test";expect(p.presets.saveUser(name,error),error);auto files=p.presets.userPresets();juce::File testFile;for(auto& f:files)if(f.getFileNameWithoutExtension()==name)testFile=f;expect(testFile.existsAsFile());expect(p.presets.loadUser(testFile,error),error);expect(p.presets.deleteUser(testFile,error),error);
    beginTest("Malformed preset fails safely");auto bad=juce::File::createTempFile(".aurorapreset");bad.replaceWithText("<wrong/>");expect(!p.presets.loadUser(bad,error));bad.deleteFile();
}
};static PresetTests presetTests;
