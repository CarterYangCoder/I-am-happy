#include "TaskSystem.h"
#include <iostream>
#include <functional>
#include "UIManager.h"
#include "Player.h"
#include "Equipment.h"

// ----------------- Task 实现 -----------------
Task::Task(std::string id, std::string name, std::string desc, int level,
    std::function<bool(Player*)> condition, std::vector<Item*> rewards,
    int exp, int gold)
    : id(id), name(name), description(desc), requiredLevel(level),
      completionCondition(condition), rewards(rewards), expReward(exp),
      goldReward(gold), status(TaskStatus::UNACCEPTED) {}

void Task::accept() { if (status == TaskStatus::UNACCEPTED) status = TaskStatus::ACCEPTED; }

bool Task::checkCompletion(Player* player) {
    if (status == TaskStatus::ACCEPTED && completionCondition && completionCondition(player)) {
        status = TaskStatus::COMPLETED;
        return true;
    }
    return false;
}

void Task::complete(Player* player) {
    if (status != TaskStatus::COMPLETED) return;
    player->addExp(expReward);
    player->addGold(goldReward);
    for (Item* item : rewards) { if (item) player->addItem(*item); }
    status = TaskStatus::REWARDED;
}

std::string Task::getID() const { return id; }
std::string Task::getName() const { return name; }
std::string Task::getDescription() const { return description; }
TaskStatus Task::getStatus() const { return status; }
int Task::getRequiredLevel() const { return requiredLevel; }
int Task::getExpReward() const { return expReward; }
int Task::getGoldReward() const { return goldReward; }
std::string Task::getId() const { return id; }
void Task::setStatus(TaskStatus newStatus) { status = newStatus; }

// ----------------- TaskSystem 实现 -----------------
TaskSystem::TaskSystem(UIManager& uiManager) : ui(uiManager) {}

TaskSystem::~TaskSystem() {
    for (auto t : allTasks) delete t;
    allTasks.clear();
}

void TaskSystem::initializeTasks() {
    addTask(new Task(
        "1", "【寻黑曜晶尘】", "前往裂隙废墟，击败守卫的蚀骨恶狼，收集 3 份黑曜晶尘。", 1,
        [](Player* p){ auto &inv = p->getInventory(); auto it=inv.find("黑曜晶尘"); return it!=inv.end() && it->second>=3; },
        {},150,75));
    addTask(new Task(
        "2", "【解封三千忠魂】", "与晋津津对话，用王血解封三千忠魂，解除胸甲上的诅咒。", 1,
        [](Player* p){ auto &inv=p->getInventory(); auto it=inv.find("铁誓胸甲"); return it!=inv.end() && it->second>=1; },
        {},200,100));
    addTask(new Task(
        "3", "【戴上银戒】", "戴上明识之戒，准备破除玛尔索的幻象。", 10,
        [](Player* p){ auto &inv=p->getInventory(); auto it=inv.find("明识之戒"); return it!=inv.end() && it->second>=1; },
        {},100,50));
    addTask(new Task(
        "4", "【寻找怜悯之链】", "前往城外山脚下，寻找城主藏匿的怜悯之链。", 15,
        [](Player* p){ auto &inv=p->getInventory(); auto it=inv.find("怜悯之链"); return it!=inv.end() && it->second>=1; },
        {},250,150));
    addTask(new Task(
        "5", "【解救三军将领】", "进入静默尖塔迷宫，解救被困的王浠珃。", 20,
        [](Player* p){ auto &inv=p->getInventory(); auto it=inv.find("晨曦披风"); return it!=inv.end() && it->second>=1; },
        {},300,200));
    addTask(new Task(
        "6", "【找出创世战靴】", "在旧图书馆废墟中寻找创世战靴。", 25,
        [](Player* p){ auto &inv=p->getInventory(); auto it=inv.find("创世战靴"); return it!=inv.end() && it->second>=1; },
        {},400,250));
    ui.displayMessage("任务系统已初始化。", UIManager::Color::GRAY);
}

void TaskSystem::addTask(Task* task) { if (task) allTasks.push_back(task); }

Task* TaskSystem::findTask(std::string id) const {
    for (auto t : allTasks) if (t->getID() == id) return t;
    return nullptr; // 修复：原来错误地返回了 nullptr
}

void TaskSystem::acceptTask(Player* player, std::string taskId) {
    Task* task = findTask(taskId);
    if (!task) { ui.displayMessage("无效的任务ID。", UIManager::Color::RED); return; }
    if (task->getStatus() != TaskStatus::UNACCEPTED) { ui.displayMessage("该任务已接取或完成。", UIManager::Color::YELLOW); return; }
    if (player->getLevel() < task->getRequiredLevel()) { ui.displayMessage("等级不足，无法接取。", UIManager::Color::RED); return; }
    task->accept();
    Task copy = *task; copy.setStatus(TaskStatus::ACCEPTED); player->taskProgress[taskId] = copy;
    ui.displayMessage("接取任务: " + task->getName(), UIManager::Color::GREEN);
}

