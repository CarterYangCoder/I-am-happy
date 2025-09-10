#pragma once
#ifndef NPC_H
#define NPC_H
#include <string>
#include <vector>
#include "TaskSystem.h"

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
    std::vector<std::string> dialogues;         // 对话内容
    std::string taskID;                         // 可发放的任务ID

public:
    NPC(std::string name, NPCType type, std::vector<std::string> dialogues, std::string taskID = "");

    // 基础信息
    std::string getName() const;
    NPCType getType() const;

    // 交互
    void showDialogue() const;                  // 显示对话
    std::string getTaskID() const;              // 获取可发放任务ID
};

#endif // NPC_H