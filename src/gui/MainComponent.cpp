#include "MainComponent.h"
#include "../App.h"

MainComponent::MainComponent()
    : transport(App::getInstance()->getTransport()),
      transportControls(transport),
      timeline(transport),
      trackList(transport)
{
    transport->addChangeListener(this);

    addAndMakeVisible(transportControls);
    addAndMakeVisible(timeline);
    addAndMakeVisible(trackList);
    addAndMakeVisible(statusLabel);

    statusLabel.setText("Cross-Platform JUCE DAW - Ready", juce::dontSendNotification);
    statusLabel.setJustificationType(juce::Justification::centred);
}

MainComponent::~MainComponent()
{
    transport->removeChangeListener(this);
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(16.0f));
    g.drawText("Cross-Platform JUCE DAW", getLocalBounds().removeFromTop(30),
               juce::Justification::centredTop, true);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    
    auto headerBounds = bounds.removeFromTop(100);
    transportControls.setBounds(headerBounds.removeFromLeft(400));
    timeline.setBounds(headerBounds);
    
    trackList.setBounds(bounds);
    
    statusLabel.setBounds(getLocalBounds().removeFromBottom(20));
}

void MainComponent::update()
{
    statusLabel.setText("Cross-Platform JUCE DAW - Running", juce::dontSendNotification);
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == transport)
    {
        // Update any components that need to respond to transport changes
        repaint();
    }
}
