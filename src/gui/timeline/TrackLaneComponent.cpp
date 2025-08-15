/*
  ==============================================================================

   TrackLaneComponent.cpp
   Created: 15 Jan 2025
   Author:  DAW Development Team

   A component that combines track controls with the timeline lane,
   providing a unified view of a track in the timeline.

  ==============================================================================
*/

#include "TrackLaneComponent.h"

TrackLaneComponent::TrackLaneComponent(Track& track, TimelineViewport& viewport)
    : track(track)
    , viewport(viewport)
    , trackControls(track)
    , timelineTrack(track, viewport)
{
    addAndMakeVisible(trackControls);
    addAndMakeVisible(timelineTrack);
    
    viewport.addChangeListener(this);
    track.addChangeListener(this);
    
    updateTrackColor();
}

TrackLaneComponent::~TrackLaneComponent()
{
    viewport.removeChangeListener(this);
    track.removeChangeListener(this);
}

void TrackLaneComponent::paint(juce::Graphics& g)
{
    // Background
    if (selected)
    {
        g.fillAll(juce::Colour(0xFF3A3A3A));
    }
    else
    {
        g.fillAll(juce::Colour(0xFF2A2A2A));
    }
    
    // Border between track controls and timeline
    g.setColour(juce::Colour(0xFF444444));
    g.drawLine(static_cast<float>(trackControlWidth), 0.0f,
               static_cast<float>(trackControlWidth), getHeight(), 1.0f);
    
    // Bottom border
    g.setColour(juce::Colour(0xFF333333));
    g.drawLine(0.0f, getHeight() - 1.0f, getWidth(), getHeight() - 1.0f, 1.0f);
}

void TrackLaneComponent::resized()
{
    auto bounds = getLocalBounds();
    
    // Track controls on the left
    trackControls.setBounds(bounds.removeFromLeft(trackControlWidth));
    
    // Timeline track takes the remaining space
    timelineTrack.setBounds(bounds);
}

void TrackLaneComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &viewport)
    {
        repaint();
    }
    else if (source == &track)
    {
        // Track properties changed, update height
        setSize(getWidth(), track.getCalculatedHeight());
        repaint();
    }
}

void TrackLaneComponent::updateTrackColor()
{
    // Update colors based on track type
    juce::Colour trackColour;
    
    switch (track.getType())
    {
        case Track::Type::Audio:
            trackColour = juce::Colour(0xFF2196F3); // Blue
            break;
        case Track::Type::Midi:
            trackColour = juce::Colour(0xFF9C27B0); // Purple
            break;
        case Track::Type::Return:
            trackColour = juce::Colour(0xFFFF9800); // Orange
            break;
        case Track::Type::Master:
            trackColour = juce::Colour(0xFFF44336); // Red
            break;
        default:
            trackColour = juce::Colour(0xFF4CAF50); // Green
            break;
    }
    
    trackControls.setTrackColour(trackColour);
    timelineTrack.setTrackColour(trackColour);
}

void TrackLaneComponent::setSelected(bool shouldBeSelected)
{
    if (selected != shouldBeSelected)
    {
        selected = shouldBeSelected;
        repaint();
    }
}
