#include "Mixer.h"

Mixer::Mixer()
{
}

Mixer::~Mixer()
{
    channels.clear();
}

ChannelStrip* Mixer::addChannel(Track* track)
{
    auto* newChannel = new ChannelStrip(track);
    channels.add(newChannel);
    sendChangeMessage();
    return newChannel;
}

void Mixer::removeChannel(int index)
{
    if (index >= 0 && index < channels.size())
    {
        channels.remove(index);
        sendChangeMessage();
    }
}

void Mixer::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    for (auto* channel : channels)
        channel->prepareToPlay(sampleRate, samplesPerBlock);
    
    masterBus.prepareToPlay(sampleRate, samplesPerBlock);
}

void Mixer::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Clear the output buffer
    buffer.clear();
    
    // Process each channel
    juce::AudioBuffer<float> channelBuffer(buffer.getNumChannels(), buffer.getNumSamples());
    
    for (auto* channel : channels)
    {
        if (channel->getMute())
            continue;
            
        channelBuffer.clear();
        channel->processBlock(channelBuffer, midiMessages);
        
        // Mix into main buffer
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            buffer.addFrom(ch, 0, channelBuffer, ch, 0, buffer.getNumSamples());
        }
    }
    
    // Process master bus
    masterBus.processBlock(buffer, midiMessages);
}

void Mixer::setSolo(int channelIndex, bool solo)
{
    if (channelIndex >= 0 && channelIndex < channels.size())
    {
        channels[channelIndex]->setSolo(solo);
        updateSoloMuteStates();
    }
}

void Mixer::setMute(int channelIndex, bool mute)
{
    if (channelIndex >= 0 && channelIndex < channels.size())
    {
        channels[channelIndex]->setMute(mute);
        updateSoloMuteStates();
    }
}

void Mixer::updateSoloMuteStates()
{
    // Check if any channels are soloed
    bool anySoloed = false;
    for (auto* channel : channels)
    {
        if (channel->getSolo())
        {
            anySoloed = true;
            break;
        }
    }
    
    // Update mute states based on solo
    for (auto* channel : channels)
    {
        if (anySoloed)
        {
            channel->setEffectiveMute(!channel->getSolo());
        }
        else
        {
            channel->setEffectiveMute(channel->getMute());
        }
    }
}
