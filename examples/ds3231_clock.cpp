#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "ssd1306.h"
#include "ds3231/ds3231.h"

// 硬件连接定义
#define SSD1306_I2C_PORT i2c0
#define SSD1306_SDA_PIN 2
#define SSD1306_SCL_PIN 3
#define DS3231_I2C_PORT i2c1
#define DS3231_SDA_PIN 4
#define DS3231_SCL_PIN 5

// 显示区域定义
#define YELLOW_HEIGHT 20
#define BLUE_HEIGHT 44

// 7段数码管模式定义
const uint8_t segment_patterns[10] = {
    0b1111110, // 0: abcdef
    0b0110000, // 1: bc
    0b1101101, // 2: abged
    0b1111001, // 3: abgcd
    0b0110011, // 4: fgbc
    0b1011011, // 5: afgcd
    0b1011111, // 6: afgcde
    0b1110000, // 7: abc
    0b1111111, // 8: abcdefg
    0b1111011  // 9: abcfgd
};

// 绘制7段数码管数字（缩小版本：14x28像素）
void draw7SegmentDigit(SSD1306& oled, int x, int y, int digit, bool highlight = false) {
    if (digit < 0 || digit > 9) return;
    
    uint8_t pattern = segment_patterns[digit];
    
    // 清除数字区域（缩小到14x28）
    oled.fillRect(x, y, 14, 28, SSD1306_BLACK);
    
    // 绘制各个段（按比例缩小）
    if (pattern & 0b1000000) { // a段 - 上横线
        oled.drawFastHLine(x + 1, y + 1, 12, SSD1306_WHITE);
    }
    if (pattern & 0b0100000) { // b段 - 右上竖线
        oled.drawFastVLine(x + 12, y + 3, 10, SSD1306_WHITE);
    }
    if (pattern & 0b0010000) { // c段 - 右下竖线
        oled.drawFastVLine(x + 12, y + 15, 10, SSD1306_WHITE);
    }
    if (pattern & 0b0001000) { // d段 - 下横线
        oled.drawFastHLine(x + 1, y + 25, 12, SSD1306_WHITE);
    }
    if (pattern & 0b0000100) { // e段 - 左下竖线
        oled.drawFastVLine(x + 1, y + 15, 10, SSD1306_WHITE);
    }
    if (pattern & 0b0000010) { // f段 - 左上竖线
        oled.drawFastVLine(x + 1, y + 3, 10, SSD1306_WHITE);
    }
    if (pattern & 0b0000001) { // g段 - 中横线
        oled.drawFastHLine(x + 1, y + 13, 12, SSD1306_WHITE);
    }
    
    // 添加高亮效果（边框）
    if (highlight) {
        oled.drawRect(x, y, 14, 28, SSD1306_WHITE);
    }
}

// 绘制冒号（缩小版本）
void drawColon(SSD1306& oled, int x, int y, bool blink = false) {
    static bool colon_state = false;
    if (blink) {
        colon_state = !colon_state;
    }
    
    if (colon_state || !blink) {
        // 上点（缩小）
        oled.fillCircle(x + 2, y + 8, 1, SSD1306_WHITE);
        // 下点（缩小）
        oled.fillCircle(x + 2, y + 20, 1, SSD1306_WHITE);
    }
}

// 绘制黄色区域内容（按方案三重新设计）
void drawYellowArea(SSD1306& oled, int year, int month, int day, int weekday, float temperature) {
    // 清除黄色区域
    oled.fillRect(0, 0, 128, YELLOW_HEIGHT, SSD1306_BLACK);
    
    // 第1行：显示日期（左对齐）
    char date_str[16];
    snprintf(date_str, sizeof(date_str), "%04d-%02d-%02d", year, month, day);
    oled.setCursor(2, 1);
    oled.setTextSize(1);
    oled.print(date_str);
    
    // 第2行：星期（左对齐）+ 温度（右对齐）
    // 修正DS3231的星期定义：0=Sunday, 1=Monday, 2=Tuesday, ..., 6=Saturday
    const char* weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    oled.setCursor(2, 9);
    oled.setTextSize(1);
    oled.print(weekdays[weekday]);
    
    // 温度显示在右侧
    char temp_str[12];
    snprintf(temp_str, sizeof(temp_str), "%.1f°C", temperature);
    oled.setCursor(90, 9); // 右对齐位置
    oled.setTextSize(1);
    oled.print(temp_str);
}

