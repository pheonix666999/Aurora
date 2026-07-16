#pragma once

#include "PluginProcessor.h"
#include "UI/AuroraLookAndFeel.h"
#include "UI/SpectrumEQComponent.h"

namespace aurora
{
class KnobControl final : public juce::Component
{
public:
    KnobControl(juce::AudioProcessorValueTreeState&, const char*, const juce::String&, const juce::String&);
    void resized() override;

private:
    juce::Label label;
    juce::Slider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class ToggleControl final : public juce::Component
{
public:
    ToggleControl(juce::AudioProcessorValueTreeState&, const char*, const juce::String&, const juce::String&);
    void resized() override;

private:
    juce::ToggleButton button;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;
};

class ChoiceControl final : public juce::Component
{
public:
    ChoiceControl(juce::AudioProcessorValueTreeState&, const char*, const juce::String&, juce::StringArray);
    void resized() override;

private:
    juce::Label label;
    juce::ComboBox box;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> attachment;
};

class ControlSection final : public juce::Component
{
public:
    enum class Kind { knob, toggle, choice };

    ControlSection(juce::String, juce::String, juce::Colour);
    void addControl(juce::Component&, Kind);
    int preferredHeight(int width) const noexcept;
    bool hasValidLayout() const noexcept;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    int gridHeight(int count, int width, int minimumCellWidth, int rowHeight, int maximumColumns) const noexcept;
    void layoutGrid(const std::vector<juce::Component*>&, juce::Rectangle<int>&, int, int, int);

    juce::String heading, description;
    juce::Colour accent;
    std::vector<juce::Component*> knobs, toggles, choices;
};

class MeterDisplay final : public juce::Component, public juce::SettableTooltipClient
{
public:
    explicit MeterDisplay(MeteringEngine&);
    void paint(juce::Graphics&) override;
    void mouseDown(const juce::MouseEvent&) override;

private:
    MeteringEngine& meters;
};

class AuroraBroadcastProcessorEditor final : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit AuroraBroadcastProcessorEditor(AuroraBroadcastProcessor&);
    ~AuroraBroadcastProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
    bool isLayoutValidForTesting() const noexcept;

private:
    void timerCallback() override;
    void beginSection(const juce::String&, const juce::String&, juce::Colour);
    void addKnob(const char*, const juce::String&, const juce::String&);
    void addToggle(const char*, const juce::String&, const juce::String&);
    void addChoice(const char*, const juce::String&, juce::StringArray);
    void selectRelativePreset(int);
    void savePreset();
    void saveAsPreset();

    AuroraBroadcastProcessor& processor;
    AuroraLookAndFeel lookAndFeel;
    juce::Label title, presetLabel, compareLabel, footer;
    juce::TextButton previous{"Previous"}, next{"Next"}, save{"Save"}, saveAs{"Export..."};
    juce::TextButton aButton{"Snapshot A"}, bButton{"Snapshot B"}, aToB{"Copy A to B"}, bToA{"Copy B to A"};
    juce::TextButton reset{"Reset Preset"}, advanced{"Hide Controls"}, info{"About"};
    juce::ComboBox presetBox;
    juce::ToggleButton bypass{"Global Bypass"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;
    SpectrumEQComponent spectrum;
    MeterDisplay meterDisplay;
    juce::Component controlContent;
    juce::Viewport viewport;
    std::vector<std::unique_ptr<ControlSection>> sections;
    ControlSection* currentSection{};
    std::vector<std::unique_ptr<KnobControl>> knobs;
    std::vector<std::unique_ptr<ToggleControl>> toggles;
    std::vector<std::unique_ptr<ChoiceControl>> choices;
    std::unique_ptr<juce::FileChooser> fileChooser;
    bool advancedMode{true};
};
}
