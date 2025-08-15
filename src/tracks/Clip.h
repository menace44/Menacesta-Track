#pragma once
#include <JuceHeader.h>

namespace Track {
    enum class ClipType {
        Audio,
        MIDI
    };

    class Clip {
    public:
        Clip(const juce::String& name, ClipType type, double startTime, double duration);
        ~Clip() = default;

        // Basic properties
        const juce::String& getName() const { return name; }
        void setName(const juce::String& newName) { name = newName; }
        
        ClipType getType() const { return type; }
        
        double getStartTime() const { return startTime; }
        void setStartTime(double newStartTime) { startTime = newStartTime; }
        
        double getDuration() const { return duration; }
        void setDuration(double newDuration) { duration = newDuration; }
        
        double getEndTime() const { return startTime + duration; }
        
        // Color and visual properties
        juce::Colour getColour() const { return colour; }
        void setColour(const juce::Colour& newColour) { colour = newColour; }
        
        // Audio-specific properties
        void setAudioFile(const juce::File& file) { audioFile = file; }
        const juce::File& getAudioFile() const { return audioFile; }
        
        // MIDI-specific properties
        void setMidiSequence(const juce::MidiMessageSequence& sequence) { midiSequence = sequence; }
        const juce::MidiMessageSequence& getMidiSequence() const { return midiSequence; }
        
        // Waveform data
        void setWaveformData(const juce::Array<float>& data) { waveformData = data; }
        const juce::Array<float>& getWaveformData() const { return waveformData; }
        
        // Selection and editing
        bool isSelected() const { return selected; }
        void setSelected(bool isSelected) { selected = isSelected; }
        
        bool isMuted() const { return muted; }
        void setMuted(bool isMuted) { muted = isMuted; }
        
        float getGain() const { return gain; }
        void setGain(float newGain) { gain = newGain; }
        
        float getPan() const { return pan; }
        void setPan(float newPan) { pan = newPan; }

        // Fade handles
        double getFadeIn() const { return fadeIn; }
        void setFadeIn(double newFadeIn) { fadeIn = juce::jlimit(0.0, duration * 0.5, newFadeIn); }
        
        double getFadeOut() const { return fadeOut; }
        void setFadeOut(double newFadeOut) { fadeOut = juce::jlimit(0.0, duration * 0.5, newFadeOut); }
        
        // Loop region
        bool isLoopRegion() const { return loopRegion; }
        void setLoopRegion(bool isLoop) { loopRegion = isLoop; }
        
        double getLoopStart() const { return loopStart; }
        void setLoopStart(double start) { loopStart = juce::jlimit(0.0, duration, start); }
        
        double getLoopEnd() const { return loopEnd; }
        void setLoopEnd(double end) { loopEnd = juce::jlimit(0.0, duration, end); }
        
        // Snap-to-grid
        static double snapToGrid(double time, double gridSize) {
            return std::round(time / gridSize) * gridSize;
        }
        
        // Copy/Paste support
        juce::String getSourceFile() const { return audioFile.getFullPathName(); }
        void setSourceFile(const juce::String& file) { audioFile = juce::File(file); }
        
        juce::String getMidiData() const { 
            juce::MemoryBlock midiData;
            midiSequence.writeTo(midiData);
            return midiData.toBase64Encoding();
        }
        
        void setMidiData(const juce::String& data) {
            juce::MemoryBlock midiData;
            midiData.fromBase64Encoding(data);
            midiSequence.readFrom(midiData);
        }

    private:
        juce::String name;
        ClipType type;
        double startTime;
        double duration;
        juce::Colour colour;
        
        juce::File audioFile;
        juce::MidiMessageSequence midiSequence;
        juce::Array<float> waveformData;
        
        bool selected = false;
        bool muted = false;
        float gain = 1.0f;
        float pan = 0.0f;
        
        // New properties
        double fadeIn = 0.0;
        double fadeOut = 0.0;
        bool loopRegion = false;
        double loopStart = 0.0;
        double loopEnd = 0.0;
    };
}
