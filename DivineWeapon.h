#pragma once
#ifndef DIVINEWEAPON_H
#define DIVINEWEAPON_H
#include "Equipment.h"

class DivineWeapon : public Equipment {
private:
    int growthLevel;  // 神剑成长等级（随玩家等级提升）

public: 
    DivineWeapon();  // 初始神剑（开场自带）
    DivineWeapon* clone() const override;
    // 神剑成长（随玩家等级提升攻击力）
    void grow(int playerLevel);

    // 解锁技能（根据成长等级解锁对应技能）
    bool unlockSkill(int playerLevel, SkillType& skillType);
};

#endif // DIVINEWEAPON_H