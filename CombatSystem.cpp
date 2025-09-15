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

// 新增：进度条与HUD渲染
std::string CombatSystem::makeBar(int current, int max, int width, const std::string& full, const std::string& empty) {
    if (max <= 0) max = 1;
    if (current < 0) current = 0;
    if (current > max) current = max;
    int filled = static_cast<int>((current * 1.0 / max) * width + 0.5);
    if (filled > width) filled = width;

    std::string bar = "[";
    for (int i = 0; i < filled; ++i) bar += full;
    for (int i = filled; i < width; ++i) bar += empty;
    bar.push_back(']');
    return bar;
}

void CombatSystem::renderBattleHUD(const Player& player, const Attribute& enemy) {
    // 顶部边框
    ui.displayMessage("┏━━━━━━━━━━━━━━ 战斗状态 ━━━━━━━━━━━━━━┓", UIManager::Color::WHITE);

    // 玩家信息
    std::string pHpBar = makeBar(player.getHP(), player.getMaxHP(), 22);
    std::string pMpBar = makeBar(player.getMP(), player.getMaxMP(), 22);
    int pHpPct = (player.getMaxHP() > 0) ? (player.getHP() * 100 / player.getMaxHP()) : 0;
    int pMpPct = (player.getMaxMP() > 0) ? (player.getMP() * 100 / player.getMaxMP()) : 0;

    ui.displayMessage("︳你 Lv" + std::to_string(player.getLevel()) + "  ｜ Buff: " + player.getBuffStatus(), UIManager::Color::CYAN);
    ui.displayMessage("︳HP " + std::to_string(player.getHP()) + "/" + std::to_string(player.getMaxHP()) + " " + pHpBar + " " + std::to_string(pHpPct) + "%", UIManager::Color::GREEN);
    ui.displayMessage("︳MP " + std::to_string(player.getMP()) + "/" + std::to_string(player.getMaxMP()) + " " + pMpBar + " " + std::to_string(pMpPct) + "%", UIManager::Color::BLUE);

    ui.displayMessage("┣──────────────────────────────────────────┫", UIManager::Color::WHITE);

    // 敌人信息（统一当作红条）
    std::string eHpBar = makeBar(enemy.getHP(), enemy.getMaxHP(), 30);
    int eHpPct = (enemy.getMaxHP() > 0) ? (enemy.getHP() * 100 / enemy.getMaxHP()) : 0;
    ui.displayMessage("︳" + enemy.getName() + " Lv" + std::to_string(enemy.getLevel()), UIManager::Color::MAGENTA);
    ui.displayMessage("︳HP " + std::to_string(enemy.getHP()) + "/" + std::to_string(enemy.getMaxHP()) + " " + eHpBar + " " + std::to_string(eHpPct) + "%", UIManager::Color::RED);

    // 底部边框
    ui.displayMessage("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛", UIManager::Color::WHITE);
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
        baseAttack = static_cast<int>(baseAttack * 1.6); // 调整：降低暴击倍数，抑制秒杀
    }
    int damage = baseAttack - defender.getDEF();
    damage = std::max(1, damage);

    // 新增：对BOSS类目标的单次伤害上限
    // 说明：将军BOSS单次伤害≤其最大生命的18%，最终BOSS≤12%
    if (const auto* fb = dynamic_cast<const BossWanEshuji*>(&defender)) {
        int cap = std::max(1, static_cast<int>(defender.getMaxHP() * 0.12));
        damage = std::min(damage, cap);
    } else if (const auto* boss = dynamic_cast<const EvilGeneral*>(&defender)) {
        int cap = std::max(1, static_cast<int>(defender.getMaxHP() * 0.18));
        damage = std::min(damage, cap);
    }
    return damage;
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
    // 新增：黄金树之誓护盾判定（免疫物理伤害）
    if (player.getPhysicalShieldTurns() > 0) {
        player.consumePhysicalShieldTurn();
        ui.displayMessage("黄金树之誓庇护生效，本回合免疫物理伤害！(剩余 " + std::to_string(player.getPhysicalShieldTurns()) + " 回合)", UIManager::Color::CYAN);
        ui.displayMessage(enemy.getName() + " 的攻击被完全吸收！", UIManager::Color::YELLOW);
    } else {
        int damage = calculateDamage(enemy, player);
        player.takeDamage(damage);
        ui.displayMessage(enemy.getName() + " 对你造成了 " + std::to_string(damage) + " 点伤害!", UIManager::Color::RED);
    }
    
    // 新增：回合结束时更新增益状态
    player.updateBuffTurns();
}

