/*
  ==============================================================================

   SnapGrid.h
   Created: 15 Jan 2025
   Author:  DAW Development Team

   Provides snap-to-grid functionality for timeline operations.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SnapGrid
{
public:
    enum class SnapMode
    {
        None,
        Bar,
        Beat,
        Division,
        Frame
    };
    
    SnapGrid();
    ~SnapGrid() = default;
    
    void setSnapMode(SnapMode mode);
    void setBPM(double bpm);
    void setTimeSignature(int numerator, int denominator);
    void setPixelsPerSecond(double pixelsPerSecond);
    
    double snapTimeToGrid(double time, bool snapToStart = true) const;
    double getSnapInterval() const;
    
    bool isSnapEnabled() const { return snapMode != SnapMode::None; }
    SnapMode getSnapMode() const { return snapMode; }
    
    juce::Array<double> getSnapPointsInRange(juce::Range<double> timeRange) const;

private:
    SnapMode snapMode = SnapMode::Beat;
    double bpm = 120.0;
    int timeSigNumerator = 4;
    int timeSigDenominator = 4;
    double pixelsPerSecond = 100.0;
    
    double getBeatLength() const;
    double getBarLength() const;
    double getDivisionLength() const;
};
