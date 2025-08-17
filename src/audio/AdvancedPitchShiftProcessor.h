#pragma once

#include <JuceHeader.h>
#include <vector>
#include <complex>

class AdvancedPitchShiftProcessor
{
public:
    AdvancedPitchShiftProcessor();
    ~AdvancedPitchShiftProcessor();
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    void setPitchShift(double semitones);
    void setFormantPreservation(bool preserve);
    void setQualityMode(int mode); // 0=fast, 1=balanced, 2=high
    
    void process(const juce::AudioBuffer<float>& input, juce::AudioBuffer<float>& output);
    
    double getLatency() const;
    
private:
    // Quality modes
    enum QualityMode
    {
        Fast = 0,
        Balanced = 1,
        High = 2
    };
    
    // Core parameters
    double pitchShift = 0.0;
    bool preserveFormants = true;
    QualityMode qualityMode = Balanced;
    
    // Audio specs
    double sampleRate = 44100.0;
    int blockSize = 512;
    
    // Phase vocoder parameters
    int fftSize = 2048;
    int hopSize = 512;
    
    // Formant preservation
    struct FormantFilter
    {
        std::vector<float> envelope;
        std::vector<float> spectralEnvelope;
        void updateEnvelope(const float* magnitude, int size, double sampleRate);
        void applyEnvelope(float* magnitude, int size);
    };
    
    // Transient detection
    struct TransientDetector
    {
        float threshold = 0.1f;
        float prevEnergy = 0.0f;
        bool detectTransient(const float* buffer, int size);
    };
    
    // Phase locking
    struct PhaseLocker
    {
        std::vector<float> prevPhase;
        std::vector<float> prevPhaseAdv;
        std::vector<float> phaseAccum;
        
        void lockPhases(std::complex<float>* fftBuffer, int size, double pitchShift);
    };
    
    // Anti-aliasing
    struct AntiAliaser
    {
        void applyAntiAliasing(std::complex<float>* fftBuffer, int size, double pitchShift);
    };
    
    // Processing buffers
    std::unique_ptr<juce::dsp::FFT> fft;
    std::unique_ptr<juce::dsp::WindowingFunction<float>> window;
    
    std::vector<std::complex<float>> fftBuffer;
    std::vector<float> analysisBuffer;
    std::vector<float> synthesisBuffer;
    std::vector<float> magnitude;
    std::vector<float> phase;
    
    // Processing components
    FormantFilter formantFilter;
    TransientDetector transientDetector;
    PhaseLocker phaseLocker;
    AntiAliaser antiAliaser;
    
    // Overlap-add buffers
    std::vector<float> inputOverlapBuffer;
    std::vector<float> outputOverlapBuffer;
    int overlapPosition = 0;
    
    // Helper methods
    void initializeBuffers();
    void performAdvancedPitchShift(const float* input, float* output, int numSamples);
    void analyzeFrame(const float* frame, int frameSize);
    void processFrame(std::complex<float>* frame, int frameSize, double pitchShift);
    void synthesizeFrame(float* frame, int frameSize);
    
    // Formant processing
    void extractFormants(const float* magnitude, int size);
    void applyFormantPreservation(float* magnitude, int size);
    
    // Quality-specific processing
    void processFast(const float* input, float* output, int numSamples);
    void processBalanced(const float* input, float* output, int numSamples);
    void processHigh(const float* input, float* output, int numSamples);
};
