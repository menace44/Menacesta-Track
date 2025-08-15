#pragma once
#include <JuceHeader.h>
#include "../tracks/Track.h"

class TrackControlPanel : public juce::Component,
                          private juce::ChangeListener
{
public:
    TrackControlPanel(Track* track);
    ~TrackControlPanel() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void updateDisplay();

private:
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;

    Track* track;
    
    juce::Label trackNameLabel;
    juce::Slider volumeSlider;
    juce::Slider panSlider;
    juce::Slider heightSlider;
    juce::TextButton muteButton;
    juce::TextButton soloButton;
    juce::TextButton recordButton;
    
    juce::Colour trackColour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackControlPanel)
};
