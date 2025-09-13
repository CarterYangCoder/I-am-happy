#pragma once
#ifndef NPC_H
#define NPC_H
#include <string>
#include <vector>
#include "TaskSystem.h"
#include "UIManager.h"

/**
 * @enum NPCType
 * @brief NPC 类型（铁匠/祭祀/守誓者/城主/将领/拾荒者）。
 */
enum class NPCType {
    BLACKSMITH, PRIEST, GUARDIAN, MAYOR, GENERAL, SCAVENGER
};

/**
 * @class NPC
 * @brief 带任务分歧的对话 NPC。
 */
class NPC {
private:
    std::string name;
    NPCType type;
    std::vector<std::string> firstMeetingDialogues;
    std::vector<std::string> taskIncompleteDialogues;
    std::vector<std::string> taskCompleteDialogues;
    std::vector<std::string> taskRewardedDialogues;
    std::string taskID;

public:
    /**
     * @brief 构造 NPC。
     */
    NPC(std::string name, NPCType type,
        std::vector<std::string> firstDialogues,
        std::vector<std::string> incompleteDialogues,
        std::vector<std::string> completeDialogues,
        std::vector<std::string> rewardedDialogues,
        std::string taskID = "");

    /** @brief 获取 NPC 名。 */
    std::string getName() const;
    /** @brief 获取 NPC 类型。 */
    NPCType getType() const;

    /**
     * @brief 根据任务状态显示对应对话并分页。
     * @param ui UI 管理器
     * @param status 任务状态
     * @param hasMetBefore 是否见过
     */
    void showDialogueByStatus(UIManager& ui, TaskStatus status, bool hasMetBefore) const;

    /** @brief 获取 NPC 关联任务ID（可能为空）。 */
    std::string getTaskID() const;
};

#endif // NPC_H