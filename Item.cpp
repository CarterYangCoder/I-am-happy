#include "Item.h"

Item::Item(int id,std::string name, ItemType type, std::string desc, int price)
    :id(id), name(name), type(type), description(desc), price(price) {}

std::string Item::getName() const { return name; }
ItemType Item::getType() const { return type; }
std::string Item::getDescription() const { return description; }
int Item::getPrice() const { return price; }
int Item::getId() const { return id; } 

// 具体物品子类实现
HealthPotion::HealthPotion() :Item(1,"生命药水", ItemType::HEALTH_POTION, "回复50%最大生命值", 50) {} // 改文案
std::string HealthPotion::use() { return "回复50%最大生命值"; } // 改文案
int HealthPotion::getHealAmount() const { return 50; } // 这里返回百分比，实际使用时需要根据玩家最大生命值计算

EnergyPotion::EnergyPotion() : Item(2,"能量药水", ItemType::ENERGY_POTION, "回复50%最大蓝量", 100) {} // 改文案
std::string EnergyPotion::use() { return "回复50%最大蓝量"; } // 改文案
bool EnergyPotion::grantsExtraAction() const { return false; } // 不再提供额外行动

MysteriousItem::MysteriousItem() : Item(3,"神秘商品", ItemType::MISC, "看起来很神秘，不知道是什么", 10) {}
std::string MysteriousItem::use() { 
    std::cout << "神秘商人突然出现！" << std::endl;
    std::cout << "按 Enter 键继续..." << std::endl;
    std::string dummy;
    std::getline(std::cin, dummy);
    
    std::cout << "神秘商人：\"恭喜你买到了一个教训！ (ಠ‿ಠ) \"" << std::endl;
    std::cout << "按 Enter 键继续..." << std::endl;
    std::getline(std::cin, dummy);
    
    std::cout << "神秘商人：\"10金币买个教训，还是挺便宜的嘛~ (╯▔皿▔)╯\"" << std::endl;
    std::cout << "按 Enter 键继续..." << std::endl;
    std::getline(std::cin, dummy);
    
    std::cout << "神秘商人：\"记住，天下没有免费的午餐哦！ (¬‿¬)\"" << std::endl;
    std::cout << "按 Enter 键继续..." << std::endl;
    std::getline(std::cin, dummy);
    
    std::cout << "神秘商人消失了..." << std::endl;
    return "获得了一个珍贵的教训"; 
}

HealthPotion* HealthPotion::clone() const {
    return new HealthPotion(*this);
}
EnergyPotion* EnergyPotion::clone() const {
    return new EnergyPotion(*this);
}
MysteriousItem* MysteriousItem::clone() const {
    return new MysteriousItem(*this);
}