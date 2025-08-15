/*
  ==============================================================================

   SelectionManager.cpp
   Created: 15 Jan 2025
   Author:  DAW Development Team

   Manages multi-selection of clips in the timeline.

  ==============================================================================
*/

#include "SelectionManager.h"
#include "ClipComponent.h"

SelectionManager::SelectionManager()
{
}

SelectionManager::~SelectionManager()
{
    deselectAll();
}

void SelectionManager::selectClip(ClipComponent* clip, bool addToSelection)
{
    if (!addToSelection)
        deselectAll();
    
    if (clip != nullptr && !selectedClips.contains(clip))
    {
        selectedClips.add(clip);
        clip->setSelected(true);
        sendChangeMessage();
    }
}

void SelectionManager::deselectClip(ClipComponent* clip)
{
    if (clip != nullptr && selectedClips.contains(clip))
    {
        selectedClips.removeFirstMatchingValue(clip);
        clip->setSelected(false);
        sendChangeMessage();
    }
}

void SelectionManager::deselectAll()
{
    for (auto* clip : selectedClips)
    {
        if (clip != nullptr)
            clip->setSelected(false);
    }
    
    selectedClips.clear();
    sendChangeMessage();
}

void SelectionManager::toggleSelection(ClipComponent* clip)
{
    if (clip == nullptr)
        return;
    
    if (isSelected(clip))
        deselectClip(clip);
    else
        selectClip(clip, true);
}

bool SelectionManager::isSelected(ClipComponent* clip) const
{
    return clip != nullptr && selectedClips.contains(clip);
}

juce::Range<double> SelectionManager::getSelectedTimeRange() const
{
    if (selectedClips.isEmpty())
        return juce::Range<double>();
    
    double start = std::numeric_limits<double>::max();
    double end = std::numeric_limits<double>::lowest();
    
    for (auto* clipComponent : selectedClips)
    {
        if (clipComponent != nullptr)
        {
            auto* clip = clipComponent->getClip();
            if (clip != nullptr)
            {
                start = juce::jmin(start, clip->getStartTime());
                end = juce::jmax(end, clip->getEndTime());
            }
        }
    }
    
    return juce::Range<double>(start, end);
}

juce::Array<Clip*> SelectionManager::getSelectedClipObjects() const
{
    juce::Array<Clip*> clips;
    
    for (auto* clipComponent : selectedClips)
    {
        if (clipComponent != nullptr && clipComponent->getClip() != nullptr)
            clips.add(clipComponent->getClip());
    }
    
    return clips;
}
