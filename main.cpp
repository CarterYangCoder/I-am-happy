/**
 * @file main.cpp
 * @brief 程序入口：设置控制台、创建依赖、执行主菜单与游戏主循环。
 */

#include "Game.h"
#include "MainMenu.h"
#include "TaskSystem.h"
#include <iostream>
#include <windows.h>

/**
 * @brief 程序入口。
 * @return int 进程返回码
 */
int main() {
    // 设置控制台编码为UTF-8，支持中文显示
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    // 设置控制台字体（Windows，避免中文乱码）
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

        // 运行主菜单，返回用户选择
        MenuChoice choice = menu.run(player, tasks);
        if (choice == MenuChoice::EXIT_GAME) return 0;

        // 根据选择启动游戏，支持读档模式传入已加载的player
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