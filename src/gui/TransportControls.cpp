#include "TransportControls.h"

TransportControls::TransportControls(Transport* transportToUse)
    : transport(transportToUse)
{
    jassert(transport != nullptr);
    
    transport->addChangeListener(this);
    
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(recordButton);
    addAndMakeVisible(rewindButton);
    addAndMakeVisible(fastForwardButton);
    addAndMakeVisible(addTrackButton);
    addAndMakeVisible(removeTrackButton);
    
    addAndMakeVisible(positionLabel);
    addAndMakeVisible(tempoLabel);
    
    playButton.onClick = [this] { playButtonClicked(); };
    stopButton.onClick = [this] { stopButtonClicked(); };
    recordButton.onClick = [this] { recordButtonClicked(); };
    rewindButton.onClick = [this] { rewindButtonClicked(); };
    fastForwardButton.onClick = [this] { fastForwardButtonClicked(); };
    addTrackButton.onClick = [this] { addTrackButtonClicked(); };
    removeTrackButton.onClick = [this] { removeTrackButtonClicked(); };
    
    positionLabel.setText("0:00.000", juce::dontSendNotification);
    positionLabel.setJustificationType(juce::Justification::centred);
    
    tempoLabel.setText("120 BPM", juce::dontSendNotification);
    tempoLabel.setJustificationType(juce::Justification::centred);
    
    updateButtonStates();
}

void TransportControls::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(12.0f));
    g.drawText("Transport", 10, 5, 100, 20, juce::Justification::left);
}

void TransportControls::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    
    int buttonWidth = 60;
    int buttonHeight = 30;
    int spacing = 5;
    
    int x = 10;
    int y = 30;
    
    rewindButton.setBounds(x, y, buttonWidth, buttonHeight);
    x += buttonWidth + spacing;
    
    playButton.setBounds(x, y, buttonWidth, buttonHeight);
    x += buttonWidth + spacing;
    
    stopButton.setBounds(x, y, buttonWidth, buttonHeight);
    x += buttonWidth + spacing;
    
    recordButton.setBounds(x, y, buttonWidth, buttonHeight);
    x += buttonWidth + spacing;
    
    fastForwardButton.setBounds(x, y, buttonWidth, buttonHeight);
    x += buttonWidth + spacing;
    
    addTrackButton.setBounds(x, y, buttonWidth, buttonHeight);
    x += buttonWidth + spacing;
    
    removeTrackButton.setBounds(x, y, buttonWidth, buttonHeight);
    
    positionLabel.setBounds(bounds.getWidth() - 200, y, 150, buttonHeight);
    tempoLabel.setBounds(bounds.getWidth() - 350, y, 100, buttonHeight);
}

void TransportControls::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == transport)
    {
        updateButtonStates();
        
        double position = transport->getCurrentPosition();
        int minutes = static_cast<int>(position / 60.0);
        double seconds = std::fmod(position, 60.0);
        
        positionLabel.setText(juce::String::formatted("%d:%06.3f", minutes, seconds), 
                            juce::dontSendNotification);
        
        tempoLabel.setText(juce::String(transport->getTempo()) + " BPM", 
                         juce::dontSendNotification);
    }
}

void TransportControls::updateButtonStates()
{
    playButton.setButtonText(transport->isPlaying() ? "Pause" : "Play");
    recordButton.setToggleState(transport->isRecording(), juce::dontSendNotification);
}

void TransportControls::playButtonClicked()
{
    if (transport->isPlaying())
        transport->stop();
    else
        transport->play();
}

void TransportControls::stopButtonClicked()
{
    transport->stop();
    transport->setPosition(0.0);
}

void TransportControls::recordButtonClicked()
{
    transport->setRecording(!transport->isRecording());
}

void TransportControls::rewindButtonClicked()
{
    double newPos = std::max(0.0, transport->getCurrentPosition() - 5.0);
    transport->setPosition(newPos);
}

void TransportControls::fastForwardButtonClicked()
{
    double newPos = transport->getCurrentPosition() + 5.0;
    transport->setPosition(newPos);
}
