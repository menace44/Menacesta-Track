#pragma once
#include <JuceHeader.h>

class MasterBus : public juce::ChangeBroadcaster
{
public:
    MasterBus();
    ~MasterBus() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    
    // Master controls
    void setVolume(float volume) { masterVolume = volume; sendChangeMessage(); }
    void setPan(float pan) { masterPan = pan; sendChangeMessage(); }
    float getVolume() const { return masterVolume; }
    float getPan() const { return masterPan; }
    
    // Metering
    float getPeakLevel(int channel) const;
    float getRmsLevel(int channel) const;

private:
    float masterVolume = 1.0f;
    float masterPan = 0.0f;
    
    // Metering
    juce::LinearSmoothedValue<float> peakLevels[2];
    juce::LinearSmoothedValue<float> rmsLevels[2];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterBus)
};
