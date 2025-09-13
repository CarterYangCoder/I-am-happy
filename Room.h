#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <map>
#include <vector>

/**
 * @class Room
 * @brief 地图房间：包含房间信息、出口、NPC、物品等。
 */
class Room {
private:
    int roomId;
    std::string roomName;
    std::string description;
    std::string hint;
    std::map<std::string, std::pair<int, std::string>> exits; ///< 方向到(房间ID, 房间名)
    std::vector<std::string> npcs;

    // 静态成员变量声明（不初始化）
    static const std::map<std::string, std::string> DIR_TO_NUM;
    static const std::map<std::string, std::string> NUM_TO_DIR;

public:
    std::vector<std::string> items; ///< 房间物品列表（示例中开放）

    /** @brief 默认构造（占位）。 */
    Room();

    /**
     * @brief 构造房间。
     * @param id 房间ID
     * @param name 名称
     * @param desc 描述
     * @param h 提示
     */
    Room(int id, const std::string& name, const std::string& desc, const std::string& h);

    /** @name 基本信息访问器 */
    ///@{
    int getRoomId() const { return roomId; }
    std::string getRoomName() const { return roomName; }
    std::string getDescription() const { return description; }
    std::string getHint() const { return hint; }
    ///@}

    /**
     * @brief 添加出口。
     * @param dir 方向（N/NE/... 或中文）
     * @param targetRoomId 目标房间ID
     * @param targetRoomName 目标房间名称
     */
    void addExit(const std::string& dir, int targetRoomId, const std::string& targetRoomName);

    /** @brief 添加NPC名。 */
    void addNPC(const std::string& npcName);

    /** @brief 添加房间物品名。 */
    void addItem(const std::string& itemName);

    /** @brief 获取出口映射。 */
    const std::map<std::string, std::pair<int, std::string>>& getExits() const { return exits; }

    /** @brief 打印房间信息。 */
    void showRoomInfo() const;

    /** @name 方向与数字转换 */
    ///@{
    static std::string numberToDir(const std::string& num);
    static std::string dirToNumber(const std::string& dir);
    ///@}
};

#endif // ROOM_H
