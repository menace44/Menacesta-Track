#include "Effect.h"

Effect::Effect(const juce::String& effectName, Type effectType)
    : name(effectName), type(effectType)
{
}

EffectChain::EffectChain()
{
}

EffectChain::~EffectChain()
{
    reset();
}

void EffectChain::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    for (auto& effect : effects)
    {
        effect->prepareToPlay(sampleRate, samplesPerBlock);
    }
}

void EffectChain::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (!enabled) return;

    for (auto& effect : effects)
    {
        if (effect->isEnabled())
        {
            effect->processBlock(buffer, midiMessages);
        }
    }
}

void EffectChain::reset()
{
    for (auto& effect : effects)
    {
        effect->reset();
    }
}

void EffectChain::addEffect(std::unique_ptr<Effect> effect)
{
    effects.push_back(std::move(effect));
    sendChangeMessage();
}

void EffectChain::removeEffect(int index)
{
    if (index >= 0 && index < effects.size())
    {
        effects.erase(effects.begin() + index);
        sendChangeMessage();
    }
}

void EffectChain::moveEffect(int fromIndex, int toIndex)
{
    if (fromIndex >= 0 && fromIndex < effects.size() &&
        toIndex >= 0 && toIndex < effects.size())
    {
        auto effect = std::move(effects[fromIndex]);
        effects.erase(effects.begin() + fromIndex);
        effects.insert(effects.begin() + toIndex, std::move(effect));
        sendChangeMessage();
    }
}

Effect* EffectChain::getEffect(int index) const
{
    if (index >= 0 && index < effects.size())
    {
        return effects[index].get();
    }
    return nullptr;
}

void EffectChain::setEnabled(bool shouldEnable)
{
    enabled = shouldEnable;
    sendChangeMessage();
}
