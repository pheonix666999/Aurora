#include "PluginEditor.h"

#include "Config/ColourPalette.h"
#include "Config/ParameterIDs.h"
#include "Config/ProductConfig.h"

namespace aurora
{
namespace
{
constexpr int sectionHeaderHeight = 52;

int rowsFor(int count, int columns) noexcept
{
    return count == 0 ? 0 : (count + columns - 1) / columns;
}
}

KnobControl::KnobControl(juce::AudioProcessorValueTreeState& state, const char* id,
                         const juce::String& name, const juce::String& tip)
{
    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setFont(juce::FontOptions(13.5f, juce::Font::bold));
    label.setMinimumHorizontalScale(0.72f);
    label.setInterceptsMouseClicks(false, false);

    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 112, 25);
    slider.setDoubleClickReturnValue(true, dynamic_cast<juce::RangedAudioParameter*>(state.getParameter(id))
                                                ->convertFrom0to1(state.getParameter(id)->getDefaultValue()));
    slider.setTooltip(tip);
    slider.setName(name);
    addAndMakeVisible(label);
    addAndMakeVisible(slider);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(state, id, slider);
}

void KnobControl::resized()
{
    auto area = getLocalBounds();
    label.setBounds(area.removeFromTop(30));
    slider.setBounds(area.reduced(2, 0));
}

ToggleControl::ToggleControl(juce::AudioProcessorValueTreeState& state, const char* id,
                             const juce::String& name, const juce::String& tip)
{
    button.setButtonText(name);
    button.setTooltip(tip);
    button.setName(name);
    addAndMakeVisible(button);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(state, id, button);
}

void ToggleControl::resized()
{
    button.setBounds(getLocalBounds().reduced(2));
}

ChoiceControl::ChoiceControl(juce::AudioProcessorValueTreeState& state, const char* id,
                             const juce::String& name, juce::StringArray items)
{
    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centredLeft);
    label.setFont(juce::FontOptions(13.5f, juce::Font::bold));
    label.setInterceptsMouseClicks(false, false);
    box.addItemList(items, 1);
    box.setName(name);
    addAndMakeVisible(label);
    addAndMakeVisible(box);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(state, id, box);
}

void ChoiceControl::resized()
{
    auto area = getLocalBounds().reduced(3, 1);
    label.setBounds(area.removeFromTop(24));
    box.setBounds(area.removeFromTop(36));
}

ControlSection::ControlSection(juce::String name, juce::String detail, juce::Colour colour)
    : heading(std::move(name)), description(std::move(detail)), accent(colour)
{
    setOpaque(false);
}

void ControlSection::addControl(juce::Component& control, Kind kind)
{
    addAndMakeVisible(control);
    if (kind == Kind::knob) knobs.push_back(&control);
    else if (kind == Kind::toggle) toggles.push_back(&control);
    else choices.push_back(&control);
}

int ControlSection::gridHeight(int count, int width, int minimumCellWidth,
                               int rowHeight, int maximumColumns) const noexcept
{
    if (count == 0) return 0;
    const auto available = juce::jmax(1, width - 32);
    const auto columns = juce::jlimit(1, maximumColumns, available / minimumCellWidth);
    return rowsFor(count, columns) * rowHeight;
}

int ControlSection::preferredHeight(int width) const noexcept
{
    auto height = 16 + sectionHeaderHeight + 16;
    auto groups = 0;
    const auto addGroup = [&](int groupHeight)
    {
        if (groupHeight <= 0) return;
        if (groups++ > 0) height += 10;
        height += groupHeight;
    };
    addGroup(gridHeight(static_cast<int>(knobs.size()), width, 132, 132, 12));
    addGroup(gridHeight(static_cast<int>(toggles.size()), width, 190, 40, 7));
    addGroup(gridHeight(static_cast<int>(choices.size()), width, 220, 66, 6));
    return height;
}

void ControlSection::layoutGrid(const std::vector<juce::Component*>& controls,
                                juce::Rectangle<int>& area, int minimumCellWidth,
                                int rowHeight, int maximumColumns)
{
    if (controls.empty()) return;
    const auto columns = juce::jlimit(1, maximumColumns, area.getWidth() / minimumCellWidth);
    const auto rows = rowsFor(static_cast<int>(controls.size()), columns);
    const auto grid = area.removeFromTop(rows * rowHeight);
    const auto cellWidth = grid.getWidth() / columns;

    for (size_t index = 0; index < controls.size(); ++index)
    {
        const auto column = static_cast<int>(index) % columns;
        const auto row = static_cast<int>(index) / columns;
        auto cell = juce::Rectangle<int>(grid.getX() + column * cellWidth,
                                         grid.getY() + row * rowHeight,
                                         column == columns - 1 ? grid.getRight() - (grid.getX() + column * cellWidth) : cellWidth,
                                         rowHeight);
        controls[index]->setBounds(cell.reduced(5, 3));
    }
}

void ControlSection::paint(juce::Graphics& g)
{
    const auto card = getLocalBounds().toFloat().reduced(1.0f);
    juce::ColourGradient gradient(colours::panelElevated, card.getX(), card.getY(),
                                  colours::panel, card.getX(), card.getBottom(), false);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(card, 10.0f);
    g.setColour(colours::border);
    g.drawRoundedRectangle(card, 10.0f, 1.0f);
    g.setColour(accent);
    g.fillRoundedRectangle(juce::Rectangle<float>(card.getX() + 14.0f, card.getY() + 12.0f, 4.0f, 34.0f), 2.0f);
    g.setColour(colours::text);
    g.setFont(juce::FontOptions(15.0f, juce::Font::bold));
    g.drawText(heading.toUpperCase(), 28, 10, getWidth() - 44, 21, juce::Justification::centredLeft, true);
    g.setColour(colours::textSecondary);
    g.setFont(juce::FontOptions(12.0f));
    g.drawFittedText(description, 28, 31, getWidth() - 44, 18, juce::Justification::centredLeft, 1, 0.82f);
}

