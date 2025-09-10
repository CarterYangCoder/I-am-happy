#include "CombatSystem.h"
#include "UIManager.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <limits>
#include <iomanip>

CombatSystem::CombatSystem(UIManager &uiManager) : ui(uiManager) {}

bool is_digits(const std::string &str)
{
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

// calculateDamage 和 attemptEscape 函数无需改动
int CombatSystem::calculateDamage(const Attribute &attacker, const Attribute &defender, int power)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    int baseAttack = (power > 0) ? power : attacker.getATK();
    bool isCrit = dis(gen) < attacker.getCritRate();
    if (isCrit)
    {
        ui.displayMessage(attacker.getName() + " 打出了致命一击!", UIManager::Color::YELLOW);
        baseAttack *= 2;
    }
    int damage = baseAttack - defender.getDEF();
    return std::max(1, damage);
}

bool CombatSystem::attemptEscape(const Player &player, const CommonEnemy &enemy)
{
    double escapeChance = 0.5 + (player.getSpeed() - enemy.getSpeed()) * 0.02;
    escapeChance = std::max(0.05, std::min(0.95, escapeChance));
    ui.displayMessage("你的逃跑成功率为 " + std::to_string(static_cast<int>(escapeChance * 100)) + "%...", UIManager::Color::GRAY);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen) < escapeChance;
}

CombatResult CombatSystem::playerTurn(Player &player, CommonEnemy &enemy, const std::map<int, std::unique_ptr<Item>> &itemDb)
{
    return handlePlayerAction(player, enemy, itemDb);
}

CombatResult CombatSystem::playerTurn(Player &player, EvilGeneral &boss, const std::map<int, std::unique_ptr<Item>> &itemDb)
{
    return handlePlayerAction(player, boss, itemDb);
}

void CombatSystem::enemyTurn(CommonEnemy &enemy, Player &player)
{
    ui.displayMessage("--- " + enemy.getName() + " 的回合 ---", UIManager::Color::MAGENTA);
    int damage = calculateDamage(enemy, player);
    player.takeDamage(damage);
    ui.displayMessage(enemy.getName() + " 对你造成了 " + std::to_string(damage) + " 点伤害!", UIManager::Color::RED);
}

void CombatSystem::enemyTurn(EvilGeneral &boss, Player &player)
{
    ui.displayMessage("--- " + boss.getName() + " 的回合 ---", UIManager::Color::MAGENTA);
    int damage = calculateDamage(boss, player);
    player.takeDamage(damage);
    ui.displayMessage(boss.getName() + " 对你造成了 " + std::to_string(damage) + " 点伤害!", UIManager::Color::RED);
}

