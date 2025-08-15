#include "TimelineGrid.h"

namespace Track {

TimelineGrid::TimelineGrid()
{
    setName("Timeline Grid");
    setSize(800, 400);
}

TimelineGrid::~TimelineGrid()
{
}

void TimelineGrid::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF2A2A2A));
    
    drawGridLines(g);
    drawTimeMarkers(g);
    drawBeatMarkers(g);
}

void TimelineGrid::resized()
{
    // Component resized
}

void TimelineGrid::setZoomLevel(double zoom)
{
    zoomLevel = zoom;
    repaint();
}

void TimelineGrid::setTimeRange(double startTime, double endTime)
{
    this->startTime = startTime;
    this->endTime = endTime;
    repaint();
}

void TimelineGrid::setSnapToGrid(bool snap)
{
    snapToGrid = snap;
}

void TimelineGrid::setGridResolution(double beatsPerDivision)
{
    this->beatsPerDivision = beatsPerDivision;
    repaint();
}

double TimelineGrid::getSnapPosition(double time) const
{
    if (!snapToGrid) return time;
    
    double divisionLength = 60.0 / (120.0 * beatsPerDivision); // Assuming 120 BPM
    return std::round(time / divisionLength) * divisionLength;
}

void TimelineGrid::setGridStyle(GridStyle style)
{
    gridStyle = style;
    repaint();
}

void TimelineGrid::drawGridLines(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    switch (gridStyle)
    {
        case GridStyle::Lines:
        {
            g.setColour(juce::Colour(0xFF404040));
            
            double totalTime = endTime - startTime;
            double pixelsPerSecond = bounds.getWidth() / totalTime;
            
            // Draw vertical grid lines
            for (double time = startTime; time <= endTime; time += 1.0)
            {
                int x = static_cast<int>((time - startTime) * pixelsPerSecond);
                g.drawVerticalLine(x, 0.0f, static_cast<float>(bounds.getHeight()));
            }
            
            // Draw horizontal grid lines
            for (int y = 0; y < bounds.getHeight(); y += 20)
            {
                g.drawHorizontalLine(y, 0.0f, static_cast<float>(bounds.getWidth()));
            }
            break;
        }
            
        case GridStyle::Blocks:
        {
            g.setColour(juce::Colour(0xFF353535));
            
            double totalTime = endTime - startTime;
            double pixelsPerBeat = bounds.getWidth() / (totalTime * 2.0); // 2 beats per second
            
            for (double beat = 0; beat < totalTime * 2.0; beat += 1.0)
            {
                int x = static_cast<int>(beat * pixelsPerBeat);
                int width = static_cast<int>(pixelsPerBeat);
                
                if (static_cast<int>(beat) % 4 == 0)
                    g.setColour(juce::Colour(0xFF404040));
                else
                    g.setColour(juce::Colour(0xFF353535));
                
                g.fillRect(x, 0, width, bounds.getHeight());
            }
            break;
        }
            
        case GridStyle::Minimal:
        {
            g.setColour(juce::Colour(0xFF505050));
            
            double totalTime = endTime - startTime;
            double pixelsPerBar = bounds.getWidth() / (totalTime / 4.0); // 4 beats per bar
            
            for (double bar = 0; bar < totalTime / 4.0; bar += 1.0)
            {
                int x = static_cast<int>(bar * pixelsPerBar);
                g.drawVerticalLine(x, 0.0f, static_cast<float>(bounds.getHeight()));
            }
            break;
        }
    }
}

void TimelineGrid::drawTimeMarkers(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    double totalTime = endTime - startTime;
    double pixelsPerSecond = bounds.getWidth() / totalTime;
    
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(10.0f));
    
    for (double time = startTime; time <= endTime; time += 5.0)
    {
        int x = static_cast<int>((time - startTime) * pixelsPerSecond);
        
        juce::String timeText;
        int minutes = static_cast<int>(time) / 60;
        int seconds = static_cast<int>(time) % 60;
        timeText = juce::String::formatted("%d:%02d", minutes, seconds);
        
        g.drawText(timeText, x - 20, 5, 40, 15, juce::Justification::centred);
    }
}

void TimelineGrid::drawBeatMarkers(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    double totalTime = endTime - startTime;
    double pixelsPerBeat = bounds.getWidth() / (totalTime * 2.0); // 120 BPM
    
    g.setColour(juce::Colour(0xFF808080));
    
    for (double beat = 0; beat < totalTime * 2.0; beat += 1.0)
    {
        int x = static_cast<int>(beat * pixelsPerBeat);
        
        if (static_cast<int>(beat) % 4 == 0)
        {
            g.drawVerticalLine(x, 0.0f, static_cast<float>(bounds.getHeight()));
        }
        else
        {
            g.drawVerticalLine(x, static_cast<float>(bounds.getHeight() - 10), static_cast<float>(bounds.getHeight()));
        }
    }
}

} // namespace Track
