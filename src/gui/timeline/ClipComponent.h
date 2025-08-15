#pragma once

#include <JuceHeader.h>
#include "../../tracks/Clip.h"

namespace Track {
    class ClipComponent : public juce::Component,
                          public juce::ChangeListener
    {
    public:
        ClipComponent(Clip& clip);
        ~ClipComponent() override;

        void paint(juce::Graphics& g) override;
        void resized() override;
        
        void changeListenerCallback(juce::ChangeBroadcaster* source) override;
        
        Clip& getClip() const { return clip; }
        
        void setZoomLevel(double zoom);
        void setSelected(bool selected);
        bool isSelected() const;

        // Drag and drop support
        void mouseDown(const juce::MouseEvent& event) override;
        void mouseDrag(const juce::MouseEvent& event) override;
        void mouseUp(const juce::MouseEvent& event) override;
        bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override;
        void itemDropped(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override;

    private:
        Clip& clip;
        double zoomLevel = 1.0;
        bool selected = false;
        
        void drawAudioClip(juce::Graphics& g);
        void drawMidiClip(juce::Graphics& g);
        void drawWaveform(juce::Graphics& g);
        void drawMidiNotes(juce::Graphics& g);
        void drawFadeHandles(juce::Graphics& g);
        void drawLoopRegion(juce::Graphics& g);
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClipComponent)
    };
}
