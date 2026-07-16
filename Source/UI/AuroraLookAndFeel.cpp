#include "AuroraLookAndFeel.h"

#include "Config/ColourPalette.h"

namespace aurora
{
AuroraLookAndFeel::AuroraLookAndFeel()
{
    setColour(juce::Label::textColourId, colours::text);
    setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);
    setColour(juce::Slider::textBoxTextColourId, colours::text);
    setColour(juce::Slider::textBoxBackgroundColourId, colours::background.withAlpha(0.9f));
    setColour(juce::Slider::textBoxOutlineColourId, colours::border);
    setColour(juce::Slider::textBoxHighlightColourId, colours::cyan.withAlpha(0.35f));
    setColour(juce::ComboBox::backgroundColourId, colours::panelSecondary);
    setColour(juce::ComboBox::textColourId, colours::text);
    setColour(juce::ComboBox::outlineColourId, colours::border);
    setColour(juce::ComboBox::arrowColourId, colours::cyan);
    setColour(juce::PopupMenu::backgroundColourId, colours::panelElevated);
    setColour(juce::PopupMenu::textColourId, colours::text);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, colours::cyan.withAlpha(0.25f));
    setColour(juce::PopupMenu::highlightedTextColourId, colours::text);
    setColour(juce::TextButton::buttonColourId, colours::panelSecondary);
    setColour(juce::TextButton::textColourOffId, colours::text);
    setColour(juce::TextButton::textColourOnId, colours::background);
    setColour(juce::ToggleButton::textColourId, colours::text);
    setColour(juce::ScrollBar::thumbColourId, colours::cyan.withAlpha(0.75f));
    setColour(juce::ScrollBar::trackColourId, colours::panel);
}

juce::Font AuroraLookAndFeel::getTextButtonFont(juce::TextButton&, int buttonHeight)
{
    return juce::Font(juce::FontOptions(juce::jlimit(12.0f, 14.0f, buttonHeight * 0.38f), juce::Font::bold));
}

juce::Font AuroraLookAndFeel::getComboBoxFont(juce::ComboBox& box)
{
    return juce::Font(juce::FontOptions(juce::jlimit(12.0f, 14.0f, box.getHeight() * 0.4f)));
}

juce::Font AuroraLookAndFeel::getLabelFont(juce::Label& label)
{
    return label.getFont();
}

void AuroraLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float position, float startAngle, float endAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float>(static_cast<float>(x), static_cast<float>(y),
                                          static_cast<float>(width), static_cast<float>(height)).reduced(9.0f);
    const auto diameter = juce::jmin(bounds.getWidth(), bounds.getHeight());
    const auto knob = juce::Rectangle<float>(diameter, diameter).withCentre(bounds.getCentre());
    const auto centre = knob.getCentre();
    const auto radius = diameter * 0.5f;

    g.setColour(colours::cyan.withAlpha(slider.isMouseOverOrDragging() ? 0.14f : 0.07f));
    g.fillEllipse(knob.expanded(5.0f));
    g.setColour(colours::background.withAlpha(0.92f));
    g.fillEllipse(knob);
    g.setColour(colours::meterTrack);
    g.drawEllipse(knob.reduced(2.0f), 5.0f);

    juce::Path valueArc;
    valueArc.addCentredArc(centre.x, centre.y, radius - 3.0f, radius - 3.0f, 0.0f,
                           startAngle, startAngle + position * (endAngle - startAngle), true);
    g.setColour(slider.isEnabled() ? colours::cyan : colours::muted);
    g.strokePath(valueArc, juce::PathStrokeType(4.5f, juce::PathStrokeType::curved,
                                                juce::PathStrokeType::rounded));

    const auto angle = startAngle + position * (endAngle - startAngle);
    const auto indicator = centre.getPointOnCircumference(radius * 0.62f, angle);
    g.setColour(slider.isEnabled() ? colours::text : colours::muted);
    g.drawLine(centre.x, centre.y, indicator.x, indicator.y, 2.2f);
    g.setColour(colours::cyan);
    g.fillEllipse(juce::Rectangle<float>(4.5f, 4.5f).withCentre(indicator));
}

void AuroraLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                             const juce::Colour&, bool highlighted, bool down)
{
    const auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);
    auto fill = button.getToggleState() ? colours::cyan : colours::panelSecondary;
    if (highlighted) fill = fill.brighter(0.10f);
    if (down) fill = fill.darker(0.14f);
    g.setColour(colours::background.withAlpha(0.35f));
    g.fillRoundedRectangle(bounds.translated(0.0f, 2.0f), 6.0f);
    g.setColour(fill);
    g.fillRoundedRectangle(bounds, 6.0f);
    g.setColour(button.getToggleState() ? colours::cyan.brighter(0.25f) : colours::border);
    g.drawRoundedRectangle(bounds, 6.0f, highlighted ? 1.5f : 1.0f);
}

void AuroraLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                         bool highlighted, bool down)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);
    auto fill = button.getToggleState() ? colours::cyan.withAlpha(0.18f) : colours::background.withAlpha(0.72f);
    if (highlighted) fill = fill.brighter(0.08f);
    if (down) fill = fill.darker(0.10f);
    g.setColour(fill);
    g.fillRoundedRectangle(bounds, 6.0f);
    g.setColour(button.getToggleState() ? colours::cyan : colours::border);
    g.drawRoundedRectangle(bounds, 6.0f, button.getToggleState() ? 1.5f : 1.0f);

    const auto switchWidth = 30.0f;
    const auto switchBounds = juce::Rectangle<float>(switchWidth, 16.0f)
                                  .withCentre({bounds.getX() + 23.0f, bounds.getCentreY()});
    g.setColour(button.getToggleState() ? colours::cyan : colours::muted.darker(0.25f));
    g.fillRoundedRectangle(switchBounds, 8.0f);
    const auto thumbX = button.getToggleState() ? switchBounds.getRight() - 8.0f : switchBounds.getX() + 8.0f;
    g.setColour(button.getToggleState() ? colours::background : colours::textSecondary);
    g.fillEllipse(juce::Rectangle<float>(11.0f, 11.0f).withCentre({thumbX, switchBounds.getCentreY()}));

    g.setColour(button.isEnabled() ? colours::text : colours::muted);
    g.setFont(juce::FontOptions(12.5f, juce::Font::bold));
    g.drawFittedText(button.getButtonText(), 48, 0, juce::jmax(1, button.getWidth() - 54), button.getHeight(),
                     juce::Justification::centredLeft, 1, 0.78f);
}

void AuroraLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                                     int, int, int, int, juce::ComboBox&)
{
    auto bounds = juce::Rectangle<float>(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)).reduced(1.0f);
    g.setColour(isButtonDown ? colours::panelElevated.brighter(0.08f) : colours::panelSecondary);
    g.fillRoundedRectangle(bounds, 6.0f);
    g.setColour(colours::border);
    g.drawRoundedRectangle(bounds, 6.0f, 1.0f);
    const auto arrowArea = bounds.removeFromRight(34.0f).reduced(10.0f, 12.0f);
    juce::Path arrow;
    arrow.startNewSubPath(arrowArea.getX(), arrowArea.getY());
    arrow.lineTo(arrowArea.getCentreX(), arrowArea.getBottom());
    arrow.lineTo(arrowArea.getRight(), arrowArea.getY());
    g.setColour(colours::cyan);
    g.strokePath(arrow, juce::PathStrokeType(2.0f, juce::PathStrokeType::curved,
                                             juce::PathStrokeType::rounded));
}
}
