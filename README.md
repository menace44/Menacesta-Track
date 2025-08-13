# Cross-Platform JUCE DAW

A modern, cross-platform Digital Audio Workstation (DAW) built with JUCE framework, supporting Windows, macOS, and Linux.

## Features

- **Cross-Platform**: Native support for Windows, macOS, and Linux
- **Audio Engine**: High-performance audio processing with low-latency
- **MIDI Support**: Full MIDI input/output handling
- **Plugin Support**: VST3/AU plugin hosting
- **Recording**: Multi-track audio recording
- **Transport Control**: Play, stop, record, and timeline navigation
- **Track Management**: Multiple audio/MIDI tracks with volume/pan controls
- **Real-time Effects**: Built-in audio effects and plugin support

## Project Structure

```
Cross-Platform-JUCE-DAW/
├── src/
│   ├── App.cpp/.h              # Main application class
│   ├── main.cpp                # Entry point
│   ├── audio/
│   │   ├── AudioEngine.cpp/.h  # Core audio processing engine
│   ├── transport/
│   │   ├── Transport.cpp/.h    # Transport control (play/stop/record)
│   ├── tracks/
│   │   ├── Track.cpp/.h        # Track management
│   ├── recording/
│   │   ├── Recorder.cpp/.h     # Audio recording functionality
│   ├── midi/
│   │   ├── MidiHandler.cpp/.h  # MIDI processing
│   ├── plugins/
│   │   ├── PluginManager.cpp/.h # VST3/AU plugin hosting
│   ├── gui/
│   │   ├── MainComponent.cpp/.h # Main GUI component
│   │   ├── TransportControls.cpp/.h # Transport control GUI
│   │   ├── TimelineComponent.cpp/.h # Timeline display
│   │   ├── TrackListComponent.cpp/.h # Track list GUI
│   │   ├── TrackControlPanel.cpp/.h # Individual track controls
│   ├── tests/
│   │   ├── AudioEngineTest.cpp  # Audio engine unit tests
│   │   ├── TestRunner.cpp       # Test runner
│   └── examples/
│       └── AudioEngineDemo.cpp  # Usage examples
├── cmake/
│   └── JUCE.cmake              # JUCE configuration
├── resources/
├── JUCE/                       # JUCE framework
├── build.sh                    # Unix build script
├── build.bat                   # Windows build script
├── CMakeLists.txt              # CMake configuration
└── CMakePresets.json          # Build presets
```

## Building

### Prerequisites
- CMake 3.15 or higher
- C++17 compatible compiler
- JUCE 6.0 or higher

### Build Instructions

#### Windows
```bash
build.bat
```

#### macOS/Linux
```bash
chmod +x build.sh
./build.sh
```

#### Manual Build
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

## Usage

1. **Launch**: Run the built executable
2. **Audio Setup**: Configure audio device settings
3. **Create Tracks**: Add audio or MIDI tracks
4. **Load Plugins**: Add VST3/AU plugins to tracks
5. **Record**: Arm tracks for recording and hit record
6. **Playback**: Use transport controls for playback

## Development

### Adding New Features
1. Follow the existing code structure
2. Add unit tests for new functionality
3. Update documentation as needed

### Testing
```bash
# Run unit tests
./build/test_runner
```

## Contributing
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests
5. Submit a pull request

## License
MIT License - see LICENSE file for details
