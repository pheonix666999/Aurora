#include "DSP/FourBandCrossover.h"

class CrossoverTests final:public juce::UnitTest
{
public:CrossoverTests():UnitTest("Four-band crossover","DSP"){}
void runTest()override{beginTest("All bands produce finite signal");aurora::FourBandCrossover c;c.prepare(48000,512,2);juce::AudioBuffer<float> in(2,512);for(int i=0;i<512;++i)for(int ch=0;ch<2;++ch)in.setSample(ch,i,0.4f*std::sin(juce::MathConstants<double>::twoPi*1000*i/48000));c.process(in,{120,1100,6000});for(auto& b:c.getBands()){expect(b.getMagnitude(0,512)>0);for(int i=0;i<512;++i)expect(std::isfinite(b.getSample(0,i)));}beginTest("Crossed automation is clamped safely");c.process(in,{300,400,3000});for(auto& b:c.getBands())expect(std::isfinite(b.getMagnitude(0,512)));}
};static CrossoverTests crossoverTests;

