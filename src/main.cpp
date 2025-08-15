#include <iostream>
#include <memory>
#include "mixing/Mixer.h"
#include "tracks/Track.h"

int main()
{
    std::cout << "Menacesta Track - Mixing Console Demo\n";
    std::cout << "=====================================\n\n";
    
    // Create a mixer
    auto mixer = std::make_unique<Mixer>();
    
    // Create some tracks
    auto track1 = std::make_unique<Track>("Lead Vocal");
    auto track2 = std::make_unique<Track>("Guitar");
    auto track3 = std::make_unique<Track>("Drums");
    
    // Add tracks to mixer
    auto* channel1 = mixer->addChannel(track1.get());
    auto* channel2 = mixer->addChannel(track2.get());
    auto* channel3 = mixer->addChannel(track3.get());
    
    // Configure channels
    channel1->setVolume(0.8f);
    channel1->setPan(-0.2f);
    
    channel2->setVolume(0.6f);
    channel2->setPan(0.3f);
    
    channel3->setVolume(0.9f);
    channel3->setPan(0.0f);
    
    // Prepare for playback
    mixer->prepareToPlay(44100.0, 512);
    
    std::cout << "Mixer initialized with " << mixer->getNumChannels() << " channels:\n";
    for (int i = 0; i < mixer->getNumChannels(); ++i)
    {
        auto* channel = mixer->getChannel(i);
        std::cout << "  Channel " << i + 1 << ": " << channel->getTrack()->getName()
                  << " (Volume: " << channel->getVolume()
                  << ", Pan: " << channel->getPan() << ")\n";
    }
    
    std::cout << "\nMixing console is ready!\n";
    std::cout << "Press Enter to exit...\n";
    std::cin.get();
    
    return 0;
}
