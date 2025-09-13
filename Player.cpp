/**
 * @file Player.cpp
 * @brief 玩家实体实现：装备/技能/任务/物品/增益与升级逻辑。
 */

#include "Player.h"
#include <sstream>
#include <iostream>

/** @brief 构造玩家并初始化状态与初始技能。 */
Player::Player(std::string name) : Attribute(name) {
    // 初始化神剑（开场自带）
    divineSword = new DivineWeapon();
    this->extraActionTurns = 0;
    this->currentRoomId = 1; // 初始位置：迷雾森林 
    this->physicalShieldTurns = 0; // 新增：初始化物理护盾状态
    
    // 新增：初始化临时增益状态
    this->speedBoostTurns = 0;
    this->speedBoostAmount = 0;
    this->defenseBoostTurns = 0;
    this->defenseBoostAmount = 0;
    
    // 检查并解锁符合初始等级的技能
    checkAndUnlockSkills();
}

Player::~Player() {
    /** @brief 释放神剑与技能资源。 */
    if (divineSword != nullptr) {
        delete divineSword;
        divineSword = nullptr;
    }
    for (auto skill : skills) {
        delete skill;
    }
    skills.clear();
}

/** @brief 装备套装部件并在集齐后触发共鸣与技能解锁。 */
void Player::equipSetPart(Equipment* part) {
    if (!part) return;
    divineSet.addPart(part);
    setATK(getATK() + part->getAtkBonus());
    setDEF(getDEF() + part->getDefBonus());
    // 检查是否集齐套装，如果是则尝试解锁终极技能
    if (hasAllSetParts()) {
        // 神明对话（分段显示）
        std::cout << "神明之声在你心中回响..." << std::endl;
        std::cout << "按 Enter 键继续..." << std::endl;
        std::string dummy;
        std::getline(std::cin, dummy);
        
        std::cout << "神明：\"恭喜你，安特国的王子。六誓圣辉套装终于重聚。\"" << std::endl;
        std::cout << "按 Enter 键继续..." << std::endl;
        std::getline(std::cin, dummy);
        
        std::cout << "神明：\"皇室血脉正在觉醒，神圣力量流淌在你的血管中。\"" << std::endl;
        std::cout << "按 Enter 键继续..." << std::endl;
        std::getline(std::cin, dummy);
        
        std::cout << "神明：\"现在，去吧！用这份力量击败强敌，拯救你的王国。\"" << std::endl;
        std::cout << "按 Enter 键继续..." << std::endl;
        std::getline(std::cin, dummy);
        
        std::cout << "神明：\"愿光明指引你的道路，愿正义伴随你的剑刃。\"" << std::endl;
        std::cout << "按 Enter 键继续..." << std::endl;
        std::getline(std::cin, dummy);
        
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

/** @brief 获取神剑指针。 */
DivineWeapon* Player::getDivineSword() const { return divineSword; }

/** @brief 是否集齐套装。 */
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

/** @brief 解锁指定技能（去重）。 */
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
        std::cout << "\033[35m✨ 解锁了新技能: \033[33m" << newSkill->getName() << "\033[35m ✨\033[0m" << std::endl;
        break;
    case SkillType::GOLDEN_TREE_VOW:
        newSkill = new Skill(type, "黄金树之誓", "加血增益", 5, SkillTarget::SELF, DamageType::BUFF, 10);
        std::cout << "\033[35m✨ 解锁了新技能: \033[33m" << newSkill->getName() << "\033[35m ✨\033[0m" << std::endl;
        break;
    case SkillType::HOLY_PRISON_JUDGMENT:
        newSkill = new Skill(type, "圣狱裁决", "魔法伤害技能", 10, SkillTarget::ENEMY, DamageType::MAGICAL, 30);
        std::cout << "\033[35m✨ 解锁了新技能: \033[33m" << newSkill->getName() << "\033[35m ✨\033[0m" << std::endl;
        break;
    case SkillType::STAR_ARMOR:
        newSkill = new Skill(type, "星辰圣铠", "提升防御", 15, SkillTarget::SELF, DamageType::STAR_ARMOR, 15);
        std::cout << "\033[35m✨ 解锁了新技能: \033[33m" << newSkill->getName() << "\033[35m ✨\033[0m" << std::endl;
        break;
    case SkillType::HOLY_MARK_SPEED:
        newSkill = new Skill(type, "圣痕疾影步", "提升速度", 20, SkillTarget::SELF, DamageType::HOLY_MARK_SPEED, 20);
        std::cout << "\033[35m✨ 解锁了新技能: \033[33m" << newSkill->getName() << "\033[35m ✨\033[0m" << std::endl;
        break;
    case SkillType::ULTIMATE_SLAY:
        if (hasAllSetParts()) { // 仅在集齐神器时解锁
            newSkill = new Skill(type, "星闪流河圣龙飞升·神界湮灭斩·最终式", "终极技能，毁灭一切", 50, SkillTarget::ENEMY, DamageType::MAGICAL, 100);
            std::cout << "\033[35m🌟 集齐六誓圣辉套装！解锁了终极技能: \033[33m" << newSkill->getName() << "\033[35m 🌟\033[0m" << std::endl;
        }
        break;
    default: 
        break;
    }
    
    if (newSkill) {
        skills.push_back(newSkill);
    }
}

