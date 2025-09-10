#include "DivineWeapon.h"
#include <iostream>
#include "Skills.h"

// DivineWeapon构造函数实现（根据头文件中的声明）
// 如果头文件中有特定的构造函数声明，请确保这里的实现匹配

// 默认构造函数实现（如果头文件中有声明） 
// DivineWeapon::DivineWeapon() : Item("神器", 1000, "传说中的神器", 0) {
// }

// 带参数的构造函数实现（如果头文件中有声明）
// DivineWeapon::DivineWeapon(std::string name, int value, std::string desc, int id) 
//     : Item(name, value, desc, id) {
// }

DivineWeapon::DivineWeapon() 
    : Equipment("六圣裁恶神剑", EquipmentPart::SWORD, "传说中的神器，随使用者成长", 20, 5, "神圣力量"),
      growthLevel(1) {}

DivineWeapon* DivineWeapon::clone() const {
    return new DivineWeapon(*this);
}

// 神剑成长（随玩家等级提升攻击力）
void DivineWeapon::grow(int playerLevel) {
    if (playerLevel > growthLevel) {
        int oldAtk = getAtkBonus();
        int newAtk = 20 + (playerLevel - 1) * 5; // 基础20攻击力，每级增加5点
        
        // 更新攻击力加成
        setAtkBonus(newAtk);
        
        growthLevel = playerLevel;
        std::cout << "六圣裁恶神剑随着你的成长而变得更加强大！攻击力从 " 
                  << oldAtk << " 提升到 " << newAtk << std::endl;
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