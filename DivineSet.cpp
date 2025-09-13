#include "DivineSet.h"

DivineSet::DivineSet() {}

// 添加套装部件
void DivineSet::addPart(Equipment* part) {
    if (part) {
        parts[part->getPart()] = part; 
    }
}

// 检查是否集齐（6个部件）
bool DivineSet::isComplete() const {
    return parts.size() == 6;
}

// 套装共鸣效果（集齐后解锁最终技能）
std::string DivineSet::getResonanceEffect() const {
    return isComplete() ? "解锁最终技能「星闪流河圣龙飞升·神界湮灭斩·最终式」，全属性+20%" : "未集齐套装，无共鸣效果";
}

// 单个部件效果
std::string DivineSet::getPartEffect(EquipmentPart part) const {
    auto it = parts.find(part);
    if (it != parts.end()) {
        return it->second->getSpecialEffect();
    }
    return "该部位未装备套装部件";
}

// 获取装备指针
Equipment* DivineSet::getEquipment(EquipmentPart part) const {
    auto it = parts.find(part);
    if (it != parts.end()) {
        return it->second;
    }
    return nullptr;
}

// 获取所有装备
const std::map<EquipmentPart, Equipment*>& DivineSet::getAllParts() const {
    return parts;
}