#include "NPC.h"
#include <iostream>

// NPC 类构造函数
NPC::NPC(std::string name, NPCType type, 
         std::vector<std::string> firstDialogues,
         std::vector<std::string> incompleteDialogues,
         std::vector<std::string> completeDialogues,
         std::vector<std::string> rewardedDialogues,
         std::string taskID)
    : name(name), type(type), 
      firstMeetingDialogues(firstDialogues),
      taskIncompleteDialogues(incompleteDialogues),
      taskCompleteDialogues(completeDialogues),
      taskRewardedDialogues(rewardedDialogues),
      taskID(taskID) {}

// 获取 NPC 名称
std::string NPC::getName() const { return name; }

// 获取 NPC 类型
NPCType NPC::getType() const { return type; }

// 根据任务状态显示对应对话
void NPC::showDialogueByStatus(UIManager& ui, TaskStatus status, bool hasMetBefore) const {
    auto starts_with = [](const std::string& s, const std::string& prefix) {
        return s.rfind(prefix, 0) == 0;
    };

    std::vector<std::string> currentDialogues;

    // 根据状态选择对话内容
    if (!hasMetBefore) {
        currentDialogues = firstMeetingDialogues;
    } else {
        switch (status) {
        case TaskStatus::UNACCEPTED:
            currentDialogues = firstMeetingDialogues; // 未接取时重复首次对话
            break;
        case TaskStatus::ACCEPTED:
            currentDialogues = taskIncompleteDialogues;
            break;
        case TaskStatus::COMPLETED:
            currentDialogues = taskCompleteDialogues;
            break;
        case TaskStatus::REWARDED:
            currentDialogues = taskRewardedDialogues;
            break;
        }
    }

    // 显示对话
    for (size_t idx = 0; idx < currentDialogues.size(); ++idx) {
        const auto& line = currentDialogues[idx];
        bool isPlayer = starts_with(line, "你：") || starts_with(line, "安特王子：") || starts_with(line, "Player:");
        bool isNarration =
            starts_with(line, "旁白：") ||
            starts_with(line, "系统提示") ||
            starts_with(line, "触发任务") ||
            starts_with(line, "（") ||
            (!line.empty() && (line.front() == '(' || line.front() == '['));

        if (isPlayer || isNarration) {
            ui.displayMessage(line, UIManager::Color::WHITE);
        } else {
            ui.displayMessage(name + ": " + line, UIManager::Color::WHITE);
        }

        if (idx + 1 < currentDialogues.size()) {
            ui.pause();
        }
    }
}

// 获取可发放任务ID
std::string NPC::getTaskID() const { return taskID; }