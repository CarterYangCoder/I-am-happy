#ifndef SHOPSYSTEM_H
#define SHOPSYSTEM_H

#include "item.h"
#include "UIManager.h"
#include"Player.h"
#include <vector>
#include <optional> // 需要包含 <optional> 头文件

class ShopSystem {
public: 
    ShopSystem(UIManager& uiManager);

    void initializeShop();
    void displayShopItems() const;
    
    void buyItem(Player& player, const Item& itemToBuy, int quantity);

    // --- 通过ID获取商品的辅助函数 ---
    const Item* getItemById(int id) const;

private:
    UIManager& ui;
    std::vector<Item*> shopInventory;
};

#endif // SHOPSYSTEM_H