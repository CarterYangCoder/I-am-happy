#include "TaskSystem.h"
#include <iostream>
#include <functional>
#include "UIManager.h"
#include "Player.h"
#include "Equipment.h"

// --------------------------------------------------------------------
// Task 类实现 
// --------------------------------------------------------------------
Task::Task(std::string id, std::string name, std::string desc, int level,
    std::function<bool(Player*)> condition, std::vector<Item*> rewards,
    int exp, int gold)
    : id(id), name(name), description(desc), requiredLevel(level),
    completionCondition(condition), rewards(rewards), expReward(exp),
    goldReward(gold), status(TaskStatus::UNACCEPTED) {}

void Task::accept() {
    if (status == TaskStatus::UNACCEPTED) {
        status = TaskStatus::ACCEPTED;
    }
}

bool Task::checkCompletion(Player* player) {
    if (status == TaskStatus::ACCEPTED && completionCondition(player)) {
        status = TaskStatus::COMPLETED;
        return true;
    }
    return false;
}

void Task::complete(Player* player) {
    if (status != TaskStatus::COMPLETED) return;

    player->addExp(expReward);
    player->addGold(goldReward);

    for (Item* item : rewards) {
        player->addItem(*item);
    }

    status = TaskStatus::REWARDED; // 标记为奖励已发放
}

// Getter 函数
std::string Task::getID() const { return id; }
std::string Task::getName() const { return name; }
std::string Task::getDescription() const { return description; }
TaskStatus Task::getStatus() const { return status; }
int Task::getRequiredLevel() const { return requiredLevel; }
int Task::getExpReward() const { return expReward; }
int Task::getGoldReward() const { return goldReward; }
std::string Task::getId() const { return id; }

void Task::setStatus(TaskStatus newStatus) {
    status = newStatus;
}

// --------------------------------------------------------------------
// TaskSystem 类实现
// --------------------------------------------------------------------
TaskSystem::TaskSystem(UIManager& uiManager) : ui(uiManager) {}

TaskSystem::~TaskSystem() {
    for (auto task : allTasks) {
        delete task;
    }
    allTasks.clear();
}

void TaskSystem::initializeTasks() {
    // 任务1: 寻黑曜晶尘 (杨思睿发布)
    addTask(new Task(
        "1", "【寻黑曜晶尘】", "前往裂隙废墟，击败守卫的蚀骨恶狼，收集 3 份黑曜晶尘。", 1,
        [](Player* player) -> bool {
            auto& inventory = player->getInventory();
            auto it = inventory.find("黑曜晶尘");
            return (it != inventory.end() && it->second >= 3);
        },
        {}, 150, 75
    ));

    // 任务2: 解封三千忠魂 (晋津津发布)
    addTask(new Task(
        "2", "【解封三千忠魂】", "用王血解封三千忠魂，解除胸甲上的诅咒。", 5,
        [](Player* player) -> bool {
            // 检查是否完成了解封仪式
            auto& inventory = player->getInventory();
            auto it = inventory.find("王血印记");
            return (it != inventory.end() && it->second >= 1);
        },
        {}, 200, 100
    ));

    // 任务3: 戴上银戒 (张焜杰发布)
    addTask(new Task(
        "3", "【戴上银戒】", "戴上明识之戒，准备破除玛尔索的幻象。", 10,
        [](Player* player) -> bool {
            // 任务完成条件：获得明识之戒
            auto& inventory = player->getInventory();
            auto it = inventory.find("明识之戒");
            return (it != inventory.end() && it->second >= 1);
        },
        {}, 100, 50
    ));

    // 任务4: 寻找怜悯之链 (钟志炜发布)
    addTask(new Task(
        "4", "【寻找怜悯之链】", "前往城外山脚下，寻找城主藏匿的怜悯之链。", 15,
        [](Player* player) -> bool {
            // 检查是否到达山脚并找到怜悯之链
            auto& inventory = player->getInventory();
            auto it = inventory.find("怜悯之链");
            return (it != inventory.end() && it->second >= 1);
        },
        {}, 250, 150
    ));

    // 任务5: 解救三军将领 (王浠珃发布)
    addTask(new Task(
        "5", "【解救三军将领】", "进入静默尖塔的迷宫，解救被困的三军将领王浠珃。", 20,
        [](Player* player) -> bool {
            // 检查是否成功解救并获得晨曦披风
            auto& inventory = player->getInventory();
            auto it = inventory.find("晨曦披风");
            return (it != inventory.end() && it->second >= 1);
        },
        {}, 300, 200
    ));

    // 任务6: 找出创世战靴 (周洋迅发布)
    addTask(new Task(
        "6", "【找出创世战靴】", "在旧图书馆废墟中寻找隐藏的创世战靴。", 25,
        [](Player* player) -> bool {
            // 检查是否获得创世战靴
            auto& inventory = player->getInventory();
            auto it = inventory.find("创世战靴");
            return (it != inventory.end() && it->second >= 1);
        },
        {}, 400, 250
    ));

    ui.displayMessage("任务系统已初始化。", UIManager::Color::GRAY);
}

