@echo off
echo 编译游戏...
g++ -g -Wall -std=c++17 -I. *.cpp -o game_cre.exe
if %errorlevel% equ 0 (
    echo 编译成功！
    echo 运行 game_cre.exe 来开始游戏
) else (
    echo 编译失败！
    pause
)