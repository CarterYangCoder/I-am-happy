#pragma once
#ifndef DIVINESET_H
#define DIVINESET_H
#include "Equipment.h"
#include <map>

/**
 * @class DivineSet
 * @brief 六誓圣辉套装：管理部件、是否集齐与共鸣效果描述。
 */
class DivineSet {
private:
    std::map<EquipmentPart, Equipment*> parts;

public:
    /** @brief 构造空套装。 */
    DivineSet();

    /** @brief 添加套装部件（覆盖同部位）。 */
    void addPart(Equipment* part);

    /** @brief 是否集齐六件套。 */
    bool isComplete() const;

    /** @brief 集齐后的共鸣效果描述。 */
    std::string getResonanceEffect() const;

    /** @brief 获取单部位的特殊效果描述。 */
    std::string getPartEffect(EquipmentPart part) const;

    /** @brief 获取指定部位装备指针。 */
    Equipment* getEquipment(EquipmentPart part) const;

    /** @brief 获取全部部件映射。 */
    const std::map<EquipmentPart, Equipment*>& getAllParts() const;
};

#endif // DIVINESET_H