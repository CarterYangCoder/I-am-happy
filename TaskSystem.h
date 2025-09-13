#pragma once
#ifndef TASKSYSTEM_H
#define TASKSYSTEM_H

#include <string>
#include <vector>
#include <functional> // Task类使用了std::function，需要此头文件
#include "Item.h"   // Task类使用了Item*，需要此头文件
#include <optional> // TaskSystem类使用了std::optional，需要此头文件
class Player; // 前向声明Player类，避免循环依赖 
// 任务状态
enum class TaskStatus {
    UNACCEPTED, ACCEPTED, COMPLETED, REWARDED
};

// --- Task 类的声明没有变化 ---
class Task {
private:
    std::string id;
    std::string name;
    std::string description;
    int requiredLevel;
    std::function<bool(Player*)> completionCondition;
    std::vector<Item*> rewards;
    int expReward;
    int goldReward;
    TaskStatus status;

public:
    // 默认构造函数
    Task() : id(""), name(""), description(""), requiredLevel(1), 
             completionCondition(nullptr), rewards(), expReward(0), goldReward(0),
             status(TaskStatus::UNACCEPTED) {}
    
    Task(std::string id, std::string name, std::string desc, int level,
        std::function<bool(Player*)> condition, std::vector<Item*> rewards,
        int exp, int gold);

    void accept();
    bool checkCompletion(Player* player);
    void complete(Player* player);

    std::string getID() const;
    std::string getName() const;
    std::string getDescription() const;
    int getRequiredLevel() const;
    int getExpReward() const;
    int getGoldReward() const;
    std::string getId() const;
    TaskStatus getStatus() const;
    void setStatus(TaskStatus newStatus);

};


// 向前声明UIManager类，避免在头文件中引入整个UIManager.h
class UIManager;

class TaskSystem {
private:
    std::vector<Task*> allTasks;
    UIManager& ui; // 新增：UIManager的引用成员

public:
    TaskSystem(UIManager& uiManager); // 新增：构造函数声明
    ~TaskSystem();

    void initializeTasks(); // 新增：初始化函数声明
    void addTask(Task* task);
    void acceptTask(Player* player, std::string taskId); // 新增：接取任务函数
    void update(Player* player); // 新增：更新/检查任务进度函数
    void submitTask(Player* player, std::string taskId); // 新增：提交任务函数

    std::vector<Task*> getAvailableTasks(Player* player) const;
    std::vector<Task*> getAcceptedTasks() const;
    std::vector<Task*> getCompletedTasks() const;

    Task* findTask(std::string id) const;
    void showTaskList(Player* player) const;
    void updateTaskProgress(Player* player, const std::string& taskId);
    void showPlayerTasks(const Player& player) const;
    
    // 自动装备神器的辅助函数
    void autoEquipArtifact(Player* player, const std::string& taskId);

};
#endif // TASKSYSTEM_H