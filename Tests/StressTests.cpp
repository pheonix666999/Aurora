#include "PluginProcessor.h"
#include <random>

class StressTests final:public juce::UnitTest
{
public:StressTests():UnitTest("Rapid automation stress","Stress"){}
void runTest()override{beginTest("Finite output during randomized automation");std::mt19937 random(0xA0B0C0);std::uniform_real_distribution<float> unit(0,1);const bool extended=juce::SystemStats::getEnvironmentVariable("AURORA_EXTENDED_TESTS","0")=="1";const int iterations=extended?3000:250;aurora::AuroraBroadcastProcessor p;p.prepareToPlay(96000,257);juce::AudioBuffer<float>b(2,257);juce::MidiBuffer midi;for(int n=0;n<iterations;++n){for(auto* parameter:p.getParameters())if(n%7==0)parameter->setValueNotifyingHost(unit(random));for(int i=0;i<b.getNumSamples();++i){const auto x=n%4==0?0.0f:n%4==1?unit(random)*2-1:n%4==2?(i%31==0?1.0f:-1.0f):0.5f*std::sin(0.03f*i);b.setSample(0,i,x);b.setSample(1,i,-x*0.7f);}p.processBlock(b,midi);for(int ch=0;ch<2;++ch)for(int i=0;i<b.getNumSamples();++i)expect(std::isfinite(b.getSample(ch,i)));}}
};static StressTests stressTests;
