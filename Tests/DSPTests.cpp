#include "PluginProcessor.h"
#include <cmath>

namespace
{
bool finite(const juce::AudioBuffer<float>& b){for(int c=0;c<b.getNumChannels();++c)for(int i=0;i<b.getNumSamples();++i)if(!std::isfinite(b.getSample(c,i)))return false;return true;}
}
class DSPTests final:public juce::UnitTest
{
public:DSPTests():UnitTest("Core audio DSP","DSP"){}
void runTest()override
{
    for(auto sr:{44100.0,48000.0,88200.0,96000.0})for(auto block:{32,64,128,256,512,1024,2048,4096}){aurora::AuroraBroadcastProcessor p;p.prepareToPlay(sr,block);juce::MidiBuffer midi;juce::AudioBuffer<float>b(2,block);
        beginTest("Silence "+juce::String(sr)+"/"+juce::String(block));b.clear();p.processBlock(b,midi);expect(finite(b));expect(b.getMagnitude(0,block)==0.0f);
        for(auto hz:{50.0,100.0,1000.0,5000.0,10000.0}){for(int i=0;i<block;++i){auto s=0.25f*std::sin(juce::MathConstants<double>::twoPi*hz*i/sr);b.setSample(0,i,s);b.setSample(1,i,s);}p.processBlock(b,midi);expect(finite(b));}
        b.clear();b.setSample(0,0,1);b.setSample(1,0,1);p.processBlock(b,midi);expect(finite(b));p.releaseResources();}
    beginTest("Invalid input is sanitised");aurora::AuroraBroadcastProcessor p;p.prepareToPlay(48000,64);juce::AudioBuffer<float>b(2,64);juce::MidiBuffer m;b.setSample(0,0,std::numeric_limits<float>::quiet_NaN());b.setSample(1,1,std::numeric_limits<float>::infinity());p.processBlock(b,m);expect(finite(b));
}
};static DSPTests dspTests;

