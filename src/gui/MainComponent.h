#pragma once
#include <JuceHeader.h>
#include "../transport/Transport.h"
#include "TransportControls.h"
#include "timeline/TimelineViewport.h"
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
    void addTrack(Track::TrackType type = Track::Audio);
    void removeTrack(int index);

private:
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    Transport* transport = nullptr;
    
    TransportControls transportControls;
    TimelineViewport timeline;
    TrackListComponent trackList;
    
    juce::Label statusLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
