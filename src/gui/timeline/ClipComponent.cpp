#include "ClipComponent.h"
#include "SelectionManager.h"
#include "DragOperation.h"
#include <unordered_map>

namespace Track {

ClipComponent::ClipComponent(Clip& clipRef)
    : clip(clipRef)
{
    setName("Clip: " + clip.getName());
    clip.addChangeListener(this);
    
    setSize(200, 50);
    
    setWantsKeyboardFocus(true);
    setMouseClickGrabsKeyboardFocus(true);
}

ClipComponent::~ClipComponent()
{
    clip.removeChangeListener(this);
}

void ClipComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    // Background based on clip type
    if (clip.getType() == ClipType::Audio)
    {
        g.setColour(juce::Colour(0xFF2196F3)); // Blue for audio
    }
    else
    {
        g.setColour(juce::Colour(0xFF9C27B0)); // Purple for MIDI
    }
    
    if (selected)
    {
        g.setColour(g.getCurrentColour().brighter(0.3f));
    }
    
    g.fillRoundedRectangle(bounds.reduced(1.0f), 4.0f);
    
    // Border
    g.setColour(juce::Colours::white.withAlpha(0.3f));
    g.drawRoundedRectangle(bounds.reduced(1.0f), 4.0f, 1.0f);
    
    // Clip type indicator
    g.setColour(juce::Colours::white.withAlpha(0.8f));
    g.setFont(juce::Font(10.0f));
    
    auto contentBounds = getContentBounds();
    
    if (clip.getType() == ClipType::Audio)
    {
        g.drawText("AUDIO", contentBounds, juce::Justification::topLeft);
        drawAudioClip(g, contentBounds);
    }
    else
    {
        g.drawText("MIDI", contentBounds, juce::Justification::topLeft);
        drawMidiClip(g, contentBounds);
    }
    
    // Clip name
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(12.0f, juce::Font::bold));
    g.drawText(clip.getName(), contentBounds, juce::Justification::centred);
    
    // Draw fade handles and loop region
    drawFadeHandles(g, bounds);
    drawLoopRegion(g, bounds);
}

void ClipComponent::resized()
{
    // Component resized
}

void ClipComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &clip)
    {
        repaint();
    }
}

void ClipComponent::setZoomLevel(double zoom)
{
    zoomLevel = zoom;
    repaint();
}

void ClipComponent::setSelected(bool selected)
{
    this->selected = selected;
    clip.setSelected(selected);
    repaint();
}

bool ClipComponent::isSelected() const
{
    return selected;
}

juce::Rectangle<float> ClipComponent::getContentBounds() const
{
    return getLocalBounds().toFloat().reduced(4.0f);
}

juce::Rectangle<float> ClipComponent::getBoundsForZoom() const
{
    auto bounds = getLocalBounds().toFloat();
    bounds.setWidth(bounds.getWidth() * zoomLevel);
    return bounds;
}

void ClipComponent::drawAudioClip(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    // Draw waveform if available
    const auto& waveformData = clip.getWaveformData();
    if (!waveformData.isEmpty())
    {
        drawWaveform(g, bounds);
    }
}

void ClipComponent::drawMidiClip(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    // Draw MIDI notes visualization
    const auto& midiSequence = clip.getMidiSequence();
    if (midiSequence.getNumEvents() > 0)
    {
        drawMidiNotes(g, bounds);
    }
}

void ClipComponent::drawWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    
    const auto& waveformData = clip.getWaveformData();
    if (waveformData.isEmpty()) return;
    
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
    g.fillPath(waveformPath);
}

