#include "TrackListComponent.h"

TrackListComponent::TrackListComponent()
{
    setOpaque(true);
}

TrackListComponent::~TrackListComponent()
{
    clearTracks();
}

void TrackListComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(12.0f));
    g.drawText("Tracks", 10, 10, 100, 20, juce::Justification::left);
}

void TrackListComponent::resized()
{
    updateLayout();
}

void TrackListComponent::addTrack(Track* track)
{
    if (track == nullptr)
        return;
        
    tracks.add(track);
    
    auto* panel = new TrackControlPanel(track);
    trackPanels.add(panel);
    addAndMakeVisible(panel);
    
    track->addChangeListener(this);
    updateLayout();
}

void TrackListComponent::removeTrack(Track* track)
{
    int index = tracks.indexOf(track);
    if (index >= 0)
    {
        tracks.remove(index);
        trackPanels.remove(index);
        track->removeChangeListener(this);
        updateLayout();
    }
}

void TrackListComponent::clearTracks()
{
    for (auto* track : tracks)
    {
        track->removeChangeListener(this);
    }
    
    tracks.clear();
    trackPanels.clear();
    updateLayout();
}

int TrackListComponent::getNumTracks() const
{
    return tracks.size();
}

Track* TrackListComponent::getTrack(int index) const
{
    if (index >= 0 && index < tracks.size())
        return tracks[index];
    return nullptr;
}

void TrackListComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}

void TrackListComponent::updateLayout()
{
    auto bounds = getLocalBounds();
    
    int trackHeight = 120;
    int y = 40;  // Leave space for header
    
    for (auto* panel : trackPanels)
    {
        panel->setBounds(bounds.removeFromTop(trackHeight).reduced(2));
        y += trackHeight;
    }
}
