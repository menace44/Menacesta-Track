#pragma once
#include <JuceHeader.h>
#include "../transport/Transport.h"

class TransportControls : public juce::Component,
                        private juce::ChangeListener
{
public:
    TransportControls(Transport* transportToUse);
    ~TransportControls() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void updateButtonStates();
    
    void playButtonClicked();
    void stopButtonClicked();
    void recordButtonClicked();
    void rewindButtonClicked();
    void fastForwardButtonClicked();

    Transport* transport;
    
    juce::TextButton playButton{"Play"};
    juce::TextButton stopButton{"Stop"};
    juce::TextButton recordButton{"Record"};
    juce::TextButton rewindButton{"<<"};
    juce::TextButton fastForwardButton{">>"};
    
    juce::Label positionLabel;
    juce::Label tempoLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportControls)
};
