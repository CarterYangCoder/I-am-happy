/**
 * @file ShopSystem.cpp
 * @brief 商店系统实现：货架初始化、展示、购买、按ID查询。
 */

#include "ShopSystem.h"
#include <iostream>

/** @brief 构造商店系统。 */
ShopSystem::ShopSystem(UIManager& uiManager) : ui(uiManager) {}

/** @brief 初始化商店货架（演示用固定清单）。 */
void ShopSystem::initializeShop() {
    // 演示：以 new 方式构建入货（真实项目建议资源集中管理）
    shopInventory.push_back(new HealthPotion());
    shopInventory.push_back(new EnergyPotion());
    shopInventory.push_back(new MysteriousItem());

    ui.displayMessage("商店系统已初始化。", UIManager::Color::GRAY);
}

/** @brief 打印商店清单与指引。 */
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

/** @brief 按 ID 查找物品。 */
const Item* ShopSystem::getItemById(int id) const {
    for (const auto& item_ptr : shopInventory) {
        if (item_ptr && item_ptr->getId() == id) {
            return item_ptr;
        }
    }
    return nullptr;
}

/**
 * @brief 购买指定数量的物品，扣除金币并加入背包。
 * @note 使用 long long 计算总价以避免乘法溢出。
 */
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