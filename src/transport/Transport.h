#pragma once
#include <JuceHeader.h>

class Transport : private juce::Timer
{
public:
    Transport(juce::AudioDeviceManager& dm);
    ~Transport() override;

    void play();
    void stop();
    void togglePlayStop();
    bool isPlaying() const { return playing; }

    double getCurrentPosition() const { return currentPosition; }
    void setCurrentPosition(double pos) { currentPosition = pos; }

    void addChangeListener(juce::ChangeListener* listener) { changeListeners.add(listener); }
    void removeChangeListener(juce::ChangeListener* listener) { changeListeners.remove(listener); }

private:
    void timerCallback() override;

    juce::AudioDeviceManager& deviceManager;
    bool playing = false;
    double currentPosition = 0.0;
    double lastUpdateTime = 0.0;
    juce::ChangeBroadcaster changeListeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Transport)
};
