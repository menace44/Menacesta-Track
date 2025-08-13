#include "ReverbEffect.h"

ReverbEffect::ReverbEffect()
    : Effect("Reverb", Type::Reverb)
{
    parameters.roomSize = roomSize;
    parameters.damping = damping;
    parameters.wetLevel = wetLevel;
    parameters.dryLevel = dryLevel;
    parameters.width = width;
}

void ReverbEffect::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    reverb.setParameters(parameters);
}

void ReverbEffect::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (!enabled) return;
    
    reverb.setParameters(parameters);
    reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
}

void ReverbEffect::reset()
{
    reverb.reset();
}

void ReverbEffect::setRoomSize(float size)
{
    roomSize = juce::jlimit(0.0f, 1.0f, size);
    parameters.roomSize = roomSize;
}

void ReverbEffect::setDamping(float dampingValue)
{
    damping = juce::jlimit(0.0f, 1.0f, dampingValue);
    parameters.damping = damping;
}

void ReverbEffect::setWetLevel(float wet)
{
    wetLevel = juce::jlimit(0.0f, 1.0f, wet);
    parameters.wetLevel = wetLevel;
}

void ReverbEffect::setDryLevel(float dry)
{
    dryLevel = juce::jlimit(0.0f, 1.0f, dry);
    parameters.dryLevel = dryLevel;
}

void ReverbEffect::setWidth(float widthValue)
{
    width = juce::jlimit(0.0f, 1.0f, widthValue);
    parameters.width = width;
}
