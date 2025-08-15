#include "TrackControlPanel.h"

TrackControlPanel::TrackControlPanel(Track* trackToControl)
    : track(trackToControl),
      volumeSlider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow),
      panSlider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow),
      heightSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
{
    jassert(track != nullptr);
    
    // Set up track name
    trackNameLabel.setText(track->getName(), juce::dontSendNotification);
    trackNameLabel.setJustificationType(juce::Justification::centred);
    trackNameLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    addAndMakeVisible(trackNameLabel);
    
    // Configure volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(track->getVolume(), juce::dontSendNotification);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    volumeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    volumeSlider.setTextValueSuffix(" dB");
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
    
    // Configure pan slider
    panSlider.setRange(-1.0, 1.0, 0.01);
    panSlider.setValue(track->getPan(), juce::dontSendNotification);
    panSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    panSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    panSlider.setTextValueSuffix(" L/R");
    panSlider.addListener(this);
    addAndMakeVisible(panSlider);

    // Configure height slider
    heightSlider.setRange(0.5, 3.0, 0.1);
    heightSlider.setValue(track->getHeightMultiplier(), juce::dontSendNotification);
    heightSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    heightSlider.setSliderStyle(juce::Slider::LinearVertical);
    heightSlider.setTextValueSuffix("x");
    heightSlider.addListener(this);
    addAndMakeVisible(heightSlider);
    
    // Configure buttons
    muteButton.setButtonText("M");
    muteButton.setClickingTogglesState(true);
    muteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    muteButton.addListener(this);
    addAndMakeVisible(muteButton);
    
    soloButton.setButtonText("S");
    soloButton.setClickingTogglesState(true);
    soloButton.setColour(juce::TextButton::buttonColourId, juce::Colours::yellow);
    soloButton.addListener(this);
    addAndMakeVisible(soloButton);
    
    recordButton.setButtonText("R");
    recordButton.setClickingTogglesState(true);
    recordButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    recordButton.addListener(this);
    addAndMakeVisible(recordButton);
    
    // Set track color based on track type
    trackColour = track->getType() == Track::AudioTrack ? 
                  juce::Colours::steelblue : juce::Colours::purple;
    
    track->addChangeListener(this);
}

TrackControlPanel::~TrackControlPanel()
{
    if (track != nullptr)
        track->removeChangeListener(this);
}

void TrackControlPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    
    // Draw track header
    g.setColour(trackColour);
    g.fillRect(0, 0, getWidth(), 30);
    
    // Draw separator
    g.setColour(juce::Colours::lightgrey);
    g.drawLine(0, getHeight() - 1, getWidth(), getHeight() - 1);
}

void TrackControlPanel::resized()
{
    auto bounds = getLocalBounds();
    
    // Track name at top
    trackNameLabel.setBounds(bounds.removeFromTop(30).reduced(5));
    
    // Volume slider
    auto volumeArea = bounds.removeFromTop(80).reduced(5);
    volumeSlider.setBounds(volumeArea);
    
    // Pan slider
    auto panArea = bounds.removeFromTop(80).reduced(5);
    panSlider.setBounds(panArea);

    // Height slider
    auto heightArea = bounds.removeFromTop(100).reduced(5);
    heightSlider.setBounds(heightArea);
    
    // Buttons at bottom
    auto buttonArea = bounds.removeFromBottom(40).reduced(5);
    auto buttonWidth = buttonArea.getWidth() / 3;
    
    muteButton.setBounds(buttonArea.removeFromLeft(buttonWidth).reduced(2));
    soloButton.setBounds(buttonArea.removeFromLeft(buttonWidth).reduced(2));
    recordButton.setBounds(buttonArea.reduced(2));
}

void TrackControlPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == track)
    {
        updateDisplay();
    }
}

void TrackControlPanel::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        track->setVolume((float)slider->getValue());
    }
    else if (slider == &panSlider)
    {
        track->setPan((float)slider->getValue());
    }
    else if (slider == &heightSlider)
    {
        track->setHeightMultiplier((float)slider->getValue());
    }
}

void TrackControlPanel::buttonClicked(juce::Button* button)
{
    if (button == &muteButton)
    {
        track->setMuted(button->getToggleState());
    }
    else if (button == &soloButton)
    {
        track->setSolo(button->getToggleState());
    }
    else if (button == &recordButton)
    {
        // Handle recording state
        if (button->getToggleState())
        {
            track->startRecording("track_" + track->getName() + "_recording.wav");
        }
        else
        {
            track->stopRecording();
        }
    }
}

void TrackControlPanel::updateDisplay()
{
    volumeSlider.setValue(track->getVolume(), juce::dontSendNotification);
    panSlider.setValue(track->getPan(), juce::dontSendNotification);
    heightSlider.setValue(track->getHeightMultiplier(), juce::dontSendNotification);
    muteButton.setToggleState(track->isMuted(), juce::dontSendNotification);
    soloButton.setToggleState(track->isSolo(), juce::dontSendNotification);
}