void TaskSystem::addTask(Task* task) {
    if (task) {
        allTasks.push_back(task);
    }
}

Task* TaskSystem::findTask(std::string id) const {
    for (auto task : allTasks) {
        if (task->getID() == id) {
            return task;
        }
    }
    return nullptr;
}

void TaskSystem::acceptTask(Player* player, std::string taskId) {
    Task* task = findTask(taskId);
    if (!task) {
        ui.displayMessage("无效的任务ID。", UIManager::Color::RED);
        return;
    }
    if (task->getStatus() != TaskStatus::UNACCEPTED) {
        ui.displayMessage("你已经接取或完成了这个任务。", UIManager::Color::YELLOW);
        return;
    }
    if (player->getLevel() < task->getRequiredLevel()) {
        ui.displayMessage("你的等级不足，无法接取这个任务。", UIManager::Color::RED);
        return;
    }

    task->accept();
    // 将任务添加到玩家的任务进度中，并确保状态一致
    Task playerTask = *task;
    playerTask.setStatus(TaskStatus::ACCEPTED);
    player->taskProgress[taskId] = playerTask;
    ui.displayMessage("接取了新任务: " + task->getName(), UIManager::Color::GREEN);
}

void TaskSystem::update(Player* player) {
    for (auto task : allTasks) {
        if (task->getStatus() == TaskStatus::ACCEPTED) {
            if (task->checkCompletion(player)) {
                ui.displayMessage("任务材料已收集齐！请前往相关NPC提交任务换取装备。", UIManager::Color::YELLOW);
            }
        }
    }
}

void TaskSystem::submitTask(Player* player, std::string taskId) {
    Task* task = findTask(taskId);
    if (!task) {
        ui.displayMessage("无效的任务ID。", UIManager::Color::RED);
        return;
    }
    if (task->getStatus() == TaskStatus::COMPLETED) {
        // 先获取奖励信息
        std::string taskName = task->getName();
        int exp = task->getExpReward();
        int gold = task->getGoldReward();

        // 再让任务发放奖励
        task->complete(player);

        // 发放神器奖励并自动装备
        autoEquipArtifact(player, taskId);

        // 更新玩家任务进度中的状态
        if (player->taskProgress.count(taskId)) {
            player->taskProgress[taskId].setStatus(TaskStatus::REWARDED);
        }

        // 最后由TaskSystem来显示UI信息
        ui.displayMessage("任务 [" + taskName + "] 完成！", UIManager::Color::GREEN);
        if (exp > 0) ui.displayMessage("获得经验: " + std::to_string(exp), UIManager::Color::YELLOW);
        if (gold > 0) ui.displayMessage("获得金币: " + std::to_string(gold), UIManager::Color::YELLOW);

    }
    else if (task->getStatus() == TaskStatus::REWARDED) {
        ui.displayMessage("你已经领取过这个任务的奖励了。", UIManager::Color::YELLOW);
    }
    else {
        ui.displayMessage("该任务还未达到完成条件。", UIManager::Color::YELLOW);
    }
}

