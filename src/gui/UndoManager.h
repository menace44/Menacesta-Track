#pragma once
#include <JuceHeader.h>
#include <memory>
#include <vector>

class TimelineCommand {
public:
    virtual ~TimelineCommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual juce::String getName() const = 0;
};

class MoveClipCommand : public TimelineCommand {
public:
    MoveClipCommand(Track::Clip* clip, double newStartTime, double oldStartTime)
        : clip(clip), newStartTime(newStartTime), oldStartTime(oldStartTime) {}
    
    void execute() override {
        clip->setStartTime(newStartTime);
    }
    
    void undo() override {
        clip->setStartTime(oldStartTime);
    }
    
    juce::String getName() const override {
        return "Move Clip";
    }
    
private:
    Track::Clip* clip;
    double newStartTime;
    double oldStartTime;
};

class ResizeClipCommand : public TimelineCommand {
public:
    ResizeClipCommand(Track::Clip* clip, double newDuration, double oldDuration)
        : clip(clip), newDuration(newDuration), oldDuration(oldDuration) {}
    
    void execute() override {
        clip->setDuration(newDuration);
    }
    
    void undo() override {
        clip->setDuration(oldDuration);
    }
    
    juce::String getName() const override {
        return "Resize Clip";
    }
    
private:
    Track::Clip* clip;
    double newDuration;
    double oldDuration;
};

class DeleteClipCommand : public TimelineCommand {
public:
    DeleteClipCommand(Track* track, Track::Clip* clip)
        : track(track), clip(clip) {
        clipStartTime = clip->getStartTime();
        clipDuration = clip->getDuration();
        clipName = clip->getName();
        clipType = clip->getType();
        clipColour = clip->getColour();
    }
    
    void execute() override {
        track->removeClip(clip);
    }
    
    void undo() override {
        auto* newClip = new Track::Clip(clipName, clipType, clipStartTime, clipDuration);
        newClip->setColour(clipColour);
        track->addClip(newClip);
        clip = newClip;
    }
    
    juce::String getName() const override {
        return "Delete Clip";
    }
    
private:
    Track* track;
    Track::Clip* clip;
    double clipStartTime;
    double clipDuration;
    juce::String clipName;
    Track::ClipType clipType;
    juce::Colour clipColour;
};

class UndoManager {
public:
    void performCommand(std::unique_ptr<TimelineCommand> command) {
        command->execute();
        undoStack.push_back(std::move(command));
        redoStack.clear();
        
        if (undoStack.size() > maxUndoLevels) {
            undoStack.erase(undoStack.begin());
        }
    }
    
    bool canUndo() const {
        return !undoStack.empty();
    }
    
    bool canRedo() const {
        return !redoStack.empty();
    }
    
    void undo() {
        if (canUndo()) {
            auto command = std::move(undoStack.back());
            undoStack.pop_back();
            command->undo();
            redoStack.push_back(std::move(command));
        }
    }
    
    void redo() {
        if (canRedo()) {
            auto command = std::move(redoStack.back());
            redoStack.pop_back();
            command->execute();
            undoStack.push_back(std::move(command));
        }
    }
    
    void clear() {
        undoStack.clear();
        redoStack.clear();
    }
    
private:
    static constexpr size_t maxUndoLevels = 50;
    std::vector<std::unique_ptr<TimelineCommand>> undoStack;
    std::vector<std::unique_ptr<TimelineCommand>> redoStack;
};
