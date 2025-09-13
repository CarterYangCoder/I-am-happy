#include "Skills.h"
#include <algorithm>

Skill::Skill(SkillType type, std::string name, std::string desc, int unlockLv,
    SkillTarget target, DamageType dmgType, int power, float growth,
    int baseMpCost, float mpCostGrowth)
    : type(type), name(name), description(desc), unlockLevel(unlockLv),
      target(target), damageType(dmgType), basePower(power), growthPerLevel(growth),
      baseMpCost(baseMpCost), mpCostGrowthPerLevel(mpCostGrowth)
{
    // 若未指定基础MP消耗，则按伤害类型套用默认
    if (this->baseMpCost < 0) {
        switch (damageType) {
        case DamageType::MAGICAL:
            this->baseMpCost = DEFAULT_MP_COST_MAGICAL; break;
        case DamageType::PHYSICAL:
            this->baseMpCost = DEFAULT_MP_COST_PHYSICAL; break;
        case DamageType::BUFF:
        case DamageType::HOLY_MARK_SPEED:
        case DamageType::STAR_ARMOR:
        default:
            this->baseMpCost = DEFAULT_MP_COST_BUFF; break;
        }
    }
}

SkillType Skill::getType() const { return type; }
std::string Skill::getName() const { return name; }
std::string Skill::getDescription() const { return description; }
int Skill::getUnlockLevel() const { return unlockLevel; }
SkillTarget Skill::getTarget() const { return target; }
DamageType Skill::getDamageType() const { return damageType; }
int Skill::getPower() const { return basePower; }

// 新增：按施放者等级计算成长后的实际威力
int Skill::getScaledPower(int casterLevel) const {
    int deltaLevel = std::max(0, casterLevel - unlockLevel); // 解锁等级起算
    // 线性成长：base * (1 + growth * delta)
    float scaled = static_cast<float>(basePower) * (1.0f + growthPerLevel * deltaLevel);
    int result = static_cast<int>(scaled + 0.5f);
    return std::max(1, result);
}

// 新增：按施放者等级计算MP消耗（解锁等级起算线性成长）
int Skill::getMpCost(int casterLevel) const {
    int deltaLevel = std::max(0, casterLevel - unlockLevel);
    float scaled = static_cast<float>(baseMpCost) * (1.0f + mpCostGrowthPerLevel * deltaLevel);
    int result = static_cast<int>(scaled + 0.5f);
    return std::max(1, result);
}

// 新增：获取技能详细描述
std::string Skill::getDetailedDescription(int casterLevel) const {
    std::string detail = "\033[36m=== " + name + " ===\033[0m\n";
    detail += "\033[37m类型：\033[0m";
    
    switch (damageType) {
        case DamageType::PHYSICAL:
            detail += "\033[31m物理攻击技能\033[0m\n";
            break;
        case DamageType::MAGICAL:
            detail += "\033[34m魔法攻击技能\033[0m\n";
            break;
        case DamageType::BUFF:
            detail += "\033[32m治疗恢复技能\033[0m\n";
            break;
        case DamageType::HOLY_MARK_SPEED:
            detail += "\033[33m速度增益技能\033[0m\n";
            break;
        case DamageType::STAR_ARMOR:
            detail += "\033[35m防御增益技能\033[0m\n";
            break;
    }
    
    detail += "\033[37m解锁等级：\033[0m" + std::to_string(unlockLevel) + "级\n";
    detail += "\033[37mMP消耗：\033[0m" + std::to_string(getMpCost(casterLevel)) + "\n";
    detail += "\033[37m当前威力：\033[0m" + std::to_string(getScaledPower(casterLevel)) + "\n";
    
    // 技能特殊说明
    switch (type) {
        case SkillType::HOLY_RIFT_SLASH:
            detail += "\033[37m效果：\033[0m对敌人造成物理伤害\n";
            detail += "\033[37m特点：\033[0m基础攻击技能，威力稳定";
            break;
        case SkillType::GOLDEN_TREE_VOW:
            detail += "\033[37m效果：\033[0m恢复生命值和蓝量，获得2回合物理免疫\n";
            detail += "\033[37m特点：\033[0m综合性防护技能，可抵御物理攻击";
            break;
        case SkillType::HOLY_PRISON_JUDGMENT:
            detail += "\033[37m效果：\033[0m对敌人造成魔法伤害\n";
            detail += "\033[37m特点：\033[0m魔法攻击，威力较高但消耗更多MP";
            break;
        case SkillType::STAR_ARMOR:
            detail += "\033[37m效果：\033[0m提升防御力，持续2回合\n";
            detail += "\033[37m特点：\033[0m临时增益，增强防御能力";
            break;
        case SkillType::HOLY_MARK_SPEED:
            detail += "\033[37m效果：\033[0m提升速度，持续2回合\n";
            detail += "\033[37m特点：\033[0m临时增益，提升行动速度和先手能力";
            break;
        case SkillType::ULTIMATE_SLAY:
            detail += "\033[37m效果：\033[0m终极魔法攻击，毁灭性伤害\n";
            detail += "\033[37m特点：\033[0m需集齐六誓圣辉套装才能解锁，威力极其强大";
            break;
    }
    
    return detail;
}