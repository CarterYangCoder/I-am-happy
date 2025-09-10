#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <map>
#include <vector>

class Room {
private:
    int roomId; 
    std::string roomName;
    std::string description;
    std::string hint;
    std::map<std::string, std::pair<int, std::string>> exits; // 方向到房间ID和名称的映射
    std::vector<std::string> npcs;

    // 静态成员变量声明（不初始化）
    static const std::map<std::string, std::string> DIR_TO_NUM;
    static const std::map<std::string, std::string> NUM_TO_DIR;

public:
    std::vector<std::string> items; // 物品列表，改为public，便于外部访问
    Room(); // 新增默认构造函数声明
    Room(int id, const std::string& name, const std::string& desc, const std::string& h);

    int getRoomId() const { return roomId; }
    std::string getRoomName() const { return roomName; }
    std::string getDescription() const { return description; }
    std::string getHint() const { return hint; }

    void addExit(const std::string& dir, int targetRoomId, const std::string& targetRoomName);
    void addNPC(const std::string& npcName);
    void addItem(const std::string& itemName);

    const std::map<std::string, std::pair<int, std::string>>& getExits() const { return exits; }

    void showRoomInfo() const;

    // 方向与数字转换 (1=北 2=东北 3=东 4=东南 5=南 6=西南 7=西 8=西北 9=上 0=下)
    static std::string numberToDir(const std::string& num) {
        if (num == "1") return "北";
        if (num == "2") return "东北"; 
        if (num == "3") return "东";
        if (num == "4") return "东南";
        if (num == "5") return "南";
        if (num == "6") return "西南";
        if (num == "7") return "西";
        if (num == "8") return "西北";
        if (num == "9") return "上";
        if (num == "0") return "下";
        return "";
    }

    static std::string dirToNumber(const std::string& dir) {
        if (dir == "N" || dir == "北") return "1";
        if (dir == "NE" || dir == "东北") return "2";
        if (dir == "E" || dir == "东") return "3";
        if (dir == "SE" || dir == "东南") return "4";
        if (dir == "S" || dir == "南") return "5";
        if (dir == "SW" || dir == "西南") return "6";
        if (dir == "W" || dir == "西") return "7";
        if (dir == "NW" || dir == "西北") return "8";
        if (dir == "U" || dir == "上") return "9";
        if (dir == "D" || dir == "下") return "0";
        return "";
    }
};

#endif // ROOM_H