CombatResult CombatSystem::startCombat(Player &player, CommonEnemy &enemy, const std::map<int, std::unique_ptr<Item>> &itemDb)
{
    ui.displayMessage("战斗开始! " + player.getName() + " vs " + enemy.getName(), UIManager::Color::MAGENTA);
    player.extraActionTurns = 0;

    Attribute *first = &player;

    if (enemy.getSpeed() > player.getSpeed())
    {
        first = &enemy;
        ui.displayMessage(enemy.getName() + " 速度更快，获得先手!", UIManager::Color::YELLOW);
    }
    else
    {
        ui.displayMessage(player.getName() + " 速度更快，获得先手!", UIManager::Color::GREEN);
    }

    while (player.isAlive() && enemy.isAlive())
    {
        // 定义一个lambda函数来处理玩家的完整回合，包括额外行动
        auto execute_player_turn_phase = [&]() -> CombatResult
        {
            // 在回合开始时应用能量药水的效果
            int totalActions = 1 + player.extraActionTurns;
            player.extraActionTurns = 0; // 重置额外行动次数
            
            for (int actionCount = 0; actionCount < totalActions && player.isAlive() && enemy.isAlive(); actionCount++)
            {
                if (actionCount > 0)
                {
                    ui.displayMessage("额外行动回合！", UIManager::Color::CYAN);
                }
                
                CombatResult result = playerTurn(player, enemy, itemDb);
                if (result != CombatResult::Continue) return result;
            }
            
            return CombatResult::Continue;
        };

        // 根据先手顺序执行回合
        if (first == &player)
        {
            CombatResult result = execute_player_turn_phase();
            if (result != CombatResult::Continue) return result;
            if (enemy.isAlive()) enemyTurn(enemy, player);
        }
        else
        {
            enemyTurn(enemy, player);
            if (player.isAlive())
            {
                CombatResult result = execute_player_turn_phase();
                if (result != CombatResult::Continue) return result;
            }
        }
    }

    if (player.isAlive())
    {
        ui.displayMessage("你赢得了战斗!", UIManager::Color::GREEN);
        player.addExp(enemy.getExpReward());
        player.addGold(enemy.getGoldReward());
        ui.displayMessage("获得经验: " + std::to_string(enemy.getExpReward()), UIManager::Color::YELLOW);
        ui.displayMessage("获得金币: " + std::to_string(enemy.getGoldReward()), UIManager::Color::YELLOW);
        return CombatResult::Victory;
    }
    else
    {
        ui.displayMessage("你被击败了...", UIManager::Color::RED);
        ui.displayMessage("选择: [1]重新挑战 [2]读取存档 [3]退出游戏", UIManager::Color::WHITE);
        int choice = 1;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
        case 1: return CombatResult::Defeat_Restart;
        case 2: return CombatResult::Defeat_Load;
        case 3: return CombatResult::Defeat_Exit;
        default: return CombatResult::Defeat_Restart;
        }
    }
}

CombatResult CombatSystem::startCombat(Player &player, EvilGeneral &boss, const std::map<int, std::unique_ptr<Item>> &itemDb)
{
    ui.displayMessage("BOSS战斗开始! " + player.getName() + " vs " + boss.getName(), UIManager::Color::MAGENTA);
    
    // 显示战前对话
    ui.displayMessage("=== 战前对话 ===", UIManager::Color::YELLOW);
    ui.displayMessage(boss.getPreBattleDialogue(), UIManager::Color::WHITE);
    ui.displayMessage("==================", UIManager::Color::YELLOW);
    
    player.extraActionTurns = 0;

    Attribute *first = &player;

    if (boss.getSpeed() > player.getSpeed())
    {
        first = &boss;
        ui.displayMessage(boss.getName() + " 速度更快，获得先手!", UIManager::Color::YELLOW);
    }
    else
    {
        ui.displayMessage(player.getName() + " 速度更快，获得先手!", UIManager::Color::GREEN);
    }

    while (player.isAlive() && boss.isAlive())
    {
        auto execute_player_turn_phase = [&]() -> CombatResult
        {
            // 在回合开始时应用能量药水的效果
            int totalActions = 1 + player.extraActionTurns;
            player.extraActionTurns = 0; // 重置额外行动次数
            
            for (int actionCount = 0; actionCount < totalActions && player.isAlive() && boss.isAlive(); actionCount++)
            {
                if (actionCount > 0)
                {
                    ui.displayMessage("额外行动回合！", UIManager::Color::CYAN);
                }
                
                CombatResult result = playerTurn(player, boss, itemDb);
                if (result != CombatResult::Continue) return result;
            }
            
            return CombatResult::Continue;
        };

        if (first == &player)
        {
            CombatResult result = execute_player_turn_phase();
            if (result != CombatResult::Continue) return result;
            if (boss.isAlive()) enemyTurn(boss, player);
        }
        else
        {
            enemyTurn(boss, player);
            if (player.isAlive())
            {
                CombatResult result = execute_player_turn_phase();
                if (result != CombatResult::Continue) return result;
            }
        }
    }

    if (player.isAlive())
    {
        ui.displayMessage("=== 胜利对话 ===", UIManager::Color::YELLOW);
        ui.displayMessage(boss.getDefeatDialogue(), UIManager::Color::WHITE);
        ui.displayMessage("==================", UIManager::Color::YELLOW);
        
        ui.displayMessage("你击败了BOSS " + boss.getName() + "!", UIManager::Color::GREEN);
        player.addExp(boss.getExpReward());
        player.addGold(boss.getGoldReward());
        ui.displayMessage("获得经验: " + std::to_string(boss.getExpReward()), UIManager::Color::YELLOW);
        ui.displayMessage("获得金币: " + std::to_string(boss.getGoldReward()), UIManager::Color::YELLOW);
        return CombatResult::Victory;
    }
    else
    {
        ui.displayMessage("=== 失败对话 ===", UIManager::Color::YELLOW);
        ui.displayMessage(boss.getVictoryDialogue(), UIManager::Color::WHITE);
        ui.displayMessage("==================", UIManager::Color::YELLOW);
        
        ui.displayMessage("你被BOSS击败了...", UIManager::Color::RED);
        ui.displayMessage("选择: [1]重新挑战 [2]读取存档 [3]退出游戏", UIManager::Color::WHITE);
        int choice = 1;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
        case 1: return CombatResult::Defeat_Restart;
        case 2: return CombatResult::Defeat_Load;
        case 3: return CombatResult::Defeat_Exit;
        default: return CombatResult::Defeat_Restart;
        }
    }
}