/** @brief 覆写升级：属性成长、技能解锁、神剑成长与提示。 */
bool Player::levelUp() {
    // 只处理属性和技能，不再递归addExp
    bool leveledUp = Attribute::levelUp();
    if (leveledUp) {
        checkAndUnlockSkills();
        if (divineSword) divineSword->grow(getLevel());
        
        // 优化技能列表显示
        std::cout << "\033[34m📜 当前已解锁技能：\033[0m";
        for (auto skill : skills) {
            std::cout << "\033[33m[" << skill->getName() << "]\033[0m ";
        }
        std::cout << std::endl;
        std::cout << std::endl; // 添加空行分隔
    }
    return leveledUp;
}

/** @brief 按等级/套装检查并解锁技能。 */
void Player::checkAndUnlockSkills() {
    int currentLevel = getLevel();
    
    // 根据等级解锁技能
    if (currentLevel >= 2 && !getSkill(SkillType::HOLY_RIFT_SLASH)) {
        unlockSkill(SkillType::HOLY_RIFT_SLASH);
    }
    if (currentLevel >= 3 && !getSkill(SkillType::GOLDEN_TREE_VOW)) {
        unlockSkill(SkillType::GOLDEN_TREE_VOW);
    }
    if (currentLevel >= 5 && !getSkill(SkillType::HOLY_PRISON_JUDGMENT)) {
        unlockSkill(SkillType::HOLY_PRISON_JUDGMENT);
    }
    if (currentLevel >= 8 && !getSkill(SkillType::STAR_ARMOR)) {
        unlockSkill(SkillType::STAR_ARMOR);
    }
    if (currentLevel >= 10 && !getSkill(SkillType::HOLY_MARK_SPEED)) {
        unlockSkill(SkillType::HOLY_MARK_SPEED);
    }
    if (currentLevel >= 15 && hasAllSetParts() && !getSkill(SkillType::ULTIMATE_SLAY)) {
        unlockSkill(SkillType::ULTIMATE_SLAY);
    }
}

/** @brief 获取技能列表副本。 */
std::vector<Skill*> Player::getSkills() const { return skills; }

/** @brief 查找指定类型技能。 */
Skill* Player::getSkill(SkillType type) const {
    for (auto skill : skills) {
        if (skill->getType() == type) return skill;
    }
    return nullptr;
}

/** @brief 更新任务进度（复制体状态同步）。 */
void Player::updateTaskProgress(std::string taskID, TaskStatus status) {
    auto it = taskProgress.find(taskID);
    if (it != taskProgress.end()) {
        it->second.setStatus(status); // 调用 Task 的 setStatus 方法更新状态
    }
}

/** @brief 判断任务是否完成。 */
bool Player::isTaskCompleted(std::string taskID) const {
    auto it = taskProgress.find(taskID);
    if (it != taskProgress.end()) {
        // 调用 Task 的 getStatus 方法，判断是否为“已完成”状态
        return it->second.getStatus() == TaskStatus::COMPLETED;
    }
    return false; // 任务不存在，视为“未完成”
}

/** @brief 向背包添加物品（按名称聚合）。 */
void Player::addItem(const Item& item, int quantity) {
    inventory[item.getName()] += quantity;
}

/** @brief 按名称添加物品（用于简化拾取/读档）。 */
void Player::addItemByName(const std::string& itemName, int quantity) {
    inventory[itemName] += quantity;
}

/** @brief 使用物品（计数—1，归零即移除）。 */
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

/** @brief 获取背包只读引用。 */
const std::map<std::string, int>& Player::getInventory() const {
    return inventory;
}

