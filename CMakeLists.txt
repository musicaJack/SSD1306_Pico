cmake_minimum_required(VERSION 3.13)

# Pull in Raspberry Pi Pico SDK (must be defined before project)
# Adjust the path if your SDK is installed elsewhere
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Force the PICO_BOARD to pico (no WiFi needed)
set(PICO_BOARD pico CACHE STRING "Target board" FORCE)

project(LiVolt C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()


# 已删除的测试文件配置已移除：
# - wifi_ip_display.cpp
# - font_test.cpp  
# - digital_clock.cpp
# - digital_clock_enhanced.cpp
# - digital_clock_32px.cpp

# dual_color_clock已移除（包含WiFi代码，不再需要）

# 创建DS3231时钟版本的可执行文件
add_executable(ds3231_clock
    examples/ds3231_clock.cpp
    src/ssd1306.cpp
    src/ds3231/ds3231_driver.cpp
)

target_include_directories(ds3231_clock PRIVATE
    include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接Pico SDK库（包含I2C支持）
target_link_libraries(ds3231_clock
    pico_stdlib
    hardware_i2c
)

# 启用DS3231时钟版本的UART输出
pico_enable_stdio_usb(ds3231_clock 1)
pico_enable_stdio_uart(ds3231_clock 1)

# 创建UF2文件
# pico_add_extra_outputs(livolt_oled)  # 注释掉原版本
pico_add_extra_outputs(ds3231_clock) 
