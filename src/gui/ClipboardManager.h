#pragma once
#include <JuceHeader.h>
#include "../tracks/Clip.h"
#include <vector>

struct ClipboardClip {
    juce::String name;
    Track::ClipType type;
    double duration;
    double startTime;
    juce::Colour colour;
    juce::String sourceFile;
    juce::String midiData;
};

class ClipboardManager {
public:
    static ClipboardManager& getInstance() {
        static ClipboardManager instance;
        return instance;
    }
    
    void copyClip(const Track::Clip* clip) {
        ClipboardClip clipboardClip;
        clipboardClip.name = clip->getName();
        clipboardClip.type = clip->getType();
        clipboardClip.duration = clip->getDuration();
        clipboardClip.startTime = clip->getStartTime();
        clipboardClip.colour = clip->getColour();
        clipboardClip.sourceFile = clip->getSourceFile();
        clipboardClip.midiData = clip->getMidiData();
        
        clipboard.push_back(clipboardClip);
    }
    
    void copyClips(const std::vector<Track::Clip*>& clips) {
        clipboard.clear();
        for (const auto* clip : clips) {
            copyClip(clip);
        }
    }
    
    std::vector<ClipboardClip> getClipboardContents() const {
        return clipboard;
    }
    
    bool hasContents() const {
        return !clipboard.empty();
    }
    
    void clear() {
        clipboard.clear();
    }
    
    std::vector<Track::Clip*> pasteClips(Track* track, double pasteTime) {
        std::vector<Track::Clip*> newClips;
        
        if (clipboard.empty()) return newClips;
        
        double firstStartTime = clipboard[0].startTime;
        
        for (const auto& clipboardClip : clipboard) {
            auto* newClip = new Track::Clip(
                clipboardClip.name,
                clipboardClip.type,
                pasteTime + (clipboardClip.startTime - firstStartTime),
                clipboardClip.duration
            );
            
            newClip->setColour(clipboardClip.colour);
            newClip->setSourceFile(clipboardClip.sourceFile);
            newClip->setMidiData(clipboardClip.midiData);
            
            track->addClip(newClip);
            newClips.push_back(newClip);
        }
        
        return newClips;
    }
    
private:
    ClipboardManager() = default;
    ~ClipboardManager() = default;
    ClipboardManager(const ClipboardManager&) = delete;
    ClipboardManager& operator=(const ClipboardManager&) = delete;
    
    std::vector<ClipboardClip> clipboard;
};
