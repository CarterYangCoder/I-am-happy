#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <string>
#include <map>
#include "GameData.h"

// 前向声明
class Player;
class Attribute;
class Equipment; 

// 使用ANSI转义码实现跨平台颜色显示。
class UIManager {
public:
    enum class Color {
        BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, GRAY,
        RESET // 重置颜色
    };

    // 显示带颜色的消息 
    void displayMessage(const std::string& message, Color color = Color::WHITE) const;

    // 显示玩家属性面板 
    void displayPlayerStatus(const class Player& player) const;
    void displayPlayerStatus(const class Player& player, const std::string& lastActionMsg) const;

    // 显示场景描述 
    void displayScene(const std::string& description) const;

    // 暂停，等待用户输入
    void pause() const;
    void displaySimpleCombatStatus(const Attribute& player, const Attribute& enemy) const;  // 简化战斗状态显示
    void displayMoveHelp() const; // 显示移动帮助
private:
    // 设置控制台文本颜色
    void setColor(Color color) const;
};

#endif // UIMANAGER_H