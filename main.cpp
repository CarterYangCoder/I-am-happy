#include "Game.h"
#include "MainMenu.h"
#include "TaskSystem.h"
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
        UIManager ui;
        MainMenu menu;
        Player player("安特王子");
        TaskSystem tasks(ui);
        
        MenuChoice choice = menu.run(player, tasks);
        
        if (choice == MenuChoice::EXIT_GAME) {
            return 0;
        }
        
        // 根据选择启动游戏
        bool loadFromSave = (choice == MenuChoice::LOAD_GAME);
        Game game(loadFromSave, loadFromSave ? &player : nullptr);
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "游戏运行时出错: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}