// 绘制蓝色区域内容（时间）
void drawBlueArea(SSD1306& oled, int hour, int minute, int second, bool colon_blink = true) {
    // 清除蓝色区域
    oled.fillRect(0, YELLOW_HEIGHT, 128, BLUE_HEIGHT, SSD1306_BLACK);
    
    // 重新计算时间显示位置（缩小字体后的布局）
    // 新布局：6个数字×14 + 2个冒号×4 + 间距 = 约100px
    // 起始位置: (128-100)/2 = 14，居中显示
    int start_x = 14;
    int y = YELLOW_HEIGHT + 8; // 蓝色区域内的垂直居中
    
    // 绘制小时 (HH)
    draw7SegmentDigit(oled, start_x, y, hour / 10);
    draw7SegmentDigit(oled, start_x + 16, y, hour % 10);
    
    // 绘制第一个冒号（小时和分钟之间，增加左右间距）
    drawColon(oled, start_x + 34, y + 2, false); // 左边间距2像素
    
    // 绘制分钟 (MM)
    draw7SegmentDigit(oled, start_x + 40, y, minute / 10); // 右边间距2像素
    draw7SegmentDigit(oled, start_x + 56, y, minute % 10);
    
    // 绘制第二个冒号（分钟和秒之间，增加左右间距）
    drawColon(oled, start_x + 70, y + 2, false); // 左边间距2像素
    
    // 绘制秒 (SS)
    draw7SegmentDigit(oled, start_x + 76, y, second / 10); // 右边间距2像素
    draw7SegmentDigit(oled, start_x + 92, y, second % 10);
}

// 移除装饰性背景
void drawBackground(SSD1306& oled) {
    // 移除所有装饰线条
}

// 绘制双色时钟
void drawDualColorClock(SSD1306& oled, ds3231_time_t& time, float temperature, bool colon_blink = true) {
    oled.clearDisplay();
    
    // 绘制背景装饰
    drawBackground(oled);
    
    // 绘制黄色区域（日期、温度和星期）
    drawYellowArea(oled, 2000 + time.year, time.month, time.date, time.day, temperature);
    
    // 绘制蓝色区域（时间）
    drawBlueArea(oled, time.hours, time.minutes, time.seconds, colon_blink);
    
    oled.display();
}

