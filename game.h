#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <optional>
#include <memory>

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

/**
 * @enum GameState
 * @brief 游戏状态：探索/商店/战斗。
 */
enum class GameState {
    Exploring,
    InShop,
    InCombat
};

/**
 * @class Game
 * @brief 游戏主控：负责主循环、命令注册与处理、场景/战斗/存档的组织与协调。
 */
class Game {
public:
    /**
     * @brief 构造函数。
     * @param loadFromSave 是否从存档启动
     * @param loadedPlayer 已加载的玩家（可空）
     */
    Game(bool loadFromSave = false, Player* loadedPlayer = nullptr);

    /** @brief 运行游戏主循环。 */
    void run();

private:
    UIManager ui;
    Player player;
    Map gameMap;
    CombatSystem combat;
    TaskSystem tasks;
    ShopSystem shop;
    CommandParser parser;
    SaveLoadSystem saveLoad;
    bool isRunning;
    GameState currentState;
    bool startedFromSave;

    std::map<int, std::unique_ptr<Item>> itemDb;

    using CommandHandler = std::function<void(const std::vector<std::string>&)>;
    std::map<std::string, CommandHandler> commandHandlers;
    std::map<std::string, std::string> commandAliases;

    std::string lastActionMsg;

    /** @brief 设置命令别名映射。 */
    void setupCommandAliases();
    /** @brief 将别名/大小写转换为标准命令键。 */
    std::string getCanonicalCommand(std::string cmd);
    /** @brief 注册所有命令与处理器。 */
    void registerCommands();
};

#endif // GAME_H