void ControlSection::resized()
{
    auto area = getLocalBounds().reduced(16);
    area.removeFromTop(sectionHeaderHeight);
    auto laidOutGroup = false;
    const auto addGap = [&]
    {
        if (laidOutGroup) area.removeFromTop(10);
        laidOutGroup = true;
    };

    if (!knobs.empty()) { addGap(); layoutGrid(knobs, area, 132, 132, 12); }
    if (!toggles.empty()) { addGap(); layoutGrid(toggles, area, 190, 40, 7); }
    if (!choices.empty()) { addGap(); layoutGrid(choices, area, 220, 66, 6); }
}

bool ControlSection::hasValidLayout() const noexcept
{
    const auto local = getLocalBounds();
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        const auto* first = getChildComponent(i);
        if (first->getWidth() <= 0 || first->getHeight() <= 0 || !local.contains(first->getBounds())) return false;
        for (int j = i + 1; j < getNumChildComponents(); ++j)
            if (first->getBounds().intersects(getChildComponent(j)->getBounds())) return false;
    }
    return true;
}

MeterDisplay::MeterDisplay(MeteringEngine& metering) : meters(metering)
{
    setTooltip("Live input and output peak/RMS levels. Click to clear peak warnings.");
}

void MeterDisplay::mouseDown(const juce::MouseEvent&)
{
    meters.clipWarning.store(false);
    meters.oversampledPeakWarning.store(false);
    repaint();
}

void MeterDisplay::paint(juce::Graphics& g)
{
    const auto card = getLocalBounds().toFloat().reduced(1.0f);
    juce::ColourGradient gradient(colours::panelElevated, card.getX(), card.getY(),
                                  colours::panel, card.getX(), card.getBottom(), false);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(card, 10.0f);
    g.setColour(colours::border);
    g.drawRoundedRectangle(card, 10.0f, 1.0f);

    auto area = getLocalBounds().reduced(13);
    auto headingArea = area.removeFromTop(42);
    g.setColour(colours::text);
    g.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    g.drawText("REAL-TIME METERING", headingArea.removeFromTop(20), juce::Justification::centredLeft);
    g.setColour(colours::textSecondary);
    g.setFont(juce::FontOptions(11.0f));
    g.drawText("Peak lines / RMS fill", headingArea, juce::Justification::centredLeft);

    auto stats = area.removeFromBottom(104);
    auto labels = area.removeFromBottom(22);
    auto meterArea = area.reduced(5, 2);
    const auto barSlot = meterArea.getWidth() / 4;
    const auto barWidth = static_cast<float>(juce::jlimit(12, 25, barSlot - 14));
    const juce::String channelNames[] { "IN L", "IN R", "OUT L", "OUT R" };
    const juce::Colour channelColours[] { colours::green, colours::green, colours::cyan, colours::cyan };
    const float peaks[] { meters.inputPeak[0].load(), meters.inputPeak[1].load(),
                          meters.outputPeak[0].load(), meters.outputPeak[1].load() };
    const float rms[] { meters.inputRms[0].load(), meters.inputRms[1].load(),
                        meters.outputRms[0].load(), meters.outputRms[1].load() };
    const auto normalise = [](float value)
    {
        return juce::jlimit(0.0f, 1.0f,
                            juce::jmap(juce::Decibels::gainToDecibels(value, -60.0f),
                                       -60.0f, 0.0f, 0.0f, 1.0f));
    };

    for (int channel = 0; channel < 4; ++channel)
    {
        const auto centreX = static_cast<float>(meterArea.getX() + channel * barSlot + barSlot / 2);
        auto bar = juce::Rectangle<float>(barWidth, static_cast<float>(meterArea.getHeight()))
                       .withCentre({centreX, static_cast<float>(meterArea.getCentreY())});
        g.setColour(colours::meterTrack);
        g.fillRoundedRectangle(bar, 4.0f);
        const auto fillHeight = bar.getHeight() * normalise(rms[channel]);
        g.setColour(channelColours[channel].darker(0.35f));
        g.fillRoundedRectangle(bar.withTop(bar.getBottom() - fillHeight), 4.0f);
        g.setColour(channelColours[channel]);
        const auto peakY = bar.getBottom() - bar.getHeight() * normalise(peaks[channel]);
        g.fillRect(bar.getX(), peakY, bar.getWidth(), 2.0f);
        g.setColour(colours::textSecondary);
        g.setFont(juce::FontOptions(10.5f, juce::Font::bold));
        g.drawText(channelNames[channel], labels.getX() + channel * barSlot, labels.getY(),
                   barSlot, labels.getHeight(), juce::Justification::centred);
    }

    g.setColour(colours::border);
    g.drawHorizontalLine(stats.getY(), static_cast<float>(stats.getX()), static_cast<float>(stats.getRight()));
    stats.removeFromTop(8);
    const auto statRow = [&](const juce::String& name, const juce::String& value, juce::Colour valueColour)
    {
        auto row = stats.removeFromTop(21);
        g.setColour(colours::textSecondary);
        g.setFont(juce::FontOptions(11.5f));
        g.drawText(name, row.removeFromLeft(92), juce::Justification::centredLeft);
        g.setColour(valueColour);
        g.setFont(juce::FontOptions(11.5f, juce::Font::bold));
        g.drawText(value, row, juce::Justification::centredRight);
    };
    statRow("AGC gain", juce::String(meters.agcGainDb.load(), 1) + " dB", colours::green);
    statRow("Limiter reduction", juce::String(meters.limiterReductionDb.load(), 1) + " dB", colours::amber);
    statRow("Stereo correlation", juce::String(meters.correlation.load(), 2), colours::cyan);
    const auto warning = meters.clipWarning.load() || meters.oversampledPeakWarning.load();
    statRow("Peak safety", warning ? "WARNING - CLICK" : "CLEAR", warning ? colours::red : colours::green);
}

ControlStripDisplay::ControlStripDisplay(juce::AudioProcessorValueTreeState& parameters, MeteringEngine& metering)
    : state(parameters), meters(metering)
{
    setTooltip("Live overview of EQ bands, compressor thresholds, harmonic enhancement, stereo width, and low-end preservation.");
    startTimerHz(20);
}

