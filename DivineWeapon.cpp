/**
 * @file DivineWeapon.cpp
 * @brief 神剑实现：成长与技能解锁。
 */
#include "DivineWeapon.h"
#include <iostream>
#include "Skills.h"

/** @brief 构造初始神剑。 */
DivineWeapon::DivineWeapon() 
    : Equipment("六圣裁恶神剑", EquipmentPart::SWORD, "传说中的神器，随使用者成长", 20, 5, "神圣力量"),
      growthLevel(1) {}

DivineWeapon* DivineWeapon::clone() const {
    return new DivineWeapon(*this);
}

/** @brief 神剑成长：按玩家等级线性提升攻击力并提示。 */
void DivineWeapon::grow(int playerLevel) {
    if (playerLevel > growthLevel) {
        int oldAtk = getAtkBonus();
        int newAtk = 20 + (playerLevel - 1) * 8; // 基础20攻击力，每级增加8点
        
        // 更新攻击力加成
        setAtkBonus(newAtk);
        
        growthLevel = playerLevel;
        std::cout << "\033[36m⚔️ 升级后，铁匠杨思睿为你的神剑附魔，神剑威力更强了！\033[0m" << std::endl;
        std::cout << "\033[31m🗡️ 攻击力从 " << oldAtk << " 提升到 " << newAtk << "\033[0m" << std::endl;
    }
}

/**
 * @brief 尝试按玩家等级解锁终极技能。
 * @return true 已解锁并设置 skillType
 */
bool DivineWeapon::unlockSkill(int playerLevel, SkillType& skillType) {
    if (playerLevel >= 50) {
        skillType = SkillType::ULTIMATE_SLAY;
        return true;
    }
    return false;
}