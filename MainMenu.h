#ifndef MAINMENU_H
#define MAINMENU_H

#include "UIManager.h"
#include "SaveLoadSystem.h"
#include "TaskSystem.h"
#include "Player.h"

/**
 * @enum MenuChoice
 * @brief 主菜单选项。
 */
enum class MenuChoice {
    START_GAME = 1,  ///< 开始新游戏
    LOAD_GAME = 2,   ///< 读取存档
    SHOW_HELP = 3,   ///< 查看帮助
    EXIT_GAME = 4    ///< 退出游戏
};

/**
 * @class MainMenu
 * @brief 主菜单：展示 UI、读取输入、分发到对应流程。
 */
class MainMenu {
private:
    UIManager ui; ///< 本地 UI 管理器

    /**
     * @brief 输出彩色 Logo 与装饰。
     */
    void displayLogo() const;

    /**
     * @brief 输出菜单选项列表。
     */
    void displayMenuOptions() const;

    /**
     * @brief 获取玩家选择（1-4），并校验输入。
     * @return MenuChoice 有效选择。
     */
    MenuChoice getPlayerChoice() const;

    /**
     * @brief 显示帮助页面并等待返回。
     */
    void showHelp() const;

    /**
     * @brief 处理存档读取的封装入口。
     * @param player 玩家对象（输出）
     * @param tasks 任务系统（输出）
     * @return true 加载成功；false 失败。
     */
    bool handleLoadGame(Player& player, TaskSystem& tasks) const;

public:
    /**
     * @brief 构造主菜单对象。
     */
    MainMenu();

    /**
     * @brief 启动主菜单循环。
     * @param player 玩家对象（供读档填充）
     * @param tasks 任务系统（供读档填充）
     * @return MenuChoice 退出原因（开始/读取/退出）。
     */
    MenuChoice run(Player& player, TaskSystem& tasks);
};

#endif // MAINMENU_H
