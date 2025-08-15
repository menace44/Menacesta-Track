/*
  ==============================================================================

   TrackLaneComponent.h
   Created: 15 Jan 2025
   Author:  DAW Development Team

   A component that combines track controls with the timeline lane,
   providing a unified view of a track in the timeline.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../TrackControlPanel.h"
#include "TimelineTrackComponent.h"
#include "TimelineViewport.h"

class TrackLaneComponent : public juce::Component,
                          public juce::ChangeListener
{
public:
    TrackLaneComponent(Track& track, TimelineViewport& viewport);
    ~TrackLaneComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    Track& getTrack() const { return track; }
    TimelineTrackComponent& getTimelineTrack() { return timelineTrack; }
    
    // ChangeListener interface
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    void updateTrackColor();
    void setSelected(bool selected);
    bool isSelected() const { return selected; }

private:
    Track& track;
    TimelineViewport& viewport;
    
    TrackControlPanel trackControls;
    TimelineTrackComponent timelineTrack;
    
    bool selected = false;
    
    static constexpr int trackControlWidth = 150;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackLaneComponent)
};
