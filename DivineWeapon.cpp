#include "DivineWeapon.h"
#include <iostream>
#include "Skills.h"

DivineWeapon::DivineWeapon() 
    : Equipment("六圣裁恶神剑", EquipmentPart::SWORD, "传说中的神器，随使用者成长", 50, 10, "神圣力量"),
      growthLevel(1) {}

DivineWeapon* DivineWeapon::clone() const {
    return new DivineWeapon(*this);
}

// 神剑成长（随玩家等级提升攻击力）
void DivineWeapon::grow(int playerLevel) {
    if (playerLevel > growthLevel) {
        int oldAtk = getAtkBonus();
        int newAtk = 50 + (playerLevel - 1) * 10; // 基础50攻击力，每级增加10点
        
        // 更新攻击力加成
        setAtkBonus(newAtk);
        
        growthLevel = playerLevel;
        std::cout << "\033[36m⚔️ 升级后，铁匠杨思睿为你的神剑附魔，神剑威力更强了！\033[0m" << std::endl;
        std::cout << "\033[31m🗡️ 攻击力从 " << oldAtk << " 提升到 " << newAtk << "\033[0m" << std::endl;
    }
}

// 解锁技能（根据成长等级解锁对应技能）
bool DivineWeapon::unlockSkill(int playerLevel, SkillType& skillType) {
    if (playerLevel >= 50) {
        skillType = SkillType::ULTIMATE_SLAY;
        return true;
    }
    return false;
}