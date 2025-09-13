#pragma once
#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>
#include <iostream>

/**
 * @enum ItemType
 * @brief 物品类型：药水/任务/其他。
 */
enum class ItemType {
    HEALTH_POTION,
    ENERGY_POTION,
    QUEST_ITEM,
    MISC
};

/**
 * @class Item
 * @brief 物品基类：包含id/名称/类型/描述/价格与使用接口。
 */
class Item {
protected:
    int id;
    std::string name;
    ItemType type;
    std::string description;
    std::vector<Item*> drops;
    int price; ///< -1 表示不可交易

public:
    /**
     * @brief 构造物品。
     */
    Item(int id,std::string name, ItemType type, std::string desc, int price = -1);
    int getId()const;
    std::string getName() const;
    ItemType getType() const;
    std::string getDescription() const;
    int getPrice() const;

    /** @brief 使用物品的效果（纯虚）。 */
    virtual std::string use() = 0;
    /** @brief 获取治疗量（默认0）。 */
    virtual int getHealAmount() const { return 0; }
    /** @brief 是否给予额外行动（默认否）。 */
    virtual bool grantsExtraAction() const { return false; }
    /** @brief 原型克隆（工厂/复制用途）。 */
    virtual Item* clone() const = 0;
    virtual ~Item() = default;
};

/** @brief 生命药水：回复 50% 生命。 */
class HealthPotion : public Item {
public:
    HealthPotion();
    std::string use() override;
    int getHealAmount() const override;
    HealthPotion* clone() const override;
};

/** @brief 能量药水：回复 50% 当前蓝量。 */
class EnergyPotion : public Item {
public:
    EnergyPotion();
    std::string use() override;
    bool grantsExtraAction() const override;
    EnergyPotion* clone() const override;
};

/** @brief 神秘商品：用于事件演出。 */
class MysteriousItem : public Item {
public:
    MysteriousItem();
    std::string use() override;
    MysteriousItem* clone() const override;
};

#endif // ITEM_H