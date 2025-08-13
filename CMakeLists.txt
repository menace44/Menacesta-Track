cmake_minimum_required(VERSION 3.15)
project(CrossPlatformJUCEDAW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find JUCE package
find_package(JUCE CONFIG REQUIRED)

# Create the main executable
juce_add_plugin(CrossPlatformJUCEDAW
    VERSION 1.0.0
    COMPANY_NAME "CrossPlatformJUCEDAW"
    PLUGIN_MANUFACTURER_CODE Cpjd
    PLUGIN_CODE Cpjd
    FORMATS Standalone VST3 AU
    PRODUCT_NAME "Cross-Platform JUCE DAW")

# Source files
target_sources(CrossPlatformJUCEDAW PRIVATE
    src/main.cpp
    src/App.cpp
    src/transport/Transport.cpp
    src/audio/AudioEngine.cpp
    src/midi/MidiHandler.cpp
    src/plugins/PluginManager.cpp
    src/recording/Recorder.cpp
    src/tracks/Track.cpp
    src/gui/MainComponent.cpp
    src/gui/TransportControls.cpp
    src/gui/TimelineComponent.cpp
    src/gui/TrackListComponent.cpp
    src/gui/TrackControlPanel.cpp
    src/tests/AudioEngineTest.cpp
    src/tests/TestRunner.cpp
    src/examples/AudioEngineDemo.cpp
)

# Header files
target_sources(CrossPlatformJUCEDAW PRIVATE
    src/App.h
    src/transport/Transport.h
    src/audio/AudioEngine.h
    src/midi/MidiHandler.h
    src/plugins/PluginManager.h
    src/recording/Recorder.h
    src/tracks/Track.h
    src/gui/MainComponent.h
    src/gui/TransportControls.h
    src/gui/TimelineComponent.h
    src/gui/TrackListComponent.h
    src/gui/TrackControlPanel.h
)

# Link JUCE modules
target_link_libraries(CrossPlatformJUCEDAW PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# Set target properties
target_compile_definitions(CrossPlatformJUCEDAW
    PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# Enable all plugin formats
juce_generate_juce_header(CrossPlatformJUCEDAW)
