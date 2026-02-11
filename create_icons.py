#!/usr/bin/env python3
"""
Create properly sized icon files from source images for TDT Stack branding.
"""
from PIL import Image
import os

def create_ico_file(source_png, output_ico, sizes=[(16, 16), (32, 32), (48, 48), (256, 256)]):
    """Create a multi-resolution .ico file from a PNG source."""
    print(f"Creating {output_ico} from {source_png}...")
    
    # Load source image
    img = Image.open(source_png)
    
    # Create list of resized images
    icon_images = []
    for size in sizes:
        resized = img.resize(size, Image.Resampling.LANCZOS)
        icon_images.append(resized)
    
    # Save as .ico with multiple resolutions
    icon_images[0].save(output_ico, format='ICO', sizes=[(s[0], s[1]) for s in sizes], append_images=icon_images[1:])
    print(f"✓ Created {output_ico} with sizes: {sizes}")

def create_splash_png(source_png, output_png, size=(680, 680)):
    """Create a properly sized splash screen PNG."""
    print(f"Creating {output_png} from {source_png}...")
    
    # Load source image
    img = Image.open(source_png)
   
    # Create a new image with the target size and transparent background
    splash = Image.new('RGBA', size, (0, 0, 0, 0))
    
    # Resize source to fit within splash dimensions while maintaining aspect ratio
    img.thumbnail((size[0] * 0.6, size[1] * 0.6), Image.Resampling.LANCZOS)
    
    # Center the image
    x = (size[0] - img.width) // 2
    y = (size[1] - img.height) // 2
    splash.paste(img, (x, y), img if img.mode == 'RGBA' else None)
    
    # Save as PNG
    splash.save(output_png, 'PNG')
    print(f"✓ Created {output_png} at {size[0]}x{size[1]}")

def main():
    # Paths
    icon_png = "icon.png"
    logo_png = "logo.png"
    
    # Create Windows .ico files with multiple resolutions
    create_ico_file(icon_png, "platform/windows/godot.ico")
    create_ico_file(icon_png, "platform/windows/godot_console.ico")
    
    # Create splash screen - centered icon on transparent background
    create_splash_png(icon_png, "main/splash.png", size=(680, 680))
    
    print("\n✅ All icon files created successfully!")
    print("Now rebuild with: python -m SCons platform=windows target=editor d3d12=no -j8")

if __name__ == "__main__":
    main()
