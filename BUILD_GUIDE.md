# Building Techno Digital Twin Stack

## Prerequisites

You have two options for building TDT Stack on Windows:

### Option 1: Visual Studio (Recommended - No SCons needed)
### Option 2: SCons Build System (Command line)

---

## Option 1: Build with Visual Studio ⭐ RECOMMENDED

Godot supports generating Visual Studio project files that you can open and build directly in Visual Studio.

### Requirements
- **Visual Studio 2022** (Community, Professional, or Enterprise)
- **Workload**: "Desktop development with C++"

### Steps

#### 1. Install Python (Required for project generation)
```powershell
# Check if Python is installed
python --version

# If not installed, download from https://www.python.org/downloads/
# OR install via winget:
winget install Python.Python.3.12
```

#### 2. Install SCons (just for VS project generation)
```powershell
python -m pip install scons
```

#### 3. Generate Visual Studio Solution
```powershell
cd R:\TS-DigitalTwin

# Generate VS 2022 solution files
python -m SCons vsproj=yes platform=windows
```

This creates: `godot.sln` in the root directory

#### 4. Open in Visual Studio
- Double-click `godot.sln` OR
- Open Visual Studio → File → Open → Project/Solution → Select `godot.sln`

#### 5. Build in Visual Studio
- Select build configuration: **Debug** or **Release**
- Right-click the **main** project → **Build**
- OR press `Ctrl+Shift+B` to build all

#### 6. Run the Editor
After successful build, the executable will be in:
```
R:\TS-DigitalTwin\bin\tdtstack.windows.editor.x86_64.exe
```

---

## Option 2: Build with SCons (Command Line)

### Requirements
- **Python 3.8+**
- **SCons** (Python package)
- **Visual Studio Build Tools** or **MSVC compiler**

### Steps

#### 1. Install SCons
```powershell
python -m pip install scons
```

#### 2. Verify Installation
```powershell
python -m SCons --version
```

#### 3. Build Commands

**Clean previous builds:**
```powershell
python -m SCons -c
```

**Build editor (debug):**
```powershell
python -m SCons platform=windows target=editor
```

**Build editor (optimized/release):**
```powershell
python -m SCons platform=windows target=editor production=yes
```

**Faster builds (use multiple CPU cores):**
```powershell
python -m SCons platform=windows target=editor -j8
```
(Replace `8` with your CPU core count)

---

## Build Configurations Explained

### Targets
- `target=editor` - Editor with full debugging capabilities
- `target=template_debug` - Runtime for testing games/apps
- `target=template_release` - Optimized runtime for production

### Platforms
- `platform=windows` - Windows build
- `platform=linux` - Linux build (cross-compile or WSL)
- `platform=web` - WebAssembly build

### Additional Options
- `production=yes` - Optimize for release (faster, no debug symbols)
- `debug_symbols=yes` - Include debug symbols even in release
- `-j<N>` - Use N parallel jobs (speeds up compilation)

---

## Troubleshooting

### "SCons not found" Error
**Problem:** SCons not installed or not in PATH

**Solution 1 - Use Python module:**
```powershell
python -m pip install scons
python -m SCons --version  # Verify
```

**Solution 2 - Add to PATH:**
```powershell
# Find Python Scripts directory
where python
# Usually: C:\Users\YourName\AppData\Local\Programs\Python\Python3XX\Scripts

# Add to PATH in System Environment Variables
```

### "MSVC not found" Error
**Problem:** Visual Studio C++ compiler not installed

**Solution:**
1. Open Visual Studio Installer
2. Modify your installation
3. Check "Desktop development with C++"
4. Install

### Build Takes Too Long
**Solution:** Use parallel compilation:
```powershell
python -m SCons platform=windows target=editor -j%NUMBER_OF_PROCESSORS%
```

---

## Recommended Workflow

For **active development**, use **Visual Studio**:
- Better debugging with breakpoints
- IntelliSense code completion
- Visual debugging tools
- Easier to navigate large codebase

For **CI/CD and automated builds**, use **SCons**:
- Scriptable and automatable
- Consistent across platforms
- Better for command-line workflows

---

## Quick Start (If You Have VS 2022)

```powershell
# 1. Install Python if needed
winget install Python.Python.3.12

# 2. Install SCons
python -m pip install scons

# 3. Generate VS solution
python -m SCons vsproj=yes platform=windows

# 4. Open godot.sln in Visual Studio

# 5. Build (Ctrl+Shift+B)

# 6. Run
.\bin\tdtstack.windows.editor.x86_64.exe
```

---

## Next Steps After Successful Build

1. **Verify Branding:** Check if window title, About dialog, and icons show TDT Stack branding
2. **Test Basic Functionality:** Open the editor, create a test project
3. **Start Phase 1:** Begin implementing digital twin modules

---

*For detailed Godot build documentation, see: https://docs.godotengine.org/en/stable/contributing/development/compiling/compiling_for_windows.html*
