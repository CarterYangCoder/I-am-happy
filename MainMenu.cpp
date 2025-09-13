/*
 * 文件：MainMenu.cpp
 * 作用：实现主菜单界面的展示与交互逻辑，包括：
 *   - 展示 ASCII/ANSI 彩色 logo 与菜单选项
 *   - 获取玩家菜单选择并进行输入校验
 *   - 展示帮助页面
 *   - 处理存档读取
 *   - 主循环调度：根据玩家选择进入不同分支
 *
 * 说明：
 *   - 终端输出使用 ANSI 转义序列设置颜色与清屏（如：\033[38;5;39m 设置 256 色前景色，
 *     \033[2J\033[H 清屏并将光标移到左上角）。
 *   - UIManager 用于统一的彩色文本输出与格式处理。
 *   - SaveLoadSystem 负责与存档系统交互。
 *   - MenuChoice 为菜单选项枚举（1=开始，2=读取存档，3=帮助，4=退出）。
 *
 * 平台注意：
 *   - Windows 默认控制台需启用 VT 模式或使用支持 ANSI 的终端，方可正确显示颜色与装饰字符。
 */

#include "MainMenu.h"
#include <iostream>
#include <limits>

/**
 * 构造主菜单对象。
 * 当前构造函数无初始化逻辑，保留以便未来扩展。
 */
MainMenu::MainMenu() {}

/**
 * 展示彩色 Logo 与装饰性标题、边框。
 * 使用 256 色 ANSI 转义码与 Unicode 装饰字符，仅进行输出无副作用。
 */
void MainMenu::displayLogo() const {
    std::cout << "\033[2J\033[H"; // 清屏并将光标移到左上角
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

/**
 * 展示主菜单选项（开始游戏/读取存档/帮助/退出）。
 * 仅进行 UI 输出，无输入读取。
 */
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

/**
 * 循环读取并校验玩家菜单选择（1-4）。
 *
 * 返回：
 *   MenuChoice - 将玩家输入转换为对应的枚举值。
 *
 * 交互细节：
 *   - 提示输入范围（1-4）。
 *   - 对无效输入（非数字/越界）清理输入流并提示错误后重试。
 *   - 对有效输入后丢弃行尾剩余内容（避免后续 getline/ignore 受影响）。
 */
MenuChoice MainMenu::getPlayerChoice() const {
    int choice = 0;
    while (true) {
        std::cout << "\033[38;5;220m请选择 (1-4): \033[0m";
        // 尝试读取数字并检查范围
        if (std::cin >> choice && choice >= 1 && choice <= 4) {
            // 丢弃本行剩余内容（包含换行）
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return static_cast<MenuChoice>(choice);
        } else {
            // 输入失败：重置错误状态并清空至行尾，提示重试
            std::cout << "\033[38;5;196m无效选择，请重新输入！\033[0m" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

/**
 * 展示“游戏帮助”页面。
 * 使用 UIManager 以统一颜色输出说明，并在末尾等待用户按下回车返回。
 *
 * 交互细节：
 *   - 先清屏，再打印分区标题与说明。
 *   - 通过 getline 等待回车，保证按任意回车即可返回菜单。
 */
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

/**
 * 读取存档的统一入口。
 *
 * 参数：
 *   - player：玩家数据对象（将由存档系统填充）
 *   - tasks：任务系统对象（将由存档系统填充）
 *
 * 返回：
 *   - bool：true 表示读取成功，false 表示失败。
 *
 * 说明：
 *   - 方法本身为 const，但 SaveLoadSystem 需要非 const UIManager，
 *     这里通过 const_cast 将 ui 传入以显示加载过程中的 UI。
 */
bool MainMenu::handleLoadGame(Player& player, TaskSystem& tasks) const {
    SaveLoadSystem saveLoad(const_cast<UIManager&>(ui));
    return saveLoad.loadGame(player, tasks);
}

/**
 * 主菜单运行循环：
 *   - 清屏后展示 Logo 与菜单
 *   - 获取玩家选择
 *   - 根据选择进行处理（开始新游戏 / 读取存档 / 帮助 / 退出）
 *
 * 返回：
 *   - MenuChoice：返回导致主循环结束的选择（开始、读取或退出）。
 *
 * 交互细节：
 *   - 在开始游戏与读取成功等情形，提示“按 Enter 键继续”，通过 ignore 阻塞等待。
 *   - 读取失败时给出错误提示并返回主菜单。
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
