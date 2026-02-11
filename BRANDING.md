# Techno Digital Twin Stack - Branding Guidelines

## Product Information

### Official Names
- **Full Product Name**: Techno Digital Twin Stack
- **Short Name**: TDT Stack
- **Company**: Technostacks Infotech Private Limited
- **Website**: https://technostacks.com/

## Visual Identity

### Logo Usage
- Primary logo: `logo.svg` (full color)
- Icon: `icon.svg` (app icon, favicons)
- Outlined variants: `logo_outlined.svg`, `icon_outlined.svg`
- Minimum size: 32px for icon, 120px width for logo
- Clear space: Minimum 20px padding around logo

### Color Palette
Based on Technostacks corporate identity:
- **Primary Blue**: `#0066CC` - Main brand color, headers, primary CTAs
- **Light Blue Accent**: `#00A3E0` - Secondary elements, highlights
- **Dark Theme Background**: `#1E1E1E` - Editor dark mode
- **Light Theme Background**: `#F5F5F5` - Documentation, light mode
- **Success Green**: `#28A745` - Positive indicators, connection status
- **Warning Orange**: `#FFA500` - Alerts, warnings
- **Error Red**: `#DC3545` - Error states, critical alerts

### Typography
- **Headings**: Inter, SF Pro Display, or system sans-serif
- **Body Text**: Inter, Roboto, or system sans-serif  
- **Code/Monospace**: JetBrains Mono, Fira Code, or Consolas

## Naming Conventions

### Product Terminology
- **Digital Twin** (always capitalized) - NOT "digital-twin" or "digitaltwin"
- **IoT Bridge** - NOT "iot-bridge" or "IOT Bridge"
- **Performance Twin, Product Twin, Production Twin** - Capitalized twin types
- **TDT Stack** - Acceptable short form in technical contexts

### Feature Naming Standards
- Use title case for feature names: "Real-Time Synchronization"
- Use sentence case for descriptions: "Sync data from industrial sensors"
- Avoid abbreviations in user-facing text (except well-known: IoT, API, SDK)

## Attribution Requirements

### Godot Engine Acknowledgment
Always acknowledge the Godot Engine foundation in:

1. **About Dialog** → "Third-Party Licenses" tab:
   ```
   This software is built upon Godot Engine (https://godotengine.org)
   Licensed under the MIT License
   Copyright (c) 2014-present Godot Engine contributors
   ```

2. **README.md** → Footer:
   ```
   *Built on modified Godot Engine technology*
   ```

3. **Documentation** → Footer on all pages:
   ```
   Powered by Godot Engine | © 2026 Technostacks Infotech Private Limited
   ```

4. **Splash Screen** (optional):
   - "Powered by Godot" text for 2 seconds on startup (can be in small print)

### Open Source Compliance
- Maintain `COPYRIGHT.txt` with both Technostacks and Godot copyrights
- Keep `LICENSE.txt` with MIT license (compatible with Godot)
- List all modifications in `MODIFICATIONS.md` (to be created)

## Brand Voice & Messaging

### Value Propositions
- **Industrial-Grade Reliability**: "Enterprise digital twin platform for mission-critical operations"
- **Real-Time Intelligence**: "Transform physical assets into intelligent, predictive systems"
- **Open Innovation**: "Built on proven open-source technology, enhanced for industry"

### Target Audiences
1. **Manufacturing Engineers**: Focus on production optimization, predictive maintenance
2. **Industrial Automation Specialists**: Emphasize OPC-UA, PLC integration
3. **IoT Architects**: Highlight scalability, protocol support (MQTT, REST)
4. **Digital Transformation Leaders**: Stress ROI, time-to-value, vendor independence

### Tone Guidelines
- **Professional**: Use industry-standard terminology
- **Approachable**: Avoid jargon where possible, explain complex concepts
- **Confident**: Assert capabilities without overpromising
- **Innovative**: Emphasize cutting-edge tech (AI, real-time simulation) while proving stability

## Usage Examples

### Correct ✅
- "Techno Digital Twin Stack helps manufacturers reduce downtime by 30%"
- "The Performance Twin continuously monitors asset health"
- "Connect via MQTT or OPC-UA protocols"
- "Built on Godot Engine technology for proven 3D rendering"

### Incorrect ❌
- "Techno digital-twin-stack..." (wrong capitalization, hyphens)
- "The performance twin..." (not capitalized)
- "IoT-Bridge module" (unnecessary hyphen)
- "Based on Godot" without proper attribution

## File Naming Conventions

### Code Files
- Module directories: `modules/digital_twin/`, `modules/iot_bridge/`
- Class names: `DTManager`, `IoTBridge`, `PerformanceTwin` (PascalCase)
- File names: `dt_manager.cpp`, `iot_bridge.h` (snake_case)

### Documentation
- User guides: `user-guide-getting-started.md` (kebab-case)
- API reference: `api-reference-dt-manager.md` (kebab-case)
- Examples: `example_production_line.gd` (snake_case for code)

## Logo Specifications

**NOTE**: Logo files need to be provided and placed in root directory.

### Required Files
1. `icon.png` (512x512px) - Application icon, PNG with transparency
2. `icon.svg` - Vector version, viewBox 0 0 512 512
3. `icon_outlined.png` (512x512px) - Outlined variant
4. `icon_outlined.svg` - Vector outlined version
5. `logo.png` (~2000x600px) - Horizontal logo with text
6. `logo.svg` - Vector version, preserve aspect ratio
7. `logo_outlined.png` - Outlined variant
8. `logo_outlined.svg` - Vector outlined version

### Logo Requirements
- **Icon**: Square format, recognizable at 16x16px (favicon size)
- **Logo**: Horizontal lockup with "Techno Digital Twin Stack" text
- **Colors**: Use Technostacks blue (#0066CC) as primary
- **Format**: PNG with transparency, SVG for scalability
- **Consistency**: Match Technostacks corporate design language

---

## Version History

**v1.0** - February 11, 2026
- Initial branding guidelines established
- Color palette defined
- Attribution requirements documented
- Naming conventions standardized

---

**For questions or clarifications**: Contact Technostacks branding team