float ControlStripDisplay::parameter(const char* id) const noexcept
{
    if (const auto* value = state.getRawParameterValue(id)) return value->load(std::memory_order_relaxed);
    return 0.0f;
}

void ControlStripDisplay::timerCallback()
{
    repaint();
}

void ControlStripDisplay::drawEqualizer(juce::Graphics& g, juce::Rectangle<float> area)
{
    g.setColour(colours::text);
    g.setFont(juce::FontOptions(10.5f, juce::Font::bold));
    g.drawText("EQ BANDS", area.removeFromTop(17.0f).toNearestInt(), juce::Justification::centredLeft);
    auto graph = area.reduced(6.0f, 4.0f).withTrimmedBottom(14.0f);
    const auto zeroY = graph.getCentreY();
    g.setColour(colours::grid);
    g.drawHorizontalLine(static_cast<int>(zeroY), graph.getX(), graph.getRight());

    juce::Path curve;
    for (int band = 0; band < 6; ++band)
    {
        const auto index = static_cast<size_t>(band);
        const auto frequency = parameter(params::eqBandFrequency[index]);
        const auto gain = parameter(params::eqBandGain[index]);
        const auto enabled = parameter(params::eqBandEnabled[index]) > 0.5f && parameter(params::eqEnabled) > 0.5f;
        const auto x = juce::jmap(std::log10(juce::jlimit(20.0f, 20000.0f, frequency)),
                                  std::log10(20.0f), std::log10(20000.0f), graph.getX(), graph.getRight());
        const auto y = juce::jmap(juce::jlimit(-12.0f, 12.0f, gain), 12.0f, -12.0f,
                                  graph.getY(), graph.getBottom());
        if (band == 0) curve.startNewSubPath(x, y); else curve.lineTo(x, y);
        g.setColour(enabled ? colours::green : colours::muted);
        g.fillEllipse(juce::Rectangle<float>(7.0f, 7.0f).withCentre({x, y}));
    }
    g.setColour(colours::green.withAlpha(parameter(params::eqEnabled) > 0.5f ? 0.9f : 0.3f));
    g.strokePath(curve, juce::PathStrokeType(1.7f, juce::PathStrokeType::curved,
                                             juce::PathStrokeType::rounded));
    g.setColour(colours::textSecondary);
    g.setFont(juce::FontOptions(8.5f));
    g.drawText("20 Hz", static_cast<int>(graph.getX()), static_cast<int>(graph.getBottom()) + 1,
               38, 12, juce::Justification::centredLeft);
    g.drawText("20 kHz", static_cast<int>(graph.getRight()) - 42, static_cast<int>(graph.getBottom()) + 1,
               42, 12, juce::Justification::centredRight);
}

void ControlStripDisplay::drawThresholds(juce::Graphics& g, juce::Rectangle<float> area)
{
    g.setColour(colours::text);
    g.setFont(juce::FontOptions(10.5f, juce::Font::bold));
    g.drawText("BAND THRESHOLDS", area.removeFromTop(17.0f).toNearestInt(), juce::Justification::centredLeft);
    auto graph = area.reduced(6.0f, 3.0f).withTrimmedBottom(14.0f);
    const auto slotWidth = graph.getWidth() / 4.0f;
    for (int band = 0; band < 4; ++band)
    {
        const auto index = static_cast<size_t>(band);
        const auto threshold = parameter(params::bandThreshold[index]);
        const auto reduction = meters.bandReductionDb[index].load(std::memory_order_relaxed);
        const auto enabled = parameter(params::bandEnabled[index]) > 0.5f && parameter(params::multibandEnabled) > 0.5f;
        const auto centreX = graph.getX() + (static_cast<float>(band) + 0.5f) * slotWidth;
        auto track = juce::Rectangle<float>(juce::jmax(6.0f, slotWidth - 12.0f), graph.getHeight())
                         .withCentre({centreX, graph.getCentreY()});
        g.setColour(colours::meterTrack);
        g.fillRoundedRectangle(track, 2.5f);
        const auto thresholdY = juce::jmap(juce::jlimit(-50.0f, 0.0f, threshold), 0.0f, -50.0f,
                                           track.getY(), track.getBottom());
        g.setColour(enabled ? colours::amber.withAlpha(0.32f) : colours::muted.withAlpha(0.2f));
        g.fillRoundedRectangle(track.withTop(thresholdY), 2.5f);
        g.setColour(enabled ? colours::amber : colours::muted);
        g.fillRect(track.getX() - 1.0f, thresholdY, track.getWidth() + 2.0f, 1.5f);
        const auto reductionHeight = track.getHeight() * juce::jlimit(0.0f, 1.0f, reduction / 18.0f);
        g.setColour(colours::red.withAlpha(enabled ? 0.72f : 0.2f));
        g.fillRoundedRectangle(track.withHeight(reductionHeight), 2.5f);
        g.setColour(colours::textSecondary);
        g.setFont(juce::FontOptions(8.5f, juce::Font::bold));
        g.drawText("B" + juce::String(band + 1), static_cast<int>(centreX - slotWidth * 0.5f),
                   static_cast<int>(graph.getBottom()) + 1, static_cast<int>(slotWidth), 12,
                   juce::Justification::centred);
    }
}

