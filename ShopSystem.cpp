#include "ShopSystem.h"
#include <iostream>

ShopSystem::ShopSystem(UIManager& uiManager) : ui(uiManager) {}

void ShopSystem::initializeShop() {
    shopInventory.push_back(new HealthPotion());
    shopInventory.push_back(new EnergyPotion());
    shopInventory.push_back(new MysteriousItem());

    ui.displayMessage("商店系统已初始化。", UIManager::Color::GRAY);
}
 
void ShopSystem::displayShopItems() const {
    ui.displayMessage("--- 欢迎光临商店 ---", UIManager::Color::CYAN);
    for (const auto& item : shopInventory) {
        if (item) { 
            std::string itemInfo = "[" + std::to_string(item->getId()) + "] " + item->getName() +
                " - " + std::to_string(item->getPrice()) + "金币 (" +
                item->getDescription() + ")";
            ui.displayMessage(itemInfo, UIManager::Color::WHITE);
        }
    }
    ui.displayMessage("--------------------", UIManager::Color::CYAN);
    ui.displayMessage("输入 'buy <序号>' 或 'buy <序号>*<数量>' 购买", UIManager::Color::YELLOW);
    ui.displayMessage("输入 'leave' 离开商店", UIManager::Color::YELLOW);
}

const Item* ShopSystem::getItemById(int id) const {
    for (const auto& item_ptr : shopInventory) {
        if (item_ptr && item_ptr->getId() == id) {
            return item_ptr;
        }
    }
    return nullptr;
}

void ShopSystem::buyItem(Player& player, const Item& itemToBuy, int quantity) {
    long long totalPrice = static_cast<long long>(itemToBuy.getPrice()) * quantity;
    if (player.getGold() >= totalPrice) {
        player.setGold(player.getGold() - totalPrice);
        player.addItem(itemToBuy, quantity);
        ui.displayMessage("购买 " + itemToBuy.getName() + " x" + std::to_string(quantity) + " 成功!", UIManager::Color::GREEN);
        ui.displayMessage("花费金币: " + std::to_string(totalPrice), UIManager::Color::YELLOW);
        ui.displayMessage("剩余金币: " + std::to_string(player.getGold()), UIManager::Color::YELLOW);
    }
    else {
        ui.displayMessage("金币不足! 你需要 " + std::to_string(totalPrice) + " 金币, 但只有 " + std::to_string(player.getGold()) + " 金币。", UIManager::Color::RED);
    }
}