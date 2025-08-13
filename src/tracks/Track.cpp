#include "Track.h"

Track::Track(const juce::String& trackName, TrackType trackType)
    : name(trackName), type(trackType)
{
}

Track::~Track()
{
    unloadPlugin();
}

void Track::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    trackBuffer.setSize(2, samplesPerBlock);
    recorder.setSampleRate(sampleRate);
    
    if (plugin)
    {
        plugin->prepareToPlay(sampleRate, samplesPerBlock);
    }
}

void Track::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (muted)
    {
        buffer.clear();
        return;
    }

    trackBuffer.makeCopyOf(buffer);
    
    if (plugin)
    {
        plugin->processBlock(trackBuffer, midiMessages);
    }
    
    // Apply volume and pan
    trackBuffer.applyGain(volume);
    
    if (pan != 0.0f)
    {
        float leftGain = (pan <= 0.0f) ? 1.0f : 1.0f - pan;
        float rightGain = (pan >= 0.0f) ? 1.0f : 1.0f + pan;
        
        trackBuffer.applyGain(0, 0, leftGain);
        trackBuffer.applyGain(1, 0, rightGain);
    }
    
    buffer.makeCopyOf(trackBuffer);
    
    if (recorder.isRecording())
    {
        recorder.addAudioBlock(buffer, buffer.getNumSamples());
    }
}

void Track::setVolume(float newVolume)
{
    volume = juce::jlimit(0.0f, 2.0f, newVolume);
    sendChangeMessage();
}

void Track::setPan(float newPan)
{
    pan = juce::jlimit(-1.0f, 1.0f, newPan);
    sendChangeMessage();
}

void Track::setMute(bool shouldMute)
{
    muted = shouldMute;
    sendChangeMessage();
}

void Track::setSolo(bool shouldSolo)
{
    soloed = shouldSolo;
    sendChangeMessage();
}

void Track::startRecording(const juce::File& file)
{
    recorder.startRecording(file);
}

void Track::stopRecording()
{
    recorder.stopRecording();
}

bool Track::isRecording() const
{
    return recorder.isRecording();
}

void Track::loadPlugin(const juce::String& pluginIdentifier)
{
    // This would use the PluginManager in a real implementation
    DBG("Loading plugin: " + pluginIdentifier);
}

void Track::unloadPlugin()
{
    if (plugin)
    {
        plugin->releaseResources();
        plugin = nullptr;
    }
}