int main() {
    stdio_init_all();
    sleep_ms(2000);
    
    printf("=====================================\n");
    printf("    DS3231 双色数字时钟\n");
    printf("=====================================\n");
    printf("硬件连接：\n");
    printf("SSD1306 VCC → Pico W 3.3V\n");
    printf("SSD1306 GND → Pico W GND\n");
    printf("SSD1306 SCL → Pico W GP3 (I2C0 SCL)\n");
    printf("SSD1306 SDA → Pico W GP2 (I2C0 SDA)\n");
    printf("SSD1306 I2C地址: 0x%02X\n", SSD1306::ADDRESS);
    printf("\n");
    printf("DS3231 VCC → Pico W 3.3V\n");
    printf("DS3231 GND → Pico W GND\n");
    printf("DS3231 SCL → Pico W GP3 (I2C1 SCL)\n");
    printf("DS3231 SDA → Pico W GP2 (I2C1 SDA)\n");
    printf("DS3231 I2C地址: 0x%02X\n", DS3231_I2C_ADDR);
    printf("=====================================\n\n");
    
    // 初始化I2C0用于SSD1306
    printf("初始化I2C0接口（SSD1306）...\n");
    i2c_init(i2c0, 400000); // SSD1306可以使用更高频率
    gpio_set_function(SSD1306_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SSD1306_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SSD1306_SDA_PIN);
    gpio_pull_up(SSD1306_SCL_PIN);
    sleep_ms(100);
    
    // 初始化I2C1用于DS3231
    printf("初始化I2C1接口（DS3231）...\n");
    i2c_init(i2c1, 100000); // DS3231使用100kHz更稳定
    gpio_set_function(DS3231_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(DS3231_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(DS3231_SDA_PIN);
    gpio_pull_up(DS3231_SCL_PIN);
    sleep_ms(100);
    
    // 初始化OLED
    printf("初始化OLED显示屏...\n");
    SSD1306 oled(i2c0, SSD1306::ADDRESS);
    if (!oled.begin()) {
        printf("错误：无法初始化SSD1306，请检查连接\n");
        return -1;
    }
    printf("SSD1306初始化成功！\n");
    oled.setContrast(0x8F);
    
    // 初始化DS3231
    printf("初始化DS3231实时时钟...\n");
    ds3231_t ds3231;
    if (!ds3231_init(&ds3231, i2c1, DS3231_SDA_PIN, DS3231_SCL_PIN)) {
        printf("错误：无法初始化DS3231，请检查连接\n");
        return -1;
    }
    printf("DS3231初始化成功！\n");
    
    // 检查振荡器状态
    bool oscillator_stopped;
    if (ds3231_is_oscillator_stopped(&ds3231, &oscillator_stopped)) {
        if (oscillator_stopped) {
            printf("警告：DS3231振荡器已停止，时间可能不准确\n");
        } else {
            printf("DS3231振荡器运行正常\n");
        }
    }
    
    // 显示启动画面
    printf("显示启动信息到屏幕...\n");
    oled.clearDisplay();
    oled.setCursor(10, 8);
    oled.setTextSize(1);
    oled.print("DS3231 Clock");
    oled.setCursor(20, 24);
    oled.print("Starting...");
    oled.display();
    sleep_ms(2000);
    
    // 读取并显示当前时间
    ds3231_time_t current_time;
    if (!ds3231_read_time(&ds3231, &current_time)) {
        printf("错误：无法读取DS3231时间\n");
        return -1;
    }
    
    char time_str[32];
    ds3231_time_to_string(&current_time, time_str, sizeof(time_str));
    printf("当前DS3231时间: %s\n", time_str);
    
    // 读取温度
    float temperature;
    if (ds3231_read_temperature(&ds3231, &temperature)) {
        printf("DS3231温度: %.1f°C\n", temperature);
    }
    
    printf("进入主循环...\n");
    
    uint8_t last_second = 255; // 确保第一次会更新显示
    
    while (true) {
        // 读取DS3231时间
        ds3231_time_t now;
        if (!ds3231_read_time(&ds3231, &now)) {
            printf("错误：无法读取DS3231时间\n");
            sleep_ms(1000);
            continue;
        }
        
        // 每秒更新一次显示
        if (now.seconds != last_second) {
            // 读取温度
            float temperature;
            if (!ds3231_read_temperature(&ds3231, &temperature)) {
                temperature = 0.0f; // 如果读取失败，使用默认值
            }
            
            // 添加调试输出
            printf("DS3231时间: %04d-%02d-%02d %02d:%02d:%02d (星期%d)\n", 
                   2000 + now.year, now.month, now.date, now.hours, now.minutes, now.seconds, now.day);
            printf("显示时间: 小时=%d(%d,%d), 分钟=%d(%d,%d), 秒=%d(%d,%d)\n",
                   now.hours, now.hours/10, now.hours%10,
                   now.minutes, now.minutes/10, now.minutes%10,
                   now.seconds, now.seconds/10, now.seconds%10);
            printf("温度: %.1f°C\n", temperature);
            
            drawDualColorClock(oled, now, temperature, true);
            last_second = now.seconds;
        }
        
        sleep_ms(100);
    }
    
    return 0;
}
