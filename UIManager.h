#pragma once
#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <string>
#include <vector>

// 前向声明，避免循环依赖
class Player;
class Attribute;

class UIManager {
public:
    enum class Color {
        BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, GRAY, RESET
    };

    // 基础输出
    void displayMessage(const std::string& message, Color color = Color::WHITE) const;
    void setColor(Color color) const;

    // 角色状态
    void displayPlayerStatus(const Player& player, const std::string& lastActionMsg) const;
    void displayPlayerStatus(const Player& player) const;

    // 战斗状态
    void displaySimpleCombatStatus(const Attribute& player, const Attribute& enemy) const;

    // 场景描述
    void displayScene(const std::string& description) const;

    // 暂停等待
    void pause() const;

    // 移动帮助
    void displayMoveHelp() const;

    // 分段对话输出（逐行输出，行间按回车继续）
    void displayDialogueBlock(const std::string& text, Color color = Color::WHITE, const std::string& continuePrompt = "按 Enter 键继续...") const;
    void displayDialogueLines(const std::vector<std::string>& lines, Color color = Color::WHITE, const std::string& continuePrompt = "按 Enter 键继续...") const;
};

#endif // UIMANAGER_H