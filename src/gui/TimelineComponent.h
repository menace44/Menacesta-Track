#pragma once
#include <JuceHeader.h>
#include "../transport/Transport.h"
#include "../tracks/Track.h"
#include "../tracks/Clip.h"
#include "timeline/TimelineViewport.h"
#include "timeline/TimelineRuler.h"
#include "timeline/TrackLaneComponent.h"

class TimelineComponent : public juce::Component,
                         private juce::ChangeListener,
                         private juce::Timer
{
public:
    TimelineComponent(Transport* transportToUse);
    ~TimelineComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void setZoomLevel(double zoom);
    void setPixelsPerSecond(double pixels);
    
    void setTracks(const std::vector<Track*>& tracksToShow);
    void setTrackHeight(int height);
    
    void updatePlayheadPosition(double position);
    
    // Mouse interaction
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;

private:
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void timerCallback() override;
    
    void createTrackLanes();
    void updateTimelineRuler();
    
    Transport* transport;
    std::vector<Track*> tracks;
    std::vector<std::unique_ptr<TrackLaneComponent>> trackLanes;
    
    TimelineViewport viewport;
    TimelineRuler ruler;
    
    double pixelsPerSecond = 100.0;
    double visibleStartTime = 0.0;
    double visibleDuration = 30.0;
    
    int trackHeight = 60;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineComponent)
};