void ControlStripDisplay::drawHarmonics(juce::Graphics& g, juce::Rectangle<float> area)
{
    g.setColour(colours::text);
    g.setFont(juce::FontOptions(10.5f, juce::Font::bold));
    g.drawText("HARMONIC SHAPE", area.removeFromTop(17.0f).toNearestInt(), juce::Justification::centredLeft);
    auto graph = area.reduced(6.0f, 3.0f).withTrimmedBottom(16.0f);
    const auto driveDb = parameter(params::enhancerDriveDb);
    const auto mix = parameter(params::enhancerMixPercent) * 0.01f;
    const auto tone = parameter(params::enhancerTonePercent) * 0.01f;
    const auto drive = juce::Decibels::decibelsToGain(driveDb);
    juce::Path transfer;
    for (int point = 0; point <= 40; ++point)
    {
        const auto normalisedX = static_cast<float>(point) / 40.0f;
        const auto input = juce::jmap(normalisedX, -1.0f, 1.0f);
        const auto shaped = std::tanh(input * drive * (1.0f + 0.18f * tone)) / juce::jmax(1.0f, drive);
        const auto output = juce::jmap(mix, input, shaped);
        const auto x = juce::jmap(normalisedX, graph.getX(), graph.getRight());
        const auto y = juce::jmap(output, 1.0f, -1.0f, graph.getY(), graph.getBottom());
        if (point == 0) transfer.startNewSubPath(x, y); else transfer.lineTo(x, y);
    }
    g.setColour(colours::grid);
    g.drawLine(graph.getX(), graph.getBottom(), graph.getRight(), graph.getY(), 1.0f);
    g.setColour(parameter(params::enhancerEnabled) > 0.5f ? colours::purple : colours::muted);
    g.strokePath(transfer, juce::PathStrokeType(2.0f, juce::PathStrokeType::curved,
                                                juce::PathStrokeType::rounded));
    g.setColour(colours::textSecondary);
    g.setFont(juce::FontOptions(8.5f));
    g.drawFittedText("DRIVE " + juce::String(driveDb, 1) + " dB   MIX " + juce::String(mix * 100.0f, 0) + "%",
                     area.withTrimmedTop(area.getHeight() - 15.0f).toNearestInt(),
                     juce::Justification::centred, 1, 0.72f);
}

void ControlStripDisplay::drawStereoAndBass(juce::Graphics& g, juce::Rectangle<float> area)
{
    g.setColour(colours::text);
    g.setFont(juce::FontOptions(10.5f, juce::Font::bold));
    g.drawText("STEREO + LOW END", area.removeFromTop(17.0f).toNearestInt(), juce::Justification::centredLeft);
    auto graph = area.reduced(6.0f, 3.0f).withTrimmedBottom(17.0f);
    const auto width = parameter(params::stereoWidthPercent);
    const auto balance = parameter(params::stereoBalancePercent);
    const auto bassFrequency = parameter(params::monoBassFrequencyHz);
    const auto correlation = meters.correlation.load(std::memory_order_relaxed);
    const auto fieldWidth = graph.getWidth() * juce::jmap(juce::jlimit(0.0f, 150.0f, width), 0.0f, 150.0f, 0.18f, 0.92f);
    const auto fieldHeight = graph.getHeight() * juce::jmap(juce::jlimit(-1.0f, 1.0f, correlation),
                                                            -1.0f, 1.0f, 0.92f, 0.28f);
    const auto field = juce::Rectangle<float>(fieldWidth, fieldHeight).withCentre(graph.getCentre());
    g.setColour(colours::grid);
    g.drawVerticalLine(static_cast<int>(graph.getCentreX()), graph.getY(), graph.getBottom());
    g.drawHorizontalLine(static_cast<int>(graph.getCentreY()), graph.getX(), graph.getRight());
    g.setColour(correlation < 0.0f ? colours::red : colours::cyan);
    g.drawEllipse(field, 2.0f);
    const auto balanceX = juce::jmap(juce::jlimit(-100.0f, 100.0f, balance), -100.0f, 100.0f,
                                     graph.getX(), graph.getRight());
    g.fillEllipse(juce::Rectangle<float>(5.0f, 5.0f).withCentre({balanceX, graph.getCentreY()}));
    const auto bassPosition = juce::jmap(juce::jlimit(40.0f, 250.0f, bassFrequency), 40.0f, 250.0f,
                                         graph.getX(), graph.getRight());
    g.setColour(parameter(params::monoBassEnabled) > 0.5f ? colours::green : colours::muted);
    g.fillRoundedRectangle(juce::Rectangle<float>(graph.getX(), graph.getBottom() - 3.0f,
                                                   bassPosition - graph.getX(), 3.0f), 1.5f);
    g.setColour(colours::textSecondary);
    g.setFont(juce::FontOptions(8.5f));
    g.drawFittedText("WIDTH " + juce::String(width, 0) + "%   BASS < " + juce::String(bassFrequency, 0) + " Hz",
                     area.withTrimmedTop(area.getHeight() - 15.0f).toNearestInt(),
                     juce::Justification::centred, 1, 0.7f);
}

void ControlStripDisplay::paint(juce::Graphics& g)
{
    const auto card = getLocalBounds().toFloat().reduced(1.0f);
    juce::ColourGradient gradient(colours::panelElevated, card.getX(), card.getY(),
                                  colours::panel, card.getX(), card.getBottom(), false);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(card, 10.0f);
    g.setColour(colours::border);
    g.drawRoundedRectangle(card, 10.0f, 1.0f);

    auto area = getLocalBounds().toFloat().reduced(11.0f);
    auto heading = area.removeFromTop(38.0f);
    g.setColour(colours::text);
    g.setFont(juce::FontOptions(13.5f, juce::Font::bold));
    g.drawText("CONTROL STRIP ACTIVITY", heading.removeFromTop(19.0f).toNearestInt(), juce::Justification::centredLeft);
    g.setColour(colours::textSecondary);
    g.setFont(juce::FontOptions(9.5f));
    g.drawText("LIVE PARAMETER OVERVIEW", heading.toNearestInt(), juce::Justification::centredLeft);

    area.removeFromTop(3.0f);
    const auto gap = 6.0f;
    const auto rowHeight = (area.getHeight() - gap) * 0.5f;
    auto top = area.removeFromTop(rowHeight);
    area.removeFromTop(gap);
    auto bottom = area;
    const auto columnWidth = (top.getWidth() - gap) * 0.5f;
    auto eqArea = top.removeFromLeft(columnWidth);
    top.removeFromLeft(gap);
    auto thresholdArea = top;
    auto harmonicArea = bottom.removeFromLeft(columnWidth);
    bottom.removeFromLeft(gap);
    auto stereoArea = bottom;

    for (const auto panel : {eqArea, thresholdArea, harmonicArea, stereoArea})
    {
        g.setColour(colours::background.withAlpha(0.48f));
        g.fillRoundedRectangle(panel, 6.0f);
        g.setColour(colours::grid);
        g.drawRoundedRectangle(panel, 6.0f, 1.0f);
    }
    drawEqualizer(g, eqArea.reduced(7.0f, 5.0f));
    drawThresholds(g, thresholdArea.reduced(7.0f, 5.0f));
    drawHarmonics(g, harmonicArea.reduced(7.0f, 5.0f));
    drawStereoAndBass(g, stereoArea.reduced(7.0f, 5.0f));
}

