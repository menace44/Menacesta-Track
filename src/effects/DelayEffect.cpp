#include "DelayEffect.h"

DelayEffect::DelayEffect()
    : Effect("Delay", Type::DELAY)
{
    delayBufferLength = static_cast<int>(std::ceil(currentSampleRate * 2.0)); // 2 seconds max delay
    delayBuffer.setSize(2, delayBufferLength);
    delayBuffer.clear();
}

void DelayEffect::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    updateDelayParameters();
    
    delayBufferLength = static_cast<int>(std::ceil(sampleRate * 2.0));
    delayBuffer.setSize(2, delayBufferLength);
    delayBuffer.clear();
    
    delayReadIndex = 0;
    delayWriteIndex = 0;
}

void DelayEffect::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (!enabled) return;
    
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        float* delayData = delayBuffer.getWritePointer(channel);
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            const float inputSample = channelData[sample];
            
            // Read from delay buffer
            float delayedSample = delayData[delayReadIndex];
            
            // Calculate output
            float wet = delayedSample * wetLevel;
            float dry = inputSample * dryLevel;
            float output = dry + (wet * mix);
            
            // Write to delay buffer with feedback
            float feedbackSample = inputSample + (delayedSample * feedback);
            delayData[delayWriteIndex] = feedbackSample;
            
            channelData[sample] = output;
            
            // Update indices
            delayReadIndex = (delayReadIndex + 1) % delayBufferLength;
            delayWriteIndex = (delayWriteIndex + 1) % delayBufferLength;
        }
    }
}

void DelayEffect::reset()
{
    delayBuffer.clear();
    delayReadIndex = 0;
    delayWriteIndex = 0;
}

void DelayEffect::setDelayTime(float delayTimeMs)
{
    this->delayTimeMs = juce::jlimit(1.0f, 2000.0f, delayTimeMs);
    updateDelayParameters();
}

void DelayEffect::setFeedback(float feedback)
{
    this->feedback = juce::jlimit(0.0f, 0.99f, feedback);
}

void DelayEffect::setMix(float mix)
{
    this->mix = juce::jlimit(0.0f, 1.0f, mix);
}

void DelayEffect::setWetLevel(float wetLevel)
{
    this->wetLevel = juce::jlimit(0.0f, 1.0f, wetLevel);
}

void DelayEffect::setDryLevel(float dryLevel)
{
    this->dryLevel = juce::jlimit(0.0f, 1.0f, dryLevel);
}

void DelayEffect::updateDelayParameters()
{
    delaySamples = static_cast<int>(std::ceil((delayTimeMs / 1000.0) * currentSampleRate));
    
    if (delaySamples >= delayBufferLength)
        delaySamples = delayBufferLength - 1;
    
    delayReadIndex = (delayWriteIndex - delaySamples + delayBufferLength) % delayBufferLength;
}
