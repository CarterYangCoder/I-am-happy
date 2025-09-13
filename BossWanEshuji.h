#pragma once
#ifndef BOSSWANESHUJI_H
#define BOSSWANESHUJI_H
#include "EvilGeneral.h"
#include <iostream>
#include <vector>

/**
 * @class BossWanEshuji
 * @brief 最终BOSS：三阶段形态，阶段间属性与技能变化。
 */
class BossWanEshuji : public EvilGeneral {
private:
    int phase;  ///< 战斗阶段（1-3）

public:
    /** @brief 构造最终BOSS并初始化属性与阶段。 */
    BossWanEshuji();

    /** @brief 进入下一阶段并触发阶段特效。 */
    void enterNextPhase();

    /** @brief 获取当前阶段。 */
    int getPhase() const;

    /** @brief 名称工具。 */
    static std::string getEnemyName() { return "万恶枢机"; }

    /** @brief 第三阶段专属的混沌风暴是否可用。 */
    bool canUseChaosStorm() const;

    /** @brief 第二阶段开始可用的暗影冲击。 */
    bool canUseShadowStrike() const;

    /** @brief 获取当前阶段的攻击描述。 */
    std::string getPhaseAttackDescription() const;

    /** @brief 阶段切换时执行的特效（回血等）。 */
    void phaseTransitionEffect();

    /** @brief 条件满足时进入下一阶段。 */
    bool shouldEnterNextPhase() const;

    /** @brief 覆写：战前/敌败/敌胜 对话。 */
    std::string getPreBattleDialogue() const override;
    std::string getDefeatDialogue() const override;
    std::string getVictoryDialogue() const override;
};

#endif // BOSSWANESHUJI_H