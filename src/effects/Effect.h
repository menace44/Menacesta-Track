#pragma once
#include <JuceHeader.h>

class Effect : public juce::ChangeBroadcaster
{
public:
    enum class Type
    {
        Reverb,
        Delay,
        EQ,
        Compressor,
        None
    };

    Effect(const juce::String& name, Type type);
    virtual ~Effect() = default;

    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) = 0;
    virtual void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) = 0;
    virtual void reset() = 0;

    virtual juce::String getName() const { return name; }
    virtual Type getType() const { return type; }

    virtual bool isEnabled() const { return enabled; }
    virtual void setEnabled(bool shouldEnable) { enabled = shouldEnable; sendChangeMessage(); }

    virtual float getWetDryMix() const { return wetDryMix; }
    virtual void setWetDryMix(float mix) { wetDryMix = juce::jlimit(0.0f, 1.0f, mix); }

protected:
    juce::String name;
    Type type;
    bool enabled = true;
    float wetDryMix = 1.0f; // 0 = dry, 1 = wet
};

class EffectChain : public juce::ChangeBroadcaster
{
public:
    EffectChain();
    ~EffectChain() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void reset();

    void addEffect(std::unique_ptr<Effect> effect);
    void removeEffect(int index);
    void moveEffect(int fromIndex, int toIndex);
    
    Effect* getEffect(int index) const;
    int getNumEffects() const { return effects.size(); }

    void setEnabled(bool shouldEnable);
    bool isEnabled() const { return enabled; }

private:
    std::vector<std::unique_ptr<Effect>> effects;
    bool enabled = true;
};
