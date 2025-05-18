@echo off

echo 正在编译Z++编译器...

g++ bin\compiler.cpp -o bin\z++.exe

if %errorlevel% neq 0 (
    echo 编译失败，错误代码: %errorlevel%
    exit /b %errorlevel%
) else (
    echo 编译成功，生成文件: bin\z++.exe
)