CombatResult CombatSystem::startCombat(Player &player, BossWanEshuji &boss, const std::map<int, std::unique_ptr<Item>> &itemDb)
{
    ui.displayMessage("终极BOSS战斗开始! " + player.getName() + " vs " + boss.getName(), UIManager::Color::MAGENTA);
    ui.displayMessage("万恶枢机散发着恐怖的气息，这将是一场多阶段的艰难战斗！", UIManager::Color::RED);
    
    // 显示战前对话
    ui.displayMessage("=== 战前对话 ===", UIManager::Color::YELLOW);
    ui.displayMessage(boss.getPreBattleDialogue(), UIManager::Color::WHITE);
    ui.displayMessage("==================", UIManager::Color::YELLOW);
    
    player.extraActionTurns = 0;

    Attribute *first = &player;

    if (boss.getSpeed() > player.getSpeed())
    {
        first = &boss;
        ui.displayMessage(boss.getName() + " 速度更快，获得先手!", UIManager::Color::YELLOW);
    }
    else
    {
        ui.displayMessage(player.getName() + " 速度更快，获得先手!", UIManager::Color::GREEN);
    }

    while (player.isAlive() && boss.isAlive())
    {
        // 检查阶段转换
        if (boss.shouldEnterNextPhase())
        {
            boss.enterNextPhase();
        }

        auto execute_player_turn_phase = [&]() -> CombatResult
        {
            CombatResult result = playerTurn(player, boss, itemDb);
            if (result != CombatResult::Continue) return result;

            while (player.extraActionTurns > 0 && player.isAlive() && boss.isAlive())
            {
                player.extraActionTurns--;
                ui.displayMessage("额外行动回合！", UIManager::Color::CYAN);
                result = playerTurn(player, boss, itemDb);
                if (result != CombatResult::Continue) return result;
            }
            return CombatResult::Continue;
        };

        if (first == &player)
        {
            CombatResult result = execute_player_turn_phase();
            if (result != CombatResult::Continue) return result;
            if (boss.isAlive()) enemyTurn(boss, player);
        }
        else
        {
            enemyTurn(boss, player);
            if (player.isAlive())
            {
                CombatResult result = execute_player_turn_phase();
                if (result != CombatResult::Continue) return result;
            }
        }
    }

    if (player.isAlive())
    {
        ui.displayMessage("=== 胜利对话 ===", UIManager::Color::YELLOW);
        ui.displayMessage(boss.getDefeatDialogue(), UIManager::Color::WHITE);
        ui.displayMessage("==================", UIManager::Color::YELLOW);
        
        ui.displayMessage("你击败了万恶枢机，拯救了世界!", UIManager::Color::GREEN);
        player.addExp(boss.getExpReward());
        player.addGold(boss.getGoldReward());
        ui.displayMessage("获得经验: " + std::to_string(boss.getExpReward()), UIManager::Color::YELLOW);
        ui.displayMessage("获得金币: " + std::to_string(boss.getGoldReward()), UIManager::Color::YELLOW);
        return CombatResult::Victory;
    }
    else
    {
        ui.displayMessage("=== 失败对话 ===", UIManager::Color::YELLOW);
        ui.displayMessage(boss.getVictoryDialogue(), UIManager::Color::WHITE);
        ui.displayMessage("==================", UIManager::Color::YELLOW);
        
        ui.displayMessage("你被万恶枢机击败了...", UIManager::Color::RED);
        ui.displayMessage("选择: [1]重新挑战 [2]读取存档 [3]退出游戏", UIManager::Color::WHITE);
        int choice = 1;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
        case 1: return CombatResult::Defeat_Restart;
        case 2: return CombatResult::Defeat_Load;
        case 3: return CombatResult::Defeat_Exit;
        default: return CombatResult::Defeat_Restart;
        }
    }
}

