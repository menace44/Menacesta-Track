#include <JuceHeader.h>
#include "effects/Effect.h"
#include "effects/ReverbEffect.h"
#include "effects/EQEffect.h"
#include "effects/DelayEffect.h"

class MainComponent : public juce::Component,
                      public juce::Timer
{
public:
    MainComponent()
    {
        setSize(600, 400);
        
        // Initialize audio
        setAudioChannels(0, 2);
        
        // Create effect chain
        effectChain.prepareToPlay(44100.0, 512);
        
        // Add some effects
        auto reverb = std::make_unique<ReverbEffect>();
        reverb->setRoomSize(0.5f);
        reverb->setDamping(0.3f);
        reverb->setWetLevel(0.2f);
        effectChain.addEffect(std::move(reverb));
        
        auto eq = std::make_unique<EQEffect>();
        eq->setBandGain(0, 3.0f);  // Boost lows
        eq->setBandGain(1, -2.0f); // Cut mids
        eq->setBandGain(2, 1.0f);  // Boost highs
        effectChain.addEffect(std::move(eq));
        
        auto delay = std::make_unique<DelayEffect>();
        delay->setDelayTime(300.0f);
        delay->setFeedback(0.4f);
        delay->setMix(0.3f);
        effectChain.addEffect(std::move(delay));
        
        startTimer(1000 / 60); // 60 FPS
    }
    
    ~MainComponent() override
    {
        shutdownAudio();
    }
    
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey);
        
        g.setColour(juce::Colours::white);
        g.setFont(20.0f);
        g.drawText("Audio Effects Suite", getLocalBounds(), juce::Justification::centredTop);
        
        g.setFont(14.0f);
        g.drawText("Effects loaded:", 20, 50, 200, 30, juce::Justification::left);
        
        for (int i = 0; i < effectChain.getNumEffects(); ++i)
        {
            if (auto* effect = effectChain.getEffect(i))
            {
                juce::String effectText = juce::String(i + 1) + ". " + effect->getName();
                g.drawText(effectText, 20, 80 + i * 30, 200, 30, juce::Justification::left);
            }
        }
    }
    
    void resized() override
    {
        // Handle component resizing
    }
    
    void timerCallback() override
    {
        repaint();
    }
    
private:
    juce::AudioDeviceManager deviceManager;
    EffectChain effectChain;
    
    void setAudioChannels(int inputChannels, int outputChannels)
    {
        juce::AudioDeviceManager::AudioDeviceSetup setup;
        deviceManager.initialise(inputChannels, outputChannels, nullptr, true);
    }
    
    void shutdownAudio()
    {
        deviceManager.closeAudioDevice();
    }
};

class AudioEffectsApplication : public juce::JUCEApplication
{
public:
    AudioEffectsApplication() {}
    
    const juce::String getApplicationName() override { return "Audio Effects Suite"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    
    void initialise(const juce::String& commandLine) override
    {
        mainWindow = std::make_unique<MainWindow>(getApplicationName());
    }
    
    void shutdown() override
    {
        mainWindow = nullptr;
    }
    
private:
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name, juce::Desktop::getInstance().getDefaultLookAndFeel()
                             .findColour(juce::ResizableWindow::backgroundColourId),
                             DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }
        
        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };
    
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(AudioEffectsApplication)