/** @brief 清空背包。 */
void Player::clearInventory() {
    inventory.clear();
}

/**
 * @brief 从背包装备指定装备并触发任务限制校验与单件移除。
 * @return true 成功穿戴；false 失败或不满足条件。
 */
bool Player::equipFromInventory(const std::string& equipmentName) {
    // 检查背包是否有该装备
    if (inventory.count(equipmentName) && inventory[equipmentName] > 0) {
        // 新增：检查任务完成状态，只有任务已提交完成才能穿戴
        bool canEquip = false;
        std::string taskRequirement = "";
        
        if (equipmentName == "自由誓约・破枷之冠") {
            taskRequirement = "1";
        } else if (equipmentName == "忠诚誓约・铁誓胸甲" || equipmentName == "铁誓胸甲") {
            taskRequirement = "2";
        } else if (equipmentName == "真理誓约・明识之戒" || equipmentName == "明识之戒") {
            taskRequirement = "3";
        } else if (equipmentName == "怜悯誓约・抚伤之链" || equipmentName == "怜悯之链") {
            taskRequirement = "4";
        } else if (equipmentName == "希望誓约・晨曦披风" || equipmentName == "晨曦披风") {
            taskRequirement = "5";
        } else if (equipmentName == "秩序誓约・创世战靴" || equipmentName == "创世战靴") {
            taskRequirement = "6";
        }
        
        // 检查任务状态
        if (!taskRequirement.empty()) {
            if (taskProgress.count(taskRequirement) && 
                taskProgress[taskRequirement].getStatus() == TaskStatus::REWARDED) {
                canEquip = true;
            } else {
                std::cout << "你还不可以穿戴这件装备！ (｡•́︿•̀｡)" << std::endl;
                std::cout << "需要先完成并提交任务" << taskRequirement << "才能获得穿戴权限。" << std::endl;
                std::cout << "神明：\"耐心点，年轻人，力量需要通过试炼才能获得！ (￣▽￣)ノ\"" << std::endl;
                return false;
            }
        } else {
            canEquip = true; // 非任务装备可以直接穿戴
        }
        
        if (canEquip) {
            // 判断是否为装备类型
            Equipment* equip = nullptr;
            // 这里只能通过装备名查找，实际项目可用装备数据库
            if (equipmentName == "自由誓约・破枷之冠")
                equip = new Equipment("自由誓约・破枷之冠", EquipmentPart::HELMET, "破除精神枷锁", 10, 5, "抵抗精神控制");
            else if (equipmentName == "忠诚誓约・铁誓胸甲" || equipmentName == "铁誓胸甲")
                equip = new Equipment("忠诚誓约・铁誓胸甲", EquipmentPart::CHESTPLATE, "忠诚加护", 15, 10, "提升防御");
            else if (equipmentName == "希望誓约・晨曦披风" || equipmentName == "晨曦披风")
                equip = new Equipment("希望誓约・晨曦披风", EquipmentPart::CAPE, "希望加持", 8, 8, "提升速度");
            else if (equipmentName == "怜悯誓约・抚伤之链")
                equip = new Equipment("怜悯誓约・抚伤之链", EquipmentPart::NECKLACE, "怜悯治愈", 5, 5, "提升回血");
            else if (equipmentName == "真理誓约・明识之戒")
                equip = new Equipment("真理誓约・明识之戒", EquipmentPart::RING, "明识真理", 5, 5, "提升暴击");
            else if (equipmentName == "秩序誓约・创世战靴")
                equip = new Equipment("秩序誓约・创世战靴", EquipmentPart::BOOTS, "秩序加速", 8, 8, "提升速度");
            
            if (equip) {
                equipSetPart(equip);
                // 重要：从背包中完全移除装备（作为消耗品）
                inventory[equipmentName]--;
                if (inventory[equipmentName] == 0) {
                    inventory.erase(equipmentName);
                }
                std::cout << "成功穿戴装备：" << equipmentName << std::endl;
                std::cout << "神明：\"装备已与你融为一体，无法取下，也无法重复获得。\"" << std::endl;
                return true;
            }
        }
    }
    std::cout << "背包中没有该装备或不是装备类型。" << std::endl;
    return false;
}

/**
 * @brief 增加经验并处理升级循环，满级友好提示。
 */
