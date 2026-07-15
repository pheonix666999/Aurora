#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_core/juce_core.h>
#include <iostream>

int main()
{
    juce::ScopedJuceInitialiser_GUI gui;juce::UnitTestRunner runner;runner.runAllTests();int failures=0;for(int i=0;i<runner.getNumResults();++i)if(auto* r=runner.getResult(i)){failures+=r->failures;if(r->failures>0){juce::String summary="AURORA_TEST_FAILURE: "+r->unitTestName+" / "+r->subcategoryName+" ("+juce::String(r->failures)+" failed)";for(const auto& message:r->messages)summary<<" | "<<message;juce::Logger::writeToLog(summary);std::cerr<<summary<<'\n';}}return failures==0?0:1;
}