AuroraBroadcastProcessorEditor::AuroraBroadcastProcessorEditor(AuroraBroadcastProcessor& p)
    : AudioProcessorEditor(p), processor(p),
      spectrum(p.parameters, p.engine().inputAnalyzer(), p.engine().outputAnalyzer(),
               [this] { return processor.getSampleRate(); }),
      controlStripDisplay(p.parameters, p.engine().meters()),
      meterDisplay(p.engine().meters())
{
    processor.engine().setAnalyzerConsumerActive(true);
    setLookAndFeel(&lookAndFeel);
    setOpaque(true);
    setResizable(true, true);
    setResizeLimits(config::minimumWidth, config::minimumHeight, config::maximumWidth, config::maximumHeight);
    const auto state = p.parameters.state;
    setSize(juce::jlimit(config::minimumWidth, config::maximumWidth,
                         static_cast<int>(state.getProperty("editorWidth", config::defaultWidth))),
            juce::jlimit(config::minimumHeight, config::maximumHeight,
                         static_cast<int>(state.getProperty("editorHeight", config::defaultHeight))));

    title.setText(juce::String(config::productName).toUpperCase(), juce::dontSendNotification);
    title.setFont(juce::FontOptions(21.0f, juce::Font::bold));
    title.setColour(juce::Label::textColourId, colours::cyan);
    title.setMinimumHorizontalScale(0.7f);
    presetLabel.setText("PRESET", juce::dontSendNotification);
    presetLabel.setColour(juce::Label::textColourId, colours::textSecondary);
    presetLabel.setFont(juce::FontOptions(12.0f, juce::Font::bold));
    compareLabel.setText("A/B COMPARE", juce::dontSendNotification);
    compareLabel.setColour(juce::Label::textColourId, colours::textSecondary);
    compareLabel.setFont(juce::FontOptions(12.0f, juce::Font::bold));
    footer.setColour(juce::Label::textColourId, colours::textSecondary);
    footer.setFont(juce::FontOptions(12.5f));

    for (const auto& preset : factoryPresets()) presetBox.addItem(preset.name, presetBox.getNumItems() + 1);
    presetBox.setSelectedItemIndex(p.presets.currentFactoryIndex(), juce::dontSendNotification);
    presetBox.setTooltip("Select a factory preset.");
    previous.setTooltip("Load the previous factory preset.");
    next.setTooltip("Load the next factory preset.");
    save.setTooltip("Save the current settings as a user preset.");
    saveAs.setTooltip("Export the current settings to an Aurora preset file.");
    reset.setTooltip("Restore the Safe Start factory preset.");
    aButton.setTooltip("Recall parameter snapshot A.");
    bButton.setTooltip("Recall parameter snapshot B.");
    aToB.setTooltip("Replace snapshot B with snapshot A.");
    bToA.setTooltip("Replace snapshot A with snapshot B.");
    advanced.setTooltip("Show or hide the detailed parameter controls.");
    info.setTooltip("Show product information and version.");
    bypass.setTooltip("Bypass all processing with latency compensation.");

    advancedMode = static_cast<bool>(state.getProperty("advancedView", true));
    advanced.setClickingTogglesState(true);
    advanced.setToggleState(advancedMode, juce::dontSendNotification);
    advanced.setButtonText(advancedMode ? "Hide Controls" : "Show Controls");
    aButton.setClickingTogglesState(true);
    bButton.setClickingTogglesState(true);
    juce::Component* components[] { &title, &presetLabel, &compareLabel, &presetBox, &previous, &next,
                                    &save, &saveAs, &aButton, &bButton, &aToB, &bToA, &reset,
                                    &advanced, &info, &bypass, &spectrum, &controlStripDisplay,
                                    &meterDisplay, &viewport, &footer };
    for (auto* component : components) addAndMakeVisible(component);

    viewport.setViewedComponent(&controlContent, false);
    viewport.setScrollBarsShown(true, false);
    viewport.setScrollBarThickness(10);
    viewport.setVisible(advancedMode);
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        p.parameters, params::globalBypass, bypass);

    presetBox.onChange = [this]
    {
        if (presetBox.getSelectedItemIndex() >= 0) processor.presets.loadFactory(presetBox.getSelectedItemIndex());
    };
    previous.onClick = [this] { selectRelativePreset(-1); };
    next.onClick = [this] { selectRelativePreset(1); };
    save.onClick = [this] { savePreset(); };
    saveAs.onClick = [this] { saveAsPreset(); };
    aButton.onClick = [this]
    {
        processor.presets.selectA();
        aButton.setToggleState(true, juce::dontSendNotification);
        bButton.setToggleState(false, juce::dontSendNotification);
    };
    bButton.onClick = [this]
    {
        processor.presets.selectB();
        aButton.setToggleState(false, juce::dontSendNotification);
        bButton.setToggleState(true, juce::dontSendNotification);
    };
    aToB.onClick = [this] { processor.presets.copyAToB(); };
    bToA.onClick = [this] { processor.presets.copyBToA(); };
    reset.onClick = [this]
    {
        processor.presets.loadFactory(0);
        presetBox.setSelectedItemIndex(0, juce::dontSendNotification);
    };
    advanced.onClick = [this]
    {
        advancedMode = advanced.getToggleState();
        advanced.setButtonText(advancedMode ? "Hide Controls" : "Show Controls");
        viewport.setVisible(advancedMode);
        processor.parameters.state.setProperty("advancedView", advancedMode, nullptr);
        resized();
    };
    info.onClick = []
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::MessageBoxIconType::InfoIcon, config::productName,
            "Digital and internet broadcast audio processing.\nNot RF equipment or a certified compliance processor.\n\nVersion "
                + juce::String(config::version));
    };
    aButton.setToggleState(true, juce::dontSendNotification);

    beginSection("Input and automatic leveling",
                 "Input conditioning, high-pass filtering, and slow linked gain control.", colours::cyan);
    addKnob(params::inputTrimDb, "Input Trim", "Input level before processing.");
    addKnob(params::highPassFrequencyHz, "High-Pass Frequency", "High-pass filter cutoff frequency.");
    addKnob(params::agcTargetDb, "AGC Target", "Slow RMS level target.");
    addKnob(params::agcMaxGainDb, "AGC Maximum Gain", "Maximum upward gain correction.");
    addKnob(params::agcMaxAttenuationDb, "AGC Maximum Attenuation", "Maximum downward gain correction.");
    addKnob(params::agcAttackMs, "AGC Attack", "Downward gain response time.");
    addKnob(params::agcReleaseMs, "AGC Release", "Upward gain recovery time.");
    addKnob(params::agcGateThresholdDb, "AGC Gate Threshold", "Level below which silence is not raised.");
    addKnob(params::agcStereoLinkPercent, "AGC Stereo Link", "Amount of shared gain control between left and right channels.");
    addToggle(params::dcBlockEnabled, "Enable DC Blocking", "Remove sub-audible DC offset.");
    addToggle(params::highPassEnabled, "Enable High-Pass Filter", "Enable the input high-pass filter.");
    addToggle(params::agcEnabled, "Enable Automatic Gain Control", "Enable slow automatic gain control.");

    beginSection("Six-band parametric equalizer",
                 "All EQ values are editable here and directly on the spectrum above.", colours::green);
    for (int band = 0; band < 6; ++band)
    {
        const auto number = juce::String(band + 1);
        addKnob(params::eqBandFrequency[static_cast<size_t>(band)], "EQ " + number + " Frequency", "Equalizer band " + number + " center frequency.");
        addKnob(params::eqBandGain[static_cast<size_t>(band)], "EQ " + number + " Gain", "Equalizer band " + number + " gain.");
        addKnob(params::eqBandQ[static_cast<size_t>(band)], "EQ " + number + " Q", "Equalizer band " + number + " bandwidth/Q.");
    }
    addToggle(params::eqEnabled, "Enable Parametric Equalizer", "Enable all equalizer bands.");
    for (int band = 0; band < 6; ++band)
        addToggle(params::eqBandEnabled[static_cast<size_t>(band)], "Enable EQ Band " + juce::String(band + 1),
                  "Enable equalizer band " + juce::String(band + 1) + ".");

    beginSection("Multiband crossover",
                 "Define the four processing ranges and the overall compression amount.", colours::purple);
    addKnob(params::crossover1Hz, "Low Crossover", "Low-to-low-mid crossover frequency.");
    addKnob(params::crossover2Hz, "Mid Crossover", "Low-mid-to-high-mid crossover frequency.");
    addKnob(params::crossover3Hz, "High Crossover", "High-mid-to-high crossover frequency.");
    addKnob(params::multibandAmountPercent, "Multiband Amount", "Overall multiband compression intensity.");
    addToggle(params::multibandEnabled, "Enable Multiband Dynamics", "Enable four-band crossover compression.");

    for (int band = 0; band < 4; ++band)
    {
        const auto number = juce::String(band + 1);
        beginSection("Dynamics band " + number,
                     "Threshold, ratio, timing, knee, makeup, and parallel mix for band " + number + ".",
                     band % 2 == 0 ? colours::cyan : colours::purple);
        addKnob(params::bandThreshold[static_cast<size_t>(band)], "Band " + number + " Threshold", "Compressor threshold for band " + number + ".");
        addKnob(params::bandRatio[static_cast<size_t>(band)], "Band " + number + " Ratio", "Compression ratio for band " + number + ".");
        addKnob(params::bandAttack[static_cast<size_t>(band)], "Band " + number + " Attack", "Compressor attack time for band " + number + ".");
        addKnob(params::bandRelease[static_cast<size_t>(band)], "Band " + number + " Release", "Compressor release time for band " + number + ".");
        addKnob(params::bandKnee[static_cast<size_t>(band)], "Band " + number + " Knee", "Soft-knee width for band " + number + ".");
        addKnob(params::bandMakeup[static_cast<size_t>(band)], "Band " + number + " Makeup Gain", "Output makeup gain for band " + number + ".");
        addKnob(params::bandMix[static_cast<size_t>(band)], "Band " + number + " Parallel Mix", "Compressed/dry parallel mix for band " + number + ".");
        addToggle(params::bandEnabled[static_cast<size_t>(band)], "Enable Dynamics Band " + number, "Enable compressor band " + number + ".");
        addToggle(params::bandSolo[static_cast<size_t>(band)], "Solo Dynamics Band " + number, "Audition compressor band " + number + " by itself.");
    }

    beginSection("Harmonics, stereo, and low-end preservation",
                 "Add controlled density, adjust the stereo image, and preserve stable center bass.", colours::green);
    addKnob(params::enhancerDriveDb, "Enhancer Drive", "Harmonic density drive.");
    addKnob(params::enhancerMixPercent, "Enhancer Mix", "Harmonic wet/dry mix.");
    addKnob(params::enhancerTonePercent, "Enhancer Tone", "Harmonic brightness balance.");
    addKnob(params::stereoWidthPercent, "Stereo Width", "Mid/side stereo width.");
    addKnob(params::stereoBalancePercent, "Stereo Balance", "Left/right stereo balance.");
    addKnob(params::monoBassFrequencyHz, "Low-End Preservation Cutoff", "Preserve center bass by filtering low-frequency side energy below this cutoff.");
    addToggle(params::enhancerEnabled, "Enable Harmonic Enhancer", "Enable harmonic enhancement.");
    addToggle(params::stereoEnabled, "Enable Stereo Processing", "Enable stereo width and balance processing.");
    addToggle(params::monoBassEnabled, "Enable Low-End Preservation", "Keep center bass intact while filtering unstable low-frequency side energy.");
    addToggle(params::correlationProtectionEnabled, "Enable Correlation Protection", "Reduce widening when stereo correlation becomes unsafe.");

    beginSection("Peak control and output",
                 "Oversampled peak shaping, final look-ahead limiting, polarity, and output level.", colours::amber);
    addKnob(params::clipperDriveDb, "Clipper Drive", "Drive into the 4x oversampled peak shaper.");
    addKnob(params::limiterDriveDb, "Limiter Drive", "Input drive into the look-ahead limiter.");
    addKnob(params::limiterCeilingDb, "Limiter Ceiling", "Final output sample ceiling.");
    addKnob(params::limiterLookaheadMs, "Limiter Lookahead", "Limiter anticipation window.");
    addKnob(params::limiterReleaseMs, "Limiter Release", "Limiter gain recovery time.");
    addKnob(params::outputTrimDb, "Output Trim", "Final output level adjustment.");
    addToggle(params::clipperEnabled, "Enable Oversampled Peak Control", "Enable 4x oversampled soft peak shaping.");
    addToggle(params::limiterEnabled, "Enable Look-Ahead Limiter", "Enable final look-ahead limiting.");
    addToggle(params::invertLeft, "Invert Left Polarity", "Invert the left channel polarity.");
    addToggle(params::invertRight, "Invert Right Polarity", "Invert the right channel polarity.");
    addChoice(params::clipperMode, "Clipper Style", { "Soft", "Firm" });
    addChoice(params::limiterStyle, "Limiter Style", { "Transparent", "Punch" });

    beginSection("Spectrum and display",
                 "Choose the spectrum source, smoothing, and live/frozen display behavior.", colours::cyan);
    addKnob(params::analyzerSmoothing, "Analyzer Smoothing", "Visual spectrum smoothing only; audio is unaffected.");
    addToggle(params::analyzerEnabled, "Enable Spectrum Analyzer", "Enable live spectrum updates.");
    addToggle(params::analyzerFreeze, "Freeze Spectrum Display", "Hold the current spectrum display.");
    addChoice(params::analyzerMode, "Spectrum Source", { "Input", "Output", "Input and Output" });

    resized();
    startTimerHz(20);
}

