#include "Item.h"

Item::Item(int id,std::string name, ItemType type, std::string desc, int price)
    :id(id), name(name), type(type), description(desc), price(price) {}

std::string Item::getName() const { return name; }
ItemType Item::getType() const { return type; }
std::string Item::getDescription() const { return description; }
int Item::getPrice() const { return price; }
int Item::getId() const { return id; } 

// 具体物品子类实现
HealthPotion::HealthPotion() :Item(1,"生命药水", ItemType::HEALTH_POTION, "回复50%生命值", 50) {}
std::string HealthPotion::use() { return "回复50%生命值"; }
int HealthPotion::getHealAmount() const { return 50; } // 这里返回百分比，实际使用时需要根据玩家最大生命值计算

EnergyPotion::EnergyPotion() : Item(2,"能量药水", ItemType::ENERGY_POTION, "下个回合行动次数+1", 100) {}
std::string EnergyPotion::use() { return "下个回合行动次数+1"; }
bool EnergyPotion::grantsExtraAction() const { return true; }

HealthPotion* HealthPotion::clone() const {
    return new HealthPotion(*this);
}
EnergyPotion* EnergyPotion::clone() const {
    return new EnergyPotion(*this);
}