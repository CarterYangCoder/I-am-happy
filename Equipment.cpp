#include "Equipment.h"

Equipment::Equipment(std::string name, EquipmentPart part, std::string desc, int atk, int def, std::string effect)
    : Item(0, name, ItemType::MISC, desc, -1), part(part), atkBonus(atk), defBonus(def), specialEffect(effect) {}

EquipmentPart Equipment::getPart() const { return part; }
int Equipment::getAtkBonus() const { return atkBonus; }
int Equipment::getDefBonus() const { return defBonus; }
std::string Equipment::getSpecialEffect() const { return specialEffect; }
 
void Equipment::setAtkBonus(int atk) { atkBonus = atk; }
void Equipment::setDefBonus(int def) { defBonus = def; }

std::string Equipment::use() {
    return "装备了 " + getName() + "，攻击力+" + std::to_string(atkBonus) + 
           "，防御力+" + std::to_string(defBonus) + "。特殊效果：" + specialEffect;
}

Equipment* Equipment::clone() const {
    return new Equipment(*this);
}