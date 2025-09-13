#ifndef MAP_H
#define MAP_H

#include "Room.h"
#include "NPC.h"
#include "EvilGeneral.h"
#include "CommonEnemy.h"
#include <map>
#include <memory>
#include <vector>

class BossWanEshuji;
class SaveLoadSystem;
class Player;
class TaskSystem;

/**
 * @class Map
 * @brief 游戏地图：房间/NPC/敌人初始化与移动/显示/查询接口。
 */
class Map {
public:
    std::map<int, Room> rooms; ///< 房间列表（示例中开放访问）

private:
    std::map<int, std::unique_ptr<NPC>> roomNPCs;
    std::map<int, std::unique_ptr<EvilGeneral>> roomBosses;
    std::map<int, std::vector<std::unique_ptr<CommonEnemy>>> roomEnemies;
    int currentRoomId;
    bool obsidianDustSpawned = false;

    /** @brief 初始化各区域房间/ NPC / Boss / 怪物。 */
    void initRooms();
    void initNPCs();
    void initBosses();
    void initEnemies();

    /** @brief 地图绘制内部实现。 */
    void drawGlobalMap() const;
    void drawLocationMap() const;

public:
    /** @brief 构造地图并初始化场景。 */
    Map();

    /** @brief 地图显示 */
    void showGlobalMap();
    void showLocationMap();
    void showCurrentRoom() const;
    void showInitialRoom() const;

    /** @brief 房间移动（字符串方向/编号）。 */
    bool switchRoom(const std::string& input);
    bool switchRoom(const std::string& input, Player* player, SaveLoadSystem* saveSystem, TaskSystem* taskSystem);
    /** @brief 直接跳房间（调试用）。 */
    void jumpToRoom(int roomId);

    /** @brief 当前房间ID访问。 */
    int getCurrentRoomId() const;
    void setCurrentRoomId(int roomId);

    /** @brief 获取当前房间的 NPC/BOSS/最终BOSS/随机敌人。 */
    NPC* getCurrentRoomNPC() const;
    EvilGeneral* getCurrentRoomBoss() const;
    BossWanEshuji* getCurrentRoomFinalBoss() const;
    CommonEnemy* getRandomEnemy() const;

    /** @brief 移除已击败的敌人/BOSS。 */
    void removeDefeatedEnemy(CommonEnemy* enemy);
    void removeDefeatedBoss();

    /** @brief 查询房间中是否存在指定类型存活敌人（门禁用途）。 */
    bool hasEnemyTypeInRoom(int roomId, EnemyType type) const;

    /** @brief 裂隙废墟黑曜晶尘是否已刷新。 */
    bool isObsidianDustSpawned() const { return obsidianDustSpawned; }
    void setObsidianDustSpawned(bool v) { obsidianDustSpawned = v; }
};

#endif // MAP_H
