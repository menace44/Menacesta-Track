#include "AdvancedPitchShiftProcessor.h"
#include <cmath>
#include <algorithm>

AdvancedPitchShiftProcessor::AdvancedPitchShiftProcessor()
{
    initializeBuffers();
}

AdvancedPitchShiftProcessor::~AdvancedPitchShiftProcessor()
{
}

void AdvancedPitchShiftProcessor::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    blockSize = static_cast<int>(spec.maximumBlockSize);
    
    // Initialize FFT with appropriate size
    fftSize = 2048;
    while (fftSize < blockSize * 4) fftSize *= 2;
    
    hopSize = fftSize / 4;
    
    fft = std::make_unique<juce::dsp::FFT>(static_cast<int>(std::log2(fftSize)));
    window = std::make_unique<juce::dsp::WindowingFunction<float>>(fftSize, juce::dsp::WindowingFunction<float>::hann);
    
    initializeBuffers();
}

void AdvancedPitchShiftProcessor::reset()
{
    std::fill(fftBuffer.begin(), fftBuffer.end(), std::complex<float>(0.0f, 0.0f));
    std::fill(analysisBuffer.begin(), analysisBuffer.end(), 0.0f);
    std::fill(synthesisBuffer.begin(), synthesisBuffer.end(), 0.0f);
    std::fill(inputOverlapBuffer.begin(), inputOverlapBuffer.end(), 0.0f);
    std::fill(outputOverlapBuffer.begin(), outputOverlapBuffer.end(), 0.0f);
    overlapPosition = 0;
}

void AdvancedPitchShiftProcessor::setPitchShift(double semitones)
{
    pitchShift = semitones;
}

void AdvancedPitchShiftProcessor::setFormantPreservation(bool preserve)
{
    preserveFormants = preserve;
}

void AdvancedPitchShiftProcessor::setQualityMode(int mode)
{
    qualityMode = static_cast<QualityMode>(juce::jlimit(0, 2, mode));
}

void AdvancedPitchShiftProcessor::process(const juce::AudioBuffer<float>& input, juce::AudioBuffer<float>& output)
{
    const int numChannels = input.getNumChannels();
    const int numSamples = input.getNumSamples();
    
    output.setSize(numChannels, numSamples, false, false, true);
    
    for (int ch = 0; ch < numChannels; ++ch)
    {
        const float* inputChannel = input.getReadPointer(ch);
        float* outputChannel = output.getWritePointer(ch);
        
        switch (qualityMode)
        {
            case Fast:
                processFast(inputChannel, outputChannel, numSamples);
                break;
            case Balanced:
                processBalanced(inputChannel, outputChannel, numSamples);
                break;
            case High:
                processHigh(inputChannel, outputChannel, numSamples);
                break;
        }
    }
}

void AdvancedPitchShiftProcessor::initializeBuffers()
{
    fftBuffer.resize(fftSize);
    analysisBuffer.resize(fftSize);
    synthesisBuffer.resize(fftSize);
    magnitude.resize(fftSize / 2 + 1);
    phase.resize(fftSize / 2 + 1);
    
    inputOverlapBuffer.resize(fftSize);
    outputOverlapBuffer.resize(fftSize);
    
    phaseLocker.prevPhase.resize(fftSize / 2 + 1);
    phaseLocker.prevPhaseAdv.resize(fftSize / 2 + 1);
    phaseLocker.phaseAccum.resize(fftSize / 2 + 1);
}

void AdvancedPitchShiftProcessor::processFast(const float* input, float* output, int numSamples)
{
    // Simple time-domain pitch shifting with linear interpolation
    const double pitchRatio = std::pow(2.0, pitchShift / 12.0);
    
    for (int i = 0; i < numSamples; ++i)
    {
        double readPos = i / pitchRatio;
        int readIndex = static_cast<int>(readPos);
        double fraction = readPos - readIndex;
        
        if (readIndex < numSamples - 1)
        {
            output[i] = static_cast<float>(
                input[readIndex] * (1.0 - fraction) + 
                input[readIndex + 1] * fraction
            );
        }
        else
        {
            output[i] = input[numSamples - 1];
        }
    }
}

