#pragma once
#include "PluginProcessor.h"
#include "UI/AuroraLookAndFeel.h"
#include "UI/SpectrumEQComponent.h"

namespace aurora
{
class KnobControl final:public juce::Component
{
public:
    KnobControl(juce::AudioProcessorValueTreeState&,const char*,const juce::String&,const juce::String&);
    void resized()override;
private:
    juce::Label label;juce::Slider slider;std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};
class ToggleControl final:public juce::Component
{
public:
    ToggleControl(juce::AudioProcessorValueTreeState&,const char*,const juce::String&,const juce::String&);void resized()override;
private:
    juce::ToggleButton button;std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;
};
class ChoiceControl final:public juce::Component
{
public:
    ChoiceControl(juce::AudioProcessorValueTreeState&,const char*,const juce::String&,juce::StringArray);void resized()override;
private:
    juce::Label label;juce::ComboBox box;std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> attachment;
};
class MeterDisplay final:public juce::Component
{
public:
    explicit MeterDisplay(MeteringEngine&);void paint(juce::Graphics&)override;void mouseDown(const juce::MouseEvent&)override;
private:MeteringEngine& meters;
};

class AuroraBroadcastProcessorEditor final:public juce::AudioProcessorEditor,private juce::Timer
{
public:
    explicit AuroraBroadcastProcessorEditor(AuroraBroadcastProcessor&);~AuroraBroadcastProcessorEditor()override;void paint(juce::Graphics&)override;void resized()override;
private:
    void timerCallback()override;void addKnob(const char*,const juce::String&,const juce::String&);void addToggle(const char*,const juce::String&,const juce::String&);void addChoice(const char*,const juce::String&,juce::StringArray);void selectRelativePreset(int);void savePreset();void saveAsPreset();
    AuroraBroadcastProcessor& processor;AuroraLookAndFeel lookAndFeel;juce::Label title,presetLabel,footer;juce::TextButton previous{"<"},next{">"},save{"Save"},saveAs{"Save As"},aButton{"A"},bButton{"B"},aToB{"A > B"},bToA{"B > A"},reset{"Reset"},advanced{"Advanced"},info{"i"};juce::ComboBox presetBox;juce::ToggleButton bypass{"Bypass"};std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>bypassAttachment;SpectrumEQComponent spectrum;MeterDisplay meterDisplay;juce::Component controlContent;juce::Viewport viewport;std::vector<std::unique_ptr<KnobControl>>knobs;std::vector<std::unique_ptr<ToggleControl>>toggles;std::vector<std::unique_ptr<ChoiceControl>>choices;std::unique_ptr<juce::FileChooser>fileChooser;bool advancedMode{true};
};
}
