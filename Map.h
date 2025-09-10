#ifndef MAP_H
#define MAP_H

#include "Room.h"
#include "NPC.h"
#include "EvilGeneral.h"
#include "CommonEnemy.h"
#include <map>
#include <memory>
#include <vector>
 
// 前向声明
class BossWanEshuji;
class SaveLoadSystem;
class Player;
class TaskSystem;

class Map {
public: // 将rooms改为public
    std::map<int, Room> rooms; // 房间列表，改为public，便于game.cpp访问
private:
    std::map<int, std::unique_ptr<NPC>> roomNPCs;
    std::map<int, std::unique_ptr<EvilGeneral>> roomBosses;
    std::map<int, std::vector<std::unique_ptr<CommonEnemy>>> roomEnemies;
    int currentRoomId;

    // 初始化方法
    void initRooms();
    void initNPCs();
    void initBosses();
    void initEnemies();

    // 内部绘制方法
    void drawGlobalMap() const;
    void drawLocationMap() const;

public:
    // 构造函数
    Map();

    // 地图显示功能
    void showGlobalMap();
    void showLocationMap();
    void showCurrentRoom() const;
    void showInitialRoom() const;

    // 房间移动功能
    bool switchRoom(const std::string& input);
    bool switchRoom(const std::string& input, Player* player, SaveLoadSystem* saveSystem, TaskSystem* taskSystem);
    void jumpToRoom(int roomId);

    // 获取器和设置器
    int getCurrentRoomId() const;
    void setCurrentRoomId(int roomId);

    // 获取当前房间的内容
    NPC* getCurrentRoomNPC() const;
    
    // 获取当前房间的BOSS
    EvilGeneral* getCurrentRoomBoss() const;
    
    // 获取当前房间的万恶枢机
    BossWanEshuji* getCurrentRoomFinalBoss() const;
    
    // 获取当前房间的随机敌人
    CommonEnemy* getRandomEnemy() const;

    // 移除已击败的敌人和BOSS
    void removeDefeatedEnemy(CommonEnemy* enemy);
    void removeDefeatedBoss();
};

#endif // MAP_H
