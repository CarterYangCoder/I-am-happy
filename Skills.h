#pragma once
#ifndef SKILLS_H
#define SKILLS_H
#include "GameData.h"
#include <string>

// 技能目标类型
enum class SkillTarget {
    ENEMY,  // 目标为敌人 
    SELF    // 目标为自己
};

// 技能伤害类型
enum class DamageType {
    PHYSICAL,  // 物理伤害
    MAGICAL,   // 魔法伤害
    BUFF,       // 增益效果（无伤害）
    HOLY_MARK_SPEED, // 圣痕疾影步（提升速度）
    STAR_ARMOR      // 星辰圣铠（提升防御）
};

class Skill {
private:
    SkillType type;
    std::string name;
    std::string description;
    int unlockLevel;  // 解锁等级
    SkillTarget target;
    DamageType damageType;
    int basePower;        // 基础威力
    float growthPerLevel; // 威力成长

    // 新增：蓝量消耗
    int baseMpCost;             // 基础MP消耗（解锁时）
    float mpCostGrowthPerLevel; // 每级MP消耗成长（线性，基于解锁等级起算）

public:
    // 新增可选参数：baseMpCost/mpCostGrowth，默认按伤害类型套用，并保持兼容
    Skill(SkillType type, std::string name, std::string desc, int unlockLv,
        SkillTarget target, DamageType dmgType, int power, float growth = DEFAULT_SKILL_GROWTH_PER_LEVEL,
        int baseMpCost = -1, float mpCostGrowth = DEFAULT_SKILL_MP_GROWTH_PER_LEVEL);

    SkillType getType() const;
    std::string getName() const;
    std::string getDescription() const;
    int getUnlockLevel() const;
    SkillTarget getTarget() const;
    DamageType getDamageType() const;
    int getPower() const;
    int getScaledPower(int casterLevel) const;

    // 新增：按施放者等级返回实际MP消耗
    int getMpCost(int casterLevel) const;

    // 新增：获取技能详细描述
    std::string getDetailedDescription(int casterLevel) const;
};

#endif // SKILLS_H