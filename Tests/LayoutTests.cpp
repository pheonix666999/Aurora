#include "PluginEditor.h"
#include "Config/ProductConfig.h"

class LayoutTests final : public juce::UnitTest
{
public:
    LayoutTests() : UnitTest("Responsive editor layout", "UI") {}

    void runTest() override
    {
        beginTest("Detailed controls never overlap at supported sizes");
        aurora::AuroraBroadcastProcessor processor;
        processor.parameters.state.setProperty("advancedView", true, nullptr);
        aurora::AuroraBroadcastProcessorEditor editor(processor);
        const juce::Point<int> sizes[] {
            {aurora::config::minimumWidth, aurora::config::minimumHeight},
            {aurora::config::defaultWidth, aurora::config::defaultHeight},
            {aurora::config::maximumWidth, aurora::config::minimumHeight},
            {aurora::config::minimumWidth, aurora::config::maximumHeight},
            {aurora::config::maximumWidth, aurora::config::maximumHeight}
        };
        for (const auto size : sizes)
        {
            editor.setSize(size.x, size.y);
            expect(editor.isLayoutValidForTesting(),
                   "Invalid layout at " + juce::String(size.x) + " x " + juce::String(size.y));
        }

        beginTest("Spectrum-only view remains valid at minimum size");
        aurora::AuroraBroadcastProcessor compactProcessor;
        compactProcessor.parameters.state.setProperty("advancedView", false, nullptr);
        aurora::AuroraBroadcastProcessorEditor compactEditor(compactProcessor);
        compactEditor.setSize(aurora::config::minimumWidth, aurora::config::minimumHeight);
        expect(compactEditor.isLayoutValidForTesting());

        beginTest("Live control-strip display renders at responsive sizes");
        aurora::ControlStripDisplay display(processor.parameters, processor.engine().meters());
        for (const auto size : {juce::Point<int>{300, 240}, juce::Point<int>{370, 380}})
        {
            display.setSize(size.x, size.y);
            const auto snapshot = display.createComponentSnapshot(display.getLocalBounds());
            expect(snapshot.isValid());
            expect(snapshot.getWidth() == size.x && snapshot.getHeight() == size.y);
            expect(snapshot.getPixelAt(size.x / 2, size.y / 2).getAlpha() > 0);
        }
    }
};

static LayoutTests layoutTests;
