/*
  ==============================================================================

   TimelineComponent.cpp
   Created: 15 Jan 2025
   Author:  DAW Development Team

   Main timeline component that integrates the ruler, viewport, and track lanes
   for a complete timeline view.

  ==============================================================================
*/

#include "TimelineComponent.h"

TimelineComponent::TimelineComponent(Transport* transportToUse)
    : transport(transportToUse)
    , viewport(transportToUse)
    , ruler(transportToUse)
{
    addAndMakeVisible(ruler);
    addAndMakeVisible(viewport);
    
    if (transport != nullptr)
    {
        transport->addChangeListener(this);
    }
    
    startTimerHz(30); // 30 FPS for smooth updates
    
    setSize(800, 400);
}

TimelineComponent::~TimelineComponent()
{
    if (transport != nullptr)
    {
        transport->removeChangeListener(this);
    }
}

void TimelineComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF1A1A1A));
}

void TimelineComponent::resized()
{
    auto bounds = getLocalBounds();
    
    // Timeline ruler at the top
    auto rulerBounds = bounds.removeFromTop(30);
    ruler.setBounds(rulerBounds);
    
    // Viewport takes the remaining space
    viewport.setBounds(bounds);
}

void TimelineComponent::setZoomLevel(double zoom)
{
    viewport.setZoomLevel(zoom);
    ruler.setZoomLevel(zoom);
}

void TimelineComponent::setPixelsPerSecond(double pixels)
{
    pixelsPerSecond = pixels;
    viewport.setPixelsPerSecond(pixels);
    ruler.setPixelsPerSecond(pixels);
}

void TimelineComponent::setTracks(const std::vector<Track*>& tracksToShow)
{
    tracks = tracksToShow;
    createTrackLanes();
}

void TimelineComponent::setTrackHeight(int height)
{
    trackHeight = height;
    createTrackLanes();
}

void TimelineComponent::updatePlayheadPosition(double position)
{
    viewport.setPlayheadPosition(position);
    ruler.setPlayheadPosition(position);
}

void TimelineComponent::mouseDown(const juce::MouseEvent& event)
{
    if (event.mods.isLeftButtonDown())
    {
        // Handle timeline clicks
        if (event.getNumberOfClicks() == 2)
        {
            // Double-click to set playhead position
            double time = ruler.getTimeAtPosition(event.x);
            if (transport != nullptr)
            {
                transport->setPosition(time);
            }
        }
    }
}

void TimelineComponent::mouseDrag(const juce::MouseEvent& event)
{
    // Handle timeline dragging
    if (event.mods.isLeftButtonDown())
    {
        double time = ruler.getTimeAtPosition(event.x);
        if (transport != nullptr)
        {
            transport->setPosition(time);
        }
    }
}

void TimelineComponent::mouseUp(const juce::MouseEvent& event)
{
    // Handle mouse up events
}

void TimelineComponent::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    if (event.mods.isCtrlDown() || event.mods.isCommandDown())
    {
        // Zoom with Ctrl/Cmd + wheel
        double zoomFactor = wheel.deltaY > 0 ? 1.1 : 0.9;
        setZoomLevel(getZoomLevel() * zoomFactor);
    }
    else
    {
        // Scroll vertically
        viewport.mouseWheelMove(event, wheel);
    }
}

void TimelineComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == transport)
    {
        updatePlayheadPosition(transport->getPosition());
    }
}

void TimelineComponent::timerCallback()
{
    // Update playhead position during playback
    if (transport != nullptr && transport->isPlaying())
    {
        updatePlayheadPosition(transport->getPosition());
    }
}

void TimelineComponent::createTrackLanes()
{
    trackLanes.clear();
    
    int totalHeight = 0;
    for (auto* track : tracks)
    {
        auto lane = std::make_unique<TrackLaneComponent>(*track, viewport);
        lane->setBounds(0, totalHeight, viewport.getWidth(), trackHeight);
        viewport.addTrackLane(lane.get());
        totalHeight += trackHeight;
        trackLanes.push_back(std::move(lane));
    }
    
    viewport.setTotalHeight(totalHeight);
}

double TimelineComponent::getZoomLevel() const
{
    return viewport.getZoomLevel();
}