void AdvancedPitchShiftProcessor::processBalanced(const float* input, float* output, int numSamples)
{
    // Phase vocoder with basic formant preservation
    const double pitchRatio = std::pow(2.0, pitchShift / 12.0);
    
    // Overlap-add processing
    for (int i = 0; i < numSamples; i += hopSize)
    {
        // Copy input to analysis buffer
        std::fill(analysisBuffer.begin(), analysisBuffer.end(), 0.0f);
        int copySize = std::min(fftSize, numSamples - i);
        for (int j = 0; j < copySize; ++j)
        {
            if (i + j < numSamples)
                analysisBuffer[j] = input[i + j];
        }
        
        // Apply window
        window->multiplyWithWindowingTable(analysisBuffer.data(), fftSize);
        
        // Perform FFT
        std::fill(fftBuffer.begin(), fftBuffer.end(), std::complex<float>(0.0f, 0.0f));
        for (int j = 0; j < fftSize; ++j)
            fftBuffer[j] = std::complex<float>(analysisBuffer[j], 0.0f);
        
        fft->performFrequencyOnlyForwardTransform(reinterpret_cast<float*>(fftBuffer.data()));
        
        // Convert to magnitude/phase
        for (int j = 0; j <= fftSize / 2; ++j)
        {
            magnitude[j] = std::abs(fftBuffer[j]);
            phase[j] = std::arg(fftBuffer[j]);
        }
        
        // Formant preservation
        if (preserveFormants)
        {
            extractFormants(magnitude.data(), fftSize / 2 + 1);
            applyFormantPreservation(magnitude.data(), fftSize / 2 + 1);
        }
        
        // Pitch shift in frequency domain
        std::vector<float> shiftedMagnitude(fftSize / 2 + 1);
        std::vector<float> shiftedPhase(fftSize / 2 + 1);
        
        for (int j = 0; j <= fftSize / 2; ++j)
        {
            int shiftedBin = static_cast<int>(j * pitchRatio + 0.5);
            if (shiftedBin <= fftSize / 2)
            {
                shiftedMagnitude[shiftedBin] = magnitude[j];
                shiftedPhase[shiftedBin] = phase[j] * pitchRatio;
            }
        }
        
        // Convert back to complex
        for (int j = 0; j <= fftSize / 2; ++j)
        {
            fftBuffer[j] = std::polar(shiftedMagnitude[j], shiftedPhase[j]);
            if (j > 0 && j < fftSize / 2)
                fftBuffer[fftSize - j] = std::conj(fftBuffer[j]);
        }
        
        // Perform inverse FFT
        fft->performFrequencyOnlyInverseTransform(reinterpret_cast<float*>(fftBuffer.data()));
        
        // Apply window and overlap-add
        for (int j = 0; j < fftSize; ++j)
        {
            synthesisBuffer[j] = fftBuffer[j].real() * window->getWindowingTable()[j];
            if (i + j < numSamples)
                output[i + j] += synthesisBuffer[j];
        }
    }
}

