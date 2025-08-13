#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    deviceManager.initialiseWithDefaultDevices(2, 2);
    deviceManager.addAudioCallback(this);
    
    // Initialize MIDI handler
    midiHandler.prepareToPlay(deviceManager.getCurrentAudioDevice()->getCurrentSampleRate(), 
                             deviceManager.getCurrentAudioDevice()->getCurrentBufferSizeSamples());
}

AudioEngine::~AudioEngine()
{
    deviceManager.removeAudioCallback(this);
}

void AudioEngine::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    currentSampleRate = sampleRate;
    bufferSize = samplesPerBlockExpected;
    
    buffer.setSize(2, bufferSize);
    masterBuffer.setSize(2, bufferSize);
    
    // Prepare all tracks
    for (auto* track : tracks)
    {
        track->prepareToPlay(sampleRate, samplesPerBlockExpected);
    }
    
    // Prepare MIDI handler
    midiHandler.prepareToPlay(sampleRate, samplesPerBlockExpected);
}

void AudioEngine::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    // Create MIDI buffer for this block
    juce::MidiBuffer midiMessages;
    
    // Process MIDI input
    midiHandler.processNextMidiBlock(midiMessages, bufferToFill.numSamples);
    
    // Process all tracks
    processTracks(*bufferToFill.buffer, midiMessages);
    
    // Apply master volume
    bufferToFill.buffer->applyGain(masterVolume);
}

void AudioEngine::releaseResources()
{
    buffer.setSize(0, 0);
    masterBuffer.setSize(0, 0);
    
    for (auto* track : tracks)
    {
        track->prepareToPlay(0, 0);
    }
    
    midiHandler.releaseResources();
}

void AudioEngine::processTracks(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (tracks.isEmpty())
    {
        // No tracks - silence
        buffer.clear();
        return;
    }
    
    // Clear master buffer
    masterBuffer.clear();
    
    // Process each track and mix into master buffer
    for (auto* track : tracks)
    {
        juce::AudioBuffer<float> trackBuffer(buffer.getNumChannels(), buffer.getNumSamples());
        trackBuffer.clear();
        
        // Copy input buffer for processing
        trackBuffer.makeCopyOf(buffer);
        
        // Process track
        track->processBlock(trackBuffer, midiMessages);
        
        // Mix into master buffer
        for (int channel = 0; channel < masterBuffer.getNumChannels(); ++channel)
        {
            masterBuffer.addFrom(channel, 0, trackBuffer, channel, 0, buffer.getNumSamples());
        }
    }
    
    // Copy master buffer to output
    buffer.makeCopyOf(masterBuffer);
}

// Track management
Track* AudioEngine::addTrack(const juce::String& name, Track::TrackType type)
{
    auto* newTrack = new Track(name, type);
    tracks.add(newTrack);
    
    // Prepare the new track
    newTrack->prepareToPlay(currentSampleRate, bufferSize);
    
    return newTrack;
}

void AudioEngine::removeTrack(int index)
{
    if (index >= 0 && index < tracks.size())
    {
        tracks.remove(index);
    }
}

void AudioEngine::setMasterVolume(float volume)
{
    masterVolume = juce::jlimit(0.0f, 2.0f, volume);
}
