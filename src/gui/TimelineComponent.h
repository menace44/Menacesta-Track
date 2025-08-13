#pragma once
#include <JuceHeader.h>
#include "../transport/Transport.h"

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
    
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

private:
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void timerCallback() override;
    
    double getTimeAtPosition(int x) const;
    int getPositionForTime(double time) const;

    Transport* transport;
    
    double visibleStartTime = 0.0;
    double visibleDuration = 10.0;
    double zoomLevel = 1.0;
    double pixelsPerSecond = 100.0;
    
    bool isDraggingPlayhead = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineComponent)
};
