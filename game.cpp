#include "Game.h"
#include <memory>
#include <iostream>
#include <algorithm>
#include <windows.h>

bool is_digits_game(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}
 
// Game类的构造函数
Game::Game() :
    player("安特王子"),
    gameMap(),              // 初始化地图
    combat(ui),
    tasks(ui),
    shop(ui),
    saveLoad(ui),
    isRunning(true),
    currentState(GameState::Exploring)
{
    // 设置控制台编码以支持中文显示
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    // 同步玩家和地图的位置
    player.setCurrentRoomId(gameMap.getCurrentRoomId());
    
    // 初始化物品数据库
    itemDb[1] = std::make_unique<HealthPotion>();
    itemDb[2] = std::make_unique<EnergyPotion>();

    tasks.initializeTasks();
    shop.initializeShop();
    registerCommands();
    setupCommandAliases();
}

// 游戏主循环
void Game::run() {
    ui.displayMessage("欢迎来到MUD世界, " + player.getName() + "!", UIManager::Color::CYAN);
    ui.displayMessage("输入 'help' 或 '帮助' 查看可用命令。", UIManager::Color::YELLOW);
    
    // 显示初始房间信息
    gameMap.showInitialRoom();

    while (isRunning) {
        Command cmd = parser.getCommand();
        if (cmd.command.empty()) continue;

        std::string canonicalCmd = getCanonicalCommand(cmd.command);
        
        // 特殊处理中文方向命令
        if (cmd.command == "北" || cmd.command == "南" || cmd.command == "东" || cmd.command == "西" ||
            cmd.command == "东北" || cmd.command == "西南" || cmd.command == "东南" || cmd.command == "西北" ||
            cmd.command == "上" || cmd.command == "下") {
            // 将方向作为参数传递给go命令
            std::vector<std::string> args = {cmd.command};
            commandHandlers["go"](args);
        }
        else if (commandHandlers.count(canonicalCmd)) {
            commandHandlers[canonicalCmd](cmd.args);
        }
        else {
            ui.displayMessage("未知命令: " + cmd.command, UIManager::Color::RED);
        }
    }
}

// 设置命令别名
void Game::setupCommandAliases() {
    commandAliases["h"] = "help"; commandAliases["帮助"] = "help";commandAliases["?"] = "help";commandAliases["？"] = "help";
    commandAliases["st"] = "status"; commandAliases["状态"] = "status";
    commandAliases["l"] = "look"; commandAliases["观察"] = "look";
    commandAliases["f"] = "fight"; commandAliases["战斗"] = "fight";
    commandAliases["t"] = "task"; commandAliases["任务"] = "task";
    commandAliases["sh"] = "shop"; commandAliases["商店"] = "shop";
    commandAliases["b"] = "buy"; commandAliases["购买"] = "buy";
    commandAliases["sa"] = "save"; commandAliases["存档"] = "save";
    commandAliases["lo"] = "load"; commandAliases["读档"] = "load";
    commandAliases["q"] = "quit"; commandAliases["退出"] = "quit";
    commandAliases["le"] = "leave"; commandAliases["离开"] = "leave";
    commandAliases["m"] = "map"; commandAliases["地图"] = "map";
    commandAliases["g"] = "go"; commandAliases["前往"] = "go";
    commandAliases["n"] = "npc"; commandAliases["对话"] = "npc";
    // 方向别名 - 这些将被转换为go命令的参数
    commandAliases["北"] = "go"; commandAliases["南"] = "go"; 
    commandAliases["东"] = "go"; commandAliases["西"] = "go";
    commandAliases["东北"] = "go"; commandAliases["西南"] = "go";
    commandAliases["东南"] = "go"; commandAliases["西北"] = "go";
    commandAliases["上"] = "go"; commandAliases["下"] = "go";
}

// 将别名或大小写命令转换为标准命令
std::string Game::getCanonicalCommand(std::string cmd) {
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
    if (commandAliases.count(cmd)) {
        return commandAliases[cmd];
    }
    return cmd;
}

