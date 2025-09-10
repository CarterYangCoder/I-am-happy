#ifndef SAVELOADSYSTEM_H
#define SAVELOADSYSTEM_H

#include "Player.h"
#include "UIManager.h"
#include "TaskSystem.h"
#include "Equipment.h"
#include <string>
#include <vector>
#include <ctime> 
#include <filesystem>

// 存档槽元数据结构
struct SaveSlot {
    int id;
    std::string filename;
    time_t timestamp;
    std::string playerName;
    int playerLevel;
};

class SaveLoadSystem {
public:
    SaveLoadSystem(UIManager& uiManager);

    void saveGame(const Player& player, const TaskSystem& tasks);
    void autoSaveGame(const Player& player, const TaskSystem& tasks);
    bool loadGame(Player& player, TaskSystem& tasks);
    
    // 读档后技能检测
    void postLoadSkillCheck(Player& player);

private:
    UIManager& ui;
    static const int MAX_SAVES = 10;
    const std::string SAVE_DIR = "save/";

    std::vector<SaveSlot> listSaveSlots();
    std::string timeToString(time_t time);
    bool is_digits_save(const std::string& str);
    Equipment* createEquipmentByName(const std::string& equipmentName);
};

#endif // SAVELOADSYSTEM_H