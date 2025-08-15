#pragma once
#include <JuceHeader.h>
#include "../tracks/Track.h"

class ChannelStrip : public juce::ChangeBroadcaster
{
public:
    ChannelStrip(Track* track);
    ~ChannelStrip() override;

    // Processing
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    
    // Controls
    void setVolume(float volume) { currentVolume = volume; sendChangeMessage(); }
    void setPan(float pan) { currentPan = pan; sendChangeMessage(); }
    void setMute(bool mute) { isMuted = mute; sendChangeMessage(); }
    void setSolo(bool solo) { isSoloed = solo; sendChangeMessage(); }
    void setEffectiveMute(bool mute) { effectiveMute = mute; }
    
    // Accessors
    float getVolume() const { return currentVolume; }
    float getPan() const { return currentPan; }
    bool getMute() const { return isMuted; }
    bool getSolo() const { return isSoloed; }
    
    // Track access
    Track* getTrack() const { return track; }

private:
    Track* track;
    float currentVolume = 1.0f;
    float currentPan = 0.0f;
    bool isMuted = false;
    bool isSoloed = false;
    bool effectiveMute = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelStrip)
};
