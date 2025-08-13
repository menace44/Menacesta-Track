#include <JuceHeader.h>
#include "AudioEngineTest.cpp"

class TestRunner : public juce::JUCEApplication
{
public:
    TestRunner() {}
    
    const juce::String getApplicationName() override { return "Audio Engine Test"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    
    void initialise(const juce::String& commandLine) override
    {
        // Run the unit tests
        juce::UnitTestRunner runner;
        runner.runAllTests();
        
        // Print results
        auto results = runner.getResults();
        DBG("Test Results:");
        DBG(results);
        
        // Exit after tests
        quit();
    }
    
    void shutdown() override {}
};

START_JUCE_APPLICATION(TestRunner)
