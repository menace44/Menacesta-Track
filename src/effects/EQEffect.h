#pragma once
#include "Effect.h"

class EQEffect : public Effect
{
public:
    struct EQBand
    {
        float frequency = 1000.0f;
        float gain = 0.0f;
        float q = 1.0f;
        bool enabled = true;
    };

    EQEffect();
    ~EQEffect() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void reset() override;

    // EQ parameters
    void setBandGain(int bandIndex, float gain);
    void setBandFrequency(int bandIndex, float frequency);
    void setBandQ(int bandIndex, float q);
    void setBandEnabled(int bandIndex, bool enabled);

    float getBandGain(int bandIndex) const;
    float getBandFrequency(int bandIndex) const;
    float getBandQ(int bandIndex) const;
    bool isBandEnabled(int bandIndex) const;

private:
    static constexpr int NUM_BANDS = 3;
    EQBand bands[NUM_BANDS];
    
    juce::dsp::IIR::Filter<float> lowBand;
    juce::dsp::IIR::Filter<float> midBand;
    juce::dsp::IIR::Filter<float> highBand;
    
    double currentSampleRate = 44100.0;
    
    void updateFilter(int bandIndex);
};
