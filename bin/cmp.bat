@echo off

echo ���ڱ���Z++������...

if not exist "%~dp0compiler.cpp" (
    echo ����: �Ҳ���������Դ�ļ� %~dp0compiler.cpp
    exit /b 1
)

g++ "%~dp0compiler.cpp" -o "%~dp0z++.exe"

if %errorlevel% neq 0 (
    echo ����ʧ�ܣ��������: %errorlevel%
    echo ����:
    echo 1. �Ƿ�װ��g++������
    echo 2. ������Դ�ļ��Ƿ����
    echo 3. �Ƿ����㹻��Ȩ��
    exit /b %errorlevel%
) else (
    echo ����ɹ��������ļ�: %~dp0z++.exe
echo ����·��: %~dp0z++.exe
echo ����ʹ�� z++ �������б�����
)