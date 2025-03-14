# DeckGUI — Demo

This is a simple audio control software demo program, supporting multi-track audio file playback, audio mixing, volume adjustment, speed up and slow down, dynamic track addition, and echo effects.  
Implemented based on the well-known open-source framework [JUCE](https://github.com/juce-framework/JUCE), installation and deployment are very easy, supporting multiple platforms including macOS, Windows, Android, and iOS.

## Core Features

- Supports loading and playing multiple audio tracks
- Supports dynamically adding and removing tracks
- Supports audio control features, including volume adjustment, speed control, and seek
- Supports audio effects, currently only ECHO, with plans to expand to more effects in the future

## Installation and Running

1. Install dependencies and JUCE: [JUCE GitHub Repository](https://github.com/juce-framework/JUCE)
2. Open Projucer in JUCE, click the "Open Existing Project" button, select `DeckGUI.jucer` from this project, then choose to export:
   - If your development environment is Visual Studio, choose Visual Studio
   - If your development environment is Xcode, choose Xcode (e.g., my environment is Xcode)
3. Run the project in Xcode/Visual Studio, and it should work as expected.

## Running Effect

![image](https://github.com/user-attachments/assets/14f197eb-2786-40af-9946-7566d711ea1c)

## Contributions

We welcome contributions in any form!  
If you find bugs or wish to add new features, you can submit an Issue or fork the project and submit a Pull Request (PR).

### Contribution Process

1. Fork this repository
2. Create a new branch:
   ```bash
   git checkout -b feature-new-effect
