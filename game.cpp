#include "Game.h"
#include <memory>
#include <iostream>
#include <algorithm>
#include <windows.h>

bool is_digits_game(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}
 
// Game类的构造函数
Game::Game(bool loadFromSave, Player* loadedPlayer) :
    player(loadedPlayer ? *loadedPlayer : Player("安特王子")),
    gameMap(),              // 初始化地图
    combat(ui),
    tasks(ui),
    shop(ui),
    saveLoad(ui),
    isRunning(true),
    currentState(GameState::Exploring),
    startedFromSave(loadFromSave)
{
    // 设置控制台编码以支持中文显示
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    
    // 如果从存档启动，同步地图位置
    if (startedFromSave && loadedPlayer) {
        gameMap.setCurrentRoomId(loadedPlayer->getCurrentRoomId());
        player.setCurrentRoomId(loadedPlayer->getCurrentRoomId());
    } else {
        // 同步玩家和地图的位置
        player.setCurrentRoomId(gameMap.getCurrentRoomId());
    }
    
    // 初始化物品数据库
    itemDb[1] = std::make_unique<HealthPotion>();
    itemDb[2] = std::make_unique<EnergyPotion>();
    itemDb[3] = std::make_unique<MysteriousItem>();

    tasks.initializeTasks();
    shop.initializeShop();
    registerCommands();
    setupCommandAliases();
}

