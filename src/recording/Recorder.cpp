#include "Recorder.h"

Recorder::Recorder()
    : writerThread("Audio Writer Thread")
{
    writerThread.startThread();
}

Recorder::~Recorder()
{
    stopRecording();
}

void Recorder::startRecording(const juce::File& file)
{
    if (recording)
        stopRecording();

    outputFile = file;
    
    juce::WavAudioFormat wavFormat;
    std::unique_ptr<juce::AudioFormatWriter> writer(wavFormat.createWriterFor(
        new juce::FileOutputStream(outputFile), sampleRate, 2, 16, {}, 0));
    
    if (writer)
    {
        threadedWriter.reset(new juce::AudioFormatWriter::ThreadedWriter(writer.release(), writerThread, 32768));
        recording = true;
    }
}

void Recorder::stopRecording()
{
    if (recording)
    {
        threadedWriter.reset();
        recording = false;
    }
}

void Recorder::addAudioBlock(const juce::AudioBuffer<float>& buffer, int numSamples)
{
    if (recording && threadedWriter)
    {
        threadedWriter->write(buffer.getArrayOfReadPointers(), numSamples);
    }
}
