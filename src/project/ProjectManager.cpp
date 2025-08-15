#include "ProjectManager.h"
#include "../tracks/Track.h"

ProjectManager::ProjectManager()
{
    newProject();
}

ProjectManager::~ProjectManager()
{
    tracks.clear();
}

bool ProjectManager::newProject()
{
    tracks.clear();
    currentProjectFile = juce::File();
    projectModified = false;
    return true;
}

bool ProjectManager::loadProject(const juce::File& file)
{
    if (!file.existsAsFile())
        return false;
    
    juce::FileInputStream input(file);
    if (!input.openedOk())
        return false;
    
    juce::String jsonString = input.readEntireStreamAsString();
    juce::var projectData = juce::JSON::parse(jsonString);
    
    if (!projectData.isObject())
        return false;
    
    if (!deserializeProject(projectData))
        return false;
    
    currentProjectFile = file;
    projectModified = false;
    return true;
}

bool ProjectManager::saveProject(const juce::File& file)
{
    juce::var projectData = serializeProject();
    juce::String jsonString = juce::JSON::toString(projectData);
    
    juce::FileOutputStream output(file);
    if (!output.openedOk())
        return false;
    
    output.writeText(jsonString, false, false, nullptr);
    output.flush();
    
    currentProjectFile = file;
    projectModified = false;
    return true;
}

bool ProjectManager::saveProject()
{
    if (currentProjectFile.exists())
        return saveProject(currentProjectFile);
    
    return false;
}

juce::String ProjectManager::getProjectName() const
{
    if (currentProjectFile.exists())
        return currentProjectFile.getFileNameWithoutExtension();
    
    return "Untitled Project";
}

juce::var ProjectManager::serializeProject() const
{
    juce::DynamicObject::Ptr projectObject = new juce::DynamicObject();
    
    // Project metadata
    projectObject->setProperty("version", "1.0");
    projectObject->setProperty("name", getProjectName());
    
    // Serialize tracks
    juce::Array<juce::var> trackArray;
    for (const auto* track : tracks)
    {
        trackArray.add(serializeTrack(*track));
    }
    projectObject->setProperty("tracks", trackArray);
    
    return juce::var(projectObject.get());
}

bool ProjectManager::deserializeProject(const juce::var& projectData)
{
    if (!projectData.isObject())
        return false;
    
    auto* projectObject = projectData.getDynamicObject();
    if (!projectObject)
        return false;
    
    // Clear existing tracks
    tracks.clear();
    
    // Load tracks
    juce::var trackArray = projectObject->getProperty("tracks");
    if (trackArray.isArray())
    {
        for (const auto& trackVar : *trackArray.getArray())
        {
            Track track;
            if (deserializeTrack(trackVar, track))
            {
                tracks.add(new Track(std::move(track)));
            }
        }
    }
    
    return true;
}

juce::var ProjectManager::serializeTrack(const Track& track) const
{
    juce::DynamicObject::Ptr trackObject = new juce::DynamicObject();
    
    trackObject->setProperty("name", track.getName());
    trackObject->setProperty("type", static_cast<int>(track.getType()));
    trackObject->setProperty("color", track.getColor().toString());
    trackObject->setProperty("height", track.getHeight());
    trackObject->setProperty("muted", track.isMuted());
    trackObject->setProperty("solo", track.isSolo());
    trackObject->setProperty("volume", track.getVolume());
    trackObject->setProperty("pan", track.getPan());
    
    // Serialize clips
    juce::Array<juce::var> clipArray;
    for (const auto& clip : track.getClips())
    {
        clipArray.add(serializeClip(*clip));
    }
    trackObject->setProperty("clips", clipArray);
    
    return juce::var(trackObject.get());
}

