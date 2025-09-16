#include "Skills.h"
#include "Player.h"
#include <sstream>
#include <algorithm>

static inline int clampInt(int v, int lo, int hi) { return std::max(lo, std::min(hi, v)); }

Skill::Skill(SkillType type,
             std::string name,
             std::string desc,
             int unlockLevel,
             SkillTarget target,
             DamageType dmgType,
             int basePower)
    : type_(type),
      name_(std::move(name)),
      desc_(std::move(desc)),
      unlockLevel_(unlockLevel),
      target_(target),
      dmgType_(dmgType),
      basePower_(basePower),
      adRatio_(0.0f),
      apRatio_(0.0f),
      baseMpCost_(DEFAULT_MP_COST_PHYSICAL),
      mpCostGrowth_(DEFAULT_SKILL_MP_GROWTH_PER_LEVEL)
{
    // 默认蓝耗
    switch (dmgType_) {
        case DamageType::PHYSICAL:       baseMpCost_ = DEFAULT_MP_COST_PHYSICAL; break;
        case DamageType::MAGICAL:        baseMpCost_ = DEFAULT_MP_COST_MAGICAL;  break;
        case DamageType::BUFF:
        case DamageType::STAR_ARMOR:
        case DamageType::HOLY_MARK_SPEED:baseMpCost_ = DEFAULT_MP_COST_BUFF;     break;
    }

    // 默认系数（类似MOBA：基础值 + AD/AP系数）
    switch (dmgType_) {
        case DamageType::PHYSICAL:
            adRatio_ = 1.0f;   // 100% AD
            apRatio_ = 0.0f;
            break;
        case DamageType::MAGICAL:
            adRatio_ = 0.2f;   // 少量受AD影响
            apRatio_ = 0.6f;   // 60% “AP”(用MaxMP近似)
            break;
        case DamageType::BUFF:
        case DamageType::STAR_ARMOR:
        case DamageType::HOLY_MARK_SPEED:
            adRatio_ = 0.0f;
            apRatio_ = 0.0f;
            break;
    }
}

// 仅按等级做线性成长
int Skill::getScaledPower(int playerLevel) const {
    int lvDelta = std::max(0, playerLevel - unlockLevel_);
    float growth = 1.0f + DEFAULT_SKILL_GROWTH_PER_LEVEL * lvDelta;
    int val = static_cast<int>(basePower_ * growth);
    return std::max(1, val);
}

// 含AD/AP与等级成长（实战用）
int Skill::getScaledPower(const Player& player) const {
    int playerLevel = player.getLevel();
    int lvDelta = std::max(0, playerLevel - unlockLevel_);
    float growth = 1.0f + DEFAULT_SKILL_GROWTH_PER_LEVEL * lvDelta;

    int adPart = static_cast<int>(player.getATK() * adRatio_);
    int apPart = static_cast<int>(player.getMaxMP() * apRatio_); // 近似AP
    int raw = basePower_ + adPart + apPart;
    int val = static_cast<int>(raw * growth);
    return std::max(1, val);
}

int Skill::getMpCost(int playerLevel) const {
    int lvDelta = std::max(0, playerLevel - unlockLevel_);
    float scale = 1.0f + mpCostGrowth_ * lvDelta;
    int cost = static_cast<int>(baseMpCost_ * scale);
    // 下限保护
    return std::max(1, cost);
}

std::string Skill::getDetailedDescription(int playerLevel) const {
    std::ostringstream oss;
    oss << name_ << " - " << desc_ << "\n";
    oss << "解锁等级: " << unlockLevel_ << "\n";
    oss << "基础威力: " << basePower_ << " | 系数: AD*" << adRatio_ << " + AP*" << apRatio_ << " (AP≈MaxMP)\n";
    oss << "当前等级威力(不含AD/AP演示): " << getScaledPower(playerLevel) << "\n";
    oss << "蓝耗(当前等级): " << getMpCost(playerLevel);
    return oss.str();
}