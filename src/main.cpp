#include <JuceHeader.h>
#include "App.h"

class CrossPlatformJUCEDAWApplication : public juce::JUCEApplication
{
public:
    CrossPlatformJUCEDAWApplication() {}
    
    const juce::String getApplicationName() override { return "Cross-Platform JUCE DAW"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    
    void initialise(const juce::String& commandLine) override
    {
        mainWindow = std::make_unique<MainWindow>(getApplicationName());
    }
    
    void shutdown() override
    {
        mainWindow = nullptr;
    }
    
private:
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name, juce::Desktop::getInstance().getDefaultLookAndFeel()
                             .findColour(juce::ResizableWindow::backgroundColourId),
                             DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new App(), true);
            setResizable(true, true);
            centreWithSize(1200, 800);
            setVisible(true);
        }
        
        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };
    
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(CrossPlatformJUCEDAWApplication)
