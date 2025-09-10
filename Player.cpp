#include "Player.h"
#include <sstream>
#include <iostream>

Player::Player(std::string name) : Attribute(name) {
    // 初始化神剑（开场自带）
    divineSword = new DivineWeapon();
    this->extraActionTurns = 0;
    this->currentRoomId = 1; // 初始位置：迷雾森林 
    
    // 检查并解锁符合初始等级的技能
    checkAndUnlockSkills();
}

Player::~Player() {

    if (divineSword != nullptr) {
        delete divineSword;
        divineSword = nullptr;
    }
    for (auto skill : skills) {
        delete skill;
    }
    skills.clear();
}

// 装备套装部件
void Player::equipSetPart(Equipment* part) {
    if (!part) return;
    divineSet.addPart(part);
    setATK(getATK() + part->getAtkBonus());
    setDEF(getDEF() + part->getDefBonus());
    // 检查是否集齐套装，如果是则尝试解锁终极技能
    if (hasAllSetParts()) {
        // 套装共鸣效果：回满状态并提升属性
        setHP(getMaxHP());
        setMP(getMaxMP());
        setATK(static_cast<int>(getATK() * 1.2));
        setDEF(static_cast<int>(getDEF() * 1.2));
        setSpeed(static_cast<int>(getSpeed() * 1.2));
        setMaxHP(static_cast<int>(getMaxHP() * 1.2));
        setMaxMP(static_cast<int>(getMaxMP() * 1.2));
        setHP(getMaxHP());
        setMP(getMaxMP());
        std::cout << "套装共鸣！全属性提升20%，状态已回满。" << std::endl;
        checkAndUnlockSkills();
    }
}

// 获取神剑
DivineWeapon* Player::getDivineSword() const { return divineSword; }

// 检查是否集齐套装（6个部件）
bool Player::hasAllSetParts() const {
    return divineSet.isComplete();
}

std::map<EquipmentPart, Equipment*> Player::getAllEquippedItems() const {
    std::map<EquipmentPart, Equipment*> allItems;

    // 获取DivineSet中的装备
    for (int i = static_cast<int>(EquipmentPart::HELMET); i <= static_cast<int>(EquipmentPart::BOOTS); ++i) {
        EquipmentPart part = static_cast<EquipmentPart>(i);
        Equipment* equipment = divineSet.getEquipment(part);
        if (equipment) {
            allItems[part] = equipment;
        }
    }

    if (divineSword != nullptr) {
        allItems[divineSword->getPart()] = divineSword;
    }

    return allItems;
}

// 重写升级方法，包含技能解锁逻辑
bool Player::levelUp() {
    // 只处理属性和技能，不再递归addExp
    bool leveledUp = Attribute::levelUp();
    if (leveledUp) {
        checkAndUnlockSkills();
        if (divineSword) divineSword->grow(getLevel());
        std::cout << "恭喜！等级提升到 " << getLevel() << " 级！" << std::endl;
        std::cout << "属性得到了提升，生命值已回复到满值。" << std::endl;
        std::cout << "已解锁技能：";
        for (auto skill : skills) {
            std::cout << "[" << skill->getName() << "] ";
        }
        std::cout << std::endl;
    }
    return leveledUp;
}

// 解锁技能
void Player::unlockSkill(SkillType type) {
    // 检查是否已经拥有该技能
    for (auto skill : skills) {
        if (skill->getType() == type) {
            return; // 已有该技能，不重复添加
        }
    }
    
    Skill* newSkill = nullptr;
    
    // 根据技能类型创建技能
    switch (type) {
    case SkillType::HOLY_RIFT_SLASH:
        newSkill = new Skill(type, "圣界裂隙斩", "物理伤害技能", 1, SkillTarget::ENEMY, DamageType::PHYSICAL, 20);
        std::cout << "解锁了新技能: " << newSkill->getName() << std::endl;
        break;
    case SkillType::GOLDEN_TREE_VOW:
        newSkill = new Skill(type, "黄金树之誓", "加血增益", 5, SkillTarget::SELF, DamageType::BUFF, 10);
        std::cout << "解锁了新技能: " << newSkill->getName() << std::endl;
        break;
    case SkillType::HOLY_PRISON_JUDGMENT:
        newSkill = new Skill(type, "圣狱裁决", "魔法伤害技能", 10, SkillTarget::ENEMY, DamageType::MAGICAL, 30);
        std::cout << "解锁了新技能: " << newSkill->getName() << std::endl;
        break;
    case SkillType::STAR_ARMOR:
        newSkill = new Skill(type, "星辰圣铠", "提升防御", 15, SkillTarget::SELF, DamageType::STAR_ARMOR, 15);
        std::cout << "解锁了新技能: " << newSkill->getName() << std::endl;
        break;
    case SkillType::HOLY_MARK_SPEED:
        newSkill = new Skill(type, "圣痕疾影步", "提升速度", 20, SkillTarget::SELF, DamageType::HOLY_MARK_SPEED, 20);
        std::cout << "解锁了新技能: " << newSkill->getName() << std::endl;
        break;
    case SkillType::ULTIMATE_SLAY:
        if (hasAllSetParts()) { // 仅在集齐神器时解锁
            newSkill = new Skill(type, "星闪流河圣龙飞升·神界湮灭斩·最终式", "终极技能，毁灭一切", 50, SkillTarget::ENEMY, DamageType::MAGICAL, 100);
            std::cout << "集齐六誓圣辉套装！解锁了终极技能: " << newSkill->getName() << std::endl;
        }
        break;
    default: 
        break;
    }
    
    if (newSkill) {
        skills.push_back(newSkill);
    }
}

