# Menacesta Track - Digital Audio Workstation

A modern digital audio workstation (DAW) built with C++ and JUCE framework, designed for music production, mixing, and mastering.

## Project Overview

Menacesta Track is a comprehensive audio production environment that provides:

- **Multi-track recording and playback**
- **Real-time audio processing with effects**
- **Professional mixing console with EQ, dynamics, and spatial processing**
- **Mastering tools with metering and analysis**
- **Plugin support (VST/AU)**
- **MIDI sequencing and control**

## Architecture

The project follows a modular architecture with clear separation of concerns:

```
src/
├── tracks/           # Audio/MIDI track management
├── mixing/          # Mixing console and channel strips
├── effects/         # Audio effects processing
├── mastering/       # Mastering tools and final processing
├── transport/       # Playback control and timeline
├── gui/            # User interface components
└── utils/          # Utility classes and helpers
```

## Key Components

### 1. Track System
- **Track.h/cpp**: Base class for all track types
- **AudioTrack**: Handles audio recording and playback
- **MIDITrack**: Manages MIDI data and virtual instruments
- **FolderTrack**: Groups tracks for organization

### 2. Mixing Console
- **Mixer.h/cpp**: Central mixing engine
- **ChannelStrip.h/cpp**: Individual channel processing
- **MasterBus.h/cpp**: Master output processing
- **ChannelGroup.h/cpp**: Bus grouping and submixes

### 3. Effects System
- **Effect.h/cpp**: Base effect class
- **ReverbEffect.h/cpp**: Reverb processing
- **EQEffect.h/cpp**: Parametric equalizer
- **DelayEffect.h/cpp**: Delay/echo effects
- **CompressorEffect.h/cpp**: Dynamics processing

### 4. Mastering Tools
- **MasteringChain.h/cpp**: Mastering signal chain
- **Limiter.h/cpp**: Final limiting
- **Metering.h/cpp**: Level and spectrum analysis

## Building the Project

### Prerequisites
- **JUCE Framework** (version 6.0 or later)
- **C++17 compatible compiler**
- **CMake** (version 3.15 or later)

### Build Instructions

#### Using CMake (Recommended)
```bash
# Clone the repository
git clone <repository-url>
cd Menacesta-Track

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release
```

#### Using Visual Studio (Windows)
1. Open the project in Visual Studio
2. Set the startup project to "Menacesta Track"
3. Build and run

#### Using Xcode (macOS)
1. Open the `.xcodeproj` file
2. Select the target scheme
3. Build and run

### Standalone Version
For a simple demonstration without JUCE dependencies:

```bash
# Build standalone version
mkdir build_standalone
cd build_standalone

# Windows (MSVC)
cl /std:c++17 /EHsc ..\src\simple_mixer_standalone.cpp /Fe:menacesta_track.exe

# Linux/macOS (GCC/Clang)
g++ -std=c++17 -o menacesta_track ../src/simple_mixer_standalone.cpp
```

## Usage

### Basic Operation
1. **Launch the application**
2. **Create new project** or open existing
3. **Add tracks** using the track panel
4. **Import audio** by drag-and-drop or file browser
5. **Apply effects** using the channel strip
6. **Mix and adjust** levels, pan, and parameters
7. **Export** final mixdown

### Keyboard Shortcuts
- **Space**: Play/Pause
- **R**: Record
- **Ctrl+Z**: Undo
- **Ctrl+S**: Save
- **Ctrl+N**: New project
- **Ctrl+O**: Open project

## Development

### Project Structure
```
Menacesta Track/
├── src/                    # Source code
│   ├── main.cpp           # Application entry point
│   ├── tracks/            # Track management
│   ├── mixing/            # Mixing console
│   ├── effects/           # Audio effects
│   ├── mastering/         # Mastering tools
│   └── gui/              # User interface
├── resources/             # Resources and assets
├── docs/                  # Documentation
└── tests/                # Unit tests
```

### Adding New Effects
1. Create new effect class inheriting from `Effect`
2. Implement required virtual methods
3. Register effect in effect factory
4. Add UI controls if needed

### Contributing
1. Fork the repository
2. Create feature branch
3. Make changes with tests
4. Submit pull request

## Technical Details

### Audio Processing
- **Sample rates**: 44.1kHz, 48kHz, 88.2kHz, 96kHz
- **Bit depths**: 16-bit, 24-bit, 32-bit float
- **Buffer sizes**: Configurable (64-2048 samples)
- **Plugin formats**: VST2, VST3, AU, AAX

### Performance
- **Multi-threaded** audio processing
- **SIMD optimized** DSP routines
- **Zero-latency** monitoring
- **Low CPU overhead**

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments
- **JUCE Framework** for cross-platform audio development
- **VST SDK** for plugin support
- **FFmpeg** for audio format support
- **RtAudio/RtMidi** for audio/MIDI I/O
