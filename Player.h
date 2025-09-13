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

/**
 * @class Player
 * @brief 玩家实体：继承 Attribute，增加装备/技能/任务/物品与临时增益。
 */
class Player : public Attribute {
private:
    DivineWeapon* divineSword;   ///< 六圣裁恶神剑
    DivineSet divineSet;         ///< 六誓圣辉套装
    std::vector<Skill*> skills;  ///< 已解锁技能
    int currentRoomId;           ///< 所在房间ID
    int physicalShieldTurns;     ///< 物理护盾剩余回合

    // 临时增益状态
    int speedBoostTurns;
    int speedBoostAmount;
    int defenseBoostTurns;
    int defenseBoostAmount;

public:
    /**
     * @brief 构造玩家。
     * @param name 名称（默认“安特王子”）
     */
    Player(std::string name = "安特王子");
    /** @brief 析构释放资源。 */
    ~Player();
    int extraActionTurns;

    /** @name 位置管理 */
    ///@{
    int getCurrentRoomId() const { return currentRoomId; }
    void setCurrentRoomId(int roomId) { currentRoomId = roomId; }
    ///@}

    /** @name 神器与套装 */
    ///@{
    void equipSetPart(Equipment* part);
    DivineWeapon* getDivineSword() const;
    bool hasAllSetParts() const;
    std::map<EquipmentPart, Equipment*> getAllEquippedItems() const;
    ///@}

    /** @brief 重写升级：同步技能解锁与神剑成长。 */
    bool levelUp() override;

    /** @name 技能管理 */
    ///@{
    void unlockSkill(SkillType type);
    void checkAndUnlockSkills();
    std::vector<Skill*> getSkills() const;
    Skill* getSkill(SkillType type) const;
    ///@}

    /** @name 任务管理 */
    ///@{
    std::map<std::string, Task> taskProgress;
    void updateTaskProgress(std::string taskID, TaskStatus status);
    bool isTaskCompleted(std::string taskID) const;
    ///@}

    /** @name 物品管理 */
    ///@{
    std::map<std::string, int> inventory;
    void addItem(const Item& item, int quantity = 1);
    void addItemByName(const std::string& itemName, int quantity = 1);
    bool useItem(const std::string& itemName);
    const std::map<std::string, int>& getInventory() const;
    void clearInventory();
    bool sellItem(const std::string& itemName, int quantity = 1);
    bool dropItem(const std::string& itemName, int quantity = 1);
    bool canSellItem(const std::string& itemName) const;
    int getItemSellPrice(const std::string& itemName) const;
    ///@}

    /** @name 蓝量管理 */
    ///@{
    int getMP() const { return Attribute::getMP(); }
    int getMaxMP() const { return Attribute::getMaxMP(); }
    void setMP(int value) { Attribute::setMP(value); }
    void setMaxMP(int value) { Attribute::setMaxMP(value); }
    ///@}

    /** @name 物理护盾 */
    ///@{
    int getPhysicalShieldTurns() const { return physicalShieldTurns; }
    void addPhysicalShieldTurns(int turns) { physicalShieldTurns = std::max(0, physicalShieldTurns + turns); }
    void consumePhysicalShieldTurn() { if (physicalShieldTurns > 0) --physicalShieldTurns; }
    ///@}

    /** @name 临时增益 */
    ///@{
    void addSpeedBoost(int amount, int turns);
    void addDefenseBoost(int amount, int turns);
    void updateBuffTurns();
    std::string getBuffStatus() const;
    ///@}

    /** @brief 从背包装备指定装备名。 */
    bool equipFromInventory(const std::string& equipmentName);

    /** @brief 增加经验（内部处理升级循环）。 */
    void addExp(int value);
};

#endif // PLAYER_H