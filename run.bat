@echo off

REM 编译main.js为main.c
.\bin\qjsc.exe -e -o .\main.c .\main.js

REM 等待编译成功
IF %ERRORLEVEL% NEQ 0 (
    echo complie main.js failed
    exit /b
)

REM 编译main.c为main.exe 参数隐藏控制台： 
gcc .\main.c -L.\lib\quickjs -I.\include\quickjs -lquickjs -o .\main.exe  -fexec-charset=utf-8 -finput-charset=utf-8 -mwindows
REM 等待编译成功
IF %ERRORLEVEL% NEQ 0 (
    echo complie main.c failed
    exit /b
)

echo complie success at ./main.exe
echo.
REM 执行main.exe
.\main.exe