CombatResult CombatSystem::playerTurn(Player &player, BossWanEshuji &boss, const std::map<int, std::unique_ptr<Item>> &itemDb)
{
    return handlePlayerAction(player, boss, itemDb);
}

void CombatSystem::enemyTurn(BossWanEshuji &boss, Player &player)
{
    ui.displayMessage("--- " + boss.getName() + " 的回合 ---", UIManager::Color::MAGENTA);
    ui.displayMessage(boss.getPhaseAttackDescription(), UIManager::Color::YELLOW);
    
    // 根据当前阶段使用不同的攻击模式
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);
    int roll = dis(gen);
    
    switch (boss.getPhase())
    {
    case 1: // 第一阶段：普通攻击为主
        {
            int damage = calculateDamage(boss, player);
            player.takeDamage(damage);
            ui.displayMessage(boss.getName() + " 用混沌之力对你造成了 " + std::to_string(damage) + " 点伤害!", UIManager::Color::RED);
        }
        break;
        
    case 2: // 第二阶段：增加特殊攻击
        if (roll <= 7)
        {
            int damage = calculateDamage(boss, player);
            player.takeDamage(damage);
            ui.displayMessage(boss.getName() + " 用觉醒的混沌之力对你造成了 " + std::to_string(damage) + " 点伤害!", UIManager::Color::RED);
        }
        else
        {
            int damage = calculateDamage(boss, player, boss.getATK() + 50);
            player.takeDamage(damage);
            ui.displayMessage(boss.getName() + " 使用暗影冲击对你造成了 " + std::to_string(damage) + " 点巨大伤害!", UIManager::Color::RED);
        }
        break;
        
    case 3: // 第三阶段：多种强力攻击
        if (roll <= 4)
        {
            int damage = calculateDamage(boss, player);
            player.takeDamage(damage);
            ui.displayMessage(boss.getName() + " 用最终形态的力量对你造成了 " + std::to_string(damage) + " 点伤害!", UIManager::Color::RED);
        }
        else if (roll <= 7)
        {
            int damage = calculateDamage(boss, player, boss.getATK() + 50);
            player.takeDamage(damage);
            ui.displayMessage(boss.getName() + " 使用暗影冲击对你造成了 " + std::to_string(damage) + " 点巨大伤害!", UIManager::Color::RED);
        }
        else
        {
            int damage = calculateDamage(boss, player, boss.getATK() + 100);
            player.takeDamage(damage);
            ui.displayMessage(boss.getName() + " 使用混沌风暴对你造成了 " + std::to_string(damage) + " 点毁灭性伤害!", UIManager::Color::RED);
        }
        break;
    }
}

