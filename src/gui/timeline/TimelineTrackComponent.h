#pragma once

#include <JuceHeader.h>
#include "../../tracks/Track.h"
#include "../../tracks/Clip.h"

class TimelineTrackComponent : public juce::Component,
                               public juce::ChangeListener
{
public:
    TimelineTrackComponent(Track& track);
    ~TimelineTrackComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    void setZoomLevel(double zoom);
    void setPlaybackPosition(double position);
    void updateClipComponents();
    
    Track& getTrack() const { return track; }

private:
    Track& track;
    double zoomLevel = 1.0;
    double playbackPosition = 0.0;
    
    juce::OwnedArray<juce::Component> clipComponents;
    
    void createClipComponents();
    void updateClipPositions();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineTrackComponent)
};
