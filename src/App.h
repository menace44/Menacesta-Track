#pragma once
#include <JuceHeader.h>
#include "audio/AudioEngine.h"
#include "gui/MainComponent.h"
#include "transport/Transport.h"

class App : public juce::Component,
            private juce::Timer
{
public:
    App();
    ~App() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;

    AudioEngine audioEngine;
    Transport transport;
    MainComponent mainComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(App)
};
