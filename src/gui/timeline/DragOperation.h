/*
  ==============================================================================

   DragOperation.h
   Created: 15 Jan 2025
   Author:  DAW Development Team

   Manages drag and resize operations for timeline clips.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SnapGrid.h"
#include "SelectionManager.h"

class ClipComponent;
class TimelineTrackComponent;

enum class DragType
{
    None,
    Move,
    ResizeStart,
    ResizeEnd
};

struct DragState
{
    DragType type = DragType::None;
    juce::Point<int> mouseDownPos;
    juce::Point<int> currentPos;
    double originalStartTime = 0.0;
    double originalEndTime = 0.0;
    double originalDuration = 0.0;
    int originalTrackIndex = 0;
    bool isDragging = false;
    bool snapEnabled = true;
};

class DragOperation : public juce::MouseListener,
                      public juce::Timer
{
public:
    DragOperation(SelectionManager& selectionManager, SnapGrid& snapGrid);
    ~DragOperation() override;
    
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    
    void timerCallback() override;
    
    bool isActive() const { return dragState.isDragging; }
    DragType getCurrentDragType() const { return dragState.type; }
    
    void setTimelineViewport(juce::Component* viewport) { timelineViewport = viewport; }
    void setTrackComponents(const juce::Array<TimelineTrackComponent*>& tracks);
    
    void beginDrag(ClipComponent* clip, const juce::MouseEvent& event, DragType type);
    void updateDrag(const juce::MouseEvent& event);
    void endDrag();
    
    void cancelDrag();
    
    juce::Array<ClipComponent*> getAffectedClips() const;
    
private:
    SelectionManager& selectionManager;
    SnapGrid& snapGrid;
    
    DragState dragState;
    ClipComponent* primaryClip = nullptr;
    juce::Array<ClipComponent*> draggedClips;
    juce::Array<TimelineTrackComponent*> trackComponents;
    juce::Component* timelineViewport = nullptr;
    
    juce::Point<int> lastMousePos;
    juce::ComponentDragger dragger;
    
    void updateClipPositions();
    void updateClipResizing();
    TimelineTrackComponent* getTrackAtPosition(const juce::Point<int>& pos);
    double getTimeAtPosition(const juce::Point<int>& pos) const;
    juce::Point<int> getPositionAtTime(double time) const;
    
    void showSnapIndicators();
    void hideSnapIndicators();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragOperation)
};
