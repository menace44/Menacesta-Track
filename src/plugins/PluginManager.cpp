#include "PluginManager.h"

PluginManager::PluginManager()
{
    formatManager.addDefaultFormats();
}

PluginManager::~PluginManager()
{
    clearPluginList();
}

void PluginManager::createPluginListIfNeeded()
{
    if (knownPluginList.getNumTypes() == 0)
    {
        scanForPlugins();
    }
}

juce::Array<PluginManager::PluginDescription> PluginManager::getAvailablePlugins()
{
    createPluginListIfNeeded();
    
    juce::Array<PluginDescription> plugins;
    
    for (int i = 0; i < knownPluginList.getNumTypes(); ++i)
    {
        auto desc = knownPluginList.getType(i);
        
        PluginDescription pluginDesc;
        pluginDesc.name = desc.name;
        pluginDesc.manufacturer = desc.manufacturerName;
        pluginDesc.pluginFormat = desc.pluginFormatName;
        pluginDesc.identifier = desc.createIdentifierString();
        pluginDesc.isInstrument = desc.isInstrument;
        
        plugins.add(pluginDesc);
    }
    
    return plugins;
}

juce::AudioPluginInstance* PluginManager::loadPlugin(const juce::String& identifier)
{
    unloadPlugin(currentPlugin);
    
    juce::PluginDescription desc;
    for (int i = 0; i < knownPluginList.getNumTypes(); ++i)
    {
        auto pluginDesc = knownPluginList.getType(i);
        if (pluginDesc.createIdentifierString() == identifier)
        {
            desc = pluginDesc;
            break;
        }
    }
    
    if (desc.name.isNotEmpty())
    {
        juce::String errorMessage;
        currentPlugin = formatManager.createPluginInstance(desc, 44100.0, 512, errorMessage);
        
        if (currentPlugin)
        {
            currentPlugin->prepareToPlay(44100.0, 512);
        }
    }
    
    return currentPlugin;
}

void PluginManager::unloadPlugin(juce::AudioPluginInstance* plugin)
{
    if (plugin && plugin == currentPlugin)
    {
        currentPlugin->releaseResources();
        currentPlugin = nullptr;
    }
}

void PluginManager::scanForPlugins()
{
    juce::PluginDirectoryScanner scanner(knownPluginList, formatManager,
                                       juce::FileSearchPath(), true, juce::File());
    scanner.scanNextFile(true);
}

void PluginManager::clearPluginList()
{
    unloadPlugin(currentPlugin);
    knownPluginList.clear();
}
