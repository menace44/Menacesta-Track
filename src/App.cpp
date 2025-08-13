#include "App.h"

App::App()
    : transport(audioEngine.getDeviceManager())
{
    addAndMakeVisible(mainComponent);
    setSize(1200, 800);
    startTimerHz(30);
}

App::~App()
{
    stopTimer();
}

void App::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkslategrey);
}

void App::resized()
{
    mainComponent.setBounds(getLocalBounds());
}

void App::timerCallback()
{
    transport.update();
    mainComponent.update();
}
