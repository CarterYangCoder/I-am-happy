#include "Room.h"
#include <iostream>
#include <algorithm>

// 初始化静态成员变量（只在cpp中初始化一次）
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

// 构造函数实现
Room::Room(int id, const std::string& name, const std::string& desc, const std::string& h)
    : roomId(id), roomName(name), description(desc), hint(h) {}

// 默认构造函数
Room::Room() : roomId(0), roomName("未知房间"), description(""), hint("") {
    // 默认构造函数，初始化成员变量
}

// 添加出口
void Room::addExit(const std::string& dir, int targetRoomId, const std::string& targetRoomName) {
    // 添加出口时，确保方向和编号一致
    exits[dir] = { targetRoomId, targetRoomName };
}

// 添加NPC
void Room::addNPC(const std::string& npcName) {
    npcs.push_back(npcName);
}

// 添加物品
void Room::addItem(const std::string& itemName) {
    items.push_back(itemName);
}

// 辅助函数：将方向编号统一转为大写
std::string toUpper(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// 显示房间信息
void Room::showRoomInfo() const {
    std::cout << "\033[34m[房间信息]\033[0m" << std::endl;
    std::cout << "名称: " << roomName << " (ID: " << roomId << ")" << std::endl;
    std::cout << "描述: " << description << std::endl;
    std::cout << "提示: " << hint << std::endl;

    std::cout << "NPC: ";
    if (npcs.empty()) {
        std::cout << "无";
    } else {
        for (size_t i = 0; i < npcs.size(); ++i) {
            std::cout << npcs[i];
            if (i != npcs.size() - 1) std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "物品: ";
    if (items.empty()) {
        std::cout << "无";
    } else {
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << items[i];
            if (i != items.size() - 1) std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "出口: ";
    if (exits.empty()) {
        std::cout << "无";
    } else {
        for (auto it = exits.begin(); it != exits.end(); ++it) {
            std::cout << it->first << "(" << dirToNumber(it->first) << ")->" << it->second.second;
            if (std::next(it) != exits.end()) std::cout << ", ";
        }
    }
    std::cout << std::endl;
}
