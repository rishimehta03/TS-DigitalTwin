# Icon Fix Required

## Issue
While you replaced the logo files in the root directory (`icon.png`, `logo.svg`, etc.), the Windows build uses **platform-specific .ico files** that are still showing Godot icons.

## Windows .ico Files Location
```
platform/windows/godot.ico          ← Used by the editor
platform/windows/godot_console.ico  ← Used by the console wrapper
```

## How to Fix

### Option 1: Convert Your PNG Icon to .ico (Recommended)
You need to create a multi-resolution `.ico` file from your `icon.png`:

**Using ImageMagick (if installed):**
```powershell
magick R:\TS-DigitalTwin\icon.png -define icon:auto-resize=256,128,96,64,48,32,16 R:\TS-DigitalTwin\platform\windows\godot.ico
```

**Using Online Converter:**
1. Go to https://convertio.co/png-ico/ or similar
2. Upload `R:\TS-DigitalTwin\icon.png`
3. Select multi-resolution ico (256x256, 128x128, 64x64, 48x48, 32x32, 16x16)
4. Download and save as `R:\TS-DigitalTwin\platform\windows\godot.ico`

**For console icon:**
- You can use the same file for both, or create a variant
- Copy: `R:\TS-DigitalTwin\platform\windows\godot_console.ico`

### Option 2: Keep Godot Icons Temporarily
- The executable will work fine with Godot icons
- You can replace them later when you have proper .ico files

## After Replacing

Once you replace the .ico files, rebuild:
```powershell
python -m SCons platform=windows target=editor d3d12=no -j8
```

The new icons will be embedded in the executable.

---

**Current Status:**
- ✅ Executable name fixed (tdtstack instead of godot)  
- ⏳ Icon files still need platform-specific .ico conversion
- 🔄 Build in progress...
