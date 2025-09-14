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
#include <set>  // 新增

class Player : public Attribute {
private:
    DivineWeapon* divineSword;                  // 六圣裁恶神剑（唯一神器）
    DivineSet divineSet;                        // 六誓圣辉救赎套装
    std::vector<Skill*> skills;                 // 已解锁技能
    int currentRoomId;                          // 当前所在房间ID
    int physicalShieldTurns;                    // 新增：物理伤害护盾剩余回合数
    
    // 新增：临时增益状态管理
    int speedBoostTurns;                        // 速度增益剩余回合数
    int speedBoostAmount;                       // 速度增益数值
    int defenseBoostTurns;                      // 防御增益剩余回合数
    int defenseBoostAmount;                     // 防御增益数值

    // 已对话任务记录（与对应NPC进行过对话/触发引导）
    std::set<std::string> talkedTasks; // 新增
    
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

    // 新增：物品售卖和丢弃功能
    bool sellItem(const std::string& itemName, int quantity = 1);
    bool dropItem(const std::string& itemName, int quantity = 1);
    bool canSellItem(const std::string& itemName) const;
    int getItemSellPrice(const std::string& itemName) const;

    // 蓝条管理
    int getMP() const { return Attribute::getMP(); }
    int getMaxMP() const { return Attribute::getMaxMP(); }
    void setMP(int value) { Attribute::setMP(value); }
    void setMaxMP(int value) { Attribute::setMaxMP(value); }

    // 新增：物理护盾状态管理
    int getPhysicalShieldTurns() const { return physicalShieldTurns; }
    void addPhysicalShieldTurns(int turns) { physicalShieldTurns = std::max(0, physicalShieldTurns + turns); }
    void consumePhysicalShieldTurn() { if (physicalShieldTurns > 0) --physicalShieldTurns; }

    // 新增：临时增益状态管理
    void addSpeedBoost(int amount, int turns);
    void addDefenseBoost(int amount, int turns);
    void updateBuffTurns();                     // 每回合结束时调用，减少增益剩余时间
    std::string getBuffStatus() const;          // 获取当前增益状态描述

    // 主动穿戴装备（通过装备名）
    bool equipFromInventory(const std::string& equipmentName);
    void addExp(int value);

    // 已对话任务记录管理
    void markTalkedToNpc(const std::string& taskId) { talkedTasks.insert(taskId); } // 新增
    bool hasTalkedToNpc(const std::string& taskId) const { return talkedTasks.count(taskId) > 0; } // 新增
    const std::set<std::string>& getTalkedTasks() const { return talkedTasks; } // 新增
};

#endif // PLAYER_H