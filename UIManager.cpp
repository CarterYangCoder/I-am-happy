#include "UIManager.h"
#include "Player.h"
#include "Attribute.h"
#include "Equipment.h"
#include <iostream>
#include <iomanip>
#include <windows.h>

void UIManager::displayMessage(const std::string& message, Color color) const {
    // 设置控制台编码为UTF-8以支持中文显示
    SetConsoleOutputCP(CP_UTF8);
    
    // 控制台颜色代码
    const char* colorCodes[] = {
        "\033[30m",  // BLACK
        "\033[31m",  // RED
        "\033[32m",  // GREEN
        "\033[33m",  // YELLOW
        "\033[34m",  // BLUE
        "\033[35m",  // MAGENTA
        "\033[36m",  // CYAN
        "\033[37m",  // WHITE
        "\033[90m",  // GRAY
        "\033[0m"    // RESET
    };
    
    std::cout << colorCodes[static_cast<int>(color)] << message << "\033[0m" << std::endl;
}

void UIManager::setColor(Color color) const {
    switch (color) {
    case Color::BLACK:   std::cout << "\033[30m"; break;
    case Color::RED:     std::cout << "\033[31m"; break;
    case Color::GREEN:   std::cout << "\033[32m"; break;
    case Color::YELLOW:  std::cout << "\033[33m"; break;
    case Color::BLUE:    std::cout << "\033[34m"; break;
    case Color::MAGENTA: std::cout << "\033[35m"; break;
    case Color::CYAN:    std::cout << "\033[36m"; break;
    case Color::WHITE:   std::cout << "\033[37m"; break;
    case Color::GRAY:    std::cout << "\033[90m"; break;
    case Color::RESET:   std::cout << "\033[0m";  break;
    }
}

std::string equipmentPartToString(EquipmentPart part) {
    switch (part) {
    case EquipmentPart::HELMET: return "头盔";
    case EquipmentPart::CHESTPLATE: return "胸甲";
    case EquipmentPart::CAPE: return "披风";
    case EquipmentPart::BOOTS: return "靴子";
    case EquipmentPart::NECKLACE: return "项链";
    case EquipmentPart::RING: return "戒指";
    default: return "未知部位";
    }
}