void ClipComponent::drawMidiNotes(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    g.setColour(juce::Colours::white.withAlpha(0.8f));
    
    const auto& midiSequence = clip.getMidiSequence();
    if (midiSequence.getNumEvents() == 0) return;
    
    // Optimized MIDI note visualization using hash map
    std::unordered_map<int, float> noteOffTimes;
    
    // First pass: collect all note-off times
    for (int i = 0; i < midiSequence.getNumEvents(); ++i)
    {
        auto* event = midiSequence.getEventPointer(i);
        if (event->message.isNoteOff())
        {
            noteOffTimes[event->message.getNoteNumber()] = event->message.getTimeStamp();
        }
    }
    
    // Second pass: draw notes with pre-calculated note-off times
    float noteHeight = 3.0f;
    float totalDuration = clip.getDuration();
    
    for (int i = 0; i < midiSequence.getNumEvents(); ++i)
    {
        auto* event = midiSequence.getEventPointer(i);
        if (event->message.isNoteOn())
        {
            int noteNumber = event->message.getNoteNumber();
            float noteStart = event->message.getTimeStamp();
            
            // Use pre-calculated note-off time or default
            float noteEnd = noteOffTimes.count(noteNumber) > 0 ? 
                           noteOffTimes[noteNumber] : 
                           noteStart + 0.1f;
            
            float x = bounds.getX() + (noteStart / totalDuration) * bounds.getWidth();
            float width = ((noteEnd - noteStart) / totalDuration) * bounds.getWidth();
            float y = bounds.getY() + (1.0f - (noteNumber / 127.0f)) * bounds.getHeight();
            
            g.fillRect(x, y - noteHeight * 0.5f, width, noteHeight);
        }
    }
}

void ClipComponent::drawFadeHandles(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    // Fade in handle
    if (clip.getFadeIn() > 0.0)
    {
        g.setColour(juce::Colours::white.withAlpha(0.5f));
        float fadeWidth = bounds.getWidth() * (clip.getFadeIn() / clip.getDuration());
        juce::Path fadePath;
        fadePath.startNewSubPath(bounds.getX(), bounds.getY());
        fadePath.lineTo(bounds.getX() + fadeWidth, bounds.getY());
        fadePath.lineTo(bounds.getX(), bounds.getY() + bounds.getHeight());
        fadePath.closeSubPath();
        g.fillPath(fadePath);
    }
    
    // Fade out handle
    if (clip.getFadeOut() > 0.0)
    {
        g.setColour(juce::Colours::white.withAlpha(0.5f));
        float fadeWidth = bounds.getWidth() * (clip.getFadeOut() / clip.getDuration());
        juce::Path fadePath;
        fadePath.startNewSubPath(bounds.getRight() - fadeWidth, bounds.getY());
        fadePath.lineTo(bounds.getRight(), bounds.getY());
        fadePath.lineTo(bounds.getRight(), bounds.getY() + bounds.getHeight());
        fadePath.lineTo(bounds.getRight() - fadeWidth, bounds.getY() + bounds.getHeight());
        fadePath.closeSubPath();
        g.fillPath(fadePath);
    }
}

void ClipComponent::drawLoopRegion(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    if (!clip.isLoopRegion()) return;
    
    g.setColour(juce::Colours::yellow.withAlpha(0.3f));
    
    float loopStartX = bounds.getX() + (clip.getLoopStart() / clip.getDuration()) * bounds.getWidth();
    float loopEndX = bounds.getX() + (clip.getLoopEnd() / clip.getDuration()) * bounds.getWidth();
    
    g.fillRect(loopStartX, bounds.getY(), loopEndX - loopStartX, bounds.getHeight());
}

void ClipComponent::mouseDown(const juce::MouseEvent& event)
{
    if (event.mods.isLeftButtonDown())
    {
        if (event.mods.isShiftDown())
        {
            // Shift+click for multi-selection
            SelectionManager::getInstance()->toggleSelection(this);
        }
        else if (!isSelected())
        {
            // Single selection
            SelectionManager::getInstance()->selectOnly(this);
        }
        
        // Start drag operation
        if (isSelected())
        {
            dragOperation = std::make_unique<ClipDragOperation>(*this);
        }
    }
}

void ClipComponent::mouseDrag(const juce::MouseEvent& event)
{
    if (dragOperation && isSelected())
    {
        dragOperation->updatePosition(event.getDistanceFromDragStartX(), 
                                      event.getDistanceFromDragStartY());
    }
}

void ClipComponent::mouseUp(const juce::MouseEvent& event)
{
    if (dragOperation)
    {
        dragOperation->finalize();
        dragOperation.reset();
    }
}

void ClipComponent::mouseDoubleClick(const juce::MouseEvent& event)
{
    // Open clip editor
    if (clip.getType() == ClipType::Audio)
    {
        // Open audio editor
    }
    else
    {
        // Open MIDI editor
    }
}

} // namespace Track
