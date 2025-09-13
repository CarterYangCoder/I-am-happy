#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <optional>
#include <memory> // 添加头文件支持智能指针
 
// 包含所有依赖的自定义类的头文件
#include "UIManager.h"
#include "BossWanEshuji.h"
#include "CombatSystem.h"
#include "TaskSystem.h"
#include "ShopSystem.h"
#include "CommandParser.h"
#include "SaveLoadSystem.h"
#include "Player.h"
#include "Map.h"
#include "NPC.h"
// 游戏状态枚举
enum class GameState {
    Exploring,
    InShop,
    InCombat
};

// Game类声明
class Game {
public:
    // 构造函数 - 支持读档启动
    Game(bool loadFromSave = false, Player* loadedPlayer = nullptr);
    // 运行游戏的主循环
    void run();

private:
    // 成员变量
    UIManager ui;
    Player player;
    Map gameMap;                // 游戏地图
    CombatSystem combat;
    TaskSystem tasks;
    ShopSystem shop;
    CommandParser parser;
    SaveLoadSystem saveLoad;
    bool isRunning;
    GameState currentState;
    bool startedFromSave; // 是否从存档启动
    
    // 游戏内所有物品的数据库
    std::map<int, std::unique_ptr<Item>> itemDb;
    
    // 命令处理相关
    using CommandHandler = std::function<void(const std::vector<std::string>&)>;
    std::map<std::string, CommandHandler> commandHandlers;
    std::map<std::string, std::string> commandAliases;

    // 最近操作物品及效果
    std::string lastActionMsg;

    // 私有辅助函数
    void setupCommandAliases();
    std::string getCanonicalCommand(std::string cmd);
    void registerCommands();
};

#endif // GAME_H