void UIManager::displayPlayerStatus(const Player& player, const std::string& lastActionMsg) const {
    displayMessage("===== 角色状态 =====", Color::CYAN);
    std::map<EquipmentPart, Equipment*> equippedItems = player.getAllEquippedItems();
    // 属性区左侧，装备区右侧，分两列对齐
    std::cout << "\033[37m" << std::setw(10) << std::left << "姓名:" << std::setw(12) << player.getName();
    std::cout << "   |  " << std::setw(8) << "武器:" << "六圣裁恶神剑" << "\033[0m" << std::endl;
    std::cout << "\033[37m" << std::setw(10) << "等级:" << std::setw(12) << player.getLevel();
    std::cout << "   |  " << std::setw(8) << "头盔:";
    if (equippedItems.count(EquipmentPart::HELMET))
        std::cout << equippedItems[EquipmentPart::HELMET]->getName();
    else
        std::cout << "[空]";
    std::cout << "\033[0m" << std::endl;
    std::cout << "\033[32m" << std::setw(10) << "生命值:" << std::setw(12) << (std::to_string(player.getHP()) + "/" + std::to_string(player.getMaxHP()));
    std::cout << "   |  " << std::setw(8) << "胸甲:";
    if (equippedItems.count(EquipmentPart::CHESTPLATE))
        std::cout << equippedItems[EquipmentPart::CHESTPLATE]->getName();
    else
        std::cout << "[空]";
    std::cout << "\033[0m" << std::endl;
    std::cout << "\033[31m" << std::setw(10) << "攻击力:" << std::setw(12) << player.getATK();
    std::cout << "   |  " << std::setw(8) << "披风:";
    if (equippedItems.count(EquipmentPart::CAPE))
        std::cout << equippedItems[EquipmentPart::CAPE]->getName();
    else
        std::cout << "[空]";
    std::cout << "\033[0m" << std::endl;
    std::cout << "\033[34m" << std::setw(10) << "防御力:" << std::setw(12) << player.getDEF();
    std::cout << "   |  " << std::setw(8) << "靴子:";
    if (equippedItems.count(EquipmentPart::BOOTS))
        std::cout << equippedItems[EquipmentPart::BOOTS]->getName();
    else
        std::cout << "[空]";
    std::cout << "\033[0m" << std::endl;
    std::cout << "\033[33m" << std::setw(10) << "速度:" << std::setw(12) << player.getSpeed();
    std::cout << "   |  " << std::setw(8) << "项链:";
    if (equippedItems.count(EquipmentPart::NECKLACE))
        std::cout << equippedItems[EquipmentPart::NECKLACE]->getName();
    else
        std::cout << "[空]";
    std::cout << "\033[0m" << std::endl;
    std::cout << "\033[36m" << std::setw(10) << "经验值:" << std::setw(12) << (std::to_string(player.getExp()) + "/" + std::to_string(player.getExpToNextLevel()));
    std::cout << "   |  " << std::setw(8) << "戒指:";
    if (equippedItems.count(EquipmentPart::RING))
        std::cout << equippedItems[EquipmentPart::RING]->getName();
    else
        std::cout << "[空]";
    std::cout << "\033[0m" << std::endl;
    std::cout << "\033[34m" << std::setw(10) << "蓝量:" << std::setw(12) << (std::to_string(player.getMP()) + "/" + std::to_string(player.getMaxMP()));
    std::cout << "   |  " << std::setw(8) << "金币:" << player.getGold() << "\033[0m" << std::endl;
    std::cout << "\033[35m" << std::setw(10) << "暴击率:" << std::setw(12) << (std::to_string(static_cast<int>(player.getCritRate() * 100)) + "%") << "\033[0m" << std::endl;
    displayMessage("==================", Color::CYAN);
    
    // 显示已解锁技能
    displayMessage("--- 已解锁技能 ---", Color::YELLOW);
    const auto& skills = player.getSkills();
    if (skills.empty()) {
        displayMessage("暂无已解锁技能", Color::GRAY);
    } else {
        for (const auto& skill : skills) {
            std::cout << "\033[33m[" << skill->getName() << "]\033[0m ";
            std::cout << "\033[37m" << skill->getDescription() << "\033[0m" << std::endl;
        }
    }
    displayMessage("-------------------", Color::YELLOW);
    
    // 显示背包物品
    displayMessage("--- 背包物品 ---", Color::GREEN);
    const auto& inventory = player.getInventory();
    if (inventory.empty()) {
        displayMessage("背包为空", Color::GRAY);
    } else {
        for (const auto& item : inventory) {
            std::cout << "\033[32m" << item.first << "\033[0m: \033[37m" << item.second << "个\033[0m" << std::endl;
        }
    }
    
    displayMessage("------------------", Color::GREEN);
    
    // 显示最近操作物品及效果
    if (!lastActionMsg.empty()) {
        displayMessage("最近操作: " + lastActionMsg, Color::MAGENTA);
    }
}

void UIManager::displayPlayerStatus(const Player& player) const {
    displayPlayerStatus(player, "");
}

void UIManager::displaySimpleCombatStatus(const Attribute& player, const Attribute& enemy) const {
    std::cout << "\033[36m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m" << std::endl;
    std::cout << "\033[32m" << std::setw(15) <<std::left<< player.getName() 
              << " Lv." << std::setw(3) << player.getLevel()
              << " H P: " << std::setw(4) << player.getHP() << "/" << std::setw(4) << player.getMaxHP() << "\033[0m";
    std::cout << "      VS ";
    std::cout << "\033[31m" << std::setw(15)<<std::left << enemy.getName()
              << " Lv." << std::setw(3) << enemy.getLevel()
              << " H P: " << std::setw(4) << enemy.getHP() << "/" << std::setw(4) << enemy.getMaxHP() << "\033[0m" << std::endl;
    std::cout << std::setw(19) << " " << "\033[19mEXP: " << std::setw(6) << player.getExp() << "/" << std::setw(4) << player.getExpToNextLevel() << "\033[0m" << std::endl;
    std::cout << std::setw(19) << " " << "\033[19mM P: " << std::setw(6) << player.getMP() << "/" << std::setw(4) << player.getMaxMP() << "\033[0m" << std::endl;
    std::cout << "\033[36m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m" << std::endl;
}

void UIManager::displayScene(const std::string& description) const {
    displayMessage("--- 当前场景 ---", Color::GREEN);
    displayMessage(description, Color::WHITE);
    displayMessage("------------------", Color::GREEN);
}

void UIManager::pause() const {
    displayMessage("按 Enter 键继续...", Color::GRAY);
    std::string dummy;
    std::getline(std::cin, dummy); 
}

void UIManager::displayMoveHelp() const {
    displayMessage("移动命令: go <方向> (如: go N, go NE, go S, go U, go D)", Color::CYAN);
    displayMessage("方向英文: N=北 NE=东北 E=东 SE=东南 S=南 SW=西南 W=西 NW=西北 U=上 D=下", Color::GRAY);
}
