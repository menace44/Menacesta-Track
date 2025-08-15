#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>

// Simple standalone mixing console implementation
class SimpleTrack
{
public:
    SimpleTrack(const std::string& name) : name_(name) {}
    
    const std::string& getName() const { return name_; }
    
    // Simple audio buffer simulation
    void process(float* buffer, int numSamples)
    {
        // Generate a simple sine wave for demonstration
        for (int i = 0; i < numSamples; ++i)
        {
            buffer[i] = 0.3f * sin(2.0f * 3.14159f * 440.0f * i / 44100.0f);
        }
    }
    
private:
    std::string name_;
};

class SimpleChannel
{
public:
    SimpleChannel(SimpleTrack* track) : track_(track), volume_(1.0f), pan_(0.0f) {}
    
    void setVolume(float volume) { volume_ = std::clamp(volume, 0.0f, 2.0f); }
    void setPan(float pan) { pan_ = std::clamp(pan, -1.0f, 1.0f); }
    
    float getVolume() const { return volume_; }
    float getPan() const { return pan_; }
    
    SimpleTrack* getTrack() const { return track_; }
    
    void process(float* buffer, int numSamples)
    {
        if (!track_) return;
        
        // Temporary buffer for track processing
        std::vector<float> trackBuffer(numSamples);
        track_->process(trackBuffer.data(), numSamples);
        
        // Apply volume and pan
        float leftGain = volume_ * (1.0f - std::max(0.0f, pan_));
        float rightGain = volume_ * (1.0f - std::max(0.0f, -pan_));
        
        for (int i = 0; i < numSamples; ++i)
        {
            buffer[i * 2] += trackBuffer[i] * leftGain;      // Left channel
            buffer[i * 2 + 1] += trackBuffer[i] * rightGain; // Right channel
        }
    }
    
private:
    SimpleTrack* track_;
    float volume_;
    float pan_;
};

class SimpleMixer
{
public:
    SimpleMixer() : masterVolume_(1.0f) {}
    
    SimpleChannel* addChannel(SimpleTrack* track)
    {
        channels_.emplace_back(std::make_unique<SimpleChannel>(track));
        return channels_.back().get();
    }
    
    void setMasterVolume(float volume) { masterVolume_ = std::clamp(volume, 0.0f, 2.0f); }
    float getMasterVolume() const { return masterVolume_; }
    
    int getNumChannels() const { return static_cast<int>(channels_.size()); }
    SimpleChannel* getChannel(int index) { return channels_[index].get(); }
    
    void process(float* buffer, int numSamples)
    {
        // Clear output buffer
        for (int i = 0; i < numSamples * 2; ++i)
        {
            buffer[i] = 0.0f;
        }
        
        // Process all channels
        for (auto& channel : channels_)
        {
            channel->process(buffer, numSamples);
        }
        
        // Apply master volume
        for (int i = 0; i < numSamples * 2; ++i)
        {
            buffer[i] *= masterVolume_;
        }
    }
    
private:
    std::vector<std::unique_ptr<SimpleChannel>> channels_;
    float masterVolume_;
};

int main()
{
    std::cout << "Menacesta Track - Simple Standalone Mixing Console\n";
    std::cout << "=================================================\n\n";
    
    // Create a mixer
    SimpleMixer mixer;
    
    // Create some tracks
    auto vocalTrack = std::make_unique<SimpleTrack>("Lead Vocal");
    auto guitarTrack = std::make_unique<SimpleTrack>("Guitar");
    auto drumTrack = std::make_unique<SimpleTrack>("Drums");
    
    // Add tracks to mixer
    auto* vocalChannel = mixer.addChannel(vocalTrack.get());
    auto* guitarChannel = mixer.addChannel(guitarTrack.get());
    auto* drumChannel = mixer.addChannel(drumTrack.get());
    
    // Configure channels
    vocalChannel->setVolume(0.8f);
    vocalChannel->setPan(-0.2f);
    
    guitarChannel->setVolume(0.6f);
    guitarChannel->setPan(0.3f);
    
    drumChannel->setVolume(0.9f);
    drumChannel->setPan(0.0f);
    
    // Set master volume
    mixer.setMasterVolume(0.85f);
    
    std::cout << "Simple Mixer initialized with " << mixer.getNumChannels() << " channels:\n";
    for (int i = 0; i < mixer.getNumChannels(); ++i)
    {
        auto* channel = mixer.getChannel(i);
        std::cout << "  Channel " << i + 1 << ": " << channel->getTrack()->getName()
                  << " (Volume: " << channel->getVolume()
                  << ", Pan: " << channel->getPan() << ")\n";
    }
    std::cout << "  Master Volume: " << mixer.getMasterVolume() << "\n";
    
    // Simulate processing a small buffer
    const int bufferSize = 512;
    float outputBuffer[bufferSize * 2]; // Stereo
    
    mixer.process(outputBuffer, bufferSize);
    
    std::cout << "\nAudio processing complete!\n";
    std::cout << "Sample output (first 4 samples):\n";
    for (int i = 0; i < 4; ++i)
    {
        std::cout << "  Sample " << i << ": L=" << outputBuffer[i * 2] 
                  << ", R=" << outputBuffer[i * 2 + 1] << "\n";
    }
    
    std::cout << "\nMixing console is ready!\n";
    std::cout << "Press Enter to exit...\n";
    std::cin.get();
    
    return 0;
}
