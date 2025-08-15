/*
  ==============================================================================

   TimelineRuler.cpp
   Created: 15 Jan 2025
   Author:  DAW Development Team

   A horizontal timeline ruler component that displays time markers and grid lines
   for the timeline view.

  ==============================================================================
*/

#include "TimelineRuler.h"

TimelineRuler::TimelineRuler(TimelineViewport& viewport)
    : viewport(viewport)
{
    viewport.addChangeListener(this);
}

TimelineRuler::~TimelineRuler()
{
    viewport.removeChangeListener(this);
}

void TimelineRuler::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF2A2A2A)); // Dark background
    
    auto bounds = getLocalBounds();
    
    // Draw time markers
    drawTimeMarkers(g);
    
    // Draw bottom border
    g.setColour(juce::Colour(0xFF444444));
    g.drawLine(0.0f, bounds.getBottom() - 1.0f, bounds.getRight(), bounds.getBottom() - 1.0f, 1.0f);
}

void TimelineRuler::resized()
{
    // Component resized
}

void TimelineRuler::setPixelsPerSecond(double newPixelsPerSecond)
{
    if (pixelsPerSecond != newPixelsPerSecond)
    {
        pixelsPerSecond = newPixelsPerSecond;
        repaint();
    }
}

void TimelineRuler::setTimeRange(double newStartTime, double newEndTime)
{
    if (startTime != newStartTime || endTime != newEndTime)
    {
        startTime = newStartTime;
        endTime = newEndTime;
        repaint();
    }
}

void TimelineRuler::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &viewport)
    {
        repaint();
    }
}

void TimelineRuler::drawTimeMarkers(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    auto viewportBounds = viewport.getViewPosition();
    
    // Calculate visible time range
    double visibleStartTime = viewportBounds.x / pixelsPerSecond;
    double visibleEndTime = (viewportBounds.x + bounds.getWidth()) / pixelsPerSecond;
    
    // Determine appropriate time interval based on zoom level
    double interval;
    if (pixelsPerSecond < 10.0)
        interval = 10.0; // 10 seconds
    else if (pixelsPerSecond < 50.0)
        interval = 5.0;  // 5 seconds
    else if (pixelsPerSecond < 100.0)
        interval = 1.0;  // 1 second
    else if (pixelsPerSecond < 500.0)
        interval = 0.5;  // 500ms
    else
        interval = 0.1;  // 100ms
    
    // Find first marker position
    double firstMarkerTime = std::floor(visibleStartTime / interval) * interval;
    
    g.setFont(juce::Font(11.0f));
    g.setColour(juce::Colours::lightgrey);
    
    // Draw markers
    for (double time = firstMarkerTime; time <= visibleEndTime; time += interval)
    {
        if (time < 0.0) continue;
        
        int x = static_cast<int>(time * pixelsPerSecond - viewportBounds.x);
        
        if (x >= 0 && x <= bounds.getWidth())
        {
            // Draw marker line
            g.setColour(juce::Colour(0xFF666666));
            g.drawLine(static_cast<float>(x), 0.0f, static_cast<float>(x), static_cast<float>(bounds.getHeight() - 5), 1.0f);
            
            // Draw time label
            g.setColour(juce::Colours::lightgrey);
            g.drawText(formatTime(time), x + 2, 2, 60, 15, juce::Justification::left);
        }
    }
    
    // Draw major markers (bars/beats if we had tempo info)
    double majorInterval = interval * 4.0;
    double firstMajorTime = std::floor(visibleStartTime / majorInterval) * majorInterval;
    
    g.setColour(juce::Colour(0xFF888888));
    for (double time = firstMajorTime; time <= visibleEndTime; time += majorInterval)
    {
        if (time < 0.0) continue;
        
        int x = static_cast<int>(time * pixelsPerSecond - viewportBounds.x);
        
        if (x >= 0 && x <= bounds.getWidth())
        {
            g.drawLine(static_cast<float>(x), 0.0f, static_cast<float>(x), static_cast<float>(bounds.getHeight()), 2.0f);
        }
    }
}

juce::String TimelineRuler::formatTime(double timeInSeconds)
{
    int minutes = static_cast<int>(timeInSeconds / 60.0);
    double seconds = std::fmod(timeInSeconds, 60.0);
    
    if (minutes > 0)
    {
        return juce::String::formatted("%d:%05.2f", minutes, seconds);
    }
    else
    {
        return juce::String::formatted("%.2fs", seconds);
    }
}
