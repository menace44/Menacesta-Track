#pragma once
#include <JuceHeader.h>
#include "Clip.h"
#include <vector>
#include <memory>

class Track : public juce::ChangeBroadcaster
{
public:
    enum class Type {
        Audio,
        MIDI
    };

    Track(const juce::String& name, Type type = Type::Audio);
    ~Track() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    
    const juce::String& getName() const { return name; }
    void setName(const juce::String& newName) { name = newName; sendChangeMessage(); }
    
    Type getType() const { return type; }
    void setType(Type newType) { type = newType; sendChangeMessage(); }
    
    // Clip management
    void addClip(std::unique_ptr<Track::Clip> clip);
    void removeClip(Track::Clip* clip);
    const std::vector<std::unique_ptr<Track::Clip>>& getClips() const { return clips; }
    
    // Track controls
    float getVolume() const { return volume; }
    void setVolume(float newVolume) { volume = newVolume; sendChangeMessage(); }
    
    float getPan() const { return pan; }
    void setPan(float newPan) { pan = newPan; sendChangeMessage(); }
    
    bool isMuted() const { return muted; }
    void setMuted(bool isMuted) { muted = isMuted; sendChangeMessage(); }
    
    bool isSolo() const { return solo; }
    void setSolo(bool isSolo) { solo = isSolo; sendChangeMessage(); }
    
    bool isArmed() const { return armed; }
    void setArmed(bool isArmed) { armed = isArmed; sendChangeMessage(); }
    
    // Visual properties
    juce::Colour getColour() const { return colour; }
    void setColour(const juce::Colour& newColour) { colour = newColour; sendChangeMessage(); }
    
    // Track height management
    int getHeight() const { return height; }
    void setHeight(int newHeight);
    
    // Height multiplier support
    float getHeightMultiplier() const { return heightMultiplier; }
    void setHeightMultiplier(float multiplier);
    int getCalculatedHeight() const;
    
    static constexpr int minHeight = 40;
    static constexpr int maxHeight = 400;
    static constexpr int defaultHeight = 60;
    static constexpr float minHeightMultiplier = 0.5f;
    static constexpr float maxHeightMultiplier = 3.0f;
    static constexpr float defaultHeightMultiplier = 1.0f;

private:
    juce::String name;
    Type type;
    std::vector<std::unique_ptr<Track::Clip>> clips;
    
    // Track controls
    float volume = 0.8f;
    float pan = 0.0f;
    bool muted = false;
    bool solo = false;
    bool armed = false;
    
    // Visual properties
    juce::Colour colour;
    int height = defaultHeight;
    float heightMultiplier = defaultHeightMultiplier;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Track)
};
