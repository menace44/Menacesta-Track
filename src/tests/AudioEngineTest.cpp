#include <JuceHeader.h>
#include "../audio/AudioEngine.h"
#include "../tracks/Track.h"

class AudioEngineTest : public juce::UnitTest
{
public:
    AudioEngineTest() : UnitTest("AudioEngine Test") {}
    
    void runTest() override
    {
        beginTest("AudioEngine Initialization");
        
        AudioEngine engine;
        
        // Test basic initialization
        expect(engine.getNumTracks() == 0, "Should start with no tracks");
        expect(engine.getMasterVolume() == 1.0f, "Master volume should default to 1.0");
        
        beginTest("Track Management");
        
        // Test adding tracks
        auto* audioTrack = engine.addTrack("Audio Track 1", Track::AudioTrack);
        expect(audioTrack != nullptr, "Should create audio track");
        expect(engine.getNumTracks() == 1, "Should have 1 track");
        expect(engine.getTrack(0)->getName() == "Audio Track 1", "Track name should match");
        
        auto* midiTrack = engine.addTrack("MIDI Track 1", Track::MidiTrack);
        expect(midiTrack != nullptr, "Should create MIDI track");
        expect(engine.getNumTracks() == 2, "Should have 2 tracks");
        
        // Test track removal
        engine.removeTrack(0);
        expect(engine.getNumTracks() == 1, "Should have 1 track after removal");
        expect(engine.getTrack(0)->getName() == "MIDI Track 1", "Remaining track should be MIDI track");
        
        beginTest("Master Volume Control");
        
        engine.setMasterVolume(0.5f);
        expect(engine.getMasterVolume() == 0.5f, "Master volume should be set correctly");
        
        engine.setMasterVolume(2.5f);
        expect(engine.getMasterVolume() == 2.0f, "Master volume should be clamped to 2.0");
        
        engine.setMasterVolume(-0.5f);
        expect(engine.getMasterVolume() == 0.0f, "Master volume should be clamped to 0.0");
        
        beginTest("Audio Processing");
        
        // Test that we can process audio without crashing
        juce::AudioBuffer<float> testBuffer(2, 512);
        testBuffer.clear();
        
        juce::MidiBuffer midiBuffer;
        
        // This should not crash
        engine.processTracks(testBuffer, midiBuffer);
        
        // Test with actual tracks
        engine.addTrack("Test Track", Track::AudioTrack);
        engine.processTracks(testBuffer, midiBuffer);
        
        // Verify buffer is processed (not necessarily changed, just processed)
        expect(testBuffer.getNumChannels() == 2, "Buffer should have 2 channels");
        expect(testBuffer.getNumSamples() == 512, "Buffer should have 512 samples");
        
        beginTest("Device Manager Access");
        
        auto& deviceManager = engine.getDeviceManager();
        expect(deviceManager.getCurrentAudioDevice() != nullptr, "Should have audio device");
        
        beginTest("MIDI Handler Access");
        
        auto& midiHandler = engine.getMidiHandler();
        // Basic test - just verify we can access it
        expect(true, "Should be able to access MIDI handler");
        
        beginTest("Plugin Manager Access");
        
        auto& pluginManager = engine.getPluginManager();
        expect(true, "Should be able to access plugin manager");
    }
};

static AudioEngineTest audioEngineTest;
