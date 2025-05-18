@echo off

:: 检查是否以管理员身份运行
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo 请以管理员身份运行此脚本
    pause
    exit /b
)

:: 检查z++.exe是否存在，不存在则从GitHub下载
if not exist z++.exe (
    echo 正在从GitHub下载z++.exe...
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/Eternity-Sky/ZPP/raw/main/z++.exe' -OutFile 'z++.exe'"
    if %errorLevel% neq 0 (
        echo 下载z++.exe失败
        pause
        exit /b
    )
)

:: 复制z++.exe到系统目录
copy /Y z++.exe "%SystemRoot%\System32\z++.exe"
if %errorLevel% neq 0 (
    echo 复制文件失败
    pause
    exit /b
)

:: 添加PATH环境变量
setx PATH "%PATH%;%SystemRoot%\System32" /M
if %errorLevel% neq 0 (
    echo 添加PATH失败
    pause
    exit /b
)

echo 安装成功！
pause