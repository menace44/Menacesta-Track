#include "TimelineTrackComponent.h"
#include "ClipComponent.h"

TimelineTrackComponent::TimelineTrackComponent(Track& trackRef)
    : track(trackRef)
{
    setName("Timeline Track: " + track.getName());
    track.addChangeListener(this);
    
    setSize(800, 60);
    createClipComponents();
}

TimelineTrackComponent::~TimelineTrackComponent()
{
    track.removeChangeListener(this);
}

void TimelineTrackComponent::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour(0xFF2A2A2A));
    
    // Track lane background
    g.setColour(juce::Colour(0xFF333333));
    g.fillRect(getLocalBounds());
    
    // Track border
    g.setColour(juce::Colour(0xFF444444));
    g.drawRect(getLocalBounds(), 1);
    
    // Grid lines (horizontal)
    g.setColour(juce::Colour(0xFF3A3A3A));
    for (int y = 10; y < getHeight(); y += 10)
    {
        g.drawHorizontalLine(y, 0.0f, (float)getWidth());
    }
    
    // Track name
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(12.0f));
    g.drawText(track.getName(), 5, 2, 100, 16, juce::Justification::left);
}

void TimelineTrackComponent::resized()
{
    updateClipPositions();
}

void TimelineTrackComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &track)
    {
        updateClipComponents();
    }
}

void TimelineTrackComponent::setZoomLevel(double zoom)
{
    zoomLevel = zoom;
    updateClipPositions();
}

void TimelineTrackComponent::setPlaybackPosition(double position)
{
    playbackPosition = position;
    repaint();
}

void TimelineTrackComponent::updateClipComponents()
{
    clipComponents.clear();
    createClipComponents();
}

void TimelineTrackComponent::createClipComponents()
{
    for (auto* clip : track.getClips())
    {
        auto* clipComp = new ClipComponent(*clip);
        addAndMakeVisible(clipComp);
        clipComponents.add(clipComp);
    }
    updateClipPositions();
}

void TimelineTrackComponent::updateClipPositions()
{
    const double pixelsPerSecond = 100.0 * zoomLevel;
    
    for (auto* clipComp : clipComponents)
    {
        if (auto* clip = dynamic_cast<ClipComponent*>(clipComp))
        {
            double startTime = clip->getClip().getStartTime();
            double duration = clip->getClip().getDuration();
            
            int x = static_cast<int>(startTime * pixelsPerSecond);
            int width = static_cast<int>(duration * pixelsPerSecond);
            
            clipComp->setBounds(x, 2, width, getHeight() - 4);
        }
    }
}