// 检查并解锁符合等级要求的技能
void Player::checkAndUnlockSkills() {
    int currentLevel = getLevel();
    
    // 根据等级解锁技能
    if (currentLevel >= 1 && !getSkill(SkillType::HOLY_RIFT_SLASH)) {
        unlockSkill(SkillType::HOLY_RIFT_SLASH);
    }
    if (currentLevel >= 5 && !getSkill(SkillType::GOLDEN_TREE_VOW)) {
        unlockSkill(SkillType::GOLDEN_TREE_VOW);
    }
    if (currentLevel >= 10 && !getSkill(SkillType::HOLY_PRISON_JUDGMENT)) {
        unlockSkill(SkillType::HOLY_PRISON_JUDGMENT);
    }
    if (currentLevel >= 15 && !getSkill(SkillType::STAR_ARMOR)) {
        unlockSkill(SkillType::STAR_ARMOR);
    }
    if (currentLevel >= 20 && !getSkill(SkillType::HOLY_MARK_SPEED)) {
        unlockSkill(SkillType::HOLY_MARK_SPEED);
    }
    if (currentLevel >= 50 && hasAllSetParts() && !getSkill(SkillType::ULTIMATE_SLAY)) {
        unlockSkill(SkillType::ULTIMATE_SLAY);
    }
}

// 获取技能列表
std::vector<Skill*> Player::getSkills() const { return skills; }

// 获取指定技能
Skill* Player::getSkill(SkillType type) const {
    for (auto skill : skills) {
        if (skill->getType() == type) return skill;
    }
    return nullptr;
}

// 更新任务进度
void Player::updateTaskProgress(std::string taskID, TaskStatus status) {
    auto it = taskProgress.find(taskID);
    if (it != taskProgress.end()) {
        it->second.setStatus(status); // 调用 Task 的 setStatus 方法更新状态
    }
}

// 检查任务是否完成
bool Player::isTaskCompleted(std::string taskID) const {
    auto it = taskProgress.find(taskID);
    if (it != taskProgress.end()) {
        // 调用 Task 的 getStatus 方法，判断是否为“已完成”状态
        return it->second.getStatus() == TaskStatus::COMPLETED;
    }
    return false; // 任务不存在，视为“未完成”
}

void Player::addItem(const Item& item, int quantity) {
    inventory[item.getName()] += quantity;
}

void Player::addItemByName(const std::string& itemName, int quantity) {
    inventory[itemName] += quantity;
}

bool Player::useItem(const std::string& itemName) {
    if (inventory.count(itemName) && inventory[itemName] > 0) {
        inventory[itemName]--;
        if (inventory[itemName] == 0) {
            inventory.erase(itemName);
        }
        return true;
    }
    return false;
}

const std::map<std::string, int>& Player::getInventory() const {
    return inventory;
}

void Player::clearInventory() {
    inventory.clear();
}

bool Player::equipFromInventory(const std::string& equipmentName) {
    // 检查背包是否有该装备
    if (inventory.count(equipmentName) && inventory[equipmentName] > 0) {
        // 判断是否为装备类型
        Equipment* equip = nullptr;
        // 这里只能通过装备名查找，实际项目可用装备数据库
        if (equipmentName == "自由誓约・破枷之冠")
            equip = new Equipment("自由誓约・破枷之冠", EquipmentPart::HELMET, "破除精神枷锁", 10, 5, "抵抗精神控制");
        else if (equipmentName == "忠诚誓约・铁誓胸甲")
            equip = new Equipment("忠诚誓约・铁誓胸甲", EquipmentPart::CHESTPLATE, "忠诚加护", 15, 10, "提升防御");
        else if (equipmentName == "希望誓约・晨曦披风")
            equip = new Equipment("希望誓约・晨曦披风", EquipmentPart::CAPE, "希望加持", 8, 8, "提升速度");
        else if (equipmentName == "怜悯誓约・抚伤之链")
            equip = new Equipment("怜悯誓约・抚伤之链", EquipmentPart::NECKLACE, "怜悯治愈", 5, 5, "提升回血");
        else if (equipmentName == "真理誓约・明识之戒")
            equip = new Equipment("真理誓约・明识之戒", EquipmentPart::RING, "明识真理", 5, 5, "提升暴击");
        else if (equipmentName == "秩序誓约・创世战靴")
            equip = new Equipment("秩序誓约・创世战靴", EquipmentPart::BOOTS, "秩序加速", 8, 8, "提升速度");
        if (equip) {
            equipSetPart(equip);
            inventory[equipmentName]--;
            if (inventory[equipmentName] == 0) inventory.erase(equipmentName);
            std::cout << "成功穿戴装备：" << equipmentName << std::endl;
            return true;
        }
    }
    std::cout << "背包中没有该装备或不是装备类型。" << std::endl;
    return false;
}

void Player::addExp(int value) {
    exp += value;
    // 升级循环，直到经验不足
    while (exp >= expToNextLevel) {
        levelUp(); // 调用自身的升级方法，保证属性和技能同步
    }
}