void CombatSystem::enemyTurn(EvilGeneral &boss, Player &player)
{
    ui.displayMessage("--- " + boss.getName() + " 的回合 ---", UIManager::Color::MAGENTA);
    // 新增：黄金树之誓护盾判定（免疫物理伤害）
    if (player.getPhysicalShieldTurns() > 0) {
        player.consumePhysicalShieldTurn();
        ui.displayMessage("黄金树之誓庇护生效，本回合免疫物理伤害！(剩余 " + std::to_string(player.getPhysicalShieldTurns()) + " 回合)", UIManager::Color::CYAN);
        ui.displayMessage(boss.getName() + " 的攻击被完全吸收！", UIManager::Color::YELLOW);
    } else {
        int damage = calculateDamage(boss, player);
        player.takeDamage(damage);
        ui.displayMessage(boss.getName() + " 对你造成了 " + std::to_string(damage) + " 点伤害!", UIManager::Color::RED);
    }
    
    // 新增：回合结束时更新增益状态
    player.updateBuffTurns();
}

void CombatSystem::enemyTurn(BossWanEshuji &boss, Player &player)
{
    ui.displayMessage("--- " + boss.getName() + " 的回合 ---", UIManager::Color::MAGENTA);
    ui.displayMessage(boss.getPhaseAttackDescription(), UIManager::Color::YELLOW);

    // 新增：黄金树之誓护盾判定（免疫物理伤害）
    if (player.getPhysicalShieldTurns() > 0) {
        player.consumePhysicalShieldTurn();
        ui.displayMessage("黄金树之誓庇护生效，本回合免疫物理伤害！(剩余 " + std::to_string(player.getPhysicalShieldTurns()) + " 回合)", UIManager::Color::CYAN);
        ui.displayMessage(boss.getName() + " 的攻击被完全吸收！", UIManager::Color::YELLOW);
        // 新增：回合结束时更新增益状态
        player.updateBuffTurns();
        return;
    }
    
    // 根据当前阶段使用不同的攻击模式
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);
    int roll = dis(gen);
    
    switch (boss.getPhase())
    {
    case 1:
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
    
    // 新增：回合结束时更新增益状态
    player.updateBuffTurns();
}

