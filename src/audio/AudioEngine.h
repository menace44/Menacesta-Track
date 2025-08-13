#pragma once
#include <JuceHeader.h>
#include "Transport.h"
#include "../midi/MidiHandler.h"
#include "../tracks/Track.h"
#include "../plugins/PluginManager.h"

class AudioEngine : public juce::AudioIODeviceCallback
{
public:
    AudioEngine();
    ~AudioEngine() override;

    juce::AudioDeviceManager& getDeviceManager() { return deviceManager; }
    MidiHandler& getMidiHandler() { return midiHandler; }
    PluginManager& getPluginManager() { return pluginManager; }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void setTransport(Transport* t) { transport = t; }

    // Track management
    Track* addTrack(const juce::String& name, Track::TrackType type);
    void removeTrack(int index);
    int getNumTracks() const { return tracks.size(); }
    Track* getTrack(int index) { return tracks[index].get(); }
    
    // Master output
    void setMasterVolume(float volume);
    float getMasterVolume() const { return masterVolume; }

private:
    juce::AudioDeviceManager deviceManager;
    juce::AudioBuffer<float> buffer;
    juce::AudioBuffer<float> masterBuffer;
    Transport* transport = nullptr;
    MidiHandler midiHandler;
    PluginManager pluginManager;
    
    double currentSampleRate = 44100.0;
    int bufferSize = 512;
    float masterVolume = 1.0f;
    
    juce::OwnedArray<Track> tracks;
    
    void processTracks(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};
