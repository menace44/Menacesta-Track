#pragma once
#include <JuceHeader.h>
#include "../tracks/Track.h"
#include "ChannelStrip.h"
#include "MasterBus.h"

class Mixer : public juce::ChangeBroadcaster
{
public:
    Mixer();
    ~Mixer() override;

    // Channel management
    ChannelStrip* addChannel(Track* track);
    void removeChannel(int index);
    int getNumChannels() const { return channels.size(); }
    ChannelStrip* getChannel(int index) { return channels[index].get(); }
    
    // Master bus
    MasterBus& getMasterBus() { return masterBus; }

    // Processing
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    
    // Solo/mute management
    void setSolo(int channelIndex, bool solo);
    void setMute(int channelIndex, bool mute);
    void updateSoloMuteStates();

private:
    juce::OwnedArray<ChannelStrip> channels;
    MasterBus masterBus;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mixer)
};
