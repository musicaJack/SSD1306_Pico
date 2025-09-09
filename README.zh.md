# DS3231 数字时钟 - SSD1306 OLED显示

![许可证](https://img.shields.io/badge/许可证-MIT-blue.svg)
![平台](https://img.shields.io/badge/平台-Raspberry%20Pi%20Pico-brightgreen.svg)
![版本](https://img.shields.io/badge/版本-3.0.0-orange.svg)

[English](README.md) | 中文

使用DS3231实时时钟和SSD1306 OLED显示屏在Raspberry Pi Pico上显示时间、日期、星期和温度的项目。

## 硬件要求

- Raspberry Pi Pico (RP2040)
- DS3231 实时时钟模块
- SSD1306 OLED显示屏（128x64像素）
- 连接线

## 硬件连接

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

**注意：**
- SSD1306 I2C地址：0x3C
- DS3231 I2C地址：0x68
- SSD1306使用I2C0（GP2/GP3），DS3231使用I2C1（GP4/GP5）
- 每个设备使用独立的GPIO引脚

## 功能特性

- **实时时钟显示**：使用DS3231 RTC准确显示时间
- **双色OLED显示**：
  - 黄色区域：日期、星期和温度
  - 蓝色区域：大型7段数码管时间显示
- **温度监控**：DS3231内置温度传感器
- **自定义7段字体**：针对OLED显示清晰度优化
- **双I2C接口**：SSD1306使用I2C0（GP2/GP3），DS3231使用I2C1（GP4/GP5）
- **无需WiFi**：完全独立运行

## 编译和烧录

### 环境要求

1. 安装Pico SDK
2. 设置PICO_SDK_PATH环境变量
3. 安装CMake和ARM GCC工具链

### 编译步骤

#### Windows
```batch
# 使用提供的批处理脚本
build_pico.bat
```

#### Linux/WSL
```bash
# 使用提供的shell脚本
./build.sh
```

#### 手动编译
```bash
# 创建构建目录
mkdir build
cd build

# 配置项目
cmake -G "Ninja" ..

# 编译
ninja
```

### 烧录

编译完成后，将生成的UF2文件复制到Pico的BOOTSEL模式：
- `ds3231_clock.uf2` - DS3231数字时钟OLED显示程序

## 使用方法

1. 按照上述连接图连接硬件
2. 将`ds3231_clock.uf2`程序烧录到Pico
3. OLED显示屏将显示：
   - **顶部区域（黄色）**：第一行显示日期，第二行显示星期和温度
   - **主区域（蓝色）**：大型7段数码管时间显示（HH:MM:SS）
4. 显示屏每秒更新一次
5. 温度从DS3231的内置传感器读取

## 显示布局

```
┌─────────────────────────┐
│ 2025-09-08              │ ← 黄色区域：日期
│ Tue        25.8°C      │ ← 黄色区域：星期 + 温度
├─────────────────────────┤
│                         │
│   19:14:16             │ ← 蓝色区域：7段数码管时间显示
│                         │
└─────────────────────────┘
```

## 串口输出示例

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

## 项目结构

```
ssd1306/
├── CMakeLists.txt          # CMake构建配置
├── build_pico.bat          # Windows构建脚本
├── include/                # 头文件目录
│   ├── ssd1306.h          # SSD1306 OLED驱动头文件
│   └── ds3231/            # DS3231驱动头文件
│       └── ds3231.h       # DS3231 RTC驱动头文件
├── src/                   # 源代码目录
│   ├── ssd1306.cpp        # SSD1306 OLED驱动实现
│   └── ds3231/            # DS3231驱动源代码
│       └── ds3231_driver.cpp # DS3231 RTC驱动实现
├── examples/              # 示例程序目录
│   └── ds3231_clock.cpp   # 主DS3231数字时钟程序
└── README.md              # 项目文档
```

## 技术规格

- **OLED分辨率**：128x64像素
- **OLED字体**：5x7像素内置字体
- **7段显示**：14x28像素自定义字体
- **I2C频率**：400kHz（SSD1306），100kHz（DS3231）
- **时间精度**：±2ppm（DS3231 RTC）
- **温度范围**：-40°C到+85°C（DS3231传感器）
- **更新频率**：1秒
- **功耗**：低功耗运行

## 更新日志

### v3.0.0（最新）
- **项目完全转型**：从电池监控转为DS3231数字时钟
- **DS3231 RTC集成**：精确的实时时钟和温度传感器
- **双色OLED显示**：黄色区域显示日期/星期/温度，蓝色区域显示时间
- **自定义7段字体**：优化的14x28像素数字时间显示
- **双I2C接口**：SSD1306使用I2C0（GP2/GP3），DS3231使用I2C1（GP4/GP5）
- **UI优化**：适当的间距和布局，提高可读性
- **移除WiFi**：完全独立运行
- **星期映射修复**：正确的DS3231星期显示（0=星期日，1=星期一，等）

### v2.0.0
- 基于Adafruit_SSD1306库重构SSD1306 OLED驱动
- 修复OLED显示雪花问题
- 清理重复字体定义
- 优化代码结构和内存管理

### v1.0.0
- 初始电池电压监控功能
- ADS1115 ADC支持
- 基本OLED显示功能

## 重要说明

1. 确保正确的I2C连接：SSD1306使用I2C0（GP2/GP3），DS3231使用I2C1（GP4/GP5）
2. 每个设备使用独立的GPIO引脚连接不同的I2C总线
3. DS3231需要备用电池以在断电时保持时间
4. 7段显示针对128x64 OLED分辨率进行了优化
5. 温度读数来自DS3231的内置传感器
6. 星期映射：0=星期日，1=星期一，2=星期二，...，6=星期六

## 许可证

本项目采用MIT许可证。详情请参阅LICENSE文件。