void TaskSystem::update(Player* player) {
    for (auto t : allTasks) {
        if (t->getStatus() == TaskStatus::ACCEPTED && t->checkCompletion(player)) {
            ui.displayMessage("任务 [" + t->getName() + "] 已完成！可提交。", UIManager::Color::GREEN);
            // 同步玩家副本状态
            if (player->taskProgress.count(t->getID())) {
                player->taskProgress[t->getID()].setStatus(TaskStatus::COMPLETED);
            }
        }
    }
}

void TaskSystem::submitTask(Player* player, std::string taskId) {
    Task* task = findTask(taskId);
    if (!task) {
        ui.displayMessage("任务不存在。", UIManager::Color::RED);
        return;
    }

    // 检查玩家是否有此任务的进度记录
    if (player->taskProgress.find(taskId) == player->taskProgress.end()) {
        ui.displayMessage("你还没有接取这个任务。", UIManager::Color::RED);
        return;
    }

    Task& playerTask = player->taskProgress[taskId];
    if (playerTask.getStatus() != TaskStatus::COMPLETED) {
        ui.displayMessage("任务尚未完成，无法提交。", UIManager::Color::RED);
        return;
    }

    // 任务1特殊处理：消耗黑曜晶尘材料
    if (taskId == "1") {
        int dustCount = player->getInventory().count("黑曜晶尘") ? player->getInventory().at("黑曜晶尘") : 0;
        if (dustCount < 3) {
            ui.displayMessage("黑曜晶尘不足！需要3份黑曜晶尘。", UIManager::Color::RED);
            return;
        }
        // 消耗材料
        for (int i = 0; i < 3; i++) {
            player->useItem("黑曜晶尘");
        }
        ui.displayMessage("杨思睿使用了3份黑曜晶尘修复皇冠！", UIManager::Color::CYAN);
    }

    // 任务2特殊处理：消耗铁誓胸甲
    if (taskId == "2") {
        int chestCount = player->getInventory().count("铁誓胸甲") ? player->getInventory().at("铁誓胸甲") : 0;
        if (chestCount < 1) {
            ui.displayMessage("没有铁誓胸甲进行解封仪式！", UIManager::Color::RED);
            return;
        }
        // 消耗胸甲（解封仪式中被净化改造）
        player->useItem("铁誓胸甲");
        ui.displayMessage("铁誓胸甲在解封仪式中被净化，重获新生！", UIManager::Color::CYAN);
    }

    // 其他任务的材料消耗
    if (taskId == "3") {
        int ringCount = player->getInventory().count("明识之戒") ? player->getInventory().at("明识之戒") : 0;
        if (ringCount < 1) {
            ui.displayMessage("没有明识之戒！", UIManager::Color::RED);
            return;
        }
        player->useItem("明识之戒");
        ui.displayMessage("明识之戒被激活，融入你的灵魂！", UIManager::Color::CYAN);
    }

    if (taskId == "4") {
        int chainCount = player->getInventory().count("怜悯之链") ? player->getInventory().at("怜悯之链") : 0;
        if (chainCount < 1) {
            ui.displayMessage("没有怜悯之链！", UIManager::Color::RED);
            return;
        }
        player->useItem("怜悯之链");
        ui.displayMessage("怜悯之链绽放出温暖的光芒，与你合为一体！", UIManager::Color::CYAN);
    }

    if (taskId == "5") {
        int capeCount = player->getInventory().count("晨曦披风") ? player->getInventory().at("晨曦披风") : 0;
        if (capeCount < 1) {
            ui.displayMessage("没有晨曦披风！", UIManager::Color::RED);
            return;
        }
        player->useItem("晨曦披风");
        ui.displayMessage("晨曦披风化作希望之光，笼罩在你的身上！", UIManager::Color::CYAN);
    }

    if (taskId == "6") {
        int bootsCount = player->getInventory().count("创世战靴") ? player->getInventory().at("创世战靴") : 0;
        if (bootsCount < 1) {
            ui.displayMessage("没有创世战靴！", UIManager::Color::RED);
            return;
        }
        player->useItem("创世战靴");
        ui.displayMessage("创世战靴与大地共鸣，成为你身体的一部分！", UIManager::Color::CYAN);
    }

    // 完成任务提交
    task->complete(player);
    playerTask.setStatus(TaskStatus::REWARDED);
    
    ui.displayMessage("任务「" + task->getName() + "」提交成功！", UIManager::Color::GREEN);
    ui.displayMessage("获得经验: " + std::to_string(task->getExpReward()), UIManager::Color::YELLOW);
    ui.displayMessage("获得金币: " + std::to_string(task->getGoldReward()), UIManager::Color::YELLOW);

    // 自动装备神器
    autoEquipArtifact(player, taskId);
}

