#include "AuroraLookAndFeel.h"
#include "Config/ColourPalette.h"

namespace aurora
{
AuroraLookAndFeel::AuroraLookAndFeel(){setColour(juce::Label::textColourId,colours::text);setColour(juce::Slider::textBoxTextColourId,colours::text);setColour(juce::Slider::textBoxBackgroundColourId,colours::panelSecondary);setColour(juce::Slider::textBoxOutlineColourId,colours::grid);setColour(juce::ComboBox::backgroundColourId,colours::panelSecondary);setColour(juce::ComboBox::textColourId,colours::text);setColour(juce::TextButton::buttonColourId,colours::panelSecondary);setColour(juce::TextButton::textColourOffId,colours::text);setColour(juce::ToggleButton::textColourId,colours::textSecondary);}
void AuroraLookAndFeel::drawRotarySlider(juce::Graphics& g,int x,int y,int w,int h,float pos,float start,float end,juce::Slider& s){auto r=juce::Rectangle<float>(static_cast<float>(x),static_cast<float>(y),static_cast<float>(w),static_cast<float>(h)).reduced(8);const auto radius=juce::jmin(r.getWidth(),r.getHeight())*0.5f;auto centre=r.getCentre();g.setColour(colours::grid);g.drawEllipse(r.withSizeKeepingCentre(radius*2,radius*2),4);juce::Path arc;arc.addCentredArc(centre.x,centre.y,radius,radius,0,start,start+pos*(end-start),true);g.setColour(s.isEnabled()?colours::cyan:colours::muted);g.strokePath(arc,juce::PathStrokeType(4,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));const auto angle=start+pos*(end-start);g.drawLine(centre.x,centre.y,centre.x+std::sin(angle)*radius*0.65f,centre.y-std::cos(angle)*radius*0.65f,2);}
void AuroraLookAndFeel::drawButtonBackground(juce::Graphics& g,juce::Button& b,const juce::Colour&,bool hover,bool down){auto c=b.getToggleState()?colours::cyan:colours::panelSecondary;if(hover)c=c.brighter(0.12f);if(down)c=c.darker(0.12f);g.setColour(c);g.fillRoundedRectangle(b.getLocalBounds().toFloat().reduced(1),4);g.setColour(colours::grid);g.drawRoundedRectangle(b.getLocalBounds().toFloat().reduced(1),4,1);}
}