void TaskSystem::showTaskList(Player* player) const {
    ui.displayMessage("--- 任务列表 ---", UIManager::Color::WHITE);

    bool hasAvailable = false;
    for (auto task : allTasks) {
        // 检查玩家任务进度中的状态
        TaskStatus currentStatus = task->getStatus();
        if (player->taskProgress.count(task->getID())) {
            currentStatus = player->taskProgress.at(task->getID()).getStatus();
        }
        
        if (currentStatus == TaskStatus::UNACCEPTED && player->getLevel() >= task->getRequiredLevel()) {
            ui.displayMessage("[" + task->getID() + "] " + task->getName() + " (等级要求: " + 
                              std::to_string(task->getRequiredLevel()) + ")", UIManager::Color::GREEN);
            ui.displayMessage("  " + task->getDescription(), UIManager::Color::GRAY);
            hasAvailable = true;
        }
    }
    if (!hasAvailable) {
        ui.displayMessage("没有可接取的任务。", UIManager::Color::GRAY);
    }

    std::cout << std::endl;

    bool hasAccepted = false;
    for (auto task : allTasks) {
        // 检查玩家任务进度中的状态
        TaskStatus currentStatus = task->getStatus();
        if (player->taskProgress.count(task->getID())) {
            currentStatus = player->taskProgress.at(task->getID()).getStatus();
        }
        
        if (currentStatus == TaskStatus::ACCEPTED) {
            ui.displayMessage("[" + task->getID() + "] " + task->getName() + " (进行中)", UIManager::Color::YELLOW);
            hasAccepted = true;
        }
        else if (currentStatus == TaskStatus::COMPLETED) {
            ui.displayMessage("[" + task->getID() + "] " + task->getName() + " (已完成，可提交)", UIManager::Color::GREEN);
            hasAccepted = true;
        }
    }
    if (!hasAccepted) {
        ui.displayMessage("没有正在进行的任务。", UIManager::Color::GRAY);
    }
    ui.displayMessage("--------------------", UIManager::Color::WHITE);
}

std::vector<Task*> TaskSystem::getAvailableTasks(Player* player) const {
    std::vector<Task*> result;
    for (auto task : allTasks) {
        if (task->getStatus() == TaskStatus::UNACCEPTED &&
            player->getLevel() >= task->getRequiredLevel()) {
            result.push_back(task);
        }
    }
    return result;
}

std::vector<Task*> TaskSystem::getAcceptedTasks() const {
    std::vector<Task*> result;
    for (auto task : allTasks) {
        if (task->getStatus() == TaskStatus::ACCEPTED) {
            result.push_back(task);
        }
    }
    return result;
}

std::vector<Task*> TaskSystem::getCompletedTasks() const {
    std::vector<Task*> result;
    for (auto task : allTasks) {
        if (task->getStatus() == TaskStatus::COMPLETED) {
            result.push_back(task);
        }
    }
    return result;
}

void TaskSystem::updateTaskProgress(Player* player, const std::string& taskId) {
    // 查找任务是否存在
    Task* task = findTask(taskId);
    if (task && player) {
        // 检查任务完成条件
        task->checkCompletion(player);
        if (task->getStatus() == TaskStatus::COMPLETED) {
            ui.displayMessage("任务完成：" + task->getName(), UIManager::Color::GREEN);
        }
    }
    
    // 同时处理基于行动的任务更新（比如"击败XX"）
    for (auto task : allTasks) {
        if (task->getStatus() == TaskStatus::ACCEPTED) {
            task->checkCompletion(player);
        }
    }
}

void TaskSystem::showPlayerTasks(const Player& player) const {
    // 获取玩家的任务进度
    const auto& playerTasks = player.taskProgress;

    if (playerTasks.empty()) {
        ui.displayMessage("你目前没有任何任务。", UIManager::Color::GRAY);
        return;
    }

    ui.displayMessage("===== 你的任务列表 =====", UIManager::Color::CYAN);
    for (const auto& pair : playerTasks) {
        const Task& task = pair.second;
        switch (task.getStatus()) {
        case TaskStatus::UNACCEPTED:
            ui.displayMessage("[" + task.getId() + "] " + task.getName() + " (未接受)", UIManager::Color::GRAY);
            break;
        case TaskStatus::ACCEPTED:
            ui.displayMessage("[" + task.getId() + "] " + task.getName() + " (进行中)", UIManager::Color::YELLOW);
            break;
        case TaskStatus::COMPLETED:
            ui.displayMessage("[" + task.getId() + "] " + task.getName() + " (已完成，可提交)", UIManager::Color::GREEN);
            break;
        case TaskStatus::REWARDED:
            ui.displayMessage("[" + task.getId() + "] " + task.getName() + " (已完成)", UIManager::Color::BLUE);
            break;
        }
    }
    ui.displayMessage("=======================", UIManager::Color::CYAN);
}

