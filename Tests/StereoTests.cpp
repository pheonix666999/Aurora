#include "DSP/StereoProcessor.h"
#include "PluginProcessor.h"

class StereoTests final:public juce::UnitTest
{
public:StereoTests():UnitTest("Stereo processing","DSP"){}
void runTest()override{aurora::AuroraBroadcastProcessor owner;aurora::ParameterSnapshot p(owner.parameters);aurora::StereoProcessor s;s.prepare(48000);juce::AudioBuffer<float>b(2,512);for(int i=0;i<512;++i){b.setSample(0,i,0.4f);b.setSample(1,i,-0.2f);}beginTest("Width zero converges to mono");p.width=0;s.process(b,p);expectWithinAbsoluteError(b.getSample(0,511),b.getSample(1,511),0.02f);beginTest("Finite correlation protection");p.width=1.5f;for(int n=0;n<100;++n)expect(std::isfinite(s.process(b,p)));}
};static StereoTests stereoTests;