// 通用函数实现
CombatResult CombatSystem::handleSkillSelection(Player& player, Attribute& target)
{
    if (player.getSkills().empty())
    {
        ui.displayMessage("你还没有学会任何技能！", UIManager::Color::YELLOW);
        return CombatResult::Continue;
    }
    
    ui.displayMessage("选择技能:", UIManager::Color::CYAN);
    for (size_t i = 0; i < player.getSkills().size(); ++i)
    {
        int scaled = player.getSkills()[i]->getScaledPower(player); // 改：按AD/AP+等级
        int mpCost = player.getSkills()[i]->getMpCost(player.getLevel());
        ui.displayMessage("[" + std::to_string(i + 1) + "] " + player.getSkills()[i]->getName() +
                          " (威力: " + std::to_string(scaled) + " | MP: " + std::to_string(mpCost) + ")",
                          UIManager::Color::WHITE);
    }
    ui.displayMessage("[0] 取消", UIManager::Color::GRAY);
    
    int skillChoice = 0;
    std::cin >> skillChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (skillChoice == 0)
    {
        return CombatResult::Continue;
    }
    else if (skillChoice > 0 && skillChoice <= player.getSkills().size())
    {
        Skill* skill = player.getSkills()[skillChoice - 1];
        int scaledPower = skill->getScaledPower(player);            // 改：按AD/AP+等级
        int mpCost = skill->getMpCost(player.getLevel());

        if (player.getMP() < mpCost) {
            ui.displayMessage("蓝量不足，无法释放技能。", UIManager::Color::RED);
            return CombatResult::Continue;
        }
        player.setMP(player.getMP() - mpCost);

        switch (skill->getDamageType())
        {
        case DamageType::PHYSICAL:
        {
            int damage = calculateDamage(player, target, scaledPower);
            target.takeDamage(damage);
            ui.displayMessage("你使用 " + skill->getName() + " 对 " + target.getName() + " 造成了 " + std::to_string(damage) + " 点物理伤害!", UIManager::Color::RED);
            break;
        }
        case DamageType::MAGICAL:
        {
            int damage = calculateDamage(player, target, scaledPower);
            target.takeDamage(damage);
            ui.displayMessage("你使用 " + skill->getName() + " 对 " + target.getName() + " 造成了 " + std::to_string(damage) + " 点魔法伤害!", UIManager::Color::BLUE);
            break;
        }
        case DamageType::BUFF:
        {
            // 黄金树之誓：恢复最大生命/最大蓝量的30%，并获得2回合物理免疫
            if (skill->getType() == SkillType::GOLDEN_TREE_VOW) {
                int healHp = (player.getMaxHP() * 30) / 100;
                int healMp = (player.getMaxMP() * 30) / 100;
                player.heal(healHp);
                player.setMP(player.getMP() + healMp); // setMP内部已做上限裁剪
                player.addPhysicalShieldTurns(2);
                ui.displayMessage("黄金树之誓发动！恢复了" + std::to_string(healHp) + "点生命（30%最大生命），恢复了" + std::to_string(healMp) + "点蓝量（30%最大蓝量）。", UIManager::Color::GREEN);
                ui.displayMessage("获得圣光庇护：接下来的2个敌方回合免疫物理伤害！", UIManager::Color::CYAN);
            } else {
                // 其他一般增益：按原逻辑回血
                player.heal(scaledPower);
                ui.displayMessage("你使用 " + skill->getName() + " 恢复了 " + std::to_string(scaledPower) + " 点生命值!", UIManager::Color::GREEN);
            }
            break;
        }
        case DamageType::HOLY_MARK_SPEED: 
        {
            int speedBoost = scaledPower;
            player.addSpeedBoost(speedBoost, 2); // 修改：使用新的增益系统，持续2回合
            ui.displayMessage(player.getName() + " 使用了【" + skill->getName() + "】！", UIManager::Color::CYAN);
            ui.displayMessage("圣痕疾影步发动！速度提升了 " + std::to_string(speedBoost) + " 点，持续2回合！", UIManager::Color::YELLOW);
            break;
        }
        case DamageType::STAR_ARMOR: 
        {
            int defenseBoost = scaledPower;
            player.addDefenseBoost(defenseBoost, 2); // 修改：使用新的增益系统，持续2回合
            ui.displayMessage(player.getName() + " 使用了【" + skill->getName() + "】！", UIManager::Color::CYAN);
            ui.displayMessage("星辰圣铠发动！防御提升了 " + std::to_string(defenseBoost) + " 点，持续2回合！", UIManager::Color::YELLOW);
            break;
        }
        }
        return CombatResult::Escaped; // 行动完成
    }
    else
    {
        ui.displayMessage("无效选择。", UIManager::Color::RED);
        return CombatResult::Continue;
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
            ui.displayMessage("药水效果瞬间生效，你可以继续行动！", UIManager::Color::CYAN);
            return CombatResult::Continue; // 不消耗回合，可以继续行动
        }
        else if (chosenItemName == "能量药水")
        {
            int mpRecoverAmount = (player.getMaxMP() * 50) / 100; // 改：回复50%最大蓝量
            player.setMP(player.getMP() + mpRecoverAmount);
            player.useItem(chosenItemName);
            ui.displayMessage("你使用了能量药水，恢复了" + std::to_string(mpRecoverAmount) + "点蓝量（50%最大蓝量）！", UIManager::Color::CYAN);
            ui.displayMessage("药水效果瞬间生效，你可以继续行动！", UIManager::Color::CYAN);
            return CombatResult::Continue; // 不消耗回合，可以继续行动
        }
        else if (chosenItemName == "神秘商品")
        {
            ui.displayMessage("战斗中不能使用这个物品。", UIManager::Color::YELLOW);
            return CombatResult::Continue; // 重新选择
        }
        else
        {
            ui.displayMessage("这个道具暂时无法使用。", UIManager::Color::YELLOW);
            return CombatResult::Continue; // 重新选择
        }
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
    // 美化操作提示栏
    ui.displayMessage("选择行动: [1]⚔️ 攻击  [2]✨ 技能  [3]🎒 道具  [4]🏃 逃跑  [5]🔎 状态", UIManager::Color::WHITE);
    
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

CombatResult CombatSystem::startCombat(Player& player, CommonEnemy& enemy, const std::map<int, std::unique_ptr<Item>>& itemDb)
{
    ui.displayMessage("战斗开始！你遭遇了 " + enemy.getName() + "!", UIManager::Color::MAGENTA);
    ui.displayMessage("敌人等级: " + std::to_string(enemy.getLevel()), UIManager::Color::WHITE);
    
    while (player.isAlive() && enemy.isAlive())
    {
        // 显示美化后的战斗面板（替换原有两行状态）
        renderBattleHUD(player, enemy);

        // 玩家回合
        CombatResult playerResult = playerTurn(player, enemy, itemDb);
        if (playerResult != CombatResult::Continue) {
            return playerResult;
        }
        
        if (!enemy.isAlive()) {
            ui.displayMessage("你击败了 " + enemy.getName() + "!", UIManager::Color::GREEN);
            player.addExp(enemy.getExpReward());
            player.addGold(enemy.getGoldReward());
            ui.displayMessage("获得 " + std::to_string(enemy.getExpReward()) + " 经验值和 " + 
                             std::to_string(enemy.getGoldReward()) + " 金币!", UIManager::Color::YELLOW);
            
            // 移除：这里不要提示黑曜晶尘显现，避免击败第一只狼时提前误导
            // （显现提示改到 game.cpp 中，且仅在两只狼都被击败时提示）
            
            return CombatResult::Victory;
        }
        
        // 敌人回合
        enemyTurn(enemy, player);
        
        if (!player.isAlive()) {
            ui.displayMessage("你被 " + enemy.getName() + " 击败了...", UIManager::Color::RED);
            ui.displayMessage("选择: [1]重试 [2]读档 [3]退出游戏", UIManager::Color::YELLOW);
            
            int choice = 0;
            while (!(std::cin >> choice) || choice < 1 || choice > 3) {
                std::cout << "无效输入，请输入 1-3: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            switch (choice) {
                case 1: return CombatResult::Defeat_Restart;
                case 2: return CombatResult::Defeat_Load;
                case 3: return CombatResult::Defeat_Exit;
                default: return CombatResult::Defeat_Restart;
            }
        }
    }
    
    return CombatResult::Continue;
}

CombatResult CombatSystem::startCombat(Player& player, EvilGeneral& boss, const std::map<int, std::unique_ptr<Item>>& itemDb)
{
    ui.displayMessage("=====================================================", UIManager::Color::MAGENTA);
    ui.displayMessage("BOSS战斗开始！你面对的是强大的 " + boss.getName() + "!", UIManager::Color::MAGENTA);
    ui.displayMessage("=====================================================", UIManager::Color::MAGENTA);
    
    // 显示战前对话
    ui.displayMessage(boss.getPreBattleDialogue(), UIManager::Color::WHITE);
    ui.pause();
    
    while (player.isAlive() && boss.isAlive())
    {
        // 美化后的战斗面板
        renderBattleHUD(player, boss);
        
        // 玩家回合
        CombatResult playerResult = playerTurn(player, boss, itemDb);
        if (playerResult != CombatResult::Continue) {
            return playerResult;
        }
        
        if (!boss.isAlive()) {
            ui.displayMessage("你击败了 " + boss.getName() + "!", UIManager::Color::GREEN);
            ui.displayMessage(boss.getDefeatDialogue(), UIManager::Color::WHITE);
            player.addExp(boss.getExpReward());
            player.addGold(boss.getGoldReward());
            ui.displayMessage("获得 " + std::to_string(boss.getExpReward()) + " 经验值和 " + 
                             std::to_string(boss.getGoldReward()) + " 金币!", UIManager::Color::YELLOW);
            return CombatResult::Victory;
        }
        
        // 敌人回合
        enemyTurn(boss, player);
        
        if (!player.isAlive()) {
            ui.displayMessage("你被 " + boss.getName() + " 击败了...", UIManager::Color::RED);
            ui.displayMessage(boss.getVictoryDialogue(), UIManager::Color::WHITE);
            ui.displayMessage("选择: [1]重试 [2]读档 [3]退出游戏", UIManager::Color::YELLOW);
            
            int choice = 0;
            while (!(std::cin >> choice) || choice < 1 || choice > 3) {
                std::cout << "无效输入，请输入 1-3: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            switch (choice) {
                case 1: return CombatResult::Defeat_Restart;
                case 2: return CombatResult::Defeat_Load;
                case 3: return CombatResult::Defeat_Exit;
                default: return CombatResult::Defeat_Restart;
            }
        }
    }
    
    return CombatResult::Continue;
}

CombatResult CombatSystem::startCombat(Player& player, BossWanEshuji& boss, const std::map<int, std::unique_ptr<Item>>& itemDb)
{
    ui.displayMessage("=====================================================", UIManager::Color::MAGENTA);
    ui.displayMessage("最终决战开始！你面对的是万恶枢机！", UIManager::Color::MAGENTA);
    ui.displayMessage("=====================================================", UIManager::Color::MAGENTA);
    
    // 显示战前对话
    ui.displayMessage(boss.getPreBattleDialogue(), UIManager::Color::WHITE);
    ui.pause();
    
    while (player.isAlive() && boss.isAlive())
    {
        // 阶段判定
        if (boss.shouldEnterNextPhase()) {
            boss.enterNextPhase();
            ui.pause();
        }
        
        // 美化后的战斗面板（含阶段信息已在顶部标题外展示于名称行）
        renderBattleHUD(player, boss);
        
        // 玩家回合
        CombatResult playerResult = playerTurn(player, boss, itemDb);
        if (playerResult != CombatResult::Continue) {
            return playerResult;
        }
        
        if (!boss.isAlive()) {
            ui.displayMessage("你击败了万恶枢机！世界得救了！", UIManager::Color::GREEN);
            ui.displayMessage(boss.getDefeatDialogue(), UIManager::Color::WHITE);
            player.addExp(boss.getExpReward());
            player.addGold(boss.getGoldReward());
            ui.displayMessage("获得 " + std::to_string(boss.getExpReward()) + " 经验值和 " + 
                             std::to_string(boss.getGoldReward()) + " 金币!", UIManager::Color::YELLOW);
            return CombatResult::Victory;
        }
        
        // 敌人回合
        enemyTurn(boss, player);
        
        if (!player.isAlive()) {
            ui.displayMessage("你被万恶枢机击败了...", UIManager::Color::RED);
            ui.displayMessage(boss.getVictoryDialogue(), UIManager::Color::WHITE);
            ui.displayMessage("选择: [1]重试 [2]读档 [3]退出游戏", UIManager::Color::YELLOW);
            
            int choice = 0;
            while (!(std::cin >> choice) || choice < 1 || choice > 3) {
                std::cout << "无效输入，请输入 1-3: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            switch (choice) {
                case 1: return CombatResult::Defeat_Restart;
                case 2: return CombatResult::Defeat_Load;
                case 3: return CombatResult::Defeat_Exit;
                default: return CombatResult::Defeat_Restart;
            }
        }
    }
    
    return CombatResult::Continue;
}

CombatResult CombatSystem::playerTurn(Player& player, BossWanEshuji& boss, const std::map<int, std::unique_ptr<Item>>& itemDb)
{
    return handlePlayerAction(player, boss, itemDb);
}