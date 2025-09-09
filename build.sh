#!/bin/bash

# LiVolt 构建脚本
# 用于编译锂电池电压检测系统

echo "LiVolt - 锂电池电压检测系统构建脚本"
echo "======================================"

# 检查PICO_SDK_PATH环境变量
if [ -z "$PICO_SDK_PATH" ]; then
    echo "错误：未设置PICO_SDK_PATH环境变量"
    echo "请设置PICO_SDK_PATH指向您的Pico SDK安装目录"
    echo "例如：export PICO_SDK_PATH=/path/to/pico-sdk"
    exit 1
fi

echo "PICO_SDK_PATH: $PICO_SDK_PATH"

# 创建构建目录
if [ ! -d "build" ]; then
    echo "创建构建目录..."
    mkdir build
fi

# 进入构建目录
cd build

# 配置项目
echo "配置项目..."
cmake ..

# 检查CMake配置是否成功
if [ $? -ne 0 ]; then
    echo "错误：CMake配置失败"
    exit 1
fi

# 编译项目
echo "编译项目..."
make -j$(nproc)

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo ""
    echo "编译成功！"
    echo "生成的文件："
    ls -la *.uf2 *.elf *.map 2>/dev/null || echo "未找到输出文件"
    echo ""
    echo "烧录说明："
    echo "1. 按住BOOTSEL按钮并插入USB线"
    echo "2. 将生成的livolt.uf2文件复制到RPI-RP2设备"
    echo "3. 设备将自动重启并运行程序"
else
    echo "错误：编译失败"
    exit 1
fi 