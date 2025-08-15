#include "TimelineViewport.h"

TimelineViewport::TimelineViewport(Transport* transportToUse)
    : transport(transportToUse)
{
    jassert(transport != nullptr);
    
    timeline = std::make_unique<TimelineComponent>(transport);
    setViewedComponent(timeline.get(), false);
    
    setScrollBarsShown(true, true);
    setScrollBarThickness(12);
    
    getHorizontalScrollBar().addListener(this);
    getVerticalScrollBar().addListener(this);
}

TimelineViewport::~TimelineViewport()
{
    getHorizontalScrollBar().removeListener(this);
    getVerticalScrollBar().removeListener(this);
}

void TimelineViewport::resized()
{
    Viewport::resized();
    
    if (timeline)
    {
        auto visibleBounds = getViewArea();
        auto contentWidth = std::max(getWidth(), visibleBounds.getWidth());
        auto contentHeight = std::max(getHeight(), visibleBounds.getHeight());
        
        timeline->setSize(contentWidth, contentHeight);
    }
}

void TimelineViewport::visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea)
{
    Viewport::visibleAreaChanged(newVisibleArea);
    
    if (timeline)
    {
        timeline->repaint();
    }
}

void TimelineViewport::setZoomLevel(double zoom)
{
    currentZoomLevel = zoom;
    
    if (timeline)
    {
        timeline->setZoomLevel(zoom);
        
        // Adjust content size based on zoom
        auto visibleBounds = getViewArea();
        auto contentWidth = std::max(getWidth(), static_cast<int>(visibleBounds.getWidth() * zoom));
        auto contentHeight = std::max(getHeight(), visibleBounds.getHeight());
        
        timeline->setSize(contentWidth, contentHeight);
    }
}

void TimelineViewport::setTracks(const std::vector<Track*>& tracksToShow)
{
    if (timeline)
    {
        timeline->setTracks(tracksToShow);
    }
}

void TimelineViewport::scrollBarMoved(juce::ScrollBar* scrollBarThatHasMoved, double newRangeStart)
{
    if (timeline)
    {
        timeline->repaint();
    }
}