// 通用函数实现
CombatResult CombatSystem::handleSkillSelection(Player& player, Attribute& target)
{
    if (player.getSkills().empty())
    {
        ui.displayMessage("你还没有学会任何技能！", UIManager::Color::YELLOW);
        return CombatResult::Continue; // 返回Continue表示需要重新选择
    }
    
    ui.displayMessage("选择技能:", UIManager::Color::CYAN);
    for (size_t i = 0; i < player.getSkills().size(); ++i)
    {
        ui.displayMessage("[" + std::to_string(i + 1) + "] " + player.getSkills()[i]->getName() + " (威力: " + std::to_string(player.getSkills()[i]->getPower()) + ")", UIManager::Color::WHITE);
    }
    ui.displayMessage("[0] 取消", UIManager::Color::GRAY);
    
    int skillChoice = 0;
    std::cin >> skillChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (skillChoice == 0)
    {
        return CombatResult::Continue; // 取消，重新选择行动
    }
    else if (skillChoice > 0 && skillChoice <= player.getSkills().size())
    {
        Skill* skill = player.getSkills()[skillChoice - 1];
        switch (skill->getDamageType())
        {
        case DamageType::PHYSICAL:
        {
            int damage = calculateDamage(player, target, skill->getPower());
            target.takeDamage(damage);
            ui.displayMessage("你使用 " + skill->getName() + " 对 " + target.getName() + " 造成了 " + std::to_string(damage) + " 点物理伤害!", UIManager::Color::RED);
            break;
        }
        case DamageType::MAGICAL:
        {
            int damage = calculateDamage(player, target, skill->getPower());
            target.takeDamage(damage);
            ui.displayMessage("你使用 " + skill->getName() + " 对 " + target.getName() + " 造成了 " + std::to_string(damage) + " 点魔法伤害!", UIManager::Color::BLUE);
            break;
        }
        case DamageType::BUFF:
        {
            player.heal(skill->getPower());
            ui.displayMessage("你使用 " + skill->getName() + " 恢复了 " + std::to_string(skill->getPower()) + " 点生命值!", UIManager::Color::GREEN);
            break;
        }
        case DamageType::HOLY_MARK_SPEED: 
        {
            int speedBoost = skill->getPower();
            player.setSpeed(player.getSpeed() + speedBoost);
            ui.displayMessage(player.getName() + " 使用了【" + skill->getName() + "】！", UIManager::Color::CYAN);
            ui.displayMessage("圣痕疾影步发动！速度提升了 " + std::to_string(speedBoost) + " 点！", UIManager::Color::YELLOW);
            break;
        }
        case DamageType::STAR_ARMOR: 
        {
            int defenseBoost = skill->getPower();
            player.setDEF(player.getDEF() + defenseBoost);
            ui.displayMessage(player.getName() + " 使用了【" + skill->getName() + "】！", UIManager::Color::CYAN);
            ui.displayMessage("星辰圣铠发动！防御提升了 " + std::to_string(defenseBoost) + " 点！", UIManager::Color::YELLOW);
            break;
        }
        }
        return CombatResult::Escaped; // 使用Escaped表示行动完成（临时解决方案）
    }
    else
    {
        ui.displayMessage("无效选择。", UIManager::Color::RED);
        return CombatResult::Continue; // 重新选择
    }
}

CombatResult CombatSystem::handleItemUsage(Player& player, const std::map<int, std::unique_ptr<Item>>& itemDb)
{
    if (player.inventory.empty())
    {
        ui.displayMessage("你的背包是空的!", UIManager::Color::YELLOW);
        return CombatResult::Continue; // 重新选择
    }
    
    ui.displayMessage("选择道具:", UIManager::Color::WHITE);
    std::vector<std::string> itemNames;
    int itemIdx = 1;
    for (const auto& pair : player.inventory)
    {
        ui.displayMessage("[" + std::to_string(itemIdx) + "] " + pair.first + " x" + std::to_string(pair.second), UIManager::Color::WHITE);
        itemNames.push_back(pair.first);
        itemIdx++;
    }
    ui.displayMessage("[0] 取消", UIManager::Color::GRAY);
    
    int itemChoice = 0;
    std::cin >> itemChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (itemChoice == 0)
    {
        return CombatResult::Continue; // 取消，重新选择行动
    }
    else if (itemChoice > 0 && itemChoice <= itemNames.size())
    {
        std::string chosenItemName = itemNames[itemChoice - 1];
        
        if (chosenItemName == "生命药水")
        {
            int healAmount = (player.getMaxHP() * 50) / 100; // 回复50%最大生命值
            player.heal(healAmount);
            player.useItem(chosenItemName);
            ui.displayMessage("你使用了生命药水，恢复了" + std::to_string(healAmount) + "点生命值（50%最大生命值）！", UIManager::Color::GREEN);
        }
        else if (chosenItemName == "能量药水")
        {
            player.extraActionTurns += 1; // 在下一回合开始时生效
            player.useItem(chosenItemName);
            ui.displayMessage("你使用了能量药水，下一回合开始时获得额外行动机会！", UIManager::Color::CYAN);
        }
        else
        {
            ui.displayMessage("这个道具暂时无法使用。", UIManager::Color::YELLOW);
            return CombatResult::Continue; // 重新选择
        }
        return CombatResult::Escaped; // 使用Escaped表示行动完成（临时解决方案）
    }
    else
    {
        ui.displayMessage("无效选择。", UIManager::Color::RED);
        return CombatResult::Continue;
    }
}

