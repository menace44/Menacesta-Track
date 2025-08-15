#pragma once

#include <JuceHeader.h>

namespace Track {
    class TimelineGrid : public juce::Component
    {
    public:
        TimelineGrid();
        ~TimelineGrid() override;

        void paint(juce::Graphics& g) override;
        void resized() override;
        
        void setZoomLevel(double zoom);
        void setTimeRange(double startTime, double endTime);
        void setSnapToGrid(bool snap);
        void setGridResolution(double beatsPerDivision);
        
        double getSnapPosition(double time) const;
        
        enum class GridStyle {
            Lines,
            Blocks,
            Minimal
        };
        
        void setGridStyle(GridStyle style);

    private:
        double zoomLevel = 1.0;
        double startTime = 0.0;
        double endTime = 60.0;
        bool snapToGrid = true;
        double beatsPerDivision = 1.0;
        GridStyle gridStyle = GridStyle::Lines;
        
        void drawGridLines(juce::Graphics& g);
        void drawTimeMarkers(juce::Graphics& g);
        void drawBeatMarkers(juce::Graphics& g);
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineGrid)
    };
}
