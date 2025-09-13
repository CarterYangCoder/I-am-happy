#ifndef SHOPSYSTEM_H
#define SHOPSYSTEM_H

#include "item.h"
#include "UIManager.h"
#include"Player.h"
#include <vector>
#include <optional>

/**
 * @class ShopSystem
 * @brief 商店系统：初始化、展示、购买与查询。
 */
class ShopSystem {
public:
    /**
     * @brief 构造商店系统。
     * @param uiManager UI 管理器引用
     */
    ShopSystem(UIManager& uiManager);

    /**
     * @brief 初始化商店货架。
     */
    void initializeShop();

    /**
     * @brief 展示商店物品列表与指引。
     */
    void displayShopItems() const;

    /**
     * @brief 购买指定物品。
     * @param player 玩家
     * @param itemToBuy 目标物品（数据库原型）
     * @param quantity 购买数量
     */
    void buyItem(Player& player, const Item& itemToBuy, int quantity);

    /**
     * @brief 通过 ID 获取商店物品（只读）。
     * @param id 物品 ID
     * @return 指向物品的指针，不存在返回 nullptr
     */
    const Item* getItemById(int id) const;

private:
    UIManager& ui;                 ///< UI 管理器
    std::vector<Item*> shopInventory; ///< 货架（指针仅演示用途）
};

#endif // SHOPSYSTEM_H