#pragma once
#include "Effect.h"

class ReverbEffect : public Effect
{
public:
    ReverbEffect();
    ~ReverbEffect() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void reset() override;

    // Reverb parameters
    void setRoomSize(float size);
    void setDamping(float damping);
    void setWetLevel(float wet);
    void setDryLevel(float dry);
    void setWidth(float width);

    float getRoomSize() const { return roomSize; }
    float getDamping() const { return damping; }
    float getWetLevel() const { return wetLevel; }
    float getDryLevel() const { return dryLevel; }
    float getWidth() const { return width; }

private:
    juce::Reverb reverb;
    juce::Reverb::Parameters parameters;
    
    float roomSize = 0.5f;
    float damping = 0.5f;
    float wetLevel = 0.33f;
    float dryLevel = 0.4f;
    float width = 1.0f;
};
