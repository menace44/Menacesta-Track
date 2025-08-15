#pragma once

#include <JuceHeader.h>
#include "TimelineComponent.h"

class TimelineViewport : public juce::Viewport,
                        private juce::ScrollBar::Listener
{
public:
    TimelineViewport(Transport* transportToUse);
    ~TimelineViewport() override;

    void resized() override;
    void visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea) override;
    
    void setZoomLevel(double zoom);
    void setTracks(const std::vector<Track*>& tracksToShow);
    
    TimelineComponent* getTimelineComponent() const { return timeline.get(); }

private:
    void scrollBarMoved(juce::ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;
    
    std::unique_ptr<TimelineComponent> timeline;
    Transport* transport;
    
    double currentZoomLevel = 1.0;
    bool isScrolling = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineViewport)
};
