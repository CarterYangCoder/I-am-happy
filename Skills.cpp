#include "Skills.h"

Skill::Skill(SkillType type, std::string name, std::string desc, int unlockLv,
    SkillTarget target, DamageType dmgType, int power)
    : type(type), name(name), description(desc), unlockLevel(unlockLv),
      target(target), damageType(dmgType), power(power) {}
 
SkillType Skill::getType() const { return type; }
std::string Skill::getName() const { return name; }
std::string Skill::getDescription() const { return description; }
int Skill::getUnlockLevel() const { return unlockLevel; }
SkillTarget Skill::getTarget() const { return target; }
DamageType Skill::getDamageType() const { return damageType; }
int Skill::getPower() const { return power; }