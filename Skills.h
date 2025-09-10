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
    int power;        // 技能威力（伤害/加成值）

public:
    Skill(SkillType type, std::string name, std::string desc, int unlockLv,
        SkillTarget target, DamageType dmgType, int power);

    SkillType getType() const;
    std::string getName() const;
    std::string getDescription() const;
    int getUnlockLevel() const;
    SkillTarget getTarget() const;
    DamageType getDamageType() const;
    int getPower() const;
};

#endif // SKILLS_H