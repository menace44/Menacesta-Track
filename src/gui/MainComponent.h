#pragma once
#include <JuceHeader.h>
#include "../transport/Transport.h"
#include "TransportControls.h"
#include "TimelineComponent.h"
#include "TrackListComponent.h"

class MainComponent : public juce::Component,
                      private juce::ChangeListener
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void update();

private:
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    Transport* transport = nullptr;
    
    TransportControls transportControls;
    TimelineComponent timeline;
    TrackListComponent trackList;
    
    juce::Label statusLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
