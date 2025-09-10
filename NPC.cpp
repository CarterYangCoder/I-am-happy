#include "NPC.h"
#include <iostream>

NPC::NPC(std::string name, NPCType type, std::vector<std::string> dialogues, std::string taskID)
    : name(name), type(type), dialogues(dialogues), taskID(taskID) {}

std::string NPC::getName() const { return name; }
NPCType NPC::getType() const { return type; }
 
// 显示对话
void NPC::showDialogue() const {
    for (const auto& line : dialogues) {
        std::cout << name << ": " << line << std::endl;
    }
}

// 获取可发放任务ID
std::string NPC::getTaskID() const { return taskID; }