void TaskSystem::showTaskList(Player* player) const {
    ui.displayMessage("--- 任务列表 ---", UIManager::Color::WHITE);
    bool anyAvail=false; bool anyAccept=false;
    for (auto t : allTasks) {
        if (t->getStatus()==TaskStatus::UNACCEPTED && player->getLevel() >= t->getRequiredLevel()) {
            ui.displayMessage("[可接取] (ID:"+t->getID()+") "+t->getName(), UIManager::Color::GREEN);
            anyAvail=true;
        }
    }
    if(!anyAvail) ui.displayMessage("无可接取任务。", UIManager::Color::GRAY);
    std::cout<<std::endl;
    for (auto t : allTasks) {
        if (t->getStatus()==TaskStatus::ACCEPTED) { ui.displayMessage("[进行中] "+t->getName(), UIManager::Color::YELLOW); anyAccept=true; }
        else if (t->getStatus()==TaskStatus::COMPLETED) { ui.displayMessage("[完成待提交] "+t->getName(), UIManager::Color::CYAN); anyAccept=true; }
    }
    if(!anyAccept) ui.displayMessage("暂无进行中任务。", UIManager::Color::GRAY);
    ui.displayMessage("--------------------", UIManager::Color::WHITE);
}

std::vector<Task*> TaskSystem::getAvailableTasks(Player* player) const {
    std::vector<Task*> v; for (auto t: allTasks) if (t->getStatus()==TaskStatus::UNACCEPTED && player->getLevel()>=t->getRequiredLevel()) v.push_back(t); return v; }
std::vector<Task*> TaskSystem::getAcceptedTasks() const { std::vector<Task*> v; for (auto t: allTasks) if (t->getStatus()==TaskStatus::ACCEPTED) v.push_back(t); return v; }
std::vector<Task*> TaskSystem::getCompletedTasks() const { std::vector<Task*> v; for (auto t: allTasks) if (t->getStatus()==TaskStatus::COMPLETED || t->getStatus()==TaskStatus::REWARDED) v.push_back(t); return v; }

void TaskSystem::updateTaskProgress(Player* player, const std::string& taskId) {
    Task* t = findTask(taskId); if (t) t->checkCompletion(player);
    for (auto each : allTasks) if (each->getStatus()==TaskStatus::ACCEPTED) each->checkCompletion(player);
}

void TaskSystem::showPlayerTasks(const Player& player) const {
    if (player.taskProgress.empty()) { ui.displayMessage("你没有已接任务。", UIManager::Color::GRAY); return; }
    ui.displayMessage("===== 你的任务列表 =====", UIManager::Color::CYAN);
    for (auto &kv : player.taskProgress) {
        const Task &t = kv.second; std::string tag;
        switch (t.getStatus()) {
            case TaskStatus::UNACCEPTED: tag="未接取"; break;
            case TaskStatus::ACCEPTED: tag="进行中"; break;
            case TaskStatus::COMPLETED: tag="可提交"; break;
            case TaskStatus::REWARDED: tag="已完成"; break;
        }
        ui.displayMessage("["+tag+"] "+t.getName()+" (ID:"+t.getID()+")", UIManager::Color::WHITE);
    }
    ui.displayMessage("=======================", UIManager::Color::CYAN);
}

void TaskSystem::autoEquipArtifact(Player* player, const std::string& taskId) {
    Equipment* artifact=nullptr;
    if (taskId=="1") artifact=new Equipment("自由誓约・破枷之冠", EquipmentPart::HELMET, "破除精神枷锁",10,5,"抵抗精神控制");
    else if (taskId=="2") artifact=new Equipment("忠诚誓约・铁誓胸甲", EquipmentPart::CHESTPLATE, "忠诚加护",15,10,"提升防御");
    else if (taskId=="3") artifact=new Equipment("真理誓约・明识之戒", EquipmentPart::RING, "明识真理",5,5,"提升暴击");
    else if (taskId=="4") artifact=new Equipment("怜悯誓约・抚伤之链", EquipmentPart::NECKLACE, "怜悯治愈",5,5,"提升回复");
    else if (taskId=="5") artifact=new Equipment("希望誓约・晨曦披风", EquipmentPart::CAPE, "希望加持",8,8,"提升速度");
    else if (taskId=="6") artifact=new Equipment("秩序誓约・创世战靴", EquipmentPart::BOOTS, "秩序创生",12,8,"提升移动");
    if (artifact) {
        player->equipSetPart(artifact);
        ui.displayMessage("获得并自动装备神器: "+artifact->getName(), UIManager::Color::YELLOW);
    }
}

