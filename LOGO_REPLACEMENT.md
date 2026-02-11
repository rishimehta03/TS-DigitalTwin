# Logo and Icon Replacement Guide

## Overview
This guide explains how to replace the Godot Engine logo and icons with Techno Digital Twin Stack branding.

## Required Files

You need to prepare **8 image files** in total:

### Application Icons (Square Format - 512x512px recommended)

1. **`icon.png`**
   - Main application icon
   - Format: PNG with transparency
   - Size: 512x512px (will be scaled down as needed)
   - Use: Windows taskbar, macOS dock, Linux desktop

2. **`icon.svg`**
   - Vector version of the icon
   - Format: SVG
   - ViewBox: `0 0 512 512`
   - Scalable for all sizes (16px favicon to 512px high-DPI displays)

3. **`icon_outlined.png`**
   - Outlined variant for dark themes
   - Same specs as `icon.png`
   - Purpose: Better visibility on dark backgrounds

4. **`icon_outlined.svg`**
   - Vector outlined version
   - Same specs as `icon.svg`

### Application Logos (Horizontal Lockup)

5. **`logo.png`**
   - Full product logo with "Techno Digital Twin Stack" text
   - Format: PNG with transparency
   - Recommended size: ~2000x600px (preserve aspect ratio)
   - Use: About dialog, splash screen, documentation headers

6. **`logo.svg`**
   - Vector version of the full logo
   - Format: SVG
   - Scalable, preserve aspect ratio

7. **`logo_outlined.png`**
   - Outlined variant for dark themes
   - Same specs as `logo.png`

8. **`logo_outlined.svg`**
   - Vector outlined version
   - Same specs as `logo.svg`

## How to Obtain Logo Files

### Option 1: Use Existing Technostacks Logo (Recommended)
1. Visit https://technostacks.com/
2. Right-click on the Technostacks logo → "Save image as..." or inspect element to find SVG/high-res version
3. Adapt it for application use:
   - Create a square icon version (just the symbol/mark without text)
   - Create a horizontal logo version (symbol + "Techno Digital Twin Stack" text)

### Option 2: Design Custom Logos
If creating from scratch, follow these guidelines from `BRANDING.md`:
- **Colors**: Primary Blue `#0066CC`, Light Blue Accent `#00A3E0`
- **Style**: Modern, minimal, tech-focused
- **Elements**: Consider incorporating circuit patterns, nodes, twin/mirror concepts, IoT connectivity symbols
- **Typography**: Modern sans-serif (Inter, Roboto, or similar)

### Option 3: Use Placeholders (Temporary)
For now, you can continue using the existing Godot logos and replace them later when final assets are ready.

## Installation Steps

Once you have the 8 files ready:

```bash
# Navigate to repository root
cd R:\\TS-DigitalTwin

# Backup existing Godot logos (optional)
mkdir -p _godot_backup
cp icon*.* _godot_backup/
cp logo*.* _godot_backup/

# Copy your new logo files to root directory
# (Replace /path/to/your/logos with actual path)
cp /path/to/your/logos/icon.png ./
cp /path/to/your/logos/icon.svg ./
cp /path/to/your/logos/icon_outlined.png ./
cp /path/to/your/logos/icon_outlined.svg ./
cp /path/to/your/logos/logo.png ./
cp /path/to/your/logos/logo.svg ./
cp /path/to/your/logos/logo_outlined.png ./
cp /path/to/your/logos/logo_outlined.svg ./

# Stage the changes
git add icon*.* logo*.*

# Commit
git commit -m "Phase 0: Replace logo and icon files with TDT Stack branding"
```

## Additional Icon Locations

After replacing root directory files, you may also want to update:

### Editor UI Icons
- `editor/icons/` - Editor interface icons (hundreds of files)
- Most critical: `editor/icons/Logo.svg` - Main logo shown in editor
- **Recommendation**: Start with just `Logo.svg`, leave other UI icons as-is initially

### Platform-Specific Icons

#### Windows
- `platform/windows/godot.ico` - Windows executable icon (multi-resolution .ico file)
  - Create from `icon.png` using an online ICO converter or tools like ImageMagick:
  ```bash
  magick icon.png -define icon:auto-resize=256,128,96,64,48,32,16 platform/windows/tdtstack.ico
  ```

#### macOS
- `platform/macos/icon.icns` - macOS application icon bundle
  - Create from `icon.png` using iconutil or third-party tools

#### Linux
- Icons are typically generated from PNG at build time
- No additional changes needed

## Verification

After replacement, verify:
1. Logo appears correctly in `README.md` (GitHub/repository view)
2. Build the editor and check the splash screen
3. Check Help → About dialog shows correct logo
4. Executable icon matches your branding (Windows Explorer, macOS Finder, Linux file manager)

## Current Status

✅ Root directory placeholder references updated in `README.md`
❌ Actual logo/icon image files not yet replaced (still using Godot branding)

**Next Steps:**
1. Obtain the 8 required image files
2. Follow installation steps above
3. Rebuild the editor to see changes in action

---

For questions, refer to `BRANDING.md` or contact the Technostacks development team.
