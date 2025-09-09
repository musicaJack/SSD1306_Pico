# DS3231 Digital Clock with SSD1306 OLED Display

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi%20Pico-brightgreen.svg)
![Version](https://img.shields.io/badge/Version-3.0.0-orange.svg)

English | [中文](README.zh.md)

A C++ project for displaying time, date, weekday, and temperature using DS3231 Real-Time Clock and SSD1306 OLED display on Raspberry Pi Pico.

## Hardware Requirements

- Raspberry Pi Pico (RP2040)
- DS3231 Real-Time Clock module
- SSD1306 OLED display (128x64 pixels)
- Jumper wires for connections

## Hardware Connection

```
SSD1306 VCC → Pico 3.3V
SSD1306 GND → Pico GND
SSD1306 SCL → Pico GP3 (I2C0 SCL)
SSD1306 SDA → Pico GP2 (I2C0 SDA)

DS3231 VCC → Pico 3.3V
DS3231 GND → Pico GND
DS3231 SCL → Pico GP5 (I2C1 SCL)
DS3231 SDA → Pico GP4 (I2C1 SDA)
```

**Note:**
- SSD1306 I2C address: 0x3C
- DS3231 I2C address: 0x68
- SSD1306 uses I2C0 (GP2/GP3), DS3231 uses I2C1 (GP4/GP5)
- Each device uses separate GPIO pins

## Features

- **Real-time Clock Display**: Accurate time display using DS3231 RTC
- **Dual-color OLED Display**: 
  - Yellow area: Date, weekday, and temperature
  - Blue area: Large 7-segment digital time display
- **Temperature Monitoring**: Built-in DS3231 temperature sensor
- **Custom 7-segment Font**: Optimized for OLED display clarity
- **Dual I2C Interface**: SSD1306 on I2C0 (GP2/GP3), DS3231 on I2C1 (GP4/GP5)
- **No WiFi Required**: Completely standalone operation

## Build and Flash

### Environment Requirements

1. Install Pico SDK
2. Set PICO_SDK_PATH environment variable
3. Install CMake and ARM GCC toolchain

### Build Steps

#### Windows
```batch
# Use the provided batch script
build_pico.bat
```

#### Linux/WSL
```bash
# Use the provided shell script
./build.sh
```

#### Manual Build
```bash
# Create build directory
mkdir build
cd build

# Configure project
cmake -G "Ninja" ..

# Build
ninja
```

### Flash

After compilation, copy the generated UF2 file to Pico in BOOTSEL mode:
- `ds3231_clock.uf2` - DS3231 digital clock with OLED display

## Usage

1. Connect the hardware according to the connection diagram above
2. Flash `ds3231_clock.uf2` program to Pico
3. The OLED display will show:
   - **Top area (Yellow)**: Date on first line, weekday and temperature on second line
   - **Main area (Blue)**: Large 7-segment digital time display (HH:MM:SS)
4. The display updates every second
5. Temperature is read from DS3231's built-in sensor

## Display Layout

```
┌─────────────────────────┐
│ 2025-09-08              │ ← Yellow area: Date
│ Tue        25.8°C      │ ← Yellow area: Weekday + Temperature
├─────────────────────────┤
│                         │
│   19:14:16             │ ← Blue area: 7-segment time display
│                         │
└─────────────────────────┘
```

## Serial Output Example

```
DS3231 Digital Clock Starting...
SSD1306 SCL → Pico GP3 (I2C0 SCL)
SSD1306 SDA → Pico GP2 (I2C0 SDA)
DS3231 SCL → Pico GP5 (I2C1 SCL)
DS3231 SDA → Pico GP4 (I2C1 SDA)
SSD1306初始化成功！
DS3231初始化成功！
DS3231时间: 2025-09-08 19:14:16 (星期2)
显示时间: 小时=19(1,9), 分钟=14(1,4), 秒=16(1,6)
温度: 25.8°C
```

## Project Structure

```
ssd1306/
├── CMakeLists.txt          # CMake build configuration
├── build_pico.bat          # Windows build script
├── include/                # Header files directory
│   ├── ssd1306.h          # SSD1306 OLED driver header
│   └── ds3231/            # DS3231 driver headers
│       └── ds3231.h       # DS3231 RTC driver header
├── src/                   # Source code directory
│   ├── ssd1306.cpp        # SSD1306 OLED driver implementation
│   └── ds3231/            # DS3231 driver source
│       └── ds3231_driver.cpp # DS3231 RTC driver implementation
├── examples/              # Example programs directory
│   └── ds3231_clock.cpp   # Main DS3231 digital clock program
└── README.md              # Project documentation
```

## Technical Specifications

- **OLED Resolution**: 128x64 pixels
- **OLED Font**: 5x7 pixel built-in font
- **7-segment Display**: 14x28 pixel custom font
- **I2C Frequency**: 400kHz (SSD1306), 100kHz (DS3231)
- **Time Accuracy**: ±2ppm (DS3231 RTC)
- **Temperature Range**: -40°C to +85°C (DS3231 sensor)
- **Update Rate**: 1 second
- **Power Consumption**: Low power operation

## Changelog

### v3.0.0 (Latest)
- **Complete project transformation**: From battery monitoring to DS3231 digital clock
- **DS3231 RTC integration**: Accurate real-time clock with temperature sensor
- **Dual-color OLED display**: Yellow area for date/weekday/temperature, blue area for time
- **Custom 7-segment font**: Optimized 14x28 pixel digital time display
- **Dual I2C interface**: SSD1306 on I2C0 (GP2/GP3), DS3231 on I2C1 (GP4/GP5)
- **UI optimization**: Proper spacing and layout for better readability
- **WiFi removal**: Completely standalone operation
- **Week mapping fix**: Correct DS3231 weekday display (0=Sunday, 1=Monday, etc.)

### v2.0.0
- Refactor SSD1306 OLED driver based on Adafruit_SSD1306 library
- Fix OLED display snowflake issue
- Clean up duplicate font definitions
- Optimize code structure and memory management

### v1.0.0
- Initial battery voltage monitoring functionality
- ADS1115 ADC support
- Basic OLED display functionality

## Important Notes

1. Ensure correct I2C connections: SSD1306 uses I2C0 (GP2/GP3), DS3231 uses I2C1 (GP4/GP5)
2. Each device uses separate GPIO pins for different I2C buses
3. DS3231 requires a backup battery for time retention when power is off
4. The 7-segment display is optimized for 128x64 OLED resolution
5. Temperature reading is from DS3231's built-in sensor
6. Weekday mapping: 0=Sunday, 1=Monday, 2=Tuesday, ..., 6=Saturday

## License

This project is licensed under the MIT License. See the LICENSE file for details. 