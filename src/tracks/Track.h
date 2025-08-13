#pragma once
#include <JuceHeader.h>
#include "../recording/Recorder.h"
#include "../plugins/PluginManager.h"

class Track : public juce::ChangeBroadcaster
{
public:
    enum TrackType
    {
        AudioTrack,
        MidiTrack
    };

    Track(const juce::String& name, TrackType type);
    ~Track() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    
    void setVolume(float newVolume);
    void setPan(float newPan);
    void setMute(bool shouldMute);
    void setSolo(bool shouldSolo);

    void startRecording(const juce::File& file);
    void stopRecording();
    bool isRecording() const;

    void loadPlugin(const juce::String& pluginIdentifier);
    void unloadPlugin();

    const juce::String& getName() const { return name; }
    TrackType getType() const { return type; }
    float getVolume() const { return volume; }
    float getPan() const { return pan; }
    bool isMuted() const { return muted; }
    bool isSoloed() const { return soloed; }

private:
    juce::String name;
    TrackType type;
    
    float volume = 1.0f;
    float pan = 0.0f;
    bool muted = false;
    bool soloed = false;
    
    Recorder recorder;
    juce::AudioBuffer<float> trackBuffer;
    juce::AudioPluginInstance* plugin = nullptr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Track)
};
