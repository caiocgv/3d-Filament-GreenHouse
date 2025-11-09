# Contributing to 3D Filament Greenhouse Dryer

Thank you for your interest in contributing! This project welcomes contributions from the community.

## Ways to Contribute

### 1. Code Contributions
- Bug fixes
- New features
- Performance improvements
- Code refactoring
- Unit tests

### 2. Hardware Contributions
- PCB design improvements
- Alternative PCB layouts
- 3D enclosure variants
- Component substitutions
- Wiring diagrams

### 3. Documentation
- Improve README
- Add tutorials
- Create videos
- Translate documentation
- Add photos of builds

### 4. Testing
- Test on different ESP8266 boards
- Test with different sensors
- Report bugs
- Verify fixes

## Getting Started

1. **Fork the Repository**
   ```bash
   # Click the "Fork" button on GitHub
   ```

2. **Clone Your Fork**
   ```bash
   git clone https://github.com/YOUR_USERNAME/3d-Filament-GreenHouse.git
   cd 3d-Filament-GreenHouse
   ```

3. **Create a Branch**
   ```bash
   git checkout -b feature/your-feature-name
   # or
   git checkout -b fix/bug-description
   ```

4. **Make Your Changes**
   - Write clean, commented code
   - Follow existing code style
   - Test your changes thoroughly

5. **Commit Your Changes**
   ```bash
   git add .
   git commit -m "Description of your changes"
   ```

6. **Push to Your Fork**
   ```bash
   git push origin feature/your-feature-name
   ```

7. **Create a Pull Request**
   - Go to the original repository
   - Click "New Pull Request"
   - Select your branch
   - Describe your changes

## Code Style Guidelines

### Arduino/C++ Code
- Use 2-space indentation
- Descriptive variable names (camelCase)
- Constants in UPPER_CASE
- Comment complex logic
- Keep functions focused and small

### Example:
```cpp
// Good
const int RELAY_PIN = 5;
float currentTemp = 0;

void controlTemperature() {
  // Check if temperature is below target
  if (currentTemp < targetTemp - TEMP_HYSTERESIS) {
    activateHeater();
  }
}

// Avoid
int r = 5;
float t = 0;
void ct() { if (t < tt - 2) ah(); }
```

### HTML/CSS/JavaScript
- Use 4-space indentation for HTML/CSS
- Use 2-space indentation for JavaScript
- Use meaningful class/id names
- Comment sections
- Keep JavaScript functions small

## Testing Guidelines

### Before Submitting
1. **Test Compilation**
   ```
   - Verify code compiles without errors
   - Check for warnings
   - Test on target hardware if possible
   ```

2. **Test Functionality**
   ```
   - Verify new features work as intended
   - Check that existing features still work
   - Test edge cases
   ```

3. **Test Safety**
   ```
   - Ensure temperature limits are respected
   - Verify relay control is safe
   - Check for potential hazards
   ```

## Pull Request Process

1. **Update Documentation**
   - Update README if needed
   - Add comments to code
   - Document new features

2. **Describe Changes**
   - Clear title
   - Detailed description
   - Link related issues
   - Add screenshots if applicable

3. **Review Process**
   - Maintainer will review
   - Address feedback
   - Make requested changes
   - Discussion is encouraged

## Reporting Bugs

### Before Reporting
- Search existing issues
- Verify it's reproducible
- Test with latest code

### Bug Report Should Include
1. **Description**: Clear description of the bug
2. **Steps to Reproduce**: Numbered steps
3. **Expected Behavior**: What should happen
4. **Actual Behavior**: What actually happens
5. **Environment**:
   - ESP8266 board type
   - Arduino IDE version
   - Library versions
   - Hardware setup
6. **Serial Output**: Copy relevant serial output
7. **Code**: Minimal code to reproduce issue

### Example Bug Report
```markdown
**Description**: Web interface doesn't update temperature

**Steps to Reproduce**:
1. Upload code to ESP8266
2. Connect to web interface
3. Observe temperature display

**Expected**: Temperature updates every 2 seconds
**Actual**: Temperature shows "--" and never updates

**Environment**:
- Board: NodeMCU 1.0
- IDE: Arduino 1.8.19
- DHT Library: 1.4.4

**Serial Output**:
```
Failed to read from DHT sensor!
```

**Additional Info**: 
Tested with two different DHT22 sensors, same result.
```

## Feature Requests

### Before Requesting
- Check if feature already exists
- Search existing issues/PRs
- Consider if it fits project scope

### Feature Request Should Include
1. **Description**: Clear description of feature
2. **Use Case**: Why is it needed?
3. **Proposed Solution**: How would it work?
4. **Alternatives**: Other approaches considered
5. **Additional Context**: Mockups, examples, etc.

## Hardware Contributions

### PCB Design
- Use KiCad (preferred) or Eagle
- Include source files (.kicad_pcb, .sch)
- Export Gerber files
- Include BOM (Bill of Materials)
- Provide assembly instructions

### 3D Models
- Provide source files (Fusion 360, OpenSCAD, etc.)
- Export STL files
- Include printing instructions
- Specify material requirements
- Test print before submitting

### Wiring Diagrams
- Use Fritzing or similar tool
- Provide source file
- Export high-res PNG
- Label all connections clearly
- Include in docs/images/

## Documentation Contributions

### README Updates
- Keep formatting consistent
- Use clear language
- Add images where helpful
- Test all instructions

### Tutorials
- Step-by-step instructions
- Include photos/screenshots
- Test on fresh setup
- Anticipate common problems

### Translations
- Create language-specific README
- Keep original structure
- Use clear, technical language
- Update links appropriately

## Code of Conduct

### Our Standards
- Be respectful and inclusive
- Accept constructive criticism
- Focus on what's best for the project
- Show empathy towards others

### Unacceptable Behavior
- Harassment or discrimination
- Trolling or insulting comments
- Publishing others' private information
- Other unprofessional conduct

## Questions?

- Open an issue for questions
- Tag with "question" label
- Be specific about what you need help with

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (MIT License for software, CERN OHL for hardware).

## Recognition

Contributors will be acknowledged in:
- CONTRIBUTORS.md file
- Project documentation
- Release notes

Thank you for contributing! 🎉
