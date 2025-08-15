/*
  ==============================================================================

   SelectionManager.h
   Created: 15 Jan 2025
   Author:  DAW Development Team

   Manages multi-selection of clips in the timeline.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../tracks/Clip.h"

class ClipComponent;

class SelectionManager : public juce::ChangeBroadcaster
{
public:
    SelectionManager();
    ~SelectionManager() override;

    void selectClip(ClipComponent* clip, bool addToSelection = false);
    void deselectClip(ClipComponent* clip);
    void deselectAll();
    void toggleSelection(ClipComponent* clip);
    
    bool isSelected(ClipComponent* clip) const;
    const juce::Array<ClipComponent*>& getSelectedClips() const { return selectedClips; }
    
    bool hasSelection() const { return !selectedClips.isEmpty(); }
    int getNumSelected() const { return selectedClips.size(); }
    
    juce::Range<double> getSelectedTimeRange() const;
    juce::Array<Clip*> getSelectedClipObjects() const;

private:
    juce::Array<ClipComponent*> selectedClips;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SelectionManager)
};
