#pragma once
#include <JuceHeader.h>
#include "../tracks/Track.h"
#include "../audio/AudioEngine.h"

class ProjectManager
{
public:
    ProjectManager();
    ~ProjectManager();

    // Project operations
    bool newProject();
    bool loadProject(const juce::File& file);
    bool saveProject(const juce::File& file);
    bool saveProject();
    
    // Project state
    bool isProjectModified() const { return projectModified; }
    void setProjectModified(bool modified = true) { projectModified = modified; }
    
    const juce::File& getCurrentProjectFile() const { return currentProjectFile; }
    juce::String getProjectName() const;
    
    // Track management
    void addTrack(Track::Type type);
    void removeTrack(int index);
    void moveTrack(int fromIndex, int toIndex);
    
    // Clip management
    void addClipToTrack(int trackIndex, std::unique_ptr<Track::Clip> clip);
    void removeClipFromTrack(int trackIndex, Track::Clip* clip);
    
    // Audio data management
    juce::File getAudioDataDirectory() const;
    juce::File getAudioFilePath(const juce::String& audioFileName) const;
    
private:
    // Serialization
    juce::var serializeProject() const;
    bool deserializeProject(const juce::var& projectData);
    
    juce::var serializeTrack(const Track& track) const;
    bool deserializeTrack(const juce::var& trackData, Track& track);
    
    juce::var serializeClip(const Track::Clip& clip) const;
    bool deserializeClip(const juce::var& clipData, std::unique_ptr<Track::Clip>& clip);
    
    // Audio file management
    juce::String importAudioFile(const juce::File& sourceFile);
    bool exportAudioFile(const juce::String& audioFileName, const juce::File& destination);
    
    // Project file
    juce::File currentProjectFile;
    bool projectModified = false;
    
    // Track storage
    juce::OwnedArray<Track> tracks;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectManager)
};