// 游戏主循环
void Game::run() {
    if (!startedFromSave) {
        ui.displayMessage("欢迎来到MUD世界, " + player.getName() + "!", UIManager::Color::CYAN);
        ui.displayMessage("输入 'help' 或 '帮助' 查看可用命令。", UIManager::Color::YELLOW);
        
        // 显示初始房间信息
        gameMap.showInitialRoom();
    } else {
        ui.displayMessage("欢迎回来, " + player.getName() + "!", UIManager::Color::CYAN);
        ui.displayMessage("存档已加载，继续你的冒险吧！", UIManager::Color::GREEN);
        ui.displayMessage("输入 'help' 或 '帮助' 查看可用命令。", UIManager::Color::YELLOW);
        
        // 显示当前房间信息
        gameMap.showCurrentRoom();
        ui.displayPlayerStatus(player);
    }

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
    // 设置别名
    commandAliases["get"] = "pick";
    commandAliases["拾取"] = "pick";
    commandAliases["sell"] = "sell";
    commandAliases["售卖"] = "sell";
    commandAliases["drop"] = "drop";
    commandAliases["丢弃"] = "drop";
    commandAliases["skill"] = "skill";
    commandAliases["技能"] = "skill";
    commandAliases["sk"] = "skill";
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
        ui.displayMessage("  pick <物品名>(拾取) - 拾取物品, sell <物品名>(售卖) - 售卖物品", UIManager::Color::GREEN);
        ui.displayMessage("  drop <物品名>(丢弃) - 丢弃物品到当前区域", UIManager::Color::GREEN);
        ui.displayMessage("装备命令: wear <装备名> - 穿戴装备，如 wear 铁誓胸甲", UIManager::Color::GREEN);
        ui.displayMessage("技能命令: skill(sk,技能) [技能名] - 查看技能列表或详细信息", UIManager::Color::BLUE);
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
        if (currentState == GameState::InShop) {
            ui.displayMessage("你现在在商店中，无法移动。", UIManager::Color::YELLOW);
            ui.displayMessage("请先输入 'leave' 或 '离开' 退出商店。", UIManager::Color::CYAN);
            ui.displayMessage("商人：\"客官，买完东西再走嘛~ (￣▽￣)ノ\"", UIManager::Color::WHITE);
            return;
        }
        
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
        if (currentState == GameState::InShop) {
            ui.displayMessage("你现在在商店中，无法与其他NPC对话。", UIManager::Color::YELLOW);
            ui.displayMessage("请先输入 'leave' 或 '离开' 退出商店。", UIManager::Color::CYAN);
            ui.displayMessage("商人：\"专心购物嘛，其他人等等再聊~ (￣▽￣)ノ\"", UIManager::Color::WHITE);
            return;
        }
        
        NPC* npc = gameMap.getCurrentRoomNPC();
        if (!npc) { ui.displayMessage("这里没有可交互的NPC。", UIManager::Color::GRAY); return; }
        
        // 优先检查：张焜杰的牛头人试炼门禁（在对话前检查）
        int roomId = gameMap.getCurrentRoomId();
        bool gateForTask3 = (roomId==7 && gameMap.hasEnemyTypeInRoom(7, EnemyType::MINOTAUR));
        if (gateForTask3 && npc->getName().find("张焜杰")!=std::string::npos) {
            ui.displayMessage("两只牛头人仍在守卫，先使用 fight 击败它们再与张焜杰对话以解锁任务。", UIManager::Color::RED);
            return;
        }
        
        // 检查任务状态和是否首次见面
        std::string taskId = npc->getTaskID();
        TaskStatus taskStatus = TaskStatus::UNACCEPTED;
        bool hasMetBefore = false;
        
        if (!taskId.empty() && player.taskProgress.count(taskId)) {
            taskStatus = player.taskProgress[taskId].getStatus();
            hasMetBefore = true;
        }
        
        // 显示对应状态的对话
        npc->showDialogueByStatus(ui, taskStatus, hasMetBefore);

        // 新增：与NPC对话后标记该任务“已对话”，用于移动/拾取前置校验
        if (!taskId.empty()) {
            player.markTalkedToNpc(taskId);
        }

        // 在对话后添加任务状态相关的提示
        if (!taskId.empty() && hasMetBefore) {
            if (taskStatus == TaskStatus::COMPLETED) {
                ui.displayMessage("", UIManager::Color::WHITE); // 空行分隔
                ui.displayMessage("任务已完成！输入: task submit " + taskId + " 来提交任务并获取奖励", UIManager::Color::GREEN);
            }
        }

        // 晋津津任务2的"解封仪式"自动触发与提交
        if (npc->getName().find("晋津津") != std::string::npos) {
            Task* t2 = tasks.findTask("2");
            if (t2) {
                bool hasCopy = player.taskProgress.count("2");
                TaskStatus st = hasCopy ? player.taskProgress["2"].getStatus() : TaskStatus::UNACCEPTED;
                int haveChest = player.getInventory().count("铁誓胸甲") ? player.getInventory().at("铁誓胸甲") : 0;
                if ((st == TaskStatus::ACCEPTED || st == TaskStatus::COMPLETED) && haveChest >= 1) {
                    ui.displayMessage("【解封仪式】晋津津将你的手按在胸甲之上，古老誓文在空气中回响……", UIManager::Color::CYAN);
                    ui.displayMessage("鲜血涌动，三千忠魂掠过你的视野，怨念消散为点点星光。", UIManager::Color::WHITE);
                    if (t2->getStatus() == TaskStatus::ACCEPTED) { t2->checkCompletion(&player); }
                    tasks.submitTask(&player, "2");
                    ui.displayMessage("解封仪式完成。忠魂得以安息。", UIManager::Color::GREEN);
                    return;
                }
            }
        }

        // 试炼完成提示（牛头人已清除，尚未接取任务3）- 移到牛头人检查后
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
        
        // 简化的任务引导提示（只在首次见面或未接取时显示）
        if (!hasMetBefore || taskStatus == TaskStatus::UNACCEPTED) {
            Task* task = tasks.findTask(taskId);
            if (task && player.getLevel() >= task->getRequiredLevel()) {
                // 张焜杰的任务引导已在上面处理，这里跳过
                if (!(roomId==7 && npc->getName().find("张焜杰")!=std::string::npos)) {
                    ui.displayMessage("可接取任务" + taskId + " " + task->getName() + " 输入: task accept " + taskId, UIManager::Color::YELLOW);
                }
            }
        }
    };

    commandHandlers["fight"] = [this](const auto& args) {
        if (currentState != GameState::Exploring) { 
            if (currentState == GameState::InShop) {
                ui.displayMessage("你现在在商店中，无法战斗。", UIManager::Color::YELLOW);
                ui.displayMessage("请先输入 'leave' 或 '离开' 退出商店，再进行战斗。", UIManager::Color::CYAN);
                ui.displayMessage("商人：\"客官，店内不可动武哦~ (￣▽￣)ノ\"", UIManager::Color::WHITE);
            } else {
                ui.displayMessage("你现在不能战斗。", UIManager::Color::RED);
            }
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
                    
                    // 新增：仅当两只狼均被击败且未刷新过时，提示黑曜晶尘显现（房间3）
                    if (gameMap.getCurrentRoomId() == 3 &&
                        !gameMap.hasEnemyTypeInRoom(3, EnemyType::CORRUPT_WOLF) &&
                        !gameMap.isObsidianDustSpawned()) {
                        ui.displayMessage("蚀骨恶狼的守卫已被清除！地面的黑曜晶尘开始发出幽蓝色的光芒...", UIManager::Color::CYAN);
                        ui.displayMessage("输入 'pick 黑曜晶尘' 来收集它们。", UIManager::Color::YELLOW);
                    }
                    
                    // 删除重复的胜利提示，CombatSystem 已经显示了
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
        if (currentState == GameState::InShop) {
            ui.displayMessage("你现在在商店中，无法管理任务。", UIManager::Color::YELLOW);
            ui.displayMessage("请先输入 'leave' 或 '离开' 退出商店。", UIManager::Color::CYAN);
            ui.displayMessage("商人：\"先把生意做完，任务的事儿出去再说~ (￣▽￣)ノ\"", UIManager::Color::WHITE);
            return;
        }
        
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
            int mpRecoverAmount = player.getMP() * 0.5;
            player.setMP(player.getMP() + mpRecoverAmount);
            player.useItem(itemName);
            lastActionMsg = "使用了能量药水，回复了 " + std::to_string(mpRecoverAmount) + " 点蓝量。";
            ui.displayMessage(lastActionMsg, UIManager::Color::CYAN);
        } else if (itemName == "神秘商品") {
            std::string result = itemDb[3]->use();
            player.useItem(itemName);
            lastActionMsg = result;
            ui.displayMessage(lastActionMsg, UIManager::Color::YELLOW);
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
        if (currentState == GameState::InShop) {
            ui.displayMessage("你现在在商店中，无法拾取物品。", UIManager::Color::YELLOW);
            ui.displayMessage("请先输入 'leave' 或 '离开' 退出商店。", UIManager::Color::CYAN);
            ui.displayMessage("商人：\"店里的东西要花钱买哦，不能白拿~ (￣▽￣)ノ\"", UIManager::Color::WHITE);
            return;
        }
        
        if (args.empty()) { ui.displayMessage("用法: pick <物品名>", UIManager::Color::YELLOW); return; }
        int roomId = gameMap.getCurrentRoomId();
        auto& room = gameMap.rooms[roomId];

        // 新增：pick all/全部 一键收集
        {
            std::string arg0 = args[0];
            std::string arg0Lower = arg0;
            std::transform(arg0Lower.begin(), arg0Lower.end(), arg0Lower.begin(), ::tolower);
            if (arg0Lower == "all" || arg0 == "全部") {

                // 新增：在裂隙废墟中，若狼已清除且黑曜晶尘尚未显现，则先刷新3份黑曜晶尘
                if (roomId == 3 &&
                    !gameMap.hasEnemyTypeInRoom(3, EnemyType::CORRUPT_WOLF) &&
                    !gameMap.isObsidianDustSpawned()) {
                    room.addItem("黑曜晶尘");
                    room.addItem("黑曜晶尘");
                    room.addItem("黑曜晶尘");
                    gameMap.setObsidianDustSpawned(true);
                    // 不额外打断流程，继续进入一键收集
                }

                if (room.items.empty()) {
                    ui.displayMessage("当前区域没有可拾取的物品。", UIManager::Color::GRAY);
                    return;
                }

                std::map<std::string,int> collected;
                std::vector<std::string> remaining;
                bool dustBlocked = false;

                for (const auto& itName : room.items) {
                    // 黑曜晶尘在裂隙废墟且有狼存活时禁止收集
                    if (itName == "黑曜晶尘" && roomId == 3 &&
                        gameMap.hasEnemyTypeInRoom(3, EnemyType::CORRUPT_WOLF)) {
                        remaining.push_back(itName);
                        dustBlocked = true;
                        continue;
                    }
                    player.addItemByName(itName, 1);
                    collected[itName]++;
                }
                room.items.swap(remaining);

                if (collected.empty()) {
                    if (dustBlocked) {
                        ui.displayMessage("蚀骨恶狼仍在守卫，黑曜晶尘无法收集。", UIManager::Color::RED);
                    } else {
                        ui.displayMessage("没有可收集的物品。", UIManager::Color::GRAY);
                    }
                    return;
                }

                // 静默更新任务（仅内部判定，不额外提示）
                struct ItemTask { std::string item; std::string taskId; int need; };
                static const std::vector<ItemTask> itemTasks = {
                    {"黑曜晶尘","1",3}, {"铁誓胸甲","2",1}, {"明识之戒","3",1},
                    {"怜悯之链","4",1}, {"晨曦披风","5",1}, {"创世战靴","6",1}
                };
                for (const auto& kv : collected) {
                    for (const auto& itg : itemTasks) if (kv.first == itg.item) {
                        Task* task = tasks.findTask(itg.taskId);
                        if (task && player.taskProgress.count(itg.taskId) &&
                            player.taskProgress[itg.taskId].getStatus()==TaskStatus::ACCEPTED) {
                            task->checkCompletion(&player);
                            if (task->getStatus()==TaskStatus::COMPLETED) {
                                player.taskProgress[itg.taskId].setStatus(TaskStatus::COMPLETED);
                            }
                        }
                        break;
                    }
                }

                // 提示：收集汇总
                int total = 0;
                for (const auto& kv : collected) total += kv.second;
                ui.displayMessage("已收集物品共 " + std::to_string(total) + " 件：", UIManager::Color::GREEN);
                for (const auto& kv : collected) {
                    ui.displayMessage("- " + kv.first + " x" + std::to_string(kv.second), UIManager::Color::WHITE);
                }
                if (collected.count("黑曜晶尘")) {
                    int have = player.getInventory().count("黑曜晶尘") ? player.getInventory().at("黑曜晶尘") : 0;
                    ui.displayMessage("收集成功：黑曜晶尘 (" + std::to_string(have) + "/3)", UIManager::Color::GREEN);
                }
                if (dustBlocked) {
                    ui.displayMessage("蚀骨恶狼仍在守卫，黑曜晶尘暂不可收集。", UIManager::Color::YELLOW);
                }
                ui.displayMessage("背包物品：", UIManager::Color::CYAN);
                const auto& inv = player.getInventory();
                if (inv.empty()) {
                    ui.displayMessage("（空）", UIManager::Color::GRAY);
                } else {
                    for (const auto& kv : inv) {
                        ui.displayMessage("- " + kv.first + " x" + std::to_string(kv.second), UIManager::Color::WHITE);
                    }
                }
                return;
            }
        }

        std::string rawName = args[0];
        // 统一"黑曜晶尘"与常见误写"黑耀晶尘"
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
                // 击败蚀骨恶狼后刷新黑曜晶尘（静默刷新，不提示，不展示房间信息）
                room.addItem("黑曜晶尘"); 
                room.addItem("黑曜晶尘"); 
                room.addItem("黑曜晶尘");
                gameMap.setObsidianDustSpawned(true);
                // 不再 showCurrentRoom、不再提前 return，继续执行后续拾取流程，使本次 pick 直接拾取1份
            }
        }
        auto& items = room.items;
        auto it = std::find(items.begin(), items.end(), itemName);
        if (it != items.end()) {
            player.addItemByName(itemName, 1);
            items.erase(it);

            // 新增：对“黑曜晶尘”专门精简输出，仅显示收集成功/进度与背包物品列表
            if (itemName == "黑曜晶尘") {
                int have = player.getInventory().count("黑曜晶尘") ? player.getInventory().at("黑曜晶尘") : 0;

                // 静默更新任务完成状态（若已接取）
                Task* t1 = tasks.findTask("1");
                if (t1 && player.taskProgress.count("1") && player.taskProgress["1"].getStatus() == TaskStatus::ACCEPTED) {
                    t1->checkCompletion(&player);
                    if (t1->getStatus() == TaskStatus::COMPLETED) {
                        player.taskProgress["1"].setStatus(TaskStatus::COMPLETED);
                    }
                }

                // 仅提示收集成功与进度
                lastActionMsg = "收集成功：黑曜晶尘 (" + std::to_string(have) + "/3)";
                ui.displayMessage(lastActionMsg, UIManager::Color::GREEN);

                // 显示背包物品列表（简洁）
                ui.displayMessage("背包物品：", UIManager::Color::CYAN);
                const auto& inv = player.getInventory();
                if (inv.empty()) {
                    ui.displayMessage("（空）", UIManager::Color::GRAY);
                } else {
                    for (const auto& kv : inv) {
                        ui.displayMessage("- " + kv.first + " x" + std::to_string(kv.second), UIManager::Color::WHITE);
                    }
                }
                return; // 结束，避免后续通用提示与状态大段显示
            }

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
            ui.displayMessage("这里没有该物品。", UIManager::Color::RED);
        }
        ui.displayPlayerStatus(player, lastActionMsg);
    };
    
    commandHandlers["sell"] = [this](const auto& args) {
        if (currentState == GameState::InShop) {
            ui.displayMessage("你现在在商店中，暂时无法向商人售卖物品。", UIManager::Color::YELLOW);
            ui.displayMessage("请先输入 'leave' 或 '离开' 退出商店，然后使用sell命令。", UIManager::Color::CYAN);
            ui.displayMessage("商人：\"我这儿只卖不收，出去找别人卖吧~ (￣▽￣)ノ\"", UIManager::Color::WHITE);
            return;
        }
        
        if (args.empty()) {
            ui.displayMessage("用法: sell <物品名> [数量]", UIManager::Color::YELLOW);
            return;
        }
        
        std::string itemName = args[0];
        int quantity = 1;
        if (args.size() > 1) {
            try {
                quantity = std::stoi(args[1]);
                if (quantity <= 0) {
                    ui.displayMessage("数量必须大于0。", UIManager::Color::RED);
                    return;
                }
            } catch (const std::exception&) {
                ui.displayMessage("无效的数量格式。", UIManager::Color::RED);
                return;
            }
        }
        
        if (!player.getInventory().count(itemName) || player.getInventory().at(itemName) < quantity) {
            ui.displayMessage("背包中没有足够的该物品。", UIManager::Color::RED);
            return;
        }
        
        if (!player.canSellItem(itemName)) {
            ui.displayMessage("该物品不可售卖，但可以丢弃。输入: drop " + itemName, UIManager::Color::YELLOW);
            return;
        }
        
        int sellPrice = player.getItemSellPrice(itemName) * quantity;
        if (player.sellItem(itemName, quantity)) {
            lastActionMsg = "售卖了 " + std::to_string(quantity) + " 个 " + itemName + "，获得 " + std::to_string(sellPrice) + " 金币。";
            ui.displayMessage(lastActionMsg, UIManager::Color::GREEN);
        } else {
            ui.displayMessage("售卖失败。", UIManager::Color::RED);
        }
        ui.displayPlayerStatus(player, lastActionMsg);
    };

    commandHandlers["drop"] = [this](const auto& args) {
        if (currentState == GameState::InShop) {
            ui.displayMessage("你现在在商店中，不能随便丢弃物品。", UIManager::Color::YELLOW);
            ui.displayMessage("请先输入 'leave' 或 '离开' 退出商店。", UIManager::Color::CYAN);
            ui.displayMessage("商人：\"别把垃圾扔我店里！要丢东西出去丢~ (￣▽￣)ノ\"", UIManager::Color::WHITE);
            return;
        }
        
        if (args.empty()) {
            ui.displayMessage("用法: drop <物品名> [数量]", UIManager::Color::YELLOW);
            return;
        }
        
        std::string itemName = args[0];
        int quantity = 1;
        if (args.size() > 1) {
            try {
                quantity = std::stoi(args[1]);
                if (quantity <= 0) {
                    ui.displayMessage("数量必须大于0。", UIManager::Color::RED);
                    return;
                }
            } catch (const std::exception&) {
                ui.displayMessage("无效的数量格式。", UIManager::Color::RED);
                return;
            }
        }
        
        if (!player.getInventory().count(itemName) || player.getInventory().at(itemName) < quantity) {
            ui.displayMessage("背包中没有足够的该物品。", UIManager::Color::RED);
            return;
        }
        
        if (player.dropItem(itemName, quantity)) {
            // 将物品添加到当前房间
            int roomId = gameMap.getCurrentRoomId();
            for (int i = 0; i < quantity; i++) {
                gameMap.rooms[roomId].addItem(itemName);
            }
            lastActionMsg = "丢弃了 " + std::to_string(quantity) + " 个 " + itemName + "。";
            ui.displayMessage(lastActionMsg, UIManager::Color::GREEN);
            ui.displayMessage("物品已丢弃在当前区域，可以重新拾取。", UIManager::Color::GRAY);
        } else {
            ui.displayMessage("丢弃失败。", UIManager::Color::RED);
        }
        ui.displayPlayerStatus(player, lastActionMsg);
    };

    commandHandlers["skill"] = [this](const auto& args) {
        if (player.getSkills().empty()) {
            ui.displayMessage("你还没有学会任何技能。", UIManager::Color::YELLOW);
            ui.displayMessage("通过升级可以解锁新技能！", UIManager::Color::GRAY);
            return;
        }
        
        if (args.empty()) {
            // 显示技能列表
            ui.displayMessage("=== 已解锁技能列表 ===", UIManager::Color::CYAN);
            for (size_t i = 0; i < player.getSkills().size(); ++i) {
                const auto& skill = player.getSkills()[i];
                int scaledPower = skill->getScaledPower(player);                // 改：按AD/AP+等级
                int mpCost = skill->getMpCost(player.getLevel());
                ui.displayMessage("[" + std::to_string(i + 1) + "] " + skill->getName() + 
                                  " (威力:" + std::to_string(scaledPower) + 
                                  " | MP:" + std::to_string(mpCost) + ")", UIManager::Color::WHITE);
            }
            ui.displayMessage("输入 'skill <技能名>' 查看详细信息", UIManager::Color::GRAY);
            ui.displayMessage("当前增益状态: " + player.getBuffStatus(), UIManager::Color::YELLOW);
        } else {
            // 显示指定技能的详细信息
            std::string targetSkillName = args[0];
            bool found = false;
            
            for (const auto& skill : player.getSkills()) {
                if (skill->getName().find(targetSkillName) != std::string::npos || 
                    targetSkillName == skill->getName()) {
                    ui.displayMessage(skill->getDetailedDescription(player.getLevel()), UIManager::Color::WHITE);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                ui.displayMessage("未找到技能: " + targetSkillName, UIManager::Color::RED);
                ui.displayMessage("输入 'skill' 查看所有已解锁技能", UIManager::Color::GRAY);
            }
        }
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
}