#include "TimelineComponent.h"

TimelineComponent::TimelineComponent(Transport* transportToUse)
    : transport(transportToUse)
{
    jassert(transport != nullptr);
    
    transport->addChangeListener(this);
    startTimerHz(30);
    
    setOpaque(true);
}

TimelineComponent::~TimelineComponent()
{
    transport->removeChangeListener(this);
    stopTimer();
}

void TimelineComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    
    auto bounds = getLocalBounds();
    
    // Draw grid
    g.setColour(juce::Colours::darkgrey);
    
    // Vertical grid lines (measures/beats)
    double timeStep = 0.5 / zoomLevel;
    double startTime = visibleStartTime;
    double endTime = visibleStartTime + visibleDuration;
    
    for (double t = std::floor(startTime / timeStep) * timeStep; t < endTime; t += timeStep)
    {
        int x = getPositionForTime(t);
        if (x >= 0 && x < getWidth())
        {
            g.drawLine(x, 0, x, getHeight(), 0.5f);
            
            // Draw time labels
            if (std::fmod(t, 1.0) < 0.01)
            {
                g.setColour(juce::Colours::lightgrey);
                g.drawText(juce::String(t, 1) + "s", x + 2, 2, 50, 20, juce::Justification::left);
                g.setColour(juce::Colours::darkgrey);
            }
        }
    }
    
    // Draw playhead
    double currentPosition = transport->getCurrentPosition();
    int playheadX = getPositionForTime(currentPosition);
    
    if (playheadX >= 0 && playheadX < getWidth())
    {
        g.setColour(juce::Colours::red);
        g.drawLine(playheadX, 0, playheadX, getHeight(), 2.0f);
        
        // Draw playhead triangle
        juce::Path playhead;
        playhead.addTriangle(playheadX - 5.0f, 0.0f,
                           playheadX + 5.0f, 0.0f,
                           playheadX, 8.0f);
        g.fillPath(playhead);
    }
    
    // Draw timeline ruler
    g.setColour(juce::Colours::lightgrey);
    g.fillRect(0, 0, getWidth(), 25);
    g.setColour(juce::Colours::black);
    g.drawText("Timeline", 10, 5, 100, 20, juce::Justification::left);
}

void TimelineComponent::resized()
{
    visibleDuration = getWidth() / pixelsPerSecond;
}

void TimelineComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == transport)
    {
        repaint();
    }
}

void TimelineComponent::timerCallback()
{
    repaint();
}

void TimelineComponent::mouseDown(const juce::MouseEvent& event)
{
    if (event.y > 25)  // Below the ruler
    {
        double newTime = getTimeAtPosition(event.x);
        transport->setPosition(newTime);
        isDraggingPlayhead = true;
    }
}

void TimelineComponent::mouseDrag(const juce::MouseEvent& event)
{
    if (isDraggingPlayhead)
    {
        double newTime = getTimeAtPosition(event.x);
        transport->setPosition(newTime);
    }
}

void TimelineComponent::mouseUp(const juce::MouseEvent& event)
{
    isDraggingPlayhead = false;
}

double TimelineComponent::getTimeAtPosition(int x) const
{
    return visibleStartTime + (x / pixelsPerSecond);
}

int TimelineComponent::getPositionForTime(double time) const
{
    return static_cast<int>((time - visibleStartTime) * pixelsPerSecond);
}

void TimelineComponent::setZoomLevel(double zoom)
{
    zoomLevel = zoom;
    pixelsPerSecond = 100.0 * zoom;
    resized();
    repaint();
}

void TimelineComponent::setPixelsPerSecond(double pixels)
{
    pixelsPerSecond = pixels;
    resized();
    repaint();
}
