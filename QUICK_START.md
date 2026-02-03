# 🚀 Quick Start Guide

Get your Washing Machine Simulator up and running in minutes!

## ⚡ Fast Setup (5 minutes)

### Step 1: Install Dependencies

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install build-essential cmake qt6-base-dev qt6-3d-dev
# OR for Qt5
sudo apt-get install build-essential cmake qtbase5-dev qt3d5-dev
```

#### Fedora/RHEL
```bash
sudo dnf install gcc-c++ cmake qt6-qtbase-devel qt6-qt3d-devel
# OR for Qt5
sudo dnf install gcc-c++ cmake qt5-qtbase-devel qt5-qt3d-devel
```

#### macOS (with Homebrew)
```bash
brew install cmake qt@6
# OR for Qt5
brew install cmake qt@5
```

#### Windows
1. Download and install [Qt](https://www.qt.io/download) (Qt 5.15+ or Qt 6.x)
2. Download and install [CMake](https://cmake.org/download/)
3. Install [MinGW](https://www.mingw-w64.org/) or [Visual Studio 2019+](https://visualstudio.microsoft.com/)

---

### Step 2: Build the Project

```bash
# Clone or navigate to the project
cd washingMachineSimulator

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .

# The assets folder will be automatically found relative to the executable
```

---

### Step 3: Run the Application

```bash
# From the build directory
./managementsystem

# OR on Windows
managementsystem.exe
```

**Important**: Make sure the `assets` folder is copied to your build directory or is accessible from the executable location:
```bash
# If needed, copy assets to build directory
cp -r ../assets ./
```

---

## 🎮 Using the Application

### Quick Tutorial

1. **Welcome Screen** 
   - You'll see a 3D model of the washing machine
   - Four steps are displayed as 3D text labels on the left

2. **Step 1: Insert Powder**
   - Click the "Insert Powder" button
   - The label turns green ✅

3. **Step 2: Select Program**
   - Choose from 8 washing programs
   - Each program has customizable options:
     - Temperature (Cold, 20°C-90°C depending on program)
     - Spin Speed (400-1400 RPM depending on program)
     - Rinse Cycles (1-4)
     - Eco Mode (On/Off)
     - Special features (Steam, Dry, Anti-Crease)

4. **Step 3: Start Program**
   - Review the estimated duration
   - Click "Start" to begin
   - Watch the progress bars fill in real-time:
     - 🌊 **Filling Water** - Water level indicator
     - 🌡️ **Heating Water** - Temperature progress
     - 🧼 **Washing** - Wash cycle progress
     - 💧 **Draining** - Water drainage
     - 🔄 **Rinsing** - Rinse cycle
     - 💨 **Spinning** - Spin cycle (RPM displayed)
     - ♨️ **Steam** (if enabled)
     - 🏜️ **Drying** (if enabled)

5. **Step 4: Emergency Stop** (Optional)
   - Click "Emergency Stop" to halt the program
   - Returns to configuration page

6. **Maintenance**
   - Monitor the 3D progress bars on the right:
     - **Water Tank** - Blue bar
     - **Lint Trap** - Orange bar
     - **Detergent Tray** - Green bar
   - Clean when bars are full (buttons available)

---

## 🖱️ 3D Interaction

- **Rotate Model**: Click and drag with left mouse button
- **Zoom**: Scroll mouse wheel
- **Labels**: 3D text labels rotate with the model

---

## 📋 Program Quick Reference

| Program | Best For | Duration | Special Features |
|---------|----------|----------|------------------|
| **Cottons** | Cotton fabrics, bedding | 2h 00m | High temp up to 90°C |
| **Cottons Eco** | Energy-saving cotton wash | 3h 00m | Lower energy, longer time |
| **Synthetics** | Polyester, mixed fabrics | 1h 00m | Medium temperature |
| **Wool/Silk** | Delicate fabrics | 0h 40m | Gentle, low spin |
| **Anti-Allergy** | Allergen removal | 2h 00m | Steam treatment option |
| **Non-Stop** | Wash and dry combo | 3h 00m | Includes drying cycle |
| **Anti-Crease** | Wrinkle prevention | 0h 50m | Extra anti-crease phase |
| **Refresh** | Quick refresh | 0h 20m | Steam + light spin |

---

## 🔧 Troubleshooting

### Application won't start
```bash
# Check if assets are accessible
ls assets/scene.gltf  # Should exist

# Check library dependencies
ldd ./managementsystem  # Linux
otool -L ./managementsystem  # macOS
```

### 3D model not loading
- Ensure `assets/scene.gltf` and `assets/scene.bin` are in the same directory
- Check file permissions: `chmod 644 assets/*`

### Spinner animations not showing
- Verify all `.gif` files are in `assets/` directory
- Check file names match exactly (case-sensitive on Linux/macOS)

### Build errors
```bash
# Clear CMake cache and rebuild
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Qt not found by CMake
```bash
# Specify Qt path explicitly
cmake .. -DCMAKE_PREFIX_PATH="/path/to/Qt/6.x/gcc_64"
# Example: -DCMAKE_PREFIX_PATH="/usr/lib/x86_64-linux-gnu/cmake/Qt6"
```

---

## 💡 Pro Tips

1. **Time Compression**: The simulator uses a 60:1 time compression ratio
   - Real 2 hours = 2 simulated minutes
   - Perfect for quick demonstrations

2. **Maintenance Tracking**: Different programs wear components differently
   - Anti-Allergy uses more detergent
   - Non-Stop fills water tank faster
   - Refresh has minimal impact

3. **Eco Mode**: Adds time but simulates energy savings
   - Redistributes time across all cycle phases
   - Lower temperatures when possible

4. **Emergency Stop**: Safe at any time
   - Returns to program configuration
   - All settings preserved

---

## 📚 Learn More

- Full documentation: See [README.md](README.md)
- Code fixes applied: See [FIXES_SUMMARY.md](FIXES_SUMMARY.md)
- Qt3D documentation: [Qt 3D Documentation](https://doc.qt.io/qt-6/qt3d-index.html)
- CMake guide: [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)

---

## 🆘 Need Help?

- Check the [README.md](README.md) for detailed information
- Review [FIXES_SUMMARY.md](FIXES_SUMMARY.md) for known issues and fixes
- Open an issue on GitHub (if applicable)

---

## 🎉 You're Ready!

Enjoy exploring the Washing Machine Simulator! This project showcases:
- Advanced C++ programming
- Qt framework expertise
- 3D graphics development
- Complex state machine implementation
- Custom data structure design

Perfect for demonstrating your skills to potential employers! 💼✨

---

*Last Updated: 2026-02-03*
