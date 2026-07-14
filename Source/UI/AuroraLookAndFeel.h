#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace aurora
{
class AuroraLookAndFeel final:public juce::LookAndFeel_V4
{
public:
    AuroraLookAndFeel();void drawRotarySlider(juce::Graphics&,int,int,int,int,float,float,float,juce::Slider&)override;void drawButtonBackground(juce::Graphics&,juce::Button&,const juce::Colour&,bool,bool)override;
};
}

