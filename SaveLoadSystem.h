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

/**
 * @struct SaveSlot
 * @brief 存档槽元数据：槽位ID、文件名、时间戳、玩家名与等级。
 */
struct SaveSlot {
    int id;
    std::string filename;
    time_t timestamp;
    std::string playerName;
    int playerLevel;
};

/**
 * @class SaveLoadSystem
 * @brief 存档/读档/自动存档与存档枚举工具。
 */
class SaveLoadSystem {
public:
    /**
     * @brief 构造存档系统。
     * @param uiManager UI 管理器
     */
    SaveLoadSystem(UIManager& uiManager);

    /**
     * @brief 保存游戏到槽位（自动选择或覆盖）。
     * @param player 玩家只读视图
     * @param tasks 任务系统只读视图
     */
    void saveGame(const Player& player, const TaskSystem& tasks);

    /**
     * @brief 自动存档（保留一份最新自动存档）。
     */
    void autoSaveGame(const Player& player, const TaskSystem& tasks);

    /**
     * @brief 读取游戏存档，恢复玩家与任务状态。
     * @return true 成功；false 失败
     */
    bool loadGame(Player& player, TaskSystem& tasks);

    /**
     * @brief 读档后技能检查与解锁（按等级/套装）。
     */
    void postLoadSkillCheck(Player& player);

private:
    UIManager& ui;
    static const int MAX_SAVES = 10;   ///< 最大手动存档数量
    const std::string SAVE_DIR = "save/"; ///< 存档目录

    /** @brief 列出所有手动存档槽位元数据。 */
    std::vector<SaveSlot> listSaveSlots();

    /** @brief 时间戳格式化为可读字符串。 */
    std::string timeToString(time_t time);

    /** @brief 纯数字字符串判定（用于输入校验）。 */
    bool is_digits_save(const std::string& str);

    /** @brief 通过装备名重建装备指针（从存档名恢复）。 */
    Equipment* createEquipmentByName(const std::string& equipmentName);
};

#endif // SAVELOADSYSTEM_H