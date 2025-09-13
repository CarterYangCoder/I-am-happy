#pragma once
#ifndef NPC_H
#define NPC_H
#include <string>
#include <vector>
#include "TaskSystem.h"
#include "UIManager.h"

// NPC类型（基于剧本中的角色）
enum class NPCType {
    BLACKSMITH,   // 铁匠（杨思睿） 
    PRIEST,       // 祭祀（晋津津）
    GUARDIAN,     // 守誓者（张焜杰）
    MAYOR,        // 城主（钟志炜）
    GENERAL,      // 将领（王浠珃）
    SCAVENGER     // 拾荒者（周洋迅）
};

class NPC {
private:
    std::string name;
    NPCType type;
    std::vector<std::string> firstMeetingDialogues;    // 首次见面对话
    std::vector<std::string> taskIncompleteDialogues;  // 任务未完成时的对话
    std::vector<std::string> taskCompleteDialogues;    // 任务完成时的对话
    std::vector<std::string> taskRewardedDialogues;    // 任务已领奖后的对话
    std::string taskID;                                // 可发放的任务ID

public:
    NPC(std::string name, NPCType type, 
        std::vector<std::string> firstDialogues,
        std::vector<std::string> incompleteDialogues,
        std::vector<std::string> completeDialogues,
        std::vector<std::string> rewardedDialogues,
        std::string taskID = "");

    // 基础信息
    std::string getName() const;
    NPCType getType() const;

    // 根据任务状态显示对应对话
    void showDialogueByStatus(UIManager& ui, TaskStatus status, bool hasMetBefore) const;
    std::string getTaskID() const;
};

#endif // NPC_H