void AdvancedPitchShiftProcessor::processHigh(const float* input, float* output, int numSamples)
{
    // High-quality processing with all features enabled
    const double pitchRatio = std::pow(2.0, pitchShift / 12.0);
    
    // Overlap-add processing with higher overlap
    const int highQualityHopSize = hopSize / 2;
    
    for (int i = 0; i < numSamples; i += highQualityHopSize)
    {
        // Copy input to analysis buffer
        std::fill(analysisBuffer.begin(), analysisBuffer.end(), 0.0f);
        int copySize = std::min(fftSize, numSamples - i);
        for (int j = 0; j < copySize; ++j)
        {
            if (i + j < numSamples)
                analysisBuffer[j] = input[i + j];
        }
        
        // Apply window
        window->multiplyWithWindowingTable(analysisBuffer.data(), fftSize);
        
        // Perform FFT
        std::fill(fftBuffer.begin(), fftBuffer.end(), std::complex<float>(0.0f, 0.0f));
        for (int j = 0; j < fftSize; ++j)
            fftBuffer[j] = std::complex<float>(analysisBuffer[j], 0.0f);
        
        fft->performFrequencyOnlyForwardTransform(reinterpret_cast<float*>(fftBuffer.data()));
        
        // Convert to magnitude/phase
        for (int j = 0; j <= fftSize / 2; ++j)
        {
            magnitude[j] = std::abs(fftBuffer[j]);
            phase[j] = std::arg(fftBuffer[j]);
        }
        
        // Formant preservation
        if (preserveFormants)
        {
            extractFormants(magnitude.data(), fftSize / 2 + 1);
            applyFormantPreservation(magnitude.data(), fftSize / 2 + 1);
        }
        
        // Phase locking
        phaseLocker.lockPhases(fftBuffer.data(), fftSize, pitchRatio);
        
        // Pitch shift with anti-aliasing
        std::vector<float> shiftedMagnitude(fftSize / 2 + 1);
        std::vector<float> shiftedPhase(fftSize / 2 + 1);
        
        for (int j = 0; j <= fftSize / 2; ++j)
        {
            double shiftedBin = j * pitchRatio;
            int binIndex = static_cast<int>(shiftedBin);
            double fraction = shiftedBin - binIndex;
            
            if (binIndex <= fftSize / 2)
            {
                // Linear interpolation for fractional bins
                float mag = magnitude[j];
                float ph = phase[j] * pitchRatio;
                
                if (binIndex < fftSize / 2)
                {
                    shiftedMagnitude[binIndex] += mag * (1.0f - fraction);
                    shiftedMagnitude[binIndex + 1] += mag * fraction;
                }
                else
                {
                    shiftedMagnitude[binIndex] += mag;
                }
                
                shiftedPhase[binIndex] = ph;
            }
        }
        
        // Apply anti-aliasing
        antiAliaser.applyAntiAliasing(fftBuffer.data(), fftSize, pitchRatio);
        
        // Convert back to complex
        for (int j = 0; j <= fftSize / 2; ++j)
        {
            fftBuffer[j] = std::polar(shiftedMagnitude[j], shiftedPhase[j]);
            if (j > 0 && j < fftSize / 2)
                fftBuffer[fftSize - j] = std::conj(fftBuffer[j]);
        }
        
        // Perform inverse FFT
        fft->performFrequencyOnlyInverseTransform(reinterpret_cast<float*>(fftBuffer.data()));
        
        // Apply window and overlap-add
        for (int j = 0; j < fftSize; ++j)
        {
            synthesisBuffer[j] = fftBuffer[j].real() * window->getWindowingTable()[j];
            if (i + j < numSamples)
                output[i + j] += synthesisBuffer[j];
        }
    }
}

void AdvancedPitchShiftProcessor::extractFormants(const float* magnitude, int size)
{
    // Simple formant extraction using spectral envelope
    formantFilter.envelope.resize(size);
    formantFilter.spectralEnvelope.resize(size);
    
    // Smooth the magnitude spectrum to get envelope
    const int smoothingWidth = 5;
    for (int i = 0; i < size; ++i)
    {
        float sum = 0.0f;
        int count = 0;
        
        for (int j = std::max(0, i - smoothingWidth); j <= std::min(size - 1, i + smoothingWidth); ++j)
        {
            sum += magnitude[j];
            ++count;
        }
        
        formantFilter.envelope[i] = sum / count;
    }
}

void AdvancedPitchShiftProcessor::applyFormantPreservation(float* magnitude, int size)
{
    if (!preserveFormants) return;
    
    // Apply formant envelope to preserve spectral characteristics
    for (int i = 0; i < size; ++i)
    {
        if (formantFilter.envelope[i] > 0.0f)
            magnitude[i] *= formantFilter.envelope[i];
    }
}

double AdvancedPitchShiftProcessor::getLatency() const
{
    return static_cast<double>(fftSize) / sampleRate;
}