bool ProjectManager::deserializeTrack(const juce::var& trackData, Track& track)
{
    if (!trackData.isObject())
        return false;
    
    auto* trackObject = trackData.getDynamicObject();
    if (!trackObject)
        return false;
    
    track.setName(trackObject->getProperty("name"));
    track.setType(static_cast<Track::Type>(static_cast<int>(trackObject->getProperty("type"))));
    track.setColour(juce::Colour::fromString(trackObject->getProperty("color").toString()));
    track.setHeight(trackObject->getProperty("height"));
    track.setMuted(trackObject->getProperty("muted"));
    track.setSolo(trackObject->getProperty("solo"));
    track.setVolume(trackObject->getProperty("volume"));
    track.setPan(trackObject->getProperty("pan"));
    
    // Load clips
    juce::var clipArray = trackObject->getProperty("clips");
    if (clipArray.isArray())
    {
        for (const auto& clipVar : *clipArray.getArray())
        {
            std::unique_ptr<Track::Clip> clip;
            if (deserializeClip(clipVar, clip))
            {
                track.addClip(std::move(clip));
            }
        }
    }
    
    return true;
}

juce::var ProjectManager::serializeClip(const Track::Clip& clip) const
{
    juce::DynamicObject::Ptr clipObject = new juce::DynamicObject();
    
    clipObject->setProperty("name", clip.getName());
    clipObject->setProperty("startTime", clip.getStartTime());
    clipObject->setProperty("length", clip.getLength());
    clipObject->setProperty("offset", clip.getOffset());
    clipObject->setProperty("color", clip.getColour().toString());
    
    // Audio file reference
    if (!clip.getAudioFile().isEmpty())
    {
        clipObject->setProperty("audioFile", clip.getAudioFile());
    }
    
    return juce::var(clipObject.get());
}

bool ProjectManager::deserializeClip(const juce::var& clipData, std::unique_ptr<Track::Clip>& clip)
{
    if (!clipData.isObject())
        return false;
    
    auto* clipObject = clipData.getDynamicObject();
    if (!clipObject)
        return false;
    
    clip = std::make_unique<Track::Clip>();
    
    clip->setName(clipObject->getProperty("name"));
    clip->setStartTime(clipObject->getProperty("startTime"));
    clip->setLength(clipObject->getProperty("length"));
    clip->setOffset(clipObject->getProperty("offset"));
    clip->setColour(juce::Colour::fromString(clipObject->getProperty("color").toString()));
    
    // Audio file reference
    juce::String audioFile = clipObject->getProperty("audioFile");
    if (!audioFile.isEmpty())
    {
        clip->setAudioFile(audioFile);
    }
    
    return true;
}

void ProjectManager::addTrack(Track::Type type)
{
    auto* newTrack = new Track();
    newTrack->setType(type);
    newTrack->setName("Track " + juce::String(tracks.size() + 1));
    tracks.add(newTrack);
    projectModified = true;
}

void ProjectManager::removeTrack(int index)
{
    if (index >= 0 && index < tracks.size())
    {
        tracks.remove(index);
        projectModified = true;
    }
}

void ProjectManager::moveTrack(int fromIndex, int toIndex)
{
    if (fromIndex >= 0 && fromIndex < tracks.size() &&
        toIndex >= 0 && toIndex < tracks.size())
    {
        tracks.move(fromIndex, toIndex);
        projectModified = true;
    }
}

void ProjectManager::addClipToTrack(int trackIndex, std::unique_ptr<Track::Clip> clip)
{
    if (trackIndex >= 0 && trackIndex < tracks.size())
    {
        tracks[trackIndex]->addClip(std::move(clip));
        projectModified = true;
    }
}

void ProjectManager::removeClipFromTrack(int trackIndex, Track::Clip* clip)
{
    if (trackIndex >= 0 && trackIndex < tracks.size())
    {
        tracks[trackIndex]->removeClip(clip);
        projectModified = true;
    }
}

juce::File ProjectManager::getAudioDataDirectory() const
{
    if (currentProjectFile.exists())
    {
        juce::File projectDir = currentProjectFile.getParentDirectory();
        juce::File audioDir = projectDir.getChildFile("Audio");
        if (!audioDir.exists())
            audioDir.createDirectory();
        return audioDir;
    }
    
    return juce::File();
}

juce::File ProjectManager::getAudioFilePath(const juce::String& audioFileName) const
{
    return getAudioDataDirectory().getChildFile(audioFileName);
}
