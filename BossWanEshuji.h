#pragma once
#ifndef BOSSWANESHUJI_H
#define BOSSWANESHUJI_H
#include "EvilGeneral.h"
#include <iostream>
#include <vector>

class BossWanEshuji : public EvilGeneral {
private:
    int phase;                  // 战斗阶段（1-3）
public:
    BossWanEshuji();
    // 多阶段战斗逻辑（阶段提升时属性变化）
    void enterNextPhase();
    int getPhase() const;
    static std::string getEnemyName() { return "万恶枢机"; }

    // 特殊技能：混沌风暴（只有第三阶段才能使用）
    bool canUseChaosStorm() const;
    
    // 特殊技能：暗影冲击（第二阶段开始可用）
    bool canUseShadowStrike() const;
    
    // 获取当前阶段的攻击描述
    std::string getPhaseAttackDescription() const;

    // 阶段转换时的特殊效果
    void phaseTransitionEffect();
    
    // 检查是否应该进入下一阶段
    bool shouldEnterNextPhase() const;

    // 对话系统（重写父类的虚函数）
    std::string getPreBattleDialogue() const override;
    std::string getDefeatDialogue() const override;
    std::string getVictoryDialogue() const override;
};

#endif // BOSSWANESHUJI_H