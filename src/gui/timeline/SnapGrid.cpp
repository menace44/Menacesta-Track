/*
  ==============================================================================

   SnapGrid.cpp
   Created: 15 Jan 2025
   Author:  DAW Development Team

   Provides snap-to-grid functionality for timeline operations.

  ==============================================================================
*/

#include "SnapGrid.h"

SnapGrid::SnapGrid()
{
}

void SnapGrid::setSnapMode(SnapMode mode)
{
    snapMode = mode;
}

void SnapGrid::setBPM(double bpm)
{
    this->bpm = juce::jmax(30.0, bpm);
}

void SnapGrid::setTimeSignature(int numerator, int denominator)
{
    timeSigNumerator = juce::jmax(1, numerator);
    timeSigDenominator = juce::jmax(1, denominator);
}

void SnapGrid::setPixelsPerSecond(double pixelsPerSecond)
{
    this->pixelsPerSecond = juce::jmax(1.0, pixelsPerSecond);
}

double SnapGrid::snapTimeToGrid(double time, bool snapToStart) const
{
    if (snapMode == SnapMode::None)
        return time;
    
    double snapInterval = getSnapInterval();
    if (snapInterval <= 0.0)
        return time;
    
    double snapped = std::round(time / snapInterval) * snapInterval;
    
    if (!snapToStart)
    {
        // For end snapping, we might want to snap to the nearest grid line
        double distanceToStart = std::abs(time - snapped);
        double distanceToEnd = std::abs(time - (snapped + snapInterval));
        
        if (distanceToEnd < distanceToStart)
            snapped += snapInterval;
    }
    
    return snapped;
}

double SnapGrid::getSnapInterval() const
{
    switch (snapMode)
    {
        case SnapMode::None:
            return 0.0;
            
        case SnapMode::Bar:
            return getBarLength();
            
        case SnapMode::Beat:
            return getBeatLength();
            
        case SnapMode::Division:
            return getDivisionLength();
            
        case SnapMode::Frame:
            return 1.0 / 30.0; // 30 fps
            
        default:
            return getBeatLength();
    }
}

double SnapGrid::getBeatLength() const
{
    return 60.0 / bpm;
}

double SnapGrid::getBarLength() const
{
    return (60.0 / bpm) * timeSigNumerator;
}

double SnapGrid::getDivisionLength() const
{
    return getBeatLength() / 4.0; // 1/16 note divisions
}

juce::Array<double> SnapGrid::getSnapPointsInRange(juce::Range<double> timeRange) const
{
    juce::Array<double> snapPoints;
    
    if (snapMode == SnapMode::None || timeRange.isEmpty())
        return snapPoints;
    
    double snapInterval = getSnapInterval();
    if (snapInterval <= 0.0)
        return snapPoints;
    
    double startTime = timeRange.getStart();
    double endTime = timeRange.getEnd();
    
    double firstSnap = std::ceil(startTime / snapInterval) * snapInterval;
    
    for (double snap = firstSnap; snap <= endTime; snap += snapInterval)
    {
        snapPoints.add(snap);
    }
    
    return snapPoints;
}