AuroraBroadcastProcessorEditor::~AuroraBroadcastProcessorEditor()
{
    processor.engine().setAnalyzerConsumerActive(false);
    processor.parameters.state.setProperty("editorWidth", getWidth(), nullptr);
    processor.parameters.state.setProperty("editorHeight", getHeight(), nullptr);
    setLookAndFeel(nullptr);
}

void AuroraBroadcastProcessorEditor::beginSection(const juce::String& name, const juce::String& detail,
                                                   juce::Colour accentColour)
{
    auto section = std::make_unique<ControlSection>(name, detail, accentColour);
    currentSection = section.get();
    controlContent.addAndMakeVisible(*section);
    sections.push_back(std::move(section));
}

void AuroraBroadcastProcessorEditor::addKnob(const char* id, const juce::String& name, const juce::String& tip)
{
    jassert(currentSection != nullptr);
    auto control = std::make_unique<KnobControl>(processor.parameters, id, name, tip);
    currentSection->addControl(*control, ControlSection::Kind::knob);
    knobs.push_back(std::move(control));
}

void AuroraBroadcastProcessorEditor::addToggle(const char* id, const juce::String& name, const juce::String& tip)
{
    jassert(currentSection != nullptr);
    auto control = std::make_unique<ToggleControl>(processor.parameters, id, name, tip);
    currentSection->addControl(*control, ControlSection::Kind::toggle);
    toggles.push_back(std::move(control));
}

