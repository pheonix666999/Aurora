#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_core/juce_core.h>

int main()
{
    juce::ScopedJuceInitialiser_GUI gui;juce::UnitTestRunner runner;runner.runAllTests();int failures=0;for(int i=0;i<runner.getNumResults();++i)if(auto* r=runner.getResult(i))failures+=r->failures;return failures==0?0:1;
}

