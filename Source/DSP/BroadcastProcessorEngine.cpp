#include "BroadcastProcessorEngine.h"
#include "DSPUtilities.h"

namespace aurora
{
void BroadcastProcessorEngine::prepare(double sr,int maxBlock,int channels)
{
    rate=sr;const auto c=juce::jlimit(1,2,channels);bypass.setSize(c,maxBlock,false,false,true);input.prepare(sr,c);agc.prepare(sr);eq.prepare(sr,maxBlock,c);crossover.prepare(sr,maxBlock,c);multiband.prepare(sr);enhancer.prepare(sr);stereo.prepare(sr);clipper.prepare(sr,maxBlock,c);limiter.prepare(sr,maxBlock);totalLatency=clipper.latencySamples()+limiter.latencySamples();for(auto& d:bypassDelay)d.assign(static_cast<size_t>(totalLatency+maxBlock+4),0.0f);for(auto* s:{&outputGain,&bypassMix,&polarityL,&polarityR})s->reset(sr,0.03);reset();
}
void BroadcastProcessorEngine::reset(){input.reset();agc.reset();eq.reset();crossover.reset();multiband.reset();enhancer.reset();stereo.reset();clipper.reset();limiter.reset();metering.reset();for(auto& d:bypassDelay)std::fill(d.begin(),d.end(),0.0f);bypassIndex=0;}
void BroadcastProcessorEngine::delayBypass(juce::AudioBuffer<float>& b) noexcept
{
    if(totalLatency<=0)return;const auto size=static_cast<int>(bypassDelay[0].size());for(int i=0;i<b.getNumSamples();++i){const auto read=(bypassIndex-totalLatency+size)%size;for(int ch=0;ch<b.getNumChannels();++ch){bypassDelay[ch][bypassIndex]=b.getSample(ch,i);b.setSample(ch,i,bypassDelay[ch][read]);}bypassIndex=(bypassIndex+1)%size;}
}
void BroadcastProcessorEngine::process(juce::AudioBuffer<float>& b,const ParameterSnapshot& p) noexcept
{
    juce::ScopedNoDenormals noDenormals;dsp::sanitiseBuffer(b);metering.measureInput(b);const auto captureAnalyzer=analyzerConsumerActive.load(std::memory_order_relaxed)&&p.analyzerEnabled&&!p.analyzerFreeze;if(captureAnalyzer&&b.getNumChannels()>0)inputFifo.push(b.getReadPointer(0),b.getNumSamples());bypass.setSize(b.getNumChannels(),b.getNumSamples(),false,false,true);for(int ch=0;ch<b.getNumChannels();++ch)bypass.copyFrom(ch,0,b,ch,0,b.getNumSamples());delayBypass(bypass);
    input.process(b,p);metering.agcGainDb.store(agc.process(b,p));eq.process(b,p);crossover.process(b,p.crossovers);auto reductions=multiband.process(crossover.getBands(),b,p);for(size_t i=0;i<reductions.size();++i)metering.bandReductionDb[i].store(reductions[i]);enhancer.process(b,p);metering.correlation.store(stereo.process(b,p));clipper.process(b,p);if(clipper.hadOversampledPeak())metering.oversampledPeakWarning.store(true);metering.limiterReductionDb.store(limiter.process(b,p));
    outputGain.setTargetValue(juce::Decibels::decibelsToGain(p.outputTrim));polarityL.setTargetValue(p.invertL?-1.0f:1.0f);polarityR.setTargetValue(p.invertR?-1.0f:1.0f);bypassMix.setTargetValue(p.bypass?1.0f:0.0f);
    for(int i=0;i<b.getNumSamples();++i){const auto g=outputGain.getNextValue(),bm=bypassMix.getNextValue(),pl=polarityL.getNextValue(),pr=polarityR.getNextValue();for(int ch=0;ch<b.getNumChannels();++ch){auto wet=b.getSample(ch,i)*g*(ch==0?pl:pr);b.setSample(ch,i,dsp::sanitise(juce::jmap(bm,wet,bypass.getSample(ch,i))));}}
    metering.measureOutput(b);if(captureAnalyzer&&b.getNumChannels()>0)outputFifo.push(b.getReadPointer(0),b.getNumSamples());
}
}
