#include "Clip.h"
#include <JuceHeader.h>

namespace Track {

Clip::Clip(const juce::String& name, ClipType type, double startTime, double duration)
    : name(name)
    , type(type)
    , startTime(startTime)
    , duration(duration)
    , colour(juce::Colours::transparentBlack)
    , fadeIn(0.0)
    , fadeOut(0.0)
    , loopRegion(false)
    , loopStart(0.0)
    , loopEnd(duration)
{
    // Set default colors based on clip type
    switch (type) {
        case ClipType::Audio:
            colour = juce::Colour(0xFF2196F3); // Blue
            break;
        case ClipType::MIDI:
            colour = juce::Colour(0xFF9C27B0); // Purple
            break;
    }
}

} // namespace Track
