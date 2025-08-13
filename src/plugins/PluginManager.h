#pragma once
#include <JuceHeader.h>

class PluginManager
{
public:
    PluginManager();
    ~PluginManager();

    struct PluginDescription
    {
        juce::String name;
        juce::String manufacturer;
        juce::String pluginFormat;
        juce::String identifier;
        bool isInstrument;
    };

    juce::Array<PluginDescription> getAvailablePlugins();
    juce::AudioPluginInstance* loadPlugin(const juce::String& identifier);
    void unloadPlugin(juce::AudioPluginInstance* plugin);

    void scanForPlugins();
    void clearPluginList();

private:
    juce::AudioPluginFormatManager formatManager;
    juce::KnownPluginList knownPluginList;
    juce::AudioPluginInstance* currentPlugin = nullptr;

    void createPluginListIfNeeded();
};
