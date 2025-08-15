#include "WaveformDisplay.h"

namespace Track {

WaveformDisplay::WaveformDisplay()
{
    setName("Waveform Display");
    setSize(400, 100);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::setClip(Clip* clip)
{
    currentClip = clip;
    needsRepaint = true;
    repaint();
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF1E1E1E));
    
    if (!currentClip)
    {
        g.setColour(juce::Colours::grey);
        g.setFont(juce::Font(14.0f));
        g.drawText("No clip loaded", getLocalBounds(), juce::Justification::centred);
        return;
    }
    
    switch (displayMode)
    {
        case DisplayMode::Waveform:
            drawWaveform(g);
            break;
        case DisplayMode::Spectrogram:
            drawSpectrogram(g);
            break;
        case DisplayMode::Minimal:
            drawMinimal(g);
            break;
    }
    
    drawPlaybackPosition(g);
    drawSelection(g);
}

void WaveformDisplay::resized()
{
    needsRepaint = true;
}

void WaveformDisplay::setZoomLevel(double zoom)
{
    zoomLevel = zoom;
    needsRepaint = true;
    repaint();
}

void WaveformDisplay::setPlaybackPosition(double position)
{
    playbackPosition = position;
    repaint();
}

void WaveformDisplay::setSelectionRange(double start, double end)
{
    selectionStart = start;
    selectionEnd = end;
    repaint();
}

void WaveformDisplay::setDisplayMode(DisplayMode mode)
{
    displayMode = mode;
    needsRepaint = true;
    repaint();
}

void WaveformDisplay::generateWaveformImage()
{
    if (!currentClip) return;
    
    const auto& waveformData = currentClip->getWaveformData();
    if (waveformData.isEmpty()) return;
    
    waveformImage = juce::Image(juce::Image::RGB, getWidth(), getHeight(), true);
    juce::Graphics g(waveformImage);
    
    g.fillAll(juce::Colours::transparentBlack);
    
    auto bounds = getLocalBounds().toFloat();
    float centerY = bounds.getCentreY();
    float height = bounds.getHeight() * 0.8f;
    
    juce::Path waveformPath;
    float stepX = bounds.getWidth() / waveformData.size();
    
    for (int i = 0; i < waveformData.size(); ++i)
    {
        float x = bounds.getX() + i * stepX;
        float amplitude = waveformData[i] * height * 0.5f;
        
        if (i == 0)
        {
            waveformPath.startNewSubPath(x, centerY - amplitude);
        }
        else
        {
            waveformPath.lineTo(x, centerY - amplitude);
        }
    }
    
    for (int i = waveformData.size() - 1; i >= 0; --i)
    {
        float x = bounds.getX() + i * stepX;
        float amplitude = waveformData[i] * height * 0.5f;
        waveformPath.lineTo(x, centerY + amplitude);
    }
    
    waveformPath.closeSubPath();
    
    g.setColour(juce::Colour(0xFF2196F3));
    g.fillPath(waveformPath);
    
    needsRepaint = false;
}

void WaveformDisplay::drawWaveform(juce::Graphics& g)
{
    if (needsRepaint)
    {
        generateWaveformImage();
    }
    
    if (waveformImage.isValid())
    {
        g.drawImageAt(waveformImage, 0, 0);
    }
}

void WaveformDisplay::drawSpectrogram(juce::Graphics& g)
{
    // Placeholder for spectrogram display
    g.setColour(juce::Colour(0xFF2196F3));
    g.fillRect(getLocalBounds());
    
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(12.0f));
    g.drawText("Spectrogram View", getLocalBounds(), juce::Justification::centred);
}

void WaveformDisplay::drawMinimal(juce::Graphics& g)
{
    // Minimal waveform display
    auto bounds = getLocalBounds().toFloat();
    
    g.setColour(juce::Colour(0xFF2196F3));
    g.fillRect(bounds);
    
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.drawRect(bounds, 1.0f);
}

void WaveformDisplay::drawPlaybackPosition(juce::Graphics& g)
{
    if (playbackPosition <= 0.0 || !currentClip) return;
    
    auto bounds = getLocalBounds().toFloat();
    float x = bounds.getX() + (playbackPosition / currentClip->getDuration()) * bounds.getWidth();
    
    g.setColour(juce::Colours::red);
    g.drawLine(x, bounds.getY(), x, bounds.getBottom(), 2.0f);
}

void WaveformDisplay::drawSelection(juce::Graphics& g)
{
    if (selectionStart >= selectionEnd || !currentClip) return;
    
    auto bounds = getLocalBounds().toFloat();
    
    float startX = bounds.getX() + (selectionStart / currentClip->getDuration()) * bounds.getWidth();
    float endX = bounds.getX() + (selectionEnd / currentClip->getDuration()) * bounds.getWidth();
    
    g.setColour(juce::Colours::yellow.withAlpha(0.3f));
    g.fillRect(startX, bounds.getY(), endX - startX, bounds.getHeight());
}

} // namespace Track