void AuroraBroadcastProcessorEditor::addChoice(const char* id, const juce::String& name, juce::StringArray items)
{
    jassert(currentSection != nullptr);
    auto control = std::make_unique<ChoiceControl>(processor.parameters, id, name, std::move(items));
    currentSection->addControl(*control, ControlSection::Kind::choice);
    choices.push_back(std::move(control));
}

void AuroraBroadcastProcessorEditor::paint(juce::Graphics& g)
{
    juce::ColourGradient background(colours::backgroundTop, 0.0f, 0.0f,
                                    colours::background, 0.0f, static_cast<float>(getHeight()), false);
    g.setGradientFill(background);
    g.fillAll();
    const auto header = getLocalBounds().reduced(12).removeFromTop(146).toFloat();
    g.setColour(colours::panel.withAlpha(0.96f));
    g.fillRoundedRectangle(header, 10.0f);
    g.setColour(colours::border);
    g.drawRoundedRectangle(header, 10.0f, 1.0f);
    g.setColour(colours::cyan.withAlpha(0.8f));
    g.fillRoundedRectangle(juce::Rectangle<float>(header.getX() + 1.0f, header.getBottom() - 3.0f,
                                                   header.getWidth() - 2.0f, 2.0f), 1.0f);
}

void AuroraBroadcastProcessorEditor::resized()
{
    processor.parameters.state.setProperty("editorWidth", getWidth(), nullptr);
    processor.parameters.state.setProperty("editorHeight", getHeight(), nullptr);

    auto area = getLocalBounds().reduced(12);
    auto header = area.removeFromTop(146).reduced(12, 8);
    auto titleRow = header.removeFromTop(38);
    header.removeFromTop(4);
    auto presetRow = header.removeFromTop(38);
    header.removeFromTop(4);
    auto compareRow = header.removeFromTop(38);

    const auto takeRight = [](juce::Rectangle<int>& row, int width)
    {
        auto result = row.removeFromRight(width);
        row.removeFromRight(6);
        return result.reduced(0, 2);
    };
    info.setBounds(takeRight(titleRow, 82));
    bypass.setBounds(takeRight(titleRow, 122));
    advanced.setBounds(takeRight(titleRow, 154));
    title.setBounds(titleRow);

    presetLabel.setBounds(presetRow.removeFromLeft(70));
    presetRow.removeFromLeft(6);
    previous.setBounds(presetRow.removeFromLeft(86).reduced(0, 2));
    presetRow.removeFromLeft(6);
    next.setBounds(presetRow.removeFromLeft(74).reduced(0, 2));
    presetRow.removeFromLeft(6);
    reset.setBounds(takeRight(presetRow, 112));
    saveAs.setBounds(takeRight(presetRow, 100));
    save.setBounds(takeRight(presetRow, 72));
    presetBox.setBounds(presetRow.reduced(0, 2));

    compareLabel.setBounds(compareRow.removeFromLeft(92));
    compareRow.removeFromLeft(6);
    aButton.setBounds(compareRow.removeFromLeft(112).reduced(0, 2));
    compareRow.removeFromLeft(6);
    bButton.setBounds(compareRow.removeFromLeft(112).reduced(0, 2));
    compareRow.removeFromLeft(6);
    aToB.setBounds(compareRow.removeFromLeft(112).reduced(0, 2));
    compareRow.removeFromLeft(6);
    bToA.setBounds(compareRow.removeFromLeft(112).reduced(0, 2));

    area.removeFromTop(8);
    auto footerArea = area.removeFromBottom(24);
    footer.setBounds(footerArea.reduced(4, 0));
    area.removeFromBottom(6);

    auto upper = advancedMode
                     ? area.removeFromTop(juce::jlimit(240, 380, static_cast<int>(area.getHeight() * 0.50f)))
                     : area;
    const auto meterWidth = juce::jlimit(210, 285, upper.getWidth() / 5);
    meterDisplay.setBounds(upper.removeFromRight(meterWidth));
    upper.removeFromRight(10);
    const auto overviewWidth = juce::jlimit(300, 370, upper.getWidth() / 3);
    controlStripDisplay.setBounds(upper.removeFromRight(overviewWidth));
    upper.removeFromRight(10);
    spectrum.setBounds(upper);

    if (advancedMode)
    {
        area.removeFromTop(10);
        viewport.setBounds(area);
        const auto contentWidth = juce::jmax(320, viewport.getWidth() - viewport.getScrollBarThickness() - 2);
        auto y = 0;
        for (auto& section : sections)
        {
            const auto height = section->preferredHeight(contentWidth - 8);
            section->setBounds(4, y, contentWidth - 8, height);
            y += height + 10;
        }
        controlContent.setSize(contentWidth, juce::jmax(viewport.getHeight(), y));
    }
    else
    {
        viewport.setBounds({});
    }

    jassert(isLayoutValidForTesting());
}

