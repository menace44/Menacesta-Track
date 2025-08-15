/*
  ==============================================================================

   DragOperation.cpp
   Created: 15 Jan 2025
   Author:  DAW Development Team

   Manages drag and resize operations for timeline clips.

  ==============================================================================
*/

#include "DragOperation.h"
#include "ClipComponent.h"
#include "TimelineTrackComponent.h"
#include "TimelineViewport.h"

DragOperation::DragOperation(SelectionManager& sm, SnapGrid& sg)
    : selectionManager(sm), snapGrid(sg)
{
    startTimerHz(60);
}

DragOperation::~DragOperation()
{
    stopTimer();
}

void DragOperation::mouseDown(const juce::MouseEvent& event)
{
    // Handled by ClipComponent
}

void DragOperation::mouseDrag(const juce::MouseEvent& event)
{
    if (dragState.isDragging)
        updateDrag(event);
}

void DragOperation::mouseUp(const juce::MouseEvent& event)
{
    if (dragState.isDragging)
        endDrag();
}

void DragOperation::timerCallback()
{
    // Update drag state if needed
}

void DragOperation::beginDrag(ClipComponent* clip, const juce::MouseEvent& event, DragType type)
{
    if (clip == nullptr)
        return;
    
    primaryClip = clip;
    dragState.type = type;
    dragState.mouseDownPos = event.getPosition();
    dragState.currentPos = event.getPosition();
    dragState.isDragging = true;
    
    auto* clipObj = clip->getClip();
    if (clipObj != nullptr)
    {
        dragState.originalStartTime = clipObj->getStartTime();
        dragState.originalEndTime = clipObj->getEndTime();
        dragState.originalDuration = clipObj->getDuration();
    }
    
    // Get all selected clips for group operations
    draggedClips.clear();
    if (selectionManager.isSelected(clip))
    {
        // If this clip is selected, drag all selected clips
        auto selectedComponents = selectionManager.getSelectedClipObjects();
        for (auto* selectedClip : selectedComponents)
        {
            if (auto* component = dynamic_cast<ClipComponent*>(selectedClip))
                draggedClips.add(component);
        }
    }
    else
    {
        // Otherwise, just drag this clip
        draggedClips.add(clip);
    }
    
    // Store original track index
    if (auto* track = clip->findParentComponentOfClass<TimelineTrackComponent>())
    {
        dragState.originalTrackIndex = trackComponents.indexOf(track);
    }
    
    lastMousePos = event.getPosition();
}

void DragOperation::updateDrag(const juce::MouseEvent& event)
{
    if (!dragState.isDragging || draggedClips.isEmpty())
        return;
    
    juce::Point<int> delta = event.getPosition() - lastMousePos;
    lastMousePos = event.getPosition();
    
    switch (dragState.type)
    {
        case DragType::Move:
            updateClipPositions();
            break;
            
        case DragType::ResizeStart:
        case DragType::ResizeEnd:
            updateClipResizing();
            break;
            
        default:
            break;
    }
}

void DragOperation::endDrag()
{
    if (!dragState.isDragging)
        return;
    
    dragState.isDragging = false;
    draggedClips.clear();
    primaryClip = nullptr;
    
    hideSnapIndicators();
}

void DragOperation::cancelDrag()
{
    if (!dragState.isDragging)
        return;
    
    // Restore original positions
    for (auto* clip : draggedClips)
    {
        if (clip != nullptr && clip->getClip() != nullptr)
        {
            clip->getClip()->setStartTime(dragState.originalStartTime);
            clip->getClip()->setEndTime(dragState.originalEndTime);
            clip->updateBounds();
        }
    }
    
    endDrag();
}

juce::Array<ClipComponent*> DragOperation::getAffectedClips() const
{
    return draggedClips;
}

void DragOperation::updateClipPositions()
{
    if (draggedClips.isEmpty())
        return;
    
    // Calculate time delta based on mouse movement
    double timeDelta = getTimeAtPosition(lastMousePos) - getTimeAtPosition(dragState.mouseDownPos);
    
    if (snapGrid.isSnapEnabled())
    {
        // Snap the primary clip's new position
        double primaryNewStart = dragState.originalStartTime + timeDelta;
        double snappedStart = snapGrid.snapTimeToGrid(primaryNewStart);
        timeDelta = snappedStart - dragState.originalStartTime;
    }
    
    // Apply movement to all dragged clips
    for (auto* clip : draggedClips)
    {
        if (clip != nullptr && clip->getClip() != nullptr)
        {
            double newStart = dragState.originalStartTime + timeDelta;
            double newEnd = newStart + clip->getClip()->getDuration();
            
            clip->getClip()->setStartTime(newStart);
            clip->getClip()->setEndTime(newEnd);
            clip->updateBounds();
        }
    }
}

void DragOperation::updateClipResizing()
{
    if (draggedClips.isEmpty() || primaryClip == nullptr)
        return;
    
    double mouseTime = getTimeAtPosition(lastMousePos);
    
    if (snapGrid.isSnapEnabled())
    {
        mouseTime = snapGrid.snapTimeToGrid(mouseTime, dragState.type == DragType::ResizeStart);
    }
