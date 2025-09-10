#ifndef PLAYER_H
#define PLAYER_H
#include "Attribute.h"
#include "Equipment.h"
#include "DivineWeapon.h"
#include "Skills.h"
#include "TaskSystem.h"
#include "DivineSet.h"
#include <vector>
#include <map> 
#include <iostream>

class Player : public Attribute {
private:
    DivineWeapon* divineSword;                  // 六圣裁恶神剑（唯一神器）
    DivineSet divineSet;                        // 六誓圣辉救赎套装
    std::vector<Skill*> skills;                 // 已解锁技能
    int currentRoomId;                          // 当前所在房间ID
public:
    Player(std::string name = "安特王子");
    ~Player();
    int extraActionTurns;
    
    // 位置管理
    int getCurrentRoomId() const { return currentRoomId; }
    void setCurrentRoomId(int roomId) { currentRoomId = roomId; }
    // 神器与装备管理
    void equipSetPart(Equipment* part);         // 装备套装部件
    DivineWeapon* getDivineSword() const;
    bool hasAllSetParts() const;                // 检查是否集齐套装
    std::map<EquipmentPart, Equipment*> getAllEquippedItems() const;

    // 重写升级方法，包含技能解锁逻辑
    bool levelUp() override;

    // 技能管理
    void unlockSkill(SkillType type);           // 解锁技能
    void checkAndUnlockSkills();                // 检查并解锁符合等级要求的技能
    std::vector<Skill*> getSkills() const;
    Skill* getSkill(SkillType type) const;

    // 任务管理
    std::map<std::string, Task> taskProgress;   // 任务进度（任务ID->是否完成）
    void updateTaskProgress(std::string taskID, TaskStatus status);
    bool isTaskCompleted(std::string taskID) const;

    //物品管理
    std::map<std::string, int> inventory;
    void addItem(const Item& item, int quantity = 1);
    void addItemByName(const std::string& itemName, int quantity = 1);
    bool useItem(const std::string& itemName);
    const std::map<std::string, int>& getInventory() const;
    void clearInventory(); // 添加清空背包方法

    // 蓝条管理
    int getMP() const { return Attribute::getMP(); }
    int getMaxMP() const { return Attribute::getMaxMP(); }
    void setMP(int value) { Attribute::setMP(value); }
    void setMaxMP(int value) { Attribute::setMaxMP(value); }

    // 主动穿戴装备（通过装备名）
    bool equipFromInventory(const std::string& equipmentName);
    void addExp(int value);
};

#endif // PLAYER_H