/*
  ==============================================================================

   TimelineRuler.h
   Created: 15 Jan 2025
   Author:  DAW Development Team

   A horizontal timeline ruler component that displays time markers and grid lines
   for the timeline view.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TimelineViewport.h"

class TimelineRuler : public juce::Component,
                      public juce::ChangeListener
{
public:
    TimelineRuler(TimelineViewport& viewport);
    ~TimelineRuler() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void setPixelsPerSecond(double pixelsPerSecond);
    void setTimeRange(double startTime, double endTime);
    
    // ChangeListener interface
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    void drawTimeMarkers(juce::Graphics& g);
    juce::String formatTime(double timeInSeconds);
    
    TimelineViewport& viewport;
    double pixelsPerSecond = 100.0;
    double startTime = 0.0;
    double endTime = 60.0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineRuler)
};