template<typename Enemy>
CombatResult CombatSystem::handleEscapeAttempt(Player& player, Enemy& enemy)
{
    // 检查是否是BOSS战斗，不同BOSS有不同的逃跑规则
    if (std::is_same<Enemy, EvilGeneral>::value)
    {
        ui.displayMessage("面对强大的BOSS，你无法逃跑！", UIManager::Color::RED);
        return CombatResult::Continue; // 重新选择
    }
    else if (std::is_same<Enemy, BossWanEshuji>::value)
    {
        ui.displayMessage("面对万恶枢机这样的终极BOSS，你无法逃跑！世界的命运就在你手中！", UIManager::Color::RED);
        return CombatResult::Continue; // 重新选择
    }
    else // CommonEnemy
    {
        if (attemptEscape(player, enemy))
        {
            ui.displayMessage("你成功逃离了战斗!", UIManager::Color::GREEN);
            return CombatResult::Escaped;  // 逃跑成功直接返回
        }
        else
        {
            ui.displayMessage("逃跑失败了!", UIManager::Color::RED);
            return CombatResult::Continue; // 逃跑失败会消耗一次行动，继续战斗
        }
    }
}

template<typename Enemy>
CombatResult CombatSystem::handlePlayerAction(Player& player, Enemy& enemy, const std::map<int, std::unique_ptr<Item>>& itemDb)
{
    ui.displaySimpleCombatStatus(player, enemy);
    
    ui.displayMessage("选择行动: [1]攻击 [2]技能 [3]道具 [4]逃跑 [5]查看状态", UIManager::Color::WHITE);
    int choice = 0;
    while (!(std::cin >> choice) || choice < 1 || choice > 5)
    {
        std::cout << "无效输入, 请输入 1-5 之间的数字: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice)
    {
    case 1: // 攻击
    {
        int damage = calculateDamage(player, enemy);
        enemy.takeDamage(damage);
        ui.displayMessage("你对 " + enemy.getName() + " 造成了 " + std::to_string(damage) + " 点伤害!", UIManager::Color::RED);
        break;
    }
    case 2: // 技能
    {
        CombatResult result = handleSkillSelection(player, enemy);
        if (result == CombatResult::Continue)
        {
            return handlePlayerAction(player, enemy, itemDb); // 重新选择
        }
        // 如果是Escaped，表示技能使用成功，结束当前回合
        break;
    }
    case 3: // 道具
    {
        CombatResult result = handleItemUsage(player, itemDb);
        if (result == CombatResult::Continue)
        {
            return handlePlayerAction(player, enemy, itemDb); // 重新选择
        }
        // 如果是Escaped，表示道具使用成功，结束当前回合
        break;
    }
    case 4: // 逃跑
    {
        return handleEscapeAttempt(player, enemy);
    }
    case 5: // 查看详细状态（不消耗回合）
    {
        ui.displayPlayerStatus(player);
        return handlePlayerAction(player, enemy, itemDb); // 重新选择行动
    }
    }
    return CombatResult::Continue; // 继续战斗
}