#include <JuceHeader.h>
#include "../audio/AudioEngine.h"
#include "../transport/Transport.h"

class AudioEngineDemo : public juce::JUCEApplication
{
public:
    AudioEngineDemo() {}
    
    const juce::String getApplicationName() override { return "Audio Engine Demo"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    
    void initialise(const juce::String& commandLine) override
    {
        DBG("=== Audio Engine Demo ===");
        
        // Create audio engine
        AudioEngine engine;
        
        // Create transport
        Transport transport;
        engine.setTransport(&transport);
        
        // Add some tracks
        DBG("Adding tracks...");
        auto* track1 = engine.addTrack("Drums", Track::AudioTrack);
        auto* track2 = engine.addTrack("Bass", Track::AudioTrack);
        auto* track3 = engine.addTrack("Lead", Track::MidiTrack);
        
        // Configure tracks
        track1->setVolume(0.8f);
        track2->setVolume(0.6f);
        track3->setVolume(0.7f);
        
        track1->setPan(-0.3f);  // Slightly left
        track2->setPan(0.3f);   // Slightly right
        track3->setPan(0.0f);    // Center
        
        // Test master volume
        engine.setMasterVolume(0.9f);
        
        // Print track information
        DBG("Tracks created:");
        for (int i = 0; i < engine.getNumTracks(); ++i)
        {
            auto* track = engine.getTrack(i);
            DBG("  " + juce::String(i + 1) + ": " + track->getName() + 
                " (Volume: " + juce::String(track->getVolume()) + 
                ", Pan: " + juce::String(track->getPan()) + ")");
        }
        
        // Test recording functionality
        juce::File testFile = juce::File::getSpecialLocation(juce::File::tempDirectory)
                             .getChildFile("test_recording.wav");
        
        track1->startRecording(testFile);
        DBG("Recording started on track: " + track1->getName());
        
        // Simulate some processing
        juce::AudioBuffer<float> testBuffer(2, 512);
        juce::MidiBuffer midiBuffer;
        
        // Process a few blocks
        for (int i = 0; i < 5; ++i)
        {
            testBuffer.clear();
            engine.processTracks(testBuffer, midiBuffer);
            DBG("Processed block " + juce::String(i + 1));
        }
        
        // Stop recording
        track1->stopRecording();
        DBG("Recording stopped");
        
        // Clean up
        testFile.deleteFile();
        
        DBG("Demo completed successfully!");
        quit();
    }
    
    void shutdown() override {}
};

START_JUCE_APPLICATION(AudioEngineDemo)
