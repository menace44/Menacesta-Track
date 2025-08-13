#pragma once
#include <JuceHeader.h>

class Recorder
{
public:
    Recorder();
    ~Recorder();

    void startRecording(const juce::File& file);
    void stopRecording();
    bool isRecording() const { return recording; }

    void addAudioBlock(const juce::AudioBuffer<float>& buffer, int numSamples);
    void setSampleRate(double rate) { sampleRate = rate; }

private:
    std::unique_ptr<juce::AudioFormatWriter::ThreadedWriter> threadedWriter;
    juce::TimeSliceThread writerThread;
    juce::File outputFile;
    bool recording = false;
    double sampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Recorder)
};
