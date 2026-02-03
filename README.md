# 🧺 Washing Machine Simulator

A comprehensive 3D washing machine simulator built with C++ and Qt3D, featuring realistic washing cycles, interactive 3D visualization, and custom data structures.

![Qt](https://img.shields.io/badge/Qt-5%20%7C%206-41CD52?logo=qt&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-064F8C?logo=cmake)
![License](https://img.shields.io/badge/license-MIT-blue)

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Technical Architecture](#technical-architecture)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Key Components](#key-components)
- [Washing Programs](#washing-programs)
- [Screenshots](#screenshots)
- [Development](#development)
- [Contributing](#contributing)
- [License](#license)

## 🎯 Overview

This project simulates a modern Samsung 7kg washing machine with a fully interactive 3D interface. Users can select different washing programs, adjust settings, and watch the washing cycle progress in real-time with visual feedback through 3D progress bars and animated sliders.

The application demonstrates advanced C++ programming concepts including:
- Custom hash table implementation
- Qt3D scene management
- Real-time 3D rendering and interaction
- Complex state machine for washing cycles
- Time compression simulation (60:1 ratio)

## ✨ Features

### Core Functionality
- **8 Washing Programs**: Cottons, Cottons Eco, Synthetics, Wool/Silk, Anti-Allergy, Non-Stop, Anti-Crease, and Refresh
- **Customizable Settings**: Temperature, spin speed, rinse cycles, eco mode, steam treatment, and drying options
- **Real-time 3D Visualization**: Interactive 3D model of the washing machine with rotating capabilities
- **Progress Tracking**: Visual progress bars for water tank, lint trap, and detergent tray
- **Maintenance System**: Track and clean water tank, lint trap, and detergent tray when needed
- **Emergency Stop**: Ability to halt any running program at any time

### Technical Features
- **Custom Hash Table**: Template-based hash table with linear probing and automatic rehashing
- **3D Text Labels**: Extruded 3D text for interactive UI elements
- **Animated Progress Bars**: Custom 3D progress bar entities that rotate with the scene
- **Time Compression**: Real-world washing times compressed 60:1 for demonstration purposes
- **Dynamic Cycle Management**: State machine handling complex washing cycle transitions

## 🏗️ Technical Architecture

### Technologies Used
- **Language**: C++17
- **UI Framework**: Qt5/Qt6 (Widgets, 3DCore, 3DExtras, 3DRender, 3DInput, 3DLogic)
- **Build System**: CMake 3.16+
- **3D Graphics**: Qt3D with custom scene modifications
- **Design Pattern**: Model-View pattern with custom scene modifiers

### Custom Data Structures
The project implements a custom hash table (`MyHashtable`) with the following features:
- Template-based for type flexibility
- Linear probing collision resolution
- Automatic rehashing at 70% load factor
- O(1) average-case insertion and lookup

## 📦 Prerequisites

### Required Dependencies
- **CMake** 3.16 or higher
- **Qt** 5.x or 6.x with the following modules:
  - Qt Widgets
  - Qt 3D Core
  - Qt 3D Extras
  - Qt 3D Render
  - Qt 3D Input
  - Qt 3D Logic
- **C++ Compiler** supporting C++17 standard
  - GCC 7+ / Clang 5+ / MSVC 2017+

### Optional
- Qt Creator IDE for easier development

## 🚀 Installation

### 1. Clone the Repository
```bash
git clone https://github.com/yourusername/washingMachineSimulator.git
cd washingMachineSimulator
```

### 2. Prepare Assets Directory
Ensure the `assets` folder is in the same directory as your executable or set up the path correctly:
```bash
mkdir -p build/assets
cp -r assets/* build/assets/
```

### 3. Build with CMake

#### Linux/macOS
```bash
mkdir build && cd build
cmake ..
make
```

#### Windows (MinGW)
```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

#### Windows (Visual Studio)
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

### 4. Run the Application
```bash
./managementsystem
```

## 🎮 Usage

### Getting Started
1. **Insert Powder**: Click the "Insert Powder" button to add detergent
2. **Select Program**: Choose from 8 available washing programs
3. **Configure Settings**: Adjust temperature, spin speed, and other options
4. **Start Program**: Click start to begin the washing cycle
5. **Monitor Progress**: Watch the 3D visualization and progress bars update in real-time
6. **Maintenance**: Clean the water tank, lint trap, and detergent tray when needed

### Interactive 3D View
- **Rotate**: Click and drag with left mouse button
- **Zoom**: Use mouse wheel to zoom in/out
- **Labels**: 3D text labels indicate current operation step

### Emergency Stop
Press the "Emergency Stop" button at any time to halt the current program and return to the configuration page.

## 📁 Project Structure

```
washingMachineSimulator/
├── assets/                      # 3D models, textures, and animations
│   ├── scene.gltf              # 3D washing machine model
│   ├── scene.bin               # Binary data for 3D model
│   ├── samsung-7kg-ww70t4020cx_baseColor.png
│   └── Spinner@*.gif           # Loading animations
├── main.cpp                     # Application entry point
├── mainwindow.h/.cpp/.ui       # Main window implementation
├── interactive3dwindow.h/.cpp  # Custom 3D window with mouse interaction
├── scenemodifier.h/.cpp        # 3D scene management and modifications
├── myhashtable.h               # Custom hash table implementation
├── CMakeLists.txt              # Build configuration
├── .gitignore
└── README.md
```

## 🔧 Key Components

### MainWindow
- Manages the washing machine UI and program logic
- Handles all washing cycle state transitions
- Calculates and displays estimated completion times
- Manages slider animations and progress indicators

### Interactive3DWindow
- Extends Qt3DWindow for custom mouse interactions
- Handles 3D scene rotation and zooming
- Manages user input for 3D object manipulation

### SceneModifier
- Creates and manages 3D text labels
- Implements custom 3D progress bar entities
- Synchronizes label and bar rotations with scene

### MyHashtable
- Template-based hash table implementation
- Used for storing material references for color changes
- Demonstrates understanding of data structures and algorithms

### ProgressBarEntity
- Custom 3D progress bar with background and fill components
- Dynamically updates visual fill level (0.0 - 1.0)
- Rotates with the main scene for consistent orientation

## 🧼 Washing Programs

| Program | Duration | Temperature Range | Spin Speed | Special Features |
|---------|----------|-------------------|------------|------------------|
| **Cottons** | 2h 00m | Cold, 20-90°C | 600-1400 RPM | Standard cotton wash |
| **Cottons Eco** | 3h 00m | Cold, 20-60°C | 600-1400 RPM | Energy efficient |
| **Synthetics** | 1h 00m | Cold, 20-60°C | 600-1200 RPM | Synthetic fabrics |
| **Wool/Silk** | 0h 40m | Cold, 20-40°C | 400-800 RPM | Delicate cycle |
| **Anti-Allergy** | 2h 00m | 60-90°C | 1000-1400 RPM | Steam treatment option |
| **Non-Stop** | 3h 00m | Cold, 30-60°C | 800-1200 RPM | Wash + Dry combo |
| **Anti-Crease** | 0h 50m | Cold, 20-40°C | 400-800 RPM | Extra anti-crease option |
| **Refresh** | 0h 20m | Cold, 20-40°C | 400 RPM | Steam + Quick spin |

### Cycle Stages
Most programs follow this cycle:
1. **Fill Water** - Water fills the drum
2. **Heat Water** - Water heated to selected temperature
3. **Wash Cycle** - Main washing agitation
4. **Drain Water** - Dirty water removed
5. **Rinse Cycle** - Clean water rinse (1-4 cycles)
6. **Drain Water** - Rinse water removed
7. **Spin Cycle** - High-speed water extraction
8. **Optional** - Steam treatment, drying, or anti-crease

## 📸 Screenshots

*(Screenshots would be added here showcasing the application interface)*

## 💻 Development

### Code Quality
- **C++17 Standard**: Modern C++ features including lambdas, smart pointers, and structured bindings
- **Memory Management**: Proper Qt parent-child relationships for automatic memory management
- **Modular Design**: Clear separation of concerns between UI, 3D rendering, and business logic
- **Type Safety**: Template-based generic programming for reusability

### Performance Optimization
- **Time Compression**: 60:1 compression ratio for realistic yet fast simulation
- **Efficient Rendering**: Qt3D optimized scene graph
- **Timer Management**: Multiple QTimer instances for concurrent animation updates

### Future Enhancements
- [ ] Add sound effects for different cycle stages
- [ ] Implement water level visualization inside drum
- [ ] Add more 3D model animations (door opening, drum rotation)
- [ ] Save and load custom washing programs
- [ ] Multi-language support
- [ ] Add wear simulation for maintenance parts
- [ ] Implement energy consumption tracking

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

### Development Setup
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 👨‍💻 Author

**Your Name**
- GitHub: [@yourusername](https://github.com/yourusername)
- LinkedIn: [Your Name](https://linkedin.com/in/yourprofile)
- Email: your.email@example.com

## 🙏 Acknowledgments

- Samsung for the washing machine 3D model reference
- Qt Company for the excellent Qt framework
- All contributors who have helped improve this project

---

**Note**: This is a simulation project for educational and demonstration purposes. It does not control any real washing machine hardware.

## 📊 Statistics

- **Lines of Code**: ~2,200+ lines of C++
- **Classes**: 5 main classes (MainWindow, Interactive3DWindow, SceneModifier, ProgressBarEntity, MyHashtable)
- **Washing Programs**: 8 fully configurable programs
- **Supported Platforms**: Linux, Windows, macOS
- **Development Time**: Educational project demonstrating C++/Qt skills

---

*Built with ❤️ using C++ and Qt*
