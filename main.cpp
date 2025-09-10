#include "Game.h"
#include <iostream>
#include <windows.h>

int main() {
    // 设置控制台编码为UTF-8，支持中文显示
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
     
    // 设置控制台字体为支持中文的字体
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 16;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    
    try {
        Game game;
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "游戏运行时出错: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}