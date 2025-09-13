#pragma once
#ifndef EQUIPMENT_H
#define EQUIPMENT_H
#include "Item.h"
#include "GameData.h"

/**
 * @class Equipment
 * @brief 装备（也是 Item）：提供部位/属性加成与使用描述。
 */
class Equipment : public Item {
private:
    EquipmentPart part;
    int atkBonus;
    int defBonus;
    std::string specialEffect;

public:
    /**
     * @brief 构造装备。
     * @param name 名称
     * @param part 部位
     * @param desc 描述
     * @param atk 攻击加成
     * @param def 防御加成
     * @param effect 特殊效果描述
     */
    Equipment(std::string name, EquipmentPart part, std::string desc, int atk = 0, int def = 0, std::string effect = "");

    EquipmentPart getPart() const;
    int getAtkBonus() const;
    int getDefBonus() const;
    std::string getSpecialEffect() const;

    /** @brief 修改加成（神剑成长用）。 */
    void setAtkBonus(int atk);
    void setDefBonus(int def);

    /** @brief 使用描述（穿戴提示）。 */
    std::string use() override;
    Equipment* clone() const override;
};

#endif // EQUIPMENT_H