#include "Game.h"
#include "MainMenu.h"
#include "TaskSystem.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <locale>
#include <io.h>
#include <fcntl.h>
#endif

int main() {
    try {
#ifdef _WIN32
        // Windows系统特定设置
        try {
            // 设置控制台编码为UTF-8，支持中文显示
            SetConsoleCP(CP_UTF8);
            SetConsoleOutputCP(CP_UTF8);
            
            // 启用ANSI转义序列支持
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut != INVALID_HANDLE_VALUE) {
                DWORD dwMode = 0;
                if (GetConsoleMode(hOut, &dwMode)) {
                    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                    SetConsoleMode(hOut, dwMode);
                }
            }
            
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
            
            // 设置C++本地化
            std::locale::global(std::locale(""));
        } catch (const std::exception& e) {
            std::cerr << "控制台设置警告: " << e.what() << std::endl;
            std::cerr << "游戏可能显示异常，但仍会继续运行。" << std::endl;
        }
#else
        // Linux/macOS系统设置
        try {
            std::locale::global(std::locale("");
        } catch (const std::exception& e) {
            std::cerr << "本地化设置警告: " << e.what() << std::endl;
        }
#endif
        
        // 初始化游戏组件
        UIManager ui;
        MainMenu menu;
        Player player("安特王子");
        TaskSystem tasks(ui);
        
        // 运行主菜单
        MenuChoice choice = menu.run(player, tasks);
        
        if (choice == MenuChoice::EXIT_GAME) {
            return 0;
        }
        
        // 根据选择启动游戏
        bool loadFromSave = (choice == MenuChoice::LOAD_GAME);
        Game game(loadFromSave, loadFromSave ? &player : nullptr);
        game.run();
        
    } catch (const std::runtime_error& e) {
        std::cerr << "运行时错误: " << e.what() << std::endl;
        std::cerr << "请检查系统环境和文件权限。" << std::endl;
        std::cout << "按 Enter 键退出..." << std::endl;
        std::cin.get();
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "游戏运行时出错: " << e.what() << std::endl;
        std::cerr << "如果问题持续，请联系开发者。" << std::endl;
        std::cout << "按 Enter 键退出..." << std::endl;
        std::cin.get();
        return 1;
    } catch (...) {
        std::cerr << "发生未知错误，游戏将退出。" << std::endl;
        std::cout << "按 Enter 键退出..." << std::endl;
        std::cin.get();
        return 1;
    }
    
    return 0;
}