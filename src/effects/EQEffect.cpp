#include "EQEffect.h"

EQEffect::EQEffect()
    : Effect("EQ", Type::EQ)
{
    // Initialize bands with default values
    bands[0].frequency = 100.0f;   // Low
    bands[1].frequency = 1000.0f;  // Mid
    bands[2].frequency = 8000.0f;  // High
    
    for (int i = 0; i < NUM_BANDS; ++i)
    {
        bands[i].gain = 0.0f;
        bands[i].q = 1.0f;
        bands[i].enabled = true;
    }
}

void EQEffect::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;
    
    lowBand.prepare(spec);
    midBand.prepare(spec);
    highBand.prepare(spec);
    
    updateFilter(0);
    updateFilter(1);
    updateFilter(2);
}

void EQEffect::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (!enabled) return;
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    
    if (bands[0].enabled)
        lowBand.process(context);
    if (bands[1].enabled)
        midBand.process(context);
    if (bands[2].enabled)
        highBand.process(context);
}

void EQEffect::reset()
{
    lowBand.reset();
    midBand.reset();
    highBand.reset();
}

void EQEffect::setBandGain(int bandIndex, float gain)
{
    if (bandIndex >= 0 && bandIndex < NUM_BANDS)
    {
        bands[bandIndex].gain = juce::jlimit(-24.0f, 24.0f, gain);
        updateFilter(bandIndex);
    }
}

void EQEffect::setBandFrequency(int bandIndex, float frequency)
{
    if (bandIndex >= 0 && bandIndex < NUM_BANDS)
    {
        bands[bandIndex].frequency = juce::jlimit(20.0f, 20000.0f, frequency);
        updateFilter(bandIndex);
    }
}

void EQEffect::setBandQ(int bandIndex, float q)
{
    if (bandIndex >= 0 && bandIndex < NUM_BANDS)
    {
        bands[bandIndex].q = juce::jlimit(0.1f, 10.0f, q);
        updateFilter(bandIndex);
    }
}

void EQEffect::setBandEnabled(int bandIndex, bool enabled)
{
    if (bandIndex >= 0 && bandIndex < NUM_BANDS)
    {
        bands[bandIndex].enabled = enabled;
    }
}

float EQEffect::getBandGain(int bandIndex) const
{
    if (bandIndex >= 0 && bandIndex < NUM_BANDS)
        return bands[bandIndex].gain;
    return 0.0f;
}

float EQEffect::getBandFrequency(int bandIndex) const
{
    if (bandIndex >= 0 && bandIndex < NUM_BANDS)
        return bands[bandIndex].frequency;
    return 1000.0f;
}

float EQEffect::getBandQ(int bandIndex) const
{
    if (bandIndex >= 0 && bandIndex < NUM_BANDS)
        return bands[bandIndex].q;
    return 1.0f;
}

bool EQEffect::isBandEnabled(int bandIndex) const
{
    if (bandIndex >= 0 && bandIndex < NUM_BANDS)
        return bands[bandIndex].enabled;
    return false;
}

void EQEffect::updateFilter(int bandIndex)
{
    if (bandIndex < 0 || bandIndex >= NUM_BANDS) return;
    
    const auto& band = bands[bandIndex];
    
    switch (bandIndex)
    {
        case 0: // Low shelf
        {
            *lowBand.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
                currentSampleRate, band.frequency, band.q, juce::Decibels::decibelsToGain(band.gain));
            break;
        }
        case 1: // Peak
        {
            *midBand.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
                currentSampleRate, band.frequency, band.q, juce::Decibels::decibelsToGain(band.gain));
            break;
        }
        case 2: // High shelf
        {
            *highBand.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
                currentSampleRate, band.frequency, band.q, juce::Decibels::decibelsToGain(band.gain));
            break;
        }
    }
}