// 注册所有命令及其处理逻辑
void Game::registerCommands() {
    commandHandlers["help"] = [this](const auto& args) {
        ui.displayMessage("基础命令: help(h,帮助), status(st,状态), look(l,观察), quit(q,退出)", UIManager::Color::WHITE);
        ui.displayMessage("地图命令: map(m,地图) [all] - 查看当前区域或全局地图", UIManager::Color::CYAN);
        ui.displayMessage("移动命令: go <方向>(g,前往) - 如: go 东北, go 2(2=东北)", UIManager::Color::CYAN);
        ui.displayMessage("  方向编号: 1=北 2=东北 3=东 4=东南 5=南 6=西南 7=西 8=西北 9=上 0=下", UIManager::Color::GRAY);
        ui.displayMessage("交互命令: npc(n,对话) - 与当前区域NPC对话", UIManager::Color::CYAN);
        ui.displayMessage("战斗命令: fight(f,战斗) - 挑战当前区域的敌人", UIManager::Color::RED);
        ui.displayMessage("任务命令: task(t,任务) [list/accept <ID>/submit <ID>]", UIManager::Color::YELLOW);
        ui.displayMessage("物品命令: use <物品名> - 使用道具，如 use 生命药水", UIManager::Color::GREEN);
        ui.displayMessage("装备命令: wear <装备名> - 穿戴装备，如 wear 铁誓胸甲", UIManager::Color::GREEN);
        ui.displayMessage("商店命令: shop(sh,商店), buy(b,购买) <ID>[*数量], leave(le,离开)", UIManager::Color::GREEN);
        ui.displayMessage("存档命令: save(sa,存档), load(lo,读档)", UIManager::Color::MAGENTA);
        if (currentState == GameState::InShop) {
            ui.displayMessage("当前在商店中，可使用: buy, leave", UIManager::Color::YELLOW);
        }
    };    commandHandlers["status"] = [this](const auto& args) { ui.displayPlayerStatus(player); };

    commandHandlers["look"] = [this](const auto& args) { 
        gameMap.showCurrentRoom(); 
    };

    commandHandlers["map"] = [this](const auto& args) {
        if (args.empty()) {
            gameMap.showLocationMap();
        } else if (args[0] == "all" || args[0] == "全部") {
            gameMap.showGlobalMap();
        } else {
            ui.displayMessage("用法: map 或 map all", UIManager::Color::YELLOW);
        }
    };

    commandHandlers["go"] = [this](const auto& args) {
        if (args.empty()) {
            ui.displayMessage("用法: go <方向> (如：go 东北, go 2)", UIManager::Color::YELLOW);
            ui.displayMessage("方向编号: 1=北 2=东北 3=东 4=东南 5=南 6=西南 7=西 8=西北 9=上 0=下", UIManager::Color::GRAY);
            return;
        }
        
        if (gameMap.switchRoom(args[0], &player, &saveLoad, &tasks)) {
            // 同步玩家位置
            player.setCurrentRoomId(gameMap.getCurrentRoomId());
            ui.displayMessage("你移动到了新的区域。", UIManager::Color::GREEN);
        }
    };

    commandHandlers["npc"] = [this](const auto& args) {
        NPC* npc = gameMap.getCurrentRoomNPC();
        if (npc) {
            ui.displayMessage("与 " + npc->getName() + " 对话:", UIManager::Color::CYAN);
            npc->showDialogue();
            
            // 检查是否有任务
            std::string taskId = npc->getTaskID();
            if (!taskId.empty()) {
                Task* task = tasks.findTask(taskId);
                if (task && task->getStatus() == TaskStatus::UNACCEPTED) {
                    ui.displayMessage("", UIManager::Color::WHITE);
                    ui.displayMessage("【" + npc->getName() + " 向你发出委托】", UIManager::Color::YELLOW);
                    ui.displayMessage("任务名：" + task->getName(), UIManager::Color::WHITE);
                    ui.displayMessage("任务描述：" + task->getDescription(), UIManager::Color::GRAY);
                    ui.displayMessage("经验奖励：" + std::to_string(task->getExpReward()), UIManager::Color::YELLOW);
                    ui.displayMessage("金币奖励：" + std::to_string(task->getGoldReward()), UIManager::Color::YELLOW);
                    ui.displayMessage("", UIManager::Color::WHITE);
                    ui.displayMessage("是否接受此任务？输入 'task accept " + taskId + "' 来接受任务", UIManager::Color::CYAN);
                } else if (task && task->getStatus() == TaskStatus::COMPLETED) {
                    ui.displayMessage("你已完成了 " + npc->getName() + " 的委托，可输入 'task submit " + taskId + "' 提交任务获得奖励", UIManager::Color::GREEN);
                }
            }
        } else {
            ui.displayMessage("这里没有可以对话的NPC。", UIManager::Color::RED);
        }
    };

    commandHandlers["fight"] = [this](const auto& args) {
        if (currentState != GameState::Exploring) { 
            ui.displayMessage("你现在不能战斗。", UIManager::Color::RED); 
            return; 
        }

        // 确保战斗状态
        currentState = GameState::InCombat;

        // 检查是否有BOSS
        EvilGeneral* boss = gameMap.getCurrentRoomBoss();
        BossWanEshuji* finalBoss = gameMap.getCurrentRoomFinalBoss();
        
        if (finalBoss) {
            // 处理万恶枢机战斗
            ui.displayMessage("你面对的是万恶枢机！", UIManager::Color::MAGENTA);
            
            // 备份玩家关键状态
            int backup_hp = player.getHP();
            int backup_exp = player.getExp();
            int backup_gold = player.getGold();
            std::map<std::string, int> backup_inventory = player.inventory;
            
            BossWanEshuji original_boss = *finalBoss;
            BossWanEshuji current_boss = original_boss;

            while (true) {
                CombatResult result = combat.startCombat(player, current_boss, itemDb);
                
                switch (result) {
                case CombatResult::Victory:
                    tasks.updateTaskProgress(&player, "击败" + finalBoss->getName());
                    gameMap.removeDefeatedBoss();
                    ui.displayMessage("恭喜！你拯救了世界！", UIManager::Color::GREEN);
                    currentState = GameState::Exploring;
                    return;
                    
                case CombatResult::Escaped:
                    // 万恶枢机不允许逃跑，不应该到达这里
                    break;
                    
                case CombatResult::Defeat_Restart:
                    ui.displayMessage("准备重新挑战万恶枢机...", UIManager::Color::YELLOW);
                    player.setHP(backup_hp);
                    player.setExp(backup_exp);
                    player.setGold(backup_gold);
                    player.inventory = backup_inventory;
                    current_boss = original_boss;
                    continue;
                    
                case CombatResult::Defeat_Load:
                    if (saveLoad.loadGame(player, tasks)) {
                        gameMap.setCurrentRoomId(player.getCurrentRoomId());
                        ui.displayMessage("游戏加载成功！", UIManager::Color::GREEN);
                    }
                    currentState = GameState::Exploring;
                    return;
                    
                case CombatResult::Defeat_Exit:
                    isRunning = false;
                    return;
                    
                default:
                    continue;
                }
            }
        }
        else if (boss) {
            ui.displayMessage("你挑战了强敌 " + boss->getName() + "！", UIManager::Color::MAGENTA);
            
            // 备份玩家关键状态而不是整个对象
            int backup_hp = player.getHP();
            int backup_exp = player.getExp();
            int backup_gold = player.getGold();
            std::map<std::string, int> backup_inventory = player.inventory;
            
            EvilGeneral original_boss = *boss;
            EvilGeneral current_boss = original_boss;

            while (true) {
                CombatResult result = combat.startCombat(player, current_boss, itemDb);
                
                switch (result) {
                case CombatResult::Victory:
                    tasks.updateTaskProgress(&player, "击败" + boss->getName());
                    gameMap.removeDefeatedBoss();
                    ui.displayMessage("战斗胜利！继续你的冒险吧。", UIManager::Color::GREEN);
                    currentState = GameState::Exploring;  // 恢复探索状态
                    return; // 结束当前命令处理
                    
                case CombatResult::Escaped:
                    // 恢复玩家状态
                    player.setHP(backup_hp);
                    player.setExp(backup_exp);
                    player.setGold(backup_gold);
                    player.inventory = backup_inventory;
                    ui.displayMessage("你回到了战前的位置。", UIManager::Color::GRAY);
                    currentState = GameState::Exploring;  // 恢复探索状态
                    return; // 结束当前命令处理
                    
                case CombatResult::Defeat_Restart:
                    ui.displayMessage("准备重新挑战...", UIManager::Color::YELLOW);
                    // 恢复玩家状态
                    player.setHP(backup_hp);
                    player.setExp(backup_exp);
                    player.setGold(backup_gold);
                    player.inventory = backup_inventory;
                    current_boss = original_boss;
                    continue; // 继续循环，重新开始战斗
                    
                case CombatResult::Defeat_Load:
                    if (saveLoad.loadGame(player, tasks)) {
                        gameMap.setCurrentRoomId(player.getCurrentRoomId());
                    }
                    currentState = GameState::Exploring;  // 恢复探索状态
                    return;
                    
                case CombatResult::Defeat_Exit:
                    isRunning = false;
                    return;
                    
                default:
                    // 处理其他情况，包括Continue
                    continue;
                }
            }
            return; // 结束当前命令处理，避免继续检查普通敌人
        }

        // 检查是否有普通敌人
        CommonEnemy* enemy = gameMap.getRandomEnemy();
        if (enemy) {
            ui.displayMessage("一只 " + enemy->getName() + " 跳了出来!", UIManager::Color::MAGENTA);
            
            // 备份玩家关键状态而不是整个对象
            int backup_hp = player.getHP();
            int backup_exp = player.getExp();
            int backup_gold = player.getGold();
            std::map<std::string, int> backup_inventory = player.inventory;
            
            CommonEnemy original_enemy = *enemy;
            CommonEnemy current_enemy = original_enemy;

            while (true) {
                CombatResult result = combat.startCombat(player, current_enemy, itemDb);

                switch (result) {
                case CombatResult::Victory:
                    tasks.updateTaskProgress(&player, "击败" + enemy->getName());
                    gameMap.removeDefeatedEnemy(enemy);
                    
                    ui.displayMessage("战斗胜利！", UIManager::Color::GREEN);
                    currentState = GameState::Exploring;  // 恢复探索状态
                    break; // 改为break而不是return

                case CombatResult::Escaped:
                    // 恢复玩家状态
                    player.setHP(backup_hp);
                    player.setExp(backup_exp);
                    player.setGold(backup_gold);
                    player.inventory = backup_inventory;
                    ui.displayMessage("你回到了战前的位置。", UIManager::Color::GRAY);
                    currentState = GameState::Exploring;  // 恢复探索状态
                    break; // 改为break而不是return

                case CombatResult::Defeat_Restart:
                    ui.displayMessage("准备重新挑战...", UIManager::Color::YELLOW);
                    // 恢复玩家状态
                    player.setHP(backup_hp);
                    player.setExp(backup_exp);
                    player.setGold(backup_gold);
                    player.inventory = backup_inventory;
                    current_enemy = original_enemy;
                    break;

                case CombatResult::Defeat_Load:
                    if (saveLoad.loadGame(player, tasks)) {
                        gameMap.setCurrentRoomId(player.getCurrentRoomId());
                    }
                    currentState = GameState::Exploring;  // 恢复探索状态
                    return;

                case CombatResult::Defeat_Exit:
                    isRunning = false;
                    return;
                }
                
                // 如果是胜利或逃跑，跳出循环
                if (result == CombatResult::Victory || result == CombatResult::Escaped) {
                    break;
                }
            }
        } else {
            ui.displayMessage("这个区域暂时没有敌人。", UIManager::Color::GRAY);
            currentState = GameState::Exploring;  // 确保状态正确
        }
    };

    // 任务系统命令
    commandHandlers["task"] = [this](const auto& args) {
        if (args.empty()) {
            tasks.showPlayerTasks(player);
        } else if (args[0] == "list") {
            tasks.showTaskList(&player);
        } else if (args[0] == "accept" && args.size() > 1) {
            tasks.acceptTask(&player, args[1]);
        } else if (args[0] == "submit" && args.size() > 1) {
            tasks.submitTask(&player, args[1]);
        } else {
            ui.displayMessage("用法: task [list/accept <ID>/submit <ID>]", UIManager::Color::YELLOW);
        }
    };

    commandHandlers["shop"] = [this](const auto& args) {
        if (currentState == GameState::Exploring) {
            currentState = GameState::InShop;
            shop.displayShopItems();
        }
        else {
            ui.displayMessage("你已经在商店中。", UIManager::Color::YELLOW);
        }
    };

    commandHandlers["buy"] = [this](const auto& args) {
        if (currentState != GameState::InShop) {
            ui.displayMessage("你需要先进入商店。", UIManager::Color::RED);
            return;
        }
        if (args.empty()) {
            ui.displayMessage("用法: buy <物品ID> 或 buy <物品ID>*<数量>", UIManager::Color::YELLOW);
            return;
        }

        std::string input = args[0];
        size_t starPos = input.find('*');
        int id = 0;
        int quantity = 1;

        try {
            if (starPos != std::string::npos) {
                id = std::stoi(input.substr(0, starPos));
                quantity = std::stoi(input.substr(starPos + 1));
            } else {
                id = std::stoi(input);
            }
        }
        catch (const std::exception&) {
            ui.displayMessage("无效的输入格式。", UIManager::Color::RED);
            return;
        }

        if (quantity <= 0) {
            ui.displayMessage("数量必须大于0。", UIManager::Color::RED);
            return;
        }

        const Item* itemToBuy = shop.getItemById(id);
        if (itemToBuy) {
            shop.buyItem(player, *itemToBuy, quantity);
        }
        else {
            ui.displayMessage("商店中没有ID为 " + std::to_string(id) + " 的物品。", UIManager::Color::RED);
        }
    };

    commandHandlers["leave"] = [this](const auto& args) {
        if (currentState == GameState::InShop) {
            currentState = GameState::Exploring;
            ui.displayMessage("你离开了商店。", UIManager::Color::GREEN);
        }
        else {
            ui.displayMessage("你不在商店中。", UIManager::Color::YELLOW);
        }
    };

    commandHandlers["save"] = [this](const auto& args) {
        saveLoad.saveGame(player, tasks);
    };

    commandHandlers["load"] = [this](const auto& args) {
        if (saveLoad.loadGame(player, tasks)) {
            // 同步地图位置
            gameMap.setCurrentRoomId(player.getCurrentRoomId());
            ui.displayMessage("游戏加载成功！当前位置已更新。", UIManager::Color::GREEN);
            ui.displayPlayerStatus(player);
        } else {
            ui.displayMessage("加载游戏失败。", UIManager::Color::RED);
        }
    };

    commandHandlers["quit"] = [this](const auto& args) {
        isRunning = false;
        ui.displayMessage("感谢游玩，再见!", UIManager::Color::CYAN);
    };

    // 最近操作物品及效果
    commandHandlers["use"] = [this](const auto& args) {
        if (args.empty()) {
            ui.displayMessage("用法: use <物品名>", UIManager::Color::YELLOW);
            return;
        }
        std::string itemName = args[0];
        if (!player.getInventory().count(itemName) || player.getInventory().at(itemName) <= 0) {
            ui.displayMessage("背包中没有该物品。", UIManager::Color::RED);
            return;
        }
        if (itemName == "生命药水") {
            int healAmount = player.getMaxHP() * 0.5;
            player.heal(healAmount);
            player.useItem(itemName);
            lastActionMsg = "使用了生命药水，回复了 " + std::to_string(healAmount) + " 点生命值。";
            ui.displayMessage(lastActionMsg, UIManager::Color::GREEN);
        } else if (itemName == "能量药水") {
            player.extraActionTurns += 1;
            player.useItem(itemName);
            lastActionMsg = "使用了能量药水，下个回合可多行动一次。";
            ui.displayMessage(lastActionMsg, UIManager::Color::GREEN);
        } else {
            if (player.equipFromInventory(itemName)) {
                lastActionMsg = "成功穿戴装备: " + itemName;
                ui.displayMessage(lastActionMsg, UIManager::Color::GREEN);
            } else {
                lastActionMsg = "该物品无法直接使用。";
                ui.displayMessage(lastActionMsg, UIManager::Color::YELLOW);
            }
        }
        ui.displayPlayerStatus(player, lastActionMsg);
    };

    commandHandlers["wear"] = [this](const auto& args) {
        if (args.empty()) {
            ui.displayMessage("用法: wear <装备名>", UIManager::Color::YELLOW);
            return;
        }
        std::string equipName = args[0];
        if (player.equipFromInventory(equipName)) {
            lastActionMsg = "成功穿戴装备: " + equipName;
            ui.displayMessage(lastActionMsg, UIManager::Color::GREEN);
        } else {
            lastActionMsg = "穿戴失败，背包中没有该装备或不是装备类型。";
            ui.displayMessage(lastActionMsg, UIManager::Color::RED);
        }
        ui.displayPlayerStatus(player, lastActionMsg);
    };
    commandHandlers["pick"] = [this](const auto& args) {
        if (args.empty()) {
            ui.displayMessage("用法: pick <物品名>，如 pick 黑曜晶尘", UIManager::Color::YELLOW);
            return;
        }
        int roomId = gameMap.getCurrentRoomId();
        auto& room = gameMap.rooms[roomId];
        std::string itemName = args[0];
        auto& items = room.items;
        auto it = std::find(items.begin(), items.end(), itemName);
        if (it != items.end()) {
            player.addItemByName(itemName, 1);
            lastActionMsg = "你获得了 " + itemName + "，已加入背包！";
            ui.displayMessage(lastActionMsg, UIManager::Color::GREEN);
            ui.displayMessage("可用 'use " + itemName + "' 使用，或 'wear " + itemName + "' 穿戴装备。", UIManager::Color::CYAN);
            items.erase(it);
        } else {
            lastActionMsg = "当前房间没有该物品。";
            ui.displayMessage(lastActionMsg, UIManager::Color::RED);
        }
        ui.displayPlayerStatus(player, lastActionMsg);
    };
    commandAliases["get"] = "pick";
    commandAliases["拾取"] = "pick";
}