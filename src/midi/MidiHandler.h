#pragma once
#include <JuceHeader.h>

class MidiHandler : public juce::MidiInputCallback,
                    public juce::ChangeBroadcaster
{
public:
    MidiHandler();
    ~MidiHandler() override;

    void openDefaultMidiInput();
    void closeMidiInput();
    void sendMidiMessage(const juce::MidiMessage& message);
    
    // MidiInputCallback
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;

    const juce::MidiMessageSequence& getMidiBuffer() const { return midiBuffer; }
    void clearMidiBuffer();

private:
    std::unique_ptr<juce::MidiInput> midiInput;
    juce::MidiMessageSequence midiBuffer;
    juce::CriticalSection midiLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiHandler)
};
