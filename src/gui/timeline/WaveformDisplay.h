#pragma once

#include <JuceHeader.h>
#include "../../tracks/Clip.h"

namespace Track {
    class WaveformDisplay : public juce::Component
    {
    public:
        WaveformDisplay();
        ~WaveformDisplay() override;

        void setClip(Clip* clip);
        void paint(juce::Graphics& g) override;
        void resized() override;
        
        void setZoomLevel(double zoom);
        void setPlaybackPosition(double position);
        void setSelectionRange(double start, double end);
        
        enum class DisplayMode {
            Waveform,
            Spectrogram,
            Minimal
        };
        
        void setDisplayMode(DisplayMode mode);

    private:
        Clip* currentClip = nullptr;
        double zoomLevel = 1.0;
        double playbackPosition = 0.0;
        double selectionStart = 0.0;
        double selectionEnd = 0.0;
        DisplayMode displayMode = DisplayMode::Waveform;
        
        juce::Image waveformImage;
        bool needsRepaint = true;
        
        void generateWaveformImage();
        void drawWaveform(juce::Graphics& g);
        void drawSpectrogram(juce::Graphics& g);
        void drawMinimal(juce::Graphics& g);
        void drawPlaybackPosition(juce::Graphics& g);
        void drawSelection(juce::Graphics& g);
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
    };
}