// 自动装备神器的辅助函数
void TaskSystem::autoEquipArtifact(Player* player, const std::string& taskId) {
    Equipment* artifact = nullptr;
    
    // 根据任务ID决定获得的神器
    if (taskId == "1") {
        // 任务1: 【寻黑曜晶尘】 -> 自由誓约・破枷之冠
        artifact = new Equipment("自由誓约・破枷之冠", EquipmentPart::HELMET, 
                                "破除权力奴役的枷锁，赋予佩戴者抵抗精神控制的能力", 15, 20, "抵抗精神控制");
        ui.displayMessage("获得神器：【自由誓约・破枷之冠】", UIManager::Color::MAGENTA);
        ui.displayMessage("神器已自动装备！", UIManager::Color::GREEN);
    }
    else if (taskId == "2") {
        // 任务2: 【解封三千忠魂】 -> 忠诚誓约・铁誓胸甲
        artifact = new Equipment("忠诚誓约・铁誓胸甲", EquipmentPart::CHESTPLATE,
                                "以契约之力凝聚圣钢，象征永不背叛的守护", 20, 30, "永不背叛");
        ui.displayMessage("获得神器：【忠诚誓约・铁誓胸甲】", UIManager::Color::MAGENTA);
        ui.displayMessage("神器已自动装备！", UIManager::Color::GREEN);
    }
    else if (taskId == "3") {
        // 任务3: 【戴上银戒】 -> 真理誓约・明识之戒
        artifact = new Equipment("真理誓约・明识之戒", EquipmentPart::RING,
                                "可识破幻象、追溯言语真实", 10, 15, "识破幻象");
        ui.displayMessage("获得神器：【真理誓约・明识之戒】", UIManager::Color::MAGENTA);
        ui.displayMessage("神器已自动装备！", UIManager::Color::GREEN);
    }
    else if (taskId == "4") {
        // 任务4: 【寻找怜悯之链】 -> 怜悯誓约・抚伤之链
        artifact = new Equipment("怜悯誓约・抚伤之链", EquipmentPart::NECKLACE,
                                "以治愈之力对抗伤害，化痛苦为生机", 12, 25, "治愈之力");
        ui.displayMessage("获得神器：【怜悯誓约・抚伤之链】", UIManager::Color::MAGENTA);
        ui.displayMessage("神器已自动装备！", UIManager::Color::GREEN);
    }
    else if (taskId == "5") {
        // 任务5: 【解救三军将领】 -> 希望誓约・晨曦披风
        artifact = new Equipment("希望誓约・晨曦披风", EquipmentPart::CAPE,
                                "以光与生命之火点燃绝望，驱散麻木与虚无", 18, 22, "点燃希望");
        ui.displayMessage("获得神器：【希望誓约・晨曦披风】", UIManager::Color::MAGENTA);
        ui.displayMessage("神器已自动装备！", UIManager::Color::GREEN);
    }
    else if (taskId == "6") {
        // 任务6: 【找出创世战靴】 -> 秩序誓约・创世战靴
        artifact = new Equipment("秩序誓约・创世战靴", EquipmentPart::BOOTS,
                                "以星轨之力稳固世界根基，阻止无序毁灭", 25, 18, "星轨之力");
        ui.displayMessage("获得神器：【秩序誓约・创世战靴】", UIManager::Color::MAGENTA);
        ui.displayMessage("神器已自动装备！", UIManager::Color::GREEN);
    }
    
    // 如果获得了神器，自动装备
    if (artifact) {
        player->equipSetPart(artifact);
        
        // 检查是否集齐全套神器
        if (player->hasAllSetParts()) {
            ui.displayMessage("恭喜！你已集齐六誓圣辉救赎套装！", UIManager::Color::CYAN);
            ui.displayMessage("解锁终极技能：【星闪流河圣龙飞升·神界湮灭斩·最终式】", UIManager::Color::MAGENTA);
            player->unlockSkill(SkillType::ULTIMATE_SLAY);
        }
    }
}

