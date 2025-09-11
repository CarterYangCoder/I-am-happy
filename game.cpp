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
        if (!npc) { ui.displayMessage("这里没有可交互的NPC。", UIManager::Color::GRAY); return; }
        ui.displayMessage("与你交互的NPC: " + npc->getName(), UIManager::Color::CYAN);
        npc->showDialogue();
        int roomId = gameMap.getCurrentRoomId();
        bool gateForTask3 = (roomId==7 && gameMap.hasEnemyTypeInRoom(7, EnemyType::MINOTAUR));
        if (gateForTask3 && npc->getName().find("张焜杰")!=std::string::npos) {
            ui.displayMessage("两只牛头人仍在守卫，先使用 fight 击败它们再与张焜杰对话以解锁任务。", UIManager::Color::RED);
        }
        // 试炼完成提示（牛头人已清除，尚未接取任务3）
        if (!gateForTask3 && roomId==7 && npc->getName().find("张焜杰")!=std::string::npos) {
            Task* t3 = tasks.findTask("3");
            if (t3) {
                bool hasCopy = player.taskProgress.count("3");
                TaskStatus st = hasCopy ? player.taskProgress["3"].getStatus() : TaskStatus::UNACCEPTED;
                if ((!hasCopy || st==TaskStatus::UNACCEPTED) && player.getLevel() >= t3->getRequiredLevel()) {
                    ui.displayMessage("试炼完成！可接取任务3: " + t3->getName() + " 输入: task accept 3", UIManager::Color::GREEN);
                }
            }
        }
        struct Guide { std::string key; std::string taskId; std::string itemName; int need; std::string route; };
        static const std::vector<Guide> guides = {
            {"杨思睿", "1", "黑曜晶尘", 3, "前往裂隙废墟 fight 清除蚀骨恶狼后 pick 黑曜晶尘(3份) 然后再次与NPC对话"},
            {"晋津津", "2", "铁誓胸甲", 1, "当前房间直接 pick 铁誓胸甲 然后再次与NPC对话"},
            {"张焜杰", "3", "明识之戒", 1, "当前房间 pick 明识之戒 然后再次与NPC对话"},
            {"钟志炜", "4", "怜悯之链", 1, "go SE 到 山脚下 pick 怜悯之链 然后再次与NPC对话"},
            {"王浠珃", "5", "晨曦披风", 1, "当前迷宫房间 pick 晨曦披风 然后再次与NPC对话"},
            {"周洋迅", "6", "创世战靴", 1, "当前房间 pick 创世战靴 然后再次与NPC对话"}
        };
        bool matched=false;
        for (const auto& g : guides) {
            if (npc->getName().find(g.key) != std::string::npos) {
                matched=true;
                if (g.taskId=="3" && gateForTask3) break; // 阻挡任务3提示
                Task* task = tasks.findTask(g.taskId);
                bool hasCopy = player.taskProgress.count(g.taskId);
                TaskStatus status = hasCopy ? player.taskProgress[g.taskId].getStatus() : TaskStatus::UNACCEPTED;
                if (!task) { ui.displayMessage("(缺少任务数据:"+g.taskId+")", UIManager::Color::RED); break; }
                if (!hasCopy || status==TaskStatus::UNACCEPTED) {
                    if (player.getLevel() < task->getRequiredLevel()) {
                        ui.displayMessage("需要等级 Lv."+std::to_string(task->getRequiredLevel())+" 才能接取该任务。", UIManager::Color::RED);
                    } else {
                        ui.displayMessage("可接取任务" + g.taskId + " " + task->getName() + " 输入: task accept " + g.taskId, UIManager::Color::YELLOW);
                        ui.displayMessage("步骤: " + g.route, UIManager::Color::GREEN);
                    }
                } else if (status==TaskStatus::ACCEPTED) {
                    auto &inv = player.getInventory(); int have = inv.count(g.itemName)?inv.at(g.itemName):0;
                    if (g.need>1) ui.displayMessage("进度: " + g.itemName + " " + std::to_string(have) + "/" + std::to_string(g.need), UIManager::Color::CYAN);
                    if (have < g.need) ui.displayMessage("继续: " + g.route, UIManager::Color::YELLOW); else {
                        if (task->getStatus()==TaskStatus::ACCEPTED) task->checkCompletion(&player);
                        if (task->getStatus()==TaskStatus::COMPLETED) { player.taskProgress[g.taskId].setStatus(TaskStatus::COMPLETED); ui.displayMessage("任务已完成，输入: task submit " + g.taskId + " 提交奖励。", UIManager::Color::GREEN);} else ui.displayMessage("已满足数量等待判定，请再次交互。", UIManager::Color::CYAN);
                    }
                } else if (status==TaskStatus::COMPLETED) ui.displayMessage("任务待提交: task submit " + g.taskId, UIManager::Color::GREEN);
                else if (status==TaskStatus::REWARDED) ui.displayMessage("任务已完成并领取奖励。", UIManager::Color::GRAY);
                break;
            }
        }
        // 兜底：晋津津若未匹配（名字被改动等）
        if (!matched && npc->getName().find("晋津津")!=std::string::npos) {
            Task* t2 = tasks.findTask("2"); bool has = player.taskProgress.count("2"); TaskStatus st = has?player.taskProgress["2"].getStatus():TaskStatus::UNACCEPTED;
            if (!has || st==TaskStatus::UNACCEPTED) ui.displayMessage("可接取任务2 输入: task accept 2", UIManager::Color::YELLOW);
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
        if (args.empty()) { ui.displayMessage("用法: pick <物品名>", UIManager::Color::YELLOW); return; }
        int roomId = gameMap.getCurrentRoomId();
        auto& room = gameMap.rooms[roomId];
        std::string rawName = args[0];
        // 统一“黑曜晶尘”与常见误写“黑耀晶尘”
        bool isObsidianDustAlias = (rawName == "黑曜晶尘" || rawName == "黑耀晶尘");
        std::string itemName = isObsidianDustAlias ? "黑曜晶尘" : rawName; // 规范化
        if (isObsidianDustAlias && rawName != itemName) {
            ui.displayMessage("已自动识别为: 黑曜晶尘", UIManager::Color::GRAY);
        }
        // 特殊逻辑：黑曜晶尘需要先击败裂隙废墟(3)的蚀骨恶狼
        if (itemName == "黑曜晶尘" && roomId == 3) {
            if (gameMap.hasEnemyTypeInRoom(3, EnemyType::CORRUPT_WOLF)) {
                ui.displayMessage("蚀骨恶狼仍在守卫，你无法收集黑曜晶尘。先使用 fight 击败它们。", UIManager::Color::RED);
                return;
            }
            if (!gameMap.isObsidianDustSpawned()) {
                bool exists = std::find(room.items.begin(), room.items.end(), "黑曜晶尘") != room.items.end();
                if (!exists) {
                    room.addItem("黑曜晶尘"); room.addItem("黑曜晶尘"); room.addItem("黑曜晶尘");
                    gameMap.setObsidianDustSpawned(true);
                    ui.displayMessage("你清除了守卫，3份黑曜晶尘显露出来。", UIManager::Color::GREEN);
                }
            }
        }
        auto& items = room.items;
        auto it = std::find(items.begin(), items.end(), itemName);
        if (it != items.end()) {
            player.addItemByName(itemName, 1);
            items.erase(it);
            lastActionMsg = "获得物品: " + itemName;
            ui.displayMessage("你拾取了: " + itemName, UIManager::Color::GREEN);
            struct ItemTask { std::string item; std::string taskId; int need; };
            static const std::vector<ItemTask> itemTasks = {
                {"黑曜晶尘","1",3}, {"铁誓胸甲","2",1}, {"明识之戒","3",1}, {"怜悯之链","4",1}, {"晨曦披风","5",1}, {"创世战靴","6",1}
            };
            for (const auto& itg : itemTasks) if (itemName == itg.item) {
                Task* task = tasks.findTask(itg.taskId);
                bool accepted = player.taskProgress.count(itg.taskId) && (player.taskProgress[itg.taskId].getStatus()==TaskStatus::ACCEPTED);
                int have = player.getInventory().count(itg.item)?player.getInventory().at(itg.item):0;
                if (itg.need > 1) ui.displayMessage("进度: " + itg.item + " " + std::to_string(have) + "/" + std::to_string(itg.need), UIManager::Color::CYAN);
                if (accepted && task) {
                    if (task->getStatus()==TaskStatus::ACCEPTED) task->checkCompletion(&player);
                    if (task->getStatus()==TaskStatus::COMPLETED) {
                        player.taskProgress[itg.taskId].setStatus(TaskStatus::COMPLETED);
                        if (itg.taskId=="1") ui.displayMessage("任务1已完成，回铁砧铁匠铺找杨思睿: task submit 1 获取奖励并自动装备。", UIManager::Color::GREEN);
                        else ui.displayMessage("任务" + itg.taskId + "已满足条件，输入: task submit " + itg.taskId + " 提交奖励。", UIManager::Color::CYAN);
                    } else if (task && itg.need>1) ui.displayMessage("继续收集直到完成。", UIManager::Color::YELLOW);
                } else if (!accepted) {
                    if (itg.taskId=="1") ui.displayMessage("提示: 回铁砧铁匠铺与杨思睿对话并输入: task accept 1 再收集更有效。", UIManager::Color::YELLOW);
                    else ui.displayMessage("提示: 可先接取任务" + itg.taskId + " 再拾取同样生效。输入: task accept " + itg.taskId, UIManager::Color::YELLOW);
                }
                break;
            }
        } else {
            // 若用户在狼已清除且未生成时用错误名字尝试，再次友好提醒
            if (isObsidianDustAlias && roomId==3) ui.displayMessage("未找到黑曜晶尘，请再次输入: pick 黑曜晶尘", UIManager::Color::YELLOW);
            else ui.displayMessage("这里没有该物品。", UIManager::Color::RED);
        }
        ui.displayPlayerStatus(player, lastActionMsg);
    };
    
    // 调试命令
    commandHandlers["debug"] = [this](const auto& args) {
        int roomId = gameMap.getCurrentRoomId();
        auto& room = gameMap.rooms[roomId];
        ui.displayMessage("当前房间ID: " + std::to_string(roomId), UIManager::Color::CYAN);
        ui.displayMessage("房间名称: " + room.getRoomName(), UIManager::Color::CYAN);
        ui.displayMessage("房间物品列表:", UIManager::Color::YELLOW);
        if (room.items.empty()) {
            ui.displayMessage("房间没有任何物品", UIManager::Color::RED);
        } else {
            for (size_t i = 0; i < room.items.size(); ++i) {
                ui.displayMessage("[" + std::to_string(i) + "] '" + room.items[i] + "'", UIManager::Color::WHITE);
            }
        }
    };
    
    // 设置别名
    commandAliases["get"] = "pick";
    commandAliases["拾取"] = "pick";
}