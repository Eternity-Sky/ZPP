@echo off
chcp 65001 >nul

:: 检查是否以管理员权限运行
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo 请以管理员权限运行此脚本
    pause
    exit /b
)

:: 获取脚本所在目录路径
for %%I in ("%~dp0.") do set "SCRIPT_DIR=%%~fI"

:: 检查当前目录是否是有效的项目目录
if not exist "%SCRIPT_DIR%\src\compiler.cpp" (
    echo 错误: 未找到有效的Z++项目目录
    echo 脚本所在目录: %SCRIPT_DIR%
    echo 请确保:
    echo 1. 脚本在项目根目录运行
    echo 2. 项目包含src/compiler.cpp文件
    echo 3. 从GitHub仓库下载完整项目
    pause
    exit /b
)

:: 检查项目目录是否已在PATH中
echo 正在检查PATH环境变量...
echo %PATH% | find /i "%SCRIPT_DIR%" >nul
if %errorLevel% equ 0 (
    echo 项目目录已在PATH环境变量中
    echo 当前PATH值: %PATH%
    pause
    exit /b
)

:: 更新PATH环境变量
echo 正在更新PATH环境变量...
echo 原PATH值: %PATH%
setx PATH "%PATH%;%SCRIPT_DIR%" >nul 2>&1
if %errorLevel% neq 0 (
    echo 错误: 更新PATH环境变量失败
    echo 请尝试以下解决方案:
    echo 1. 手动将当前目录(%cd%)添加到PATH中
    echo 2. 以管理员权限重新运行此脚本
    pause
    exit /b
)

echo 安装成功！
echo 新PATH值: %PATH%;%SCRIPT_DIR%
pause