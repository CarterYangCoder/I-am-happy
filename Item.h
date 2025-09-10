#pragma once
#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>

// 物品类型
enum class ItemType {
    HEALTH_POTION,  // 生命药水 
    ENERGY_POTION,  // 能量药水
    QUEST_ITEM,     // 任务物品（如黑曜晶尘）
    MISC            // 其他（如教训）
};

class Item {
protected:
    int id;
    std::string name;
    ItemType type;
    std::string description;
    std::vector<Item*> drops;
    int price;       // 价格（-1表示不可交易）


public:
    Item(int id,std::string name, ItemType type, std::string desc, int price = -1);
    int getId()const;
    std::string getName() const;
    ItemType getType() const;
    std::string getDescription() const;
    int getPrice() const;

    // 物品使用效果（纯虚函数，子类实现）
    virtual std::string use() = 0;
    virtual int getHealAmount() const { return 0; }  // 获取治疗量
    virtual bool grantsExtraAction() const { return false; }  // 是否给予额外行动
    virtual Item* clone() const = 0;
    virtual ~Item() = default; // 虚析构函数
};

class HealthPotion : public Item {
public:
    HealthPotion();
    std::string use() override;
    int getHealAmount() const override;
    HealthPotion* clone() const override;
};

class EnergyPotion : public Item {
public:
    EnergyPotion();
    std::string use() override;
    bool grantsExtraAction() const override;
    EnergyPotion* clone() const override;
};

#endif // ITEM_H