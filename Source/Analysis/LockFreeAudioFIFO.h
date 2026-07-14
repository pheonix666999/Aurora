#pragma once
#include <array>
#include <juce_core/juce_core.h>

namespace aurora
{
template <int Capacity>
class LockFreeAudioFIFO
{
public:
    void push(const float* data,int count) noexcept
    {
        int s1,n1,s2,n2;fifo.prepareToWrite(juce::jmin(count,Capacity),s1,n1,s2,n2);for(int i=0;i<n1;++i)storage[static_cast<size_t>(s1+i)]=data[i];for(int i=0;i<n2;++i)storage[static_cast<size_t>(s2+i)]=data[n1+i];fifo.finishedWrite(n1+n2);
    }
    int pop(float* target,int count) noexcept
    {
        int s1,n1,s2,n2;fifo.prepareToRead(juce::jmin(count,Capacity),s1,n1,s2,n2);for(int i=0;i<n1;++i)target[i]=storage[static_cast<size_t>(s1+i)];for(int i=0;i<n2;++i)target[n1+i]=storage[static_cast<size_t>(s2+i)];fifo.finishedRead(n1+n2);return n1+n2;
    }
    int ready()const noexcept{return fifo.getNumReady();}
private:
    juce::AbstractFifo fifo{Capacity};std::array<float,Capacity> storage{};
};
using AnalyzerFIFO=LockFreeAudioFIFO<32768>;
}

