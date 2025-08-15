#pragma once

#include <JuceHeader.h>

namespace Track {
    class PlayheadIndicator : public juce::Component
    {
    public:
        PlayheadIndicator();
        ~PlayheadIndicator() override;

        void paint(juce::Graphics& g) override;
        void resized() override;
        
        void setPlaybackPosition(double position);
        void setDuration(double duration);
        void setVisibleRange(double startTime, double endTime);
        
        void setColor(const juce::Colour& color);
        void setThickness(float thickness);
        
        void updatePosition();

    private:
        double playbackPosition = 0.0;
        double duration = 60.0;
        double visibleStartTime = 0.0;
        double visibleEndTime = 60.0;
        juce::Colour playheadColor = juce::Colours::red;
        float thickness = 2.0f;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayheadIndicator)
    };
}
