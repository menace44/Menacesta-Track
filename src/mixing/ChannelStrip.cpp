#include "ChannelStrip.h"

ChannelStrip::ChannelStrip(Track* trackToUse)
    : track(trackToUse)
{
}

ChannelStrip::~ChannelStrip()
{
}

void ChannelStrip::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Prepare any processing components here
    // For now, just pass through to track
    if (track)
        track->prepareToPlay(sampleRate, samplesPerBlock);
}

void ChannelStrip::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (effectiveMute || !track)
    {
        buffer.clear();
        return;
    }
    
    // Get audio from track
    track->processBlock(buffer, midiMessages);
    
    // Apply volume and pan
    if (currentVolume != 1.0f)
    {
        buffer.applyGain(currentVolume);
    }
    
    if (currentPan != 0.0f)
    {
        // Simple pan law
        float leftGain = 1.0f - (currentPan > 0.0f ? currentPan : 0.0f);
        float rightGain = 1.0f - (currentPan < 0.0f ? -currentPan : 0.0f);
        
        if (buffer.getNumChannels() >= 2)
        {
            buffer.applyGain(0, 0, buffer.getNumSamples(), leftGain);
            buffer.applyGain(1, 0, buffer.getNumSamples(), rightGain);
        }
    }
}
