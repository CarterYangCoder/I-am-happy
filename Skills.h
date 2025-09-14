#pragma once
#ifndef SKILLS_H
#define SKILLS_H

#include <string>
#include "GameData.h"

// 目标与伤害类型（CombatSystem 已使用这些枚举值）
enum class SkillTarget { SELF, ENEMY };
enum class DamageType { PHYSICAL, MAGICAL, BUFF, STAR_ARMOR, HOLY_MARK_SPEED };

// 前向声明，供接口使用（实现里包含 Player.h）
class Player;

class Skill {
private:
    SkillType    type_;
    std::string  name_;
    std::string  desc_;
    int          unlockLevel_;
    SkillTarget  target_;
    DamageType   dmgType_;
    // 数值与成长
    int          basePower_;          // 基础威力（或增益强度基准）
    float        adRatio_;            // 物攻系数（AD）
    float        apRatio_;            // 法强系数（用 MaxMP 近似）
    int          baseMpCost_;         // 基础蓝耗
    float        mpCostGrowth_;       // 蓝耗成长（/级）

public:
    Skill(SkillType type,
          std::string name,
          std::string desc,
          int unlockLevel,
          SkillTarget target,
          DamageType dmgType,
          int basePower);

    // 基本信息
    const std::string& getName() const { return name_; }
    const std::string& getDescription() const { return desc_; }
    SkillType getType() const { return type_; }
    DamageType getDamageType() const { return dmgType_; }
    SkillTarget getTarget() const { return target_; }
    int getUnlockLevel() const { return unlockLevel_; }

    // 伤害/数值计算
    // 展示用（仅按等级成长，不含AD/AP；保留兼容）
    int getScaledPower(int playerLevel) const;
    // 实际计算（含AD/AP与等级成长）
    int getScaledPower(const Player& player) const;

    // MP 消耗（随等级线性增长）
    int getMpCost(int playerLevel) const;

    // 详细描述（含系数说明）
    std::string getDetailedDescription(int playerLevel) const;
};

#endif // SKILLS_H