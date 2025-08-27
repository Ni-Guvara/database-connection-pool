#!/bin/bash

# 检查当前目录
echo "当前目录: $(pwd)"

# 检查是否存在 build 目录
if [ ! -d "build" ]; then
    echo "创建 build 目录..."
    mkdir build
fi

# 进入 build 目录
cd build

# 清理 build 目录中的文件（但保留目录本身）
echo "清理 build 目录..."
rm -rf *

# 执行 cmake
echo "执行 cmake .."
cmake ..

# 执行 make
echo "执行 make"
make

# 返回上级目录
cd ..