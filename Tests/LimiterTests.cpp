#include "DSP/BroadcastLimiter.h"
#include "PluginProcessor.h"

class LimiterTests final:public juce::UnitTest
{
public:LimiterTests():UnitTest("Look-ahead limiter","DSP"){}
void runTest()override{aurora::AuroraBroadcastProcessor owner;aurora::ParameterSnapshot p(owner.parameters);p.limiter=true;p.limiterDrive=6;p.limiterCeiling=-0.8f;aurora::BroadcastLimiter l;l.prepare(48000,512);juce::AudioBuffer<float>b(2,512);b.clear();for(int i=0;i<512;++i)for(int ch=0;ch<2;++ch)b.setSample(ch,i,(i%37==0)?1.0f:0.2f);beginTest("Ceiling and finite recovery");for(int n=0;n<20;++n){auto gr=l.process(b,p);expect(std::isfinite(gr));expect(b.getMagnitude(0,512)<=juce::Decibels::decibelsToGain(p.limiterCeiling)+0.0001f);}expect(l.latencySamples()>=480);}
};static LimiterTests limiterTests;

