#include "PlayheadIndicator.h"

namespace Track {

PlayheadIndicator::PlayheadIndicator()
{
    setName("Playhead Indicator");
    setSize(2, 400);
    setOpaque(false);
}

PlayheadIndicator::~PlayheadIndicator()
{
}

void PlayheadIndicator::paint(juce::Graphics& g)
{
    g.setColour(playheadColor);
    g.fillRect(getLocalBounds().toFloat());
}

void PlayheadIndicator::resized()
{
    // Component resized
}

void PlayheadIndicator::setPlaybackPosition(double position)
{
    playbackPosition = position;
    updatePosition();
}

void PlayheadIndicator::setDuration(double duration)
{
    this->duration = duration;
    updatePosition();
}

void PlayheadIndicator::setVisibleRange(double startTime, double endTime)
{
    visibleStartTime = startTime;
    visibleEndTime = endTime;
    updatePosition();
}

void PlayheadIndicator::setColor(const juce::Colour& color)
{
    playheadColor = color;
    repaint();
}

void PlayheadIndicator::setThickness(float newThickness)
{
    thickness = newThickness;
    setSize(static_cast<int>(thickness), getHeight());
    repaint();
}

void PlayheadIndicator::updatePosition()
{
    if (duration <= 0.0 || visibleEndTime <= visibleStartTime)
    {
        setVisible(false);
        return;
    }
    
    double visibleDuration = visibleEndTime - visibleStartTime;
    double relativePosition = (playbackPosition - visibleStartTime) / visibleDuration;
    
    if (relativePosition < 0.0 || relativePosition > 1.0)
    {
        setVisible(false);
        return;
    }
    
    int x = static_cast<int>(relativePosition * getParentWidth());
    setTopLeftPosition(x, 0);
    setVisible(true);
}

} // namespace Track
