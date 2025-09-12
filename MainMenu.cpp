#include "MainMenu.h"
#include <iostream>
#include <limits>

MainMenu::MainMenu() {}

void MainMenu::displayLogo() const {
    std::cout << "\033[38;5;39m"; // 亮蓝色
    std::cout << "  ██╗    ██╗ █████╗ ███╗   ██╗ ██████╗ ███████╗██╗" << std::endl;
    std::cout << "  ██║    ██║██╔══██╗████╗  ██║██╔════╝ ██╔════╝██║" << std::endl;
    std::cout << "  ██║ █╗ ██║███████║██╔██╗ ██║██║  ███╗███████╗██║" << std::endl;
    std::cout << "  ██║███╗██║██╔══██║██║╚██╗██║██║   ██║╚════██║██║" << std::endl;
    std::cout << "  ╚███╔███╔╝██║  ██║██║ ╚████║╚██████╔╝███████║██║" << std::endl;
    std::cout << "   ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝" << std::endl;
    std::cout << "\033[0m" << std::endl;
    
    std::cout << "\033[38;5;208m"; // 橙色
    std::cout << "   ████████╗██╗  ██╗███████╗    ██████╗ ██████╗ ██╗███╗   ██╗ ██████╗███████╗" << std::endl;
    std::cout << "   ╚══██╔══╝██║  ██║██╔════╝    ██╔══██╗██╔══██╗██║████╗  ██║██╔════╝██╔════╝" << std::endl;
    std::cout << "      ██║   ███████║█████╗      ██████╔╝██████╔╝██║██╔██╗ ██║██║     █████╗  " << std::endl;
    std::cout << "      ██║   ██╔══██║██╔══╝      ██╔═══╝ ██╔══██╗██║██║╚██╗██║██║     ██╔══╝  " << std::endl;
    std::cout << "      ██║   ██║  ██║███████╗    ██║     ██║  ██║██║██║ ╚████║╚██████╗███████╗" << std::endl;
    std::cout << "      ╚═╝   ╚═╝  ╚═╝╚══════╝    ╚═╝     ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝" << std::endl;
    std::cout << "\033[0m" << std::endl;
    
    std::cout << "\033[38;5;196m"; // 红色
    std::cout << "              ██████╗ ███████╗██╗   ██╗███████╗███╗   ██╗ ██████╗ ███████╗" << std::endl;
    std::cout << "              ██╔══██╗██╔════╝██║   ██║██╔════╝████╗  ██║██╔════╝ ██╔════╝" << std::endl;
    std::cout << "              ██████╔╝█████╗  ██║   ██║█████╗  ██╔██╗ ██║██║  ███╗█████╗  " << std::endl;
    std::cout << "              ██╔══██╗██╔══╝  ╚██╗ ██╔╝██╔══╝  ██║╚██╗██║██║   ██║██╔══╝  " << std::endl;
    std::cout << "              ██║  ██║███████╗ ╚████╔╝ ███████╗██║ ╚████║╚██████╔╝███████╗" << std::endl;
    std::cout << "              ╚═╝  ╚═╝╚══════╝  ╚═══╝  ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝" << std::endl;
    std::cout << "\033[0m" << std::endl;
    
    // 副标题 - 居中对齐
    std::cout << "\033[38;5;226m"; // 黄色
    std::cout << "                            ——— 从一把剑开始 ———" << std::endl;
    std::cout << "\033[0m" << std::endl;
    
    // 装饰性边框 - 居中对齐
    std::cout << "\033[38;5;33m"; // 深蓝色
    std::cout << "      ╔══════════════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "      ║                          ⚔️  MUD 文字冒险游戏 v1.0  ⚔️                         ║" << std::endl;
    std::cout << "      ╚══════════════════════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << "\033[0m" << std::endl;
}

void MainMenu::displayMenuOptions() const {
    std::cout << "\033[38;5;255m"; // 白色
    std::cout << "                    ┌─────────────────────────────────────────────┐" << std::endl;
    std::cout << "                    │                  🎮 主菜单 🎮               │" << std::endl;
    std::cout << "                    ├─────────────────────────────────────────────┤" << std::endl;
    std::cout << "\033[38;5;46m"; // 绿色
    std::cout << "                    │  [1] ⚔️  开始新的冒险                        │" << std::endl;
    std::cout << "\033[38;5;51m"; // 青色
    std::cout << "                    │  [2] 📂 读取存档                            │" << std::endl;
    std::cout << "\033[38;5;214m"; // 橙色
    std::cout << "                    │  [3] ❓ 查看帮助                            │" << std::endl;
    std::cout << "\033[38;5;196m"; // 红色
    std::cout << "                    │  [4] 🚪 退出游戏                            │" << std::endl;
    std::cout << "\033[38;5;255m"; // 白色
    std::cout << "                    └─────────────────────────────────────────────┘" << std::endl;
    std::cout << "\033[0m" << std::endl;
}

MenuChoice MainMenu::getPlayerChoice() const {
    int choice = 0;
    while (true) {
        std::cout << "\033[38;5;220m请选择 (1-4): \033[0m";
        if (std::cin >> choice && choice >= 1 && choice <= 4) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return static_cast<MenuChoice>(choice);
        } else {
            std::cout << "\033[38;5;196m无效选择，请重新输入！\033[0m" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void MainMenu::showHelp() const {
    std::cout << "\033[2J\033[H"; // 清屏
    std::cout << "\033[38;5;226m"; // 黄色
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

bool MainMenu::handleLoadGame(Player& player, TaskSystem& tasks) const {
    SaveLoadSystem saveLoad(const_cast<UIManager&>(ui));
    return saveLoad.loadGame(player, tasks);
}

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
