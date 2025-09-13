#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H
#include <map>
#include <memory>
#include"BossWanEshuji.h"
#include"Player.h"
#include "UIManager.h"

/**
 * @enum CombatResult
 * @brief 战斗结果：胜利/失败的后续选择/逃跑/继续。
 */
enum class CombatResult {
    Victory,
    Defeat_Restart,
    Defeat_Load,
    Defeat_Exit,
    Escaped,
    Continue
};

/**
 * @class CombatSystem
 * @brief 战斗系统：玩家回合/敌人回合/技能与道具/逃跑判定。
 */
class CombatSystem {
public:
    /** @brief 构造战斗系统。 */
    CombatSystem(UIManager& uiManager);

    /** @brief 开始与普通敌人的战斗。 */
    CombatResult startCombat(Player& player, CommonEnemy& enemy, const std::map<int, std::unique_ptr<Item>>& itemDb);
    /** @brief 开始与恶念将军的战斗。 */
    CombatResult startCombat(Player& player, EvilGeneral& boss, const std::map<int, std::unique_ptr<Item>>& itemDb);
    /** @brief 开始与最终BOSS的战斗（多阶段）。 */
    CombatResult startCombat(Player& player, BossWanEshuji& boss, const std::map<int, std::unique_ptr<Item>>& itemDb);

private:
    UIManager& ui;

    /** @brief 玩家回合（多态）。 */
    CombatResult playerTurn(Player& player, CommonEnemy& enemy, const std::map<int, std::unique_ptr<Item>>& itemDb);
    CombatResult playerTurn(Player& player, EvilGeneral& boss, const std::map<int, std::unique_ptr<Item>>& itemDb);
    CombatResult playerTurn(Player& player, BossWanEshuji& boss, const std::map<int, std::unique_ptr<Item>>& itemDb);

    /** @brief 敌人/BOSS/最终BOSS 回合行为。 */
    void enemyTurn(CommonEnemy& enemy, Player& player);
    void enemyTurn(EvilGeneral& boss, Player& player);
    void enemyTurn(BossWanEshuji& boss, Player& player);

    /** @brief 通用处理：玩家的行动选择与执行。 */
    template<typename Enemy>
    CombatResult handlePlayerAction(Player& player, Enemy& enemy, const std::map<int, std::unique_ptr<Item>>& itemDb);

    /** @brief 处理技能选择/释放。 */
    CombatResult handleSkillSelection(Player& player, Attribute& target);

    /** @brief 处理战斗中道具使用。 */
    CombatResult handleItemUsage(Player& player, const std::map<int, std::unique_ptr<Item>>& itemDb);

    /** @brief 处理逃跑逻辑（BOSS 禁止逃跑）。 */
    template<typename Enemy>
    CombatResult handleEscapeAttempt(Player& player, Enemy& enemy);

    /** @brief 普通敌人的逃跑成功率计算。 */
    bool attemptEscape(const Player& player, const CommonEnemy& enemy);

    /** @brief 通用伤害计算：支持传入固定威力。 */
    int calculateDamage(const Attribute& attacker, const Attribute& defender, int power = 0);
};

#endif // COMBATSYSTEM_H