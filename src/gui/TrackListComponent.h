#pragma once
#include <JuceHeader.h>
#include "../tracks/Track.h"
#include "TrackControlPanel.h"

class TrackListComponent : public juce::Component,
                          private juce::ChangeListener
{
public:
    TrackListComponent();
    ~TrackListComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void addTrack(Track* track);
    void removeTrack(Track* track);
    void clearTracks();
    
    int getNumTracks() const;
    Track* getTrack(int index) const;

private:
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void updateLayout();

    juce::OwnedArray<TrackControlPanel> trackPanels;
    juce::Array<Track*> tracks;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackListComponent)
};
