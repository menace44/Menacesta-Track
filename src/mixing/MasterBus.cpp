#include "MasterBus.h"

MasterBus::MasterBus()
{
    // Initialize metering
    for (int i = 0; i < 2; ++i)
    {
        peakLevels[i].reset(10.0);
        rmsLevels[i].reset(10.0);
    }
}

MasterBus::~MasterBus()
{
}

void MasterBus::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Initialize metering
    for (int i = 0; i < 2; ++i)
    {
        peakLevels[i].reset(sampleRate, 0.01);
        rmsLevels[i].reset(sampleRate, 0.1);
    }
}

void MasterBus::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Apply master volume and pan
    if (masterVolume != 1.0f)
    {
        buffer.applyGain(masterVolume);
    }
    
    if (masterPan != 0.0f)
    {
        float leftGain = 1.0f - (masterPan > 0.0f ? masterPan : 0.0f);
        float rightGain = 1.0f - (masterPan < 0.0f ? -masterPan : 0.0f);
        
        if (buffer.getNumChannels() >= 2)
        {
            buffer.applyGain(0, 0, buffer.getNumSamples(), leftGain);
            buffer.applyGain(1, 0, buffer.getNumSamples(), rightGain);
        }
    }
    
    // Update metering
    for (int ch = 0; ch < buffer.getNumChannels() && ch < 2; ++ch)
    {
        auto channelData = buffer.getReadPointer(ch);
        float peak = 0.0f;
        float rms = 0.0f;
        
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float sample = std::abs(channelData[i]);
            peak = std::max(peak, sample);
            rms += sample * sample;
        }
        
        rms = std::sqrt(rms / buffer.getNumSamples());
        
        peakLevels[ch] = peak;
        rmsLevels[ch] = rms;
    }
}

float MasterBus::getPeakLevel(int channel) const
{
    if (channel >= 0 && channel < 2)
        return peakLevels[channel].getCurrentValue();
    return 0.0f;
}

float MasterBus::getRmsLevel(int channel) const
{
    if (channel >= 0 && channel < 2)
        return rmsLevels[channel].getCurrentValue();
    return 0.0f;
}
