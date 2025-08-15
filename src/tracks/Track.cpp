#include "Track.h"

Track::Track(const juce::String& nameToUse, Type typeToUse)
    : name(nameToUse), type(typeToUse)
{
    // Set default color based on track type
    if (type == Type::Audio) {
        colour = juce::Colour(0xFF2196F3); // Blue for audio tracks
    } else {
        colour = juce::Colour(0xFF9C27B0); // Purple for MIDI tracks
    }
}

Track::~Track()
{
}

void Track::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Initialize any track-specific processing
}

void Track::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (muted) {
        buffer.clear();
        return;
    }
    
    // For now, just generate a simple test tone
    static float phase = 0.0f;
    const float frequency = 440.0f;
    const float sampleRate = 44100.0f;
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = volume * 0.1f * std::sin(phase);
            phase += 2.0f * juce::MathConstants<float>::pi * frequency / sampleRate;
            
            if (phase >= 2.0f * juce::MathConstants<float>::pi)
                phase -= 2.0f * juce::MathConstants<float>::pi;
        }
    }
}

void Track::addClip(std::unique_ptr<Track::Clip> clip)
{
    clips.push_back(std::move(clip));
    sendChangeMessage();
}

void Track::removeClip(Track::Clip* clipToRemove)
{
    auto it = std::find_if(clips.begin(), clips.end(),
        [clipToRemove](const std::unique_ptr<Track::Clip>& clip) {
            return clip.get() == clipToRemove;
        });
    
    if (it != clips.end()) {
        clips.erase(it);
        sendChangeMessage();
    }
}

void Track::setHeight(int newHeight)
{
    newHeight = juce::jlimit(minHeight, maxHeight, newHeight);
    if (height != newHeight)
    {
        height = newHeight;
        sendChangeMessage();
    }
}

void Track::setHeightMultiplier(float multiplier)
{
    multiplier = juce::jlimit(minHeightMultiplier, maxHeightMultiplier, multiplier);
    if (heightMultiplier != multiplier)
    {
        heightMultiplier = multiplier;
        sendChangeMessage();
    }
}

int Track::getCalculatedHeight() const
{
    return static_cast<int>(height * heightMultiplier);
}
