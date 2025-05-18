@echo off

echo 正在编译Z++编译器...

if not exist "%~dp0compiler.cpp" (
    echo 错误: 找不到编译器源文件 %~dp0compiler.cpp
    exit /b 1
)

g++ "%~dp0compiler.cpp" -o "%~dp0z++.exe"

if %errorlevel% neq 0 (
    echo 编译失败，错误代码: %errorlevel%
    echo 请检查:
    echo 1. 是否安装了g++编译器
    echo 2. 编译器源文件是否存在
    echo 3. 是否有足够的权限
    exit /b %errorlevel%
) else (
    echo 编译成功，生成文件: %~dp0z++.exe
echo 完整路径: %~dp0z++.exe
echo 可以使用 z++ 命令运行编译器
)