/**
 * @file MainMenu.cpp
 * @brief 实现主菜单界面的展示与交互逻辑：显示logo/菜单、输入校验、帮助页、读档入口与循环调度。
 * @note 终端颜色使用 ANSI 序列，Windows 控制台需启用 VT 模式或使用支持的终端。
 */

#include "MainMenu.h"
#include <iostream>
#include <limits>

/**
 * @brief 构造主菜单对象。
 */
MainMenu::MainMenu() {}

/**
 * @brief 展示彩色 Logo 与装饰性标题、边框。
 * @details 使用 256 色 ANSI 转义码与 Unicode 装饰字符。
 */
void MainMenu::displayLogo() const {
    std::cout << "\033[38;5;39m";
    std::cout << "  ██╗    ██╗ █████╗ ███╗   ██╗ ██████╗ ███████╗██╗" << std::endl;
    std::cout << "  ██║    ██║██╔══██╗████╗  ██║██╔════╝ ██╔════╝██║" << std::endl;
    std::cout << "  ██║ █╗ ██║███████║██╔██╗ ██║██║  ███╗███████╗██║" << std::endl;
    std::cout << "  ██║███╗██║██╔══██║██║╚██╗██║██║   ██║╚════██║██║" << std::endl;
    std::cout << "  ╚███╔███╔╝██║  ██║██║ ╚████║╚██████╔╝███████║██║" << std::endl;
    std::cout << "   ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝" << std::endl;
    std::cout << "\033[0m" << std::endl;

    std::cout << "\033[38;5;208m";
    std::cout << "   ████████╗██╗  ██╗███████╗    ██████╗ ██████╗ ██╗███╗   ██╗ ██████╗███████╗" << std::endl;
    std::cout << "   ╚══██╔══╝██║  ██║██╔════╝    ██╔══██╗██╔══██╗██║████╗  ██║██╔════╝██╔════╝" << std::endl;
    std::cout << "      ██║   ███████║█████╗      ██████╔╝██████╔╝██║██╔██╗ ██║██║     █████╗  " << std::endl;
    std::cout << "      ██║   ██╔══██║██╔══╝      ██╔═══╝ ██╔══██╗██║██║╚██╗██║██║     ██╔══╝  " << std::endl;
    std::cout << "      ██║   ██║  ██║███████╗    ██║     ██║  ██║██║██║ ╚████║╚██████╗███████╗" << std::endl;
    std::cout << "      ╚═╝   ╚═╝  ╚═╝╚══════╝    ╚═╝     ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝" << std::endl;
    std::cout << "\033[0m" << std::endl;

    std::cout << "\033[38;5;196m";
    std::cout << "              ██████╗ ███████╗██╗   ██╗███████╗███╗   ██╗ ██████╗ ███████╗" << std::endl;
    std::cout << "              ██╔══██╗██╔════╝██║   ██║██╔════╝████╗  ██║██╔════╝ ██╔════╝" << std::endl;
    std::cout << "              ██████╔╝█████╗  ██║   ██║█████╗  ██╔██╗ ██║██║  ███╗█████╗  " << std::endl;
    std::cout << "              ██╔══██╗██╔══╝  ╚██╗ ██╔╝██╔══╝  ██║╚██╗██║██║   ██║██╔══╝  " << std::endl;
    std::cout << "              ██║  ██║███████╗ ╚████╔╝ ███████╗██║ ╚████║╚██████╔╝███████╗" << std::endl;
    std::cout << "              ╚═╝  ╚═╝╚══════╝  ╚═══╝  ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝" << std::endl;
    std::cout << "\033[0m" << std::endl;

    std::cout << "\033[38;5;226m";
    std::cout << "                            ——— 从一把剑开始 ———" << std::endl;
    std::cout << "\033[0m" << std::endl;

    std::cout << "\033[38;5;33m";
    std::cout << "      ╔══════════════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "      ║                          ⚔️  MUD 文字冒险游戏 v1.0  ⚔️                         ║" << std::endl;
    std::cout << "      ╚══════════════════════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << "\033[0m" << std::endl;
}

/**
 * @brief 展示主菜单选项（开始/读取/帮助/退出）。
 */
void MainMenu::displayMenuOptions() const {
    std::cout << "\033[38;5;255m";
    std::cout << "                    ┌─────────────────────────────────────────────┐" << std::endl;
    std::cout << "                    │                  🎮 主菜单 🎮               │" << std::endl;
    std::cout << "                    ├─────────────────────────────────────────────┤" << std::endl;
    std::cout << "\033[38;5;46m";
    std::cout << "                    │  [1] ⚔️  开始新的冒险                        │" << std::endl;
    std::cout << "\033[38;5;51m";
    std::cout << "                    │  [2] 📂 读取存档                            │" << std::endl;
    std::cout << "\033[38;5;214m";
    std::cout << "                    │  [3] ❓ 查看帮助                            │" << std::endl;
    std::cout << "\033[38;5;196m";
    std::cout << "                    │  [4] 🚪 退出游戏                            │" << std::endl;
    std::cout << "\033[38;5;255m";
    std::cout << "                    └─────────────────────────────────────────────┘" << std::endl;
    std::cout << "\033[0m" << std::endl;
}

/**
 * @brief 获取玩家菜单选择并进行输入校验。
 * @return MenuChoice 玩家有效选择（1-4）。
 * @details 非法输入将清理输入流并重试，防止后续 getline 读取残留换行。
 */
MenuChoice MainMenu::getPlayerChoice() const {
    int choice = 0;
    while (true) {
        std::cout << "\033[38;5;220m请选择 (1-4): \033[0m";
        if (std::cin >> choice && choice >= 1 && choice <= 4) {
            // 丢弃本行剩余内容（避免影响后续等待回车）
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return static_cast<MenuChoice>(choice);
        } else {
            std::cout << "\033[38;5;196m无效选择，请重新输入！\033[0m" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

/**
 * @brief 展示“游戏帮助”页面，并等待回车返回主菜单。
 */
void MainMenu::showHelp() const {
    std::cout << "\033[2J\033[H";
    std::cout << "\033[38;5;226m";
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                                   🎯 游戏帮助 🎯                             ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << "\033[0m" << std::endl;

    ui.displayMessage("📖 游戏背景:", UIManager::Color::CYAN);
    ui.displayMessage("你是安特国的王子，王国被邪恶势力摧毁。现在你手持神剑，踏上复仇之路。", UIManager::Color::WHITE);
    ui.displayMessage("收集六誓圣辉套装，解锁强大技能，最终击败万恶枢机，拯救世界！", UIManager::Color::WHITE);
    std::cout << std::endl;

    ui.displayMessage("🎮 基础操作:", UIManager::Color::CYAN);
    ui.displayMessage("• 移动: go <方向> 或直接输入方向 (如: go 东北, 东北, 2)", UIManager::Color::WHITE);
    ui.displayMessage("• 对话: npc 或 对话 - 与当前区域NPC交流", UIManager::Color::WHITE);
    ui.displayMessage("• 战斗: fight 或 战斗 - 挑战区域内的敌人", UIManager::Color::WHITE);
    ui.displayMessage("• 查看: look 或 观察 - 查看当前区域信息", UIManager::Color::WHITE);
    std::cout << std::endl;

    ui.displayMessage("⚔️ 战斗系统:", UIManager::Color::CYAN);
    ui.displayMessage("• 战斗中可以选择攻击、技能、道具或逃跑", UIManager::Color::WHITE);
    ui.displayMessage("• 技能有MP消耗，增益技能持续2回合", UIManager::Color::WHITE);
    ui.displayMessage("• 药水可以在战斗中使用且不消耗回合", UIManager::Color::WHITE);
    std::cout << std::endl;

    ui.displayMessage("📋 任务系统:", UIManager::Color::CYAN);
    ui.displayMessage("• task list - 查看可接任务", UIManager::Color::WHITE);
    ui.displayMessage("• task accept <ID> - 接取任务", UIManager::Color::WHITE);
    ui.displayMessage("• task submit <ID> - 提交完成的任务", UIManager::Color::WHITE);
    std::cout << std::endl;

    ui.displayMessage("🎒 物品管理:", UIManager::Color::CYAN);
    ui.displayMessage("• pick <物品名> - 拾取物品", UIManager::Color::WHITE);
    ui.displayMessage("• use <物品名> - 使用道具", UIManager::Color::WHITE);
    ui.displayMessage("• wear <装备名> - 穿戴装备", UIManager::Color::WHITE);
    ui.displayMessage("• shop - 进入商店购买道具", UIManager::Color::WHITE);
    std::cout << std::endl;

    ui.displayMessage("💾 存档系统:", UIManager::Color::CYAN);
    ui.displayMessage("• save - 手动存档", UIManager::Color::WHITE);
    ui.displayMessage("• load - 读取存档", UIManager::Color::WHITE);
    ui.displayMessage("• 重要位置会自动存档", UIManager::Color::WHITE);
    std::cout << std::endl;

    ui.displayMessage("📖 更多命令:", UIManager::Color::CYAN);
    ui.displayMessage("• help - 查看所有命令", UIManager::Color::WHITE);
    ui.displayMessage("• skill - 查看技能信息", UIManager::Color::WHITE);
    ui.displayMessage("• map - 查看地图", UIManager::Color::WHITE);
    ui.displayMessage("• status - 查看角色状态", UIManager::Color::WHITE);
    std::cout << std::endl;

    std::cout << "\033[38;5;220m按 Enter 键返回主菜单...\033[0m";
    std::string dummy;
    std::getline(std::cin, dummy);
}

/**
 * @brief 处理加载存档的流程，封装 SaveLoadSystem 调用。
 * @param player 玩家对象（输出参数）
 * @param tasks 任务系统（输出参数）
 * @return true 加载成功；false 加载失败。
 * @note 本方法为 const，但 SaveLoadSystem 需要非 const UIManager，因此使用 const_cast。
 */
bool MainMenu::handleLoadGame(Player& player, TaskSystem& tasks) const {
    SaveLoadSystem saveLoad(const_cast<UIManager&>(ui));
    return saveLoad.loadGame(player, tasks);
}

/**
 * @brief 主菜单循环：处理用户选择并分发。
 * @param player 玩家对象（供读档填充）
 * @param tasks 任务系统（供读档填充）
 * @return MenuChoice 结束循环的选项（开始、读取或退出）。
 */
MenuChoice MainMenu::run(Player& player, TaskSystem& tasks) {
    while (true) {
        std::cout << "\033[2J\033[H"; // 清屏
        displayLogo();
        displayMenuOptions();

        MenuChoice choice = getPlayerChoice();

        switch (choice) {
            case MenuChoice::START_GAME:
                ui.displayMessage("🌟 开始新的冒险！愿光明指引你的道路...", UIManager::Color::GREEN);
                std::cout << std::endl;
                std::cout << "\033[38;5;220m按 Enter 键开始游戏...\033[0m";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return MenuChoice::START_GAME;

            case MenuChoice::LOAD_GAME:
                if (handleLoadGame(player, tasks)) {
                    ui.displayMessage("📂 存档加载成功！继续你的冒险...", UIManager::Color::GREEN);
                    std::cout << std::endl;
                    std::cout << "\033[38;5;220m按 Enter 键继续游戏...\033[0m";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return MenuChoice::LOAD_GAME;
                } else {
                    ui.displayMessage("❌ 加载存档失败，请重新选择。", UIManager::Color::RED);
                    std::cout << std::endl;
                    std::cout << "\033[38;5;220m按 Enter 键返回主菜单...\033[0m";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                break;

            case MenuChoice::SHOW_HELP:
                showHelp();
                break;

            case MenuChoice::EXIT_GAME:
                std::cout << "\033[38;5;208m";
                std::cout << "感谢游玩《王子复仇记》！" << std::endl;
                std::cout << "⚔️ 愿荣耀与你同在，安特王子！ ⚔️" << std::endl;
                std::cout << "\033[0m" << std::endl;
                return MenuChoice::EXIT_GAME;
        }
    }
}
 */
MenuChoice MainMenu::run(Player& player, TaskSystem& tasks) {
    while (true) {
        std::cout << "\033[2J\033[H"; // 清屏
        displayLogo();
        displayMenuOptions();
        
        MenuChoice choice = getPlayerChoice();
        
        switch (choice) {
            case MenuChoice::START_GAME:
                ui.displayMessage("🌟 开始新的冒险！愿光明指引你的道路...", UIManager::Color::GREEN);
                std::cout << std::endl;
                std::cout << "\033[38;5;220m按 Enter 键开始游戏...\033[0m";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return MenuChoice::START_GAME;
                
            case MenuChoice::LOAD_GAME:
                if (handleLoadGame(player, tasks)) {
                    ui.displayMessage("📂 存档加载成功！继续你的冒险...", UIManager::Color::GREEN);
                    std::cout << std::endl;
                    std::cout << "\033[38;5;220m按 Enter 键继续游戏...\033[0m";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return MenuChoice::LOAD_GAME;
                } else {
                    ui.displayMessage("❌ 加载存档失败，请重新选择。", UIManager::Color::RED);
                    std::cout << std::endl;
                    std::cout << "\033[38;5;220m按 Enter 键返回主菜单...\033[0m";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                break;
                
            case MenuChoice::SHOW_HELP:
                showHelp();
                break;
                
            case MenuChoice::EXIT_GAME:
                std::cout << "\033[38;5;208m"; // 橙色
                std::cout << "感谢游玩《王子复仇记》！" << std::endl;
                std::cout << "⚔️ 愿荣耀与你同在，安特王子！ ⚔️" << std::endl;
                std::cout << "\033[0m" << std::endl;
                return MenuChoice::EXIT_GAME;
        }
    }
}