bool AuroraBroadcastProcessorEditor::isLayoutValidForTesting() const noexcept
{
    const auto local = getLocalBounds();
    const juce::Component* headerComponents[] { &title, &presetLabel, &compareLabel, &presetBox, &previous,
                                                &next, &save, &saveAs, &aButton, &bButton, &aToB, &bToA,
                                                &reset, &advanced, &info, &bypass };
    for (size_t firstIndex = 0; firstIndex < std::size(headerComponents); ++firstIndex)
    {
        const auto* first = headerComponents[firstIndex];
        if (first->getWidth() <= 0 || first->getHeight() <= 0 || !local.contains(first->getBounds())) return false;
        for (size_t secondIndex = firstIndex + 1; secondIndex < std::size(headerComponents); ++secondIndex)
            if (first->getBounds().intersects(headerComponents[secondIndex]->getBounds())) return false;
    }
    if (spectrum.getWidth() < 300 || spectrum.getHeight() < 180 || meterDisplay.getWidth() < 200
        || controlStripDisplay.getWidth() < 280 || controlStripDisplay.getHeight() < 180
        || spectrum.getBounds().intersects(controlStripDisplay.getBounds())
        || spectrum.getBounds().intersects(meterDisplay.getBounds())
        || controlStripDisplay.getBounds().intersects(meterDisplay.getBounds())
        || spectrum.getBounds().intersects(footer.getBounds())
        || controlStripDisplay.getBounds().intersects(footer.getBounds())
        || meterDisplay.getBounds().intersects(footer.getBounds())) return false;
    if (advancedMode && (viewport.getWidth() <= 0 || viewport.getHeight() <= 0
                         || viewport.getBounds().intersects(spectrum.getBounds())
                         || viewport.getBounds().intersects(controlStripDisplay.getBounds())
                         || viewport.getBounds().intersects(meterDisplay.getBounds()))) return false;
    if (advancedMode)
        for (const auto& section : sections)
            if (!section->hasValidLayout()) return false;
    return true;
}

void AuroraBroadcastProcessorEditor::timerCallback()
{
    meterDisplay.repaint();
    footer.setText("Sample rate: " + juce::String(processor.getSampleRate(), 0)
                       + " Hz    |    Block: " + juce::String(processor.currentBlockSize())
                       + " samples    |    Processing latency: " + juce::String(processor.getLatencySamples())
                       + " samples    |    Aurora v" + config::version,
                   juce::dontSendNotification);
}

void AuroraBroadcastProcessorEditor::selectRelativePreset(int delta)
{
    const auto index = (presetBox.getSelectedItemIndex() + delta + processor.getNumPrograms())
                       % processor.getNumPrograms();
    presetBox.setSelectedItemIndex(index, juce::sendNotificationSync);
}

void AuroraBroadcastProcessorEditor::savePreset()
{
    juce::String error;
    const auto name = "User " + juce::Time::getCurrentTime().formatted("%Y%m%d-%H%M%S");
    if (!processor.presets.saveUser(name, error))
        juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon,
                                               "Preset save failed", error);
}

void AuroraBroadcastProcessorEditor::saveAsPreset()
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Export Aurora preset",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("AuroraPreset.aurorapreset"),
        "*.aurorapreset");
    fileChooser->launchAsync(juce::FileBrowserComponent::saveMode
                                 | juce::FileBrowserComponent::canSelectFiles
                                 | juce::FileBrowserComponent::warnAboutOverwriting,
                             [this](const juce::FileChooser& chooser)
                             {
                                 const auto file = chooser.getResult();
                                 if (file != juce::File{})
                                 {
                                     juce::String error;
                                     if (!processor.presets.exportPreset(file, error))
                                         juce::AlertWindow::showMessageBoxAsync(
                                             juce::MessageBoxIconType::WarningIcon,
                                             "Preset export failed", error);
                                 }
                                 fileChooser.reset();
                             });
}
}
