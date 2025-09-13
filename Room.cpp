/**
 * @file Room.cpp
 * @brief 房间实体实现：构造、管理出口/NPC/物品、信息展示。
 */

#include "Room.h"
#include <iostream>
#include <algorithm>

// 初始化静态成员映射
const std::map<std::string, std::string> Room::DIR_TO_NUM = {
    {"北", "1"},   {"东北", "2"}, {"东", "3"},  {"东南", "4"},
    {"南", "5"},   {"西南", "6"}, {"西", "7"},  {"西北", "8"},
    {"上", "9"},   {"下", "0"}
};
const std::map<std::string, std::string> Room::NUM_TO_DIR = {
    {"1", "北"},   {"2", "东北"}, {"3", "东"},  {"4", "东南"},
    {"5", "南"},   {"6", "西南"}, {"7", "西"},  {"8", "西北"},
    {"9", "上"},   {"0", "下"}
};

/** @brief 参数构造函数。 */
Room::Room(int id, const std::string& name, const std::string& desc, const std::string& h)
    : roomId(id), roomName(name), description(desc), hint(h) {}

/** @brief 默认构造：设置占位初值。 */
Room::Room() : roomId(0), roomName("未知房间"), description(""), hint("") {}

/** @brief 注册一个可到达出口。 */
void Room::addExit(const std::string& dir, int targetRoomId, const std::string& targetRoomName) {
    exits[dir] = { targetRoomId, targetRoomName };
}

/** @brief 添加NPC名。 */
void Room::addNPC(const std::string& npcName) { npcs.push_back(npcName); }

/** @brief 添加物品名。 */
void Room::addItem(const std::string& itemName) { items.push_back(itemName); }

/** @brief 显示房间信息（名称/描述/提示/NPC/物品/出口）。 */
void Room::showRoomInfo() const {
    std::cout << "\033[34m[房间信息]\033[0m" << std::endl;
    std::cout << "名称: " << roomName << " (ID: " << roomId << ")" << std::endl;
    std::cout << "描述: " << description << std::endl;
    std::cout << "提示: " << hint << std::endl;

    std::cout << "NPC: ";
    for (size_t i = 0; i < npcs.size(); ++i) {
        std::cout << npcs[i];
        if (i != npcs.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    std::cout << "物品: ";
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << items[i];
        if (i != items.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    std::cout << "出口: ";
    for (auto it = exits.begin(); it != exits.end(); ++it) {
        std::cout << it->first << "(" << dirToNumber(it->first) << ")->" << it->second.second;
        if (std::next(it) != exits.end()) std::cout << ", ";
    }
    std::cout << std::endl;
}
    std::cout << std::endl;

    std::cout << "出口: ";
    for (auto it = exits.begin(); it != exits.end(); ++it) {
        std::cout << it->first << "(" << dirToNumber(it->first) << ")->" << it->second.second;
        if (std::next(it) != exits.end()) std::cout << ", ";
    }
    std::cout << std::endl;
}
