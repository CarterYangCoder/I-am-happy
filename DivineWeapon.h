#pragma once
#ifndef DIVINEWEAPON_H
#define DIVINEWEAPON_H
#include "Equipment.h"

/**
 * @class DivineWeapon
 * @brief 神剑：随玩家成长提升攻击力，并可触发技能解锁。
 */
class DivineWeapon : public Equipment {
private:
    int growthLevel;  ///< 神剑成长等级（随玩家等级提升）

public:
    /** @brief 构造初始神剑。 */
    DivineWeapon();
    DivineWeapon* clone() const override;

    /** @brief 神剑成长（更新攻击力）。 */
    void grow(int playerLevel);

    /**
     * @brief 按玩家等级尝试解锁技能。
     * @return true 解锁成功，并返回 skillType
     */
    bool unlockSkill(int playerLevel, SkillType& skillType);
};

#endif // DIVINEWEAPON_H