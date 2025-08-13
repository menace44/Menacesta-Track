#pragma once
#include "Effect.h"

class DelayEffect : public Effect
{
public:
    DelayEffect();
    ~DelayEffect() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void reset() override;

    // Delay parameters
    void setDelayTime(float delayTimeMs);
    void setFeedback(float feedback);
    void setMix(float mix);
    void setWetLevel(float wetLevel);
    void setDryLevel(float dryLevel);

    float getDelayTime() const { return delayTimeMs; }
    float getFeedback() const { return feedback; }
    float getMix() const { return mix; }
    float getWetLevel() const { return wetLevel; }
    float getDryLevel() const { return dryLevel; }

private:
    float delayTimeMs = 500.0f;
    float feedback = 0.3f;
    float mix = 0.5f;
    float wetLevel = 0.5f;
    float dryLevel = 1.0f;
    
    double currentSampleRate = 44100.0;
    int delaySamples = 22050;
    
    juce::AudioBuffer<float> delayBuffer;
    int delayBufferLength = 0;
    int delayReadIndex = 0;
    int delayWriteIndex = 0;
    
    void updateDelayParameters();
};
