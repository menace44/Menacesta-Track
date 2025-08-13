#include "MidiHandler.h"

MidiHandler::MidiHandler()
{
    openDefaultMidiInput();
}

MidiHandler::~MidiHandler()
{
    closeMidiInput();
}

void MidiHandler::openDefaultMidiInput()
{
    auto midiInputs = juce::MidiInput::getAvailableDevices();
    
    if (!midiInputs.isEmpty())
    {
        midiInput = juce::MidiInput::openDevice(midiInputs[0].identifier, this);
        if (midiInput)
        {
            midiInput->start();
        }
    }
}

void MidiHandler::closeMidiInput()
{
    if (midiInput)
    {
        midiInput->stop();
        midiInput.reset();
    }
}

void MidiHandler::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    juce::ScopedLock lock(midiLock);
    
    if (message.isNoteOnOrOff() || message.isController())
    {
        midiBuffer.addEvent(message, 0);
        sendChangeMessage();
    }
}

void MidiHandler::sendMidiMessage(const juce::MidiMessage& message)
{
    // This would connect to MIDI output in a real implementation
    DBG("Sending MIDI: " + message.getDescription());
}

void MidiHandler::clearMidiBuffer()
{
    juce::ScopedLock lock(midiLock);
    midiBuffer.clear();
}