void Player::addExp(int value) {
    if (isMaxLevel()) {
        // 满级后不再获得经验，友好提示
        std::cout << "\033[36m你已达到最高等级，无需再获得经验。\033[0m" << std::endl;
        std::cout << "\033[37m神明：\"你的力量已经完美无缺了！\"\033[0m" << std::endl;
        return;
    }
    
    exp += value;
    // 升级循环，直到经验不足或达到满级
    while (exp >= expToNextLevel && !isMaxLevel()) {
        levelUp(); // 调用自身的升级方法，保证属性和技能同步
    }
}

/** @brief 售卖物品并结算金币。 */
bool Player::sellItem(const std::string& itemName, int quantity) {
    if (!inventory.count(itemName) || inventory[itemName] < quantity) {
        return false; // 物品不足
    }
    
    if (!canSellItem(itemName)) {
        return false; // 不可售卖
    }
    
    int sellPrice = getItemSellPrice(itemName) * quantity;
    inventory[itemName] -= quantity;
    if (inventory[itemName] == 0) {
        inventory.erase(itemName);
    }
    addGold(sellPrice);
    return true;
}

/** @brief 丢弃物品到当前区域。 */
bool Player::dropItem(const std::string& itemName, int quantity) {
    if (!inventory.count(itemName) || inventory[itemName] < quantity) {
        return false; // 物品不足
    }
    
    inventory[itemName] -= quantity;
    if (inventory[itemName] == 0) {
        inventory.erase(itemName);
    }
    return true;
}

/** @brief 物品是否允许售卖。 */
bool Player::canSellItem(const std::string& itemName) const {
    // 商店购买的物品可以售卖
    if (itemName == "生命药水" || itemName == "能量药水" || itemName == "神秘商品") {
        return true;
    }
    
    // 任务物品和装备不可售卖
    return false;
}

/** @brief 计算可售物品的单价。 */
int Player::getItemSellPrice(const std::string& itemName) const {
    // 商店物品按原价50%出售
    if (itemName == "生命药水") return 25;  // 原价50的50%
    if (itemName == "能量药水") return 50;  // 原价100的50%
    if (itemName == "神秘商品") return 5;   // 原价10的50%
    
    return 0; // 不可售卖物品
}

/** @brief 增益：速度提升（覆盖旧增益）。 */
void Player::addSpeedBoost(int amount, int turns) {
    if (speedBoostTurns > 0) {
        // 如果已有速度增益，先移除旧的效果
        setSpeed(getSpeed() - speedBoostAmount);
    }
    speedBoostAmount = amount;
    speedBoostTurns = turns;
    setSpeed(getSpeed() + amount);
}

/** @brief 增益：防御提升（覆盖旧增益）。 */
void Player::addDefenseBoost(int amount, int turns) {
    if (defenseBoostTurns > 0) {
        // 如果已有防御增益，先移除旧的效果
        setDEF(getDEF() - defenseBoostAmount);
    }
    defenseBoostAmount = amount;
    defenseBoostTurns = turns;
    setDEF(getDEF() + amount);
}

/** @brief 回合结束：衰减增益并在结束时撤回属性。 */
void Player::updateBuffTurns() {
    // 更新速度增益
    if (speedBoostTurns > 0) {
        speedBoostTurns--;
        if (speedBoostTurns == 0) {
            setSpeed(getSpeed() - speedBoostAmount);
            speedBoostAmount = 0;
            std::cout << "\033[33m圣痕疾影步效果消失，速度恢复正常。\033[0m" << std::endl;
        }
    }
    
    // 更新防御增益
    if (defenseBoostTurns > 0) {
        defenseBoostTurns--;
        if (defenseBoostTurns == 0) {
            setDEF(getDEF() - defenseBoostAmount);
            defenseBoostAmount = 0;
            std::cout << "\033[33m星辰圣铠效果消失，防御恢复正常。\033[0m" << std::endl;
        }
    }
}

/** @brief 获取当前增益状态文本。 */
std::string Player::getBuffStatus() const {
    std::string status = "";
    if (physicalShieldTurns > 0) {
        status += "【圣光庇护:" + std::to_string(physicalShieldTurns) + "回合】 ";
    }
    if (speedBoostTurns > 0) {
        status += "【疾影步:+" + std::to_string(speedBoostAmount) + "速度," + std::to_string(speedBoostTurns) + "回合】 ";
    }
    if (defenseBoostTurns > 0) {
        status += "【圣铠:+" + std::to_string(defenseBoostAmount) + "防御," + std::to_string(defenseBoostTurns) + "回合】 ";
    }
    return status.empty() ? "无" : status;
}