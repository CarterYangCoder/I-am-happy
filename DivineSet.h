#pragma once
#ifndef DIVINESET_H
#define DIVINESET_H
#include "Equipment.h"
#include <map>

class DivineSet {
private:
    std::map<EquipmentPart, Equipment*> parts;  // 套装部件（部位->装备）
 
public:
    DivineSet();

    // 添加套装部件
    void addPart(Equipment* part);

    // 检查是否集齐所有部件
    bool isComplete() const;

    // 获取套装共鸣效果（集齐后触发）
    std::string getResonanceEffect() const;

    // 获取单个部件的特殊效果
    std::string getPartEffect(EquipmentPart part) const;
    
    // 获取装备指针
    Equipment* getEquipment(EquipmentPart part) const;
    
    // 获取所有装备
    const std::map<EquipmentPart, Equipment*>& getAllParts() const;
};

#endif // DIVINESET_H