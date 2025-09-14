#include "SaveLoadSystem.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <regex>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

bool SaveLoadSystem::is_digits_save(const std::string& str)
{
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

std::string SaveLoadSystem::timeToString(time_t time)
{
    std::tm tm_buf;
#ifdef _WIN32
    localtime_s(&tm_buf, &time);
#else
    localtime_r(&time, &tm_buf);
#endif
    std::stringstream ss;
    ss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

SaveLoadSystem::SaveLoadSystem(UIManager& uiManager) : ui(uiManager)
{
    try {
        // 确保存档目录存在，如果失败则使用当前目录
        if (!std::filesystem::exists(SAVE_DIR))
        {
            std::error_code ec;
            if (!std::filesystem::create_directory(SAVE_DIR, ec)) {
                // 如果创建失败，使用当前目录
                ui.displayMessage("警告：无法创建存档目录，将使用当前目录保存", UIManager::Color::YELLOW);
                const_cast<std::string&>(SAVE_DIR) = "./";
            }
        }
    } catch (const std::exception& e) {
        ui.displayMessage("存档系统初始化警告: " + std::string(e.what()), UIManager::Color::YELLOW);
        const_cast<std::string&>(SAVE_DIR) = "./";
    }
}

std::vector<SaveSlot> SaveLoadSystem::listSaveSlots()
{
    std::vector<SaveSlot> slots;
    
    try {
        const std::regex file_regex("save_slot_(\\d+)_(\\d{14})\\.sav");

        for (const auto& entry : std::filesystem::directory_iterator(SAVE_DIR))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().filename().string();
                std::smatch match;
                if (std::regex_match(filename, match, file_regex))
                {
                    SaveSlot slot;
                    slot.id = std::stoi(match[1].str());
                    slot.filename = filename;

                    std::tm tm = {};
                    std::stringstream ss(match[2].str());
                    ss >> std::get_time(&tm, "%Y%m%d%H%M%S");
                    slot.timestamp = mktime(&tm);

                    try {
                        std::ifstream file(entry.path().string());
                        if (file.is_open())
                        {
                            std::string line, key;
                            if (std::getline(file, line))
                            {
                                std::stringstream meta_ss(line);
                                std::string meta_key;
                                meta_ss >> meta_key >> slot.playerName >> slot.playerLevel;
                                if (meta_key == "META") {
                                    slots.push_back(slot);
                                }
                            }
                            file.close();
                        }
                    } catch (const std::exception& e) {
                        ui.displayMessage("读取存档文件失败: " + filename, UIManager::Color::RED);
                        continue;
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        ui.displayMessage("扫描存档目录失败: " + std::string(e.what()), UIManager::Color::RED);
    }
    
    return slots;
}

void SaveLoadSystem::saveGame(const Player& player, const TaskSystem& taskProgress)
{
    try {
        auto slots = listSaveSlots();
        int slotToUse = -1;

        if (slots.size() < MAX_SAVES)
        {
            for (int i = 0; i < MAX_SAVES; ++i)
            {
                bool is_used = false;
                for (const auto& s : slots)
                {
                    if (s.id == i)
                    {
                        is_used = true;
                        break;
                    }
                }
                if (!is_used)
                {
                    slotToUse = i;
                    break;
                }
            }
        }
        else
        {
            ui.displayMessage("存档已满！请选择一个要覆盖的存档，或输入 'c' 取消。", UIManager::Color::YELLOW);
            for (const auto& slot : slots)
            {
                std::string text = "[" + std::to_string(slot.id) + "] " + slot.playerName +
                    " - 等级 " + std::to_string(slot.playerLevel) +
                    " (存档时间: " + timeToString(slot.timestamp) + ")";
                ui.displayMessage(text, UIManager::Color::WHITE);
            }

            while (true)
            {
                std::cout << "请输入要覆盖的存档槽位ID (0-9) 或 'c' 取消: ";
                std::string choice_str;
                std::cin >> choice_str;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (choice_str == "c" || choice_str == "C")
                {
                    ui.displayMessage("保存已取消。", UIManager::Color::GRAY);
                    return;
                }
                if (is_digits_save(choice_str))
                {
                    int choice_id = std::stoi(choice_str);
                    if (choice_id >= 0 && choice_id < MAX_SAVES)
                    {
                        slotToUse = choice_id;
                        break;
                    }
                }
                std::cout << "无效输入，请重试。" << std::endl;
            }
        }

        // 删除旧存档
        for (const auto& slot : slots)
        {
            if (slot.id == slotToUse)
            {
                try {
                    std::filesystem::remove(std::filesystem::path(SAVE_DIR) / slot.filename);
                } catch (const std::exception& e) {
                    ui.displayMessage("删除旧存档失败，但会继续保存新存档", UIManager::Color::YELLOW);
                }
                break;
            }
        }

        // 创建新存档文件名
        time_t now = time(nullptr);
        std::tm tm_buf;
#ifdef _WIN32
        localtime_s(&tm_buf, &now);
#else
        localtime_r(&now, &tm_buf);
#endif
        std::stringstream filename_ss;
        filename_ss << "save_slot_" << slotToUse << "_" << std::put_time(&tm_buf, "%Y%m%d%H%M%S") << ".sav";
        std::string filename = filename_ss.str();

        // 保存文件
        std::string fullPath = (std::filesystem::path(SAVE_DIR) / filename).string();
        std::ofstream saveFile(fullPath);
        if (!saveFile)
        {
            ui.displayMessage("错误: 无法创建存档文件 " + filename, UIManager::Color::RED);
            ui.displayMessage("请检查目录权限或磁盘空间", UIManager::Color::RED);
            return;
        }

        // 写入存档数据
        saveFile << "META " << player.getName() << " " << player.getLevel() << std::endl;
        saveFile << "Level " << player.getLevel() << std::endl;
        saveFile << "Hp " << player.getHP() << std::endl;
        saveFile << "MaxHp " << player.getMaxHP() << std::endl;
        saveFile << "Attack " << player.getATK() << std::endl;
        saveFile << "Defense " << player.getDEF() << std::endl;
        saveFile << "Speed " << player.getSpeed() << std::endl;
        saveFile << "Experience " << player.getExp() << std::endl;
        saveFile << "Gold " << player.getGold() << std::endl;
        saveFile << "CritRate " << player.getCritRate() << std::endl;
        saveFile << "CurrentRoom " << player.getCurrentRoomId() << std::endl;

        // 保存装备神器
        const auto& equippedItems = player.getAllEquippedItems();
        for (const auto& pair : equippedItems) {
            if (pair.second && pair.first != EquipmentPart::SWORD) { // 神剑默认装备，不需要保存
                saveFile << "Equipment " << static_cast<int>(pair.first) << " " << pair.second->getName() << std::endl;
            }
        }

        // 保存背包道具
        const auto& inventory = player.getInventory();
        for (const auto& item : inventory)
        {
            saveFile << "Item " << item.first << " " << item.second << std::endl;
        }

        for (const auto& pair : player.taskProgress)
        {
            const Task& task = pair.second;
            saveFile << "Task " << task.getId() << " " << static_cast<int>(task.getStatus()) << std::endl;
        }

        // 新增：保存“已对话任务”标记
        for (const auto& tid : player.getTalkedTasks()) {
            saveFile << "Talked " << tid << std::endl;
        }

        saveFile.close();
        
        if (saveFile.good()) {
            ui.displayMessage("游戏已成功保存到槽位 " + std::to_string(slotToUse), UIManager::Color::GREEN);
        } else {
            ui.displayMessage("存档可能未完整保存，请重试", UIManager::Color::YELLOW);
        }
    } catch (const std::exception& e) {
        ui.displayMessage("保存游戏时发生错误: " + std::string(e.what()), UIManager::Color::RED);
    } catch (...) {
        ui.displayMessage("保存游戏时发生未知错误", UIManager::Color::RED);
    }
}

void SaveLoadSystem::autoSaveGame(const Player& player, const TaskSystem& taskProgress)
{
    // 删除旧的自动存档
    for (const auto& entry : std::filesystem::directory_iterator(SAVE_DIR))
    {
        if (entry.is_regular_file())
        {
            std::string filename = entry.path().filename().string();
            if (filename.find("auto_save_") == 0)
            {
                std::filesystem::remove(entry.path());
            }
        }
    }

    // 创建新的自动存档
    time_t now = time(nullptr);
    std::tm tm_buf;
#ifdef _WIN32
    localtime_s(&tm_buf, &now);
#else
    localtime_r(&now, &tm_buf);
#endif
    std::stringstream filename_ss;
    filename_ss << "auto_save_" << std::put_time(&tm_buf, "%Y%m%d%H%M%S") << ".sav";
    std::string filename = filename_ss.str();

    std::ofstream saveFile((std::filesystem::path(SAVE_DIR) / filename).string());
    if (!saveFile)
    {
        ui.displayMessage("错误: 无法创建自动存档文件 " + filename, UIManager::Color::RED);
        return;
    }

    saveFile << "META " << player.getName() << " " << player.getLevel() << std::endl;
    saveFile << "Level " << player.getLevel() << std::endl;
    saveFile << "Hp " << player.getHP() << std::endl;
    saveFile << "MaxHp " << player.getMaxHP() << std::endl;
    saveFile << "Attack " << player.getATK() << std::endl;
    saveFile << "Defense " << player.getDEF() << std::endl;
    saveFile << "Speed " << player.getSpeed() << std::endl;
    saveFile << "Experience " << player.getExp() << std::endl;
    saveFile << "Gold " << player.getGold() << std::endl;
    saveFile << "CritRate " << player.getCritRate() << std::endl;
    saveFile << "CurrentRoom " << player.getCurrentRoomId() << std::endl;

    // 保存装备神器
    const auto& equippedItems = player.getAllEquippedItems();
    for (const auto& pair : equippedItems) {
        if (pair.second && pair.first != EquipmentPart::SWORD) { // 神剑默认装备，不需要保存
            saveFile << "Equipment " << static_cast<int>(pair.first) << " " << pair.second->getName() << std::endl;
        }
    }

    // 保存背包道具
    const auto& inventory = player.getInventory();
    for (const auto& item : inventory)
    {
        saveFile << "Item " << item.first << " " << item.second << std::endl;
    }

    for (const auto& pair : player.taskProgress)
    {
        const Task& task = pair.second;
        saveFile << "Task " << task.getId() << " " << static_cast<int>(task.getStatus()) << std::endl;
    }

    saveFile.close();
    ui.displayMessage("游戏已自动保存", UIManager::Color::BLUE);
}

bool SaveLoadSystem::loadGame(Player& player, TaskSystem& taskProgress)
{
    try {
        auto slots = listSaveSlots();
        
        // 检查是否有自动存档
        std::vector<std::string> autoSaves;
        try {
            for (const auto& entry : std::filesystem::directory_iterator(SAVE_DIR))
            {
                if (entry.is_regular_file())
                {
                    std::string filename = entry.path().filename().string();
                    if (filename.find("auto_save_") == 0)
                    {
                        autoSaves.push_back(filename);
                    }
                }
            }
        } catch (const std::exception& e) {
            ui.displayMessage("扫描自动存档失败: " + std::string(e.what()), UIManager::Color::YELLOW);
        }
        
        if (slots.empty() && autoSaves.empty())
        {
            ui.displayMessage("没有可用的存档。", UIManager::Color::YELLOW);
            return false;
        }

        ui.displayMessage("请选择要加载的存档:", UIManager::Color::CYAN);
        
        // 显示自动存档选项
        if (!autoSaves.empty())
        {
            ui.displayMessage("[auto] 自动存档", UIManager::Color::BLUE);
        }
        
        // 显示手动存档
        for (const auto& slot : slots)
        {
            std::string text = "[" + std::to_string(slot.id) + "] " + slot.playerName +
                " - 等级 " + std::to_string(slot.playerLevel) +
                " (存档时间: " + timeToString(slot.timestamp) + ")";
            ui.displayMessage(text, UIManager::Color::WHITE);
        }

        int choice_id = -1;
        std::string filename_to_load;

        while (true)
        {
            std::cout << "请输入存档槽位ID、'auto'(自动存档) 或 'c'(取消): ";
            std::string choice_str;
            std::cin >> choice_str;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (choice_str == "c" || choice_str == "C")
            {
                ui.displayMessage("读档已取消。", UIManager::Color::GRAY);
                return false;
            }
            
            if (choice_str == "auto" && !autoSaves.empty())
            {
                // 选择最新的自动存档
                filename_to_load = autoSaves[0]; // 假设第一个是最新的
                break;
            }

            if (is_digits_save(choice_str))
            {
                choice_id = std::stoi(choice_str);
                bool valid = false;
                for (const auto& slot : slots)
                {
                    if (slot.id == choice_id)
                    {
                        filename_to_load = slot.filename;
                        valid = true;
                        break;
                    }
                }
                if (valid)
                    break;
            }
            std::cout << "无效输入, 请重新输入。" << std::endl;
        }

        std::string fullPath = (std::filesystem::path(SAVE_DIR) / filename_to_load).string();
        std::ifstream loadFile(fullPath);
        if (!loadFile.is_open())
        {
            ui.displayMessage("错误：无法打开存档文件 " + filename_to_load, UIManager::Color::RED);
            return false;
        }

        // 完全重置玩家数据，确保存档独立性
        player.taskProgress.clear();
        player.clearInventory();
        
        // 重置玩家属性到初始状态
        player.setName("安特王子");
        player.setLevel(1);
        player.setHP(100);
        player.setMaxHP(100);
        player.setATK(10);
        player.setDEF(5);
        player.setSpeed(5);
        player.setExp(0);
        player.setGold(0);
        player.setCritRate(0.05f);
        player.setMP(50);
        player.setMaxMP(50);
        player.setCurrentRoomId(1);

        // 读取存档数据
        std::string dummy;
        std::getline(loadFile, dummy);
        std::stringstream meta_ss(dummy);
        std::string meta_key;
        std::string player_name;
        meta_ss >> meta_key >> player_name;
        if (meta_key == "META") {
            player.setName(player_name);
        }
        
        std::string key;
        while (loadFile >> key)
        {
            if (key == "Level") {
                int level;
                loadFile >> level;
                player.setLevel(level);
            }
            else if (key == "Hp") {
                int hp;
                loadFile >> hp;
                player.setHP(hp);
            }
            else if (key == "MaxHp") {
                int maxHp;
                loadFile >> maxHp;
                player.setMaxHP(maxHp);
            }
            else if (key == "Attack") {
                int atk;
                loadFile >> atk;
                player.setATK(atk);
            }
            else if (key == "Defense") {
                int def;
                loadFile >> def;
                player.setDEF(def);
            }
            else if (key == "Speed") {
                int speed;
                loadFile >> speed;
                player.setSpeed(speed);
            }
            else if (key == "Experience") {
                int exp;
                loadFile >> exp;
                player.setExp(exp);
            }
            else if (key == "Gold") {
                int gold;
                loadFile >> gold;
                player.setGold(gold);
            }
            else if (key == "CritRate") {
                float critRate;
                loadFile >> critRate;
                player.setCritRate(critRate);
            }
            else if (key == "CurrentRoom") {
                int roomId;
                loadFile >> roomId;
                player.setCurrentRoomId(roomId);
            }
            else if (key == "Item") {
                std::string itemName;
                int quantity;
                loadFile >> itemName >> quantity;
                player.addItemByName(itemName, quantity);
            }
            else if (key == "Task") {
                std::string taskId;
                int statusInt;
                loadFile >> taskId >> statusInt;
                TaskStatus status = static_cast<TaskStatus>(statusInt);
                
                // 重要修复：正确恢复任务状态
                Task* originalTask = taskProgress.findTask(taskId);
                if (originalTask) {
                    // 创建任务副本并设置正确状态
                    Task taskCopy = *originalTask;
                    taskCopy.setStatus(status);
                    player.taskProgress[taskId] = taskCopy;
                    
                    // 同步任务系统中的任务状态
                    originalTask->setStatus(status);
                }
            }
            else if (key == "Equipment") {
                int partInt;
                std::string equipName;
                loadFile >> partInt;
                loadFile.ignore();
                std::getline(loadFile, equipName);
                
                // 重新装备神器
                EquipmentPart part = static_cast<EquipmentPart>(partInt);
                Equipment* equipment = createEquipmentByName(equipName);
                
                if (equipment) {
                    player.equipSetPart(equipment);
                }
            }
            else if (key == "Talked") {
                // 新增：读取“已对话任务”标记
                std::string tid;
                loadFile >> tid;
                player.markTalkedToNpc(tid);
            }
        }

        loadFile.close();
        
        // 读档后检测并添加技能
        postLoadSkillCheck(player);
        
        if (choice_id != -1) {
            ui.displayMessage("游戏已从槽位 " + std::to_string(choice_id) + " 成功加载!", UIManager::Color::GREEN);
        } else {
            ui.displayMessage("游戏已从自动存档成功加载!", UIManager::Color::GREEN);
        }
        
        return true;
    } catch (const std::exception& e) {
        ui.displayMessage("加载游戏时发生错误: " + std::string(e.what()), UIManager::Color::RED);
        return false;
    } catch (...) {
        ui.displayMessage("加载游戏时发生未知错误", UIManager::Color::RED);
        return false;
    }
}

// 在读档完成后，根据等级和装备状态检测并解锁技能
// 在读档完成后，根据等级和装备状态检测并解锁技能
void SaveLoadSystem::postLoadSkillCheck(Player& player) {
    // 检查并解锁符合等级要求的技能
    player.checkAndUnlockSkills();
    
    // 显示技能恢复状态
    const auto& skills = player.getSkills();
    if (!skills.empty()) {
        ui.displayMessage("已根据等级和装备状态恢复技能:", UIManager::Color::CYAN);
        for (const auto& skill : skills) {
            ui.displayMessage("- " + skill->getName(), UIManager::Color::WHITE);
        }
    }
    
    // 如果集齐套装，显示特殊提示
    if (player.hasAllSetParts()) {
        ui.displayMessage("检测到已集齐六誓圣辉套装！", UIManager::Color::MAGENTA);
        if (player.getLevel() >= 50 && player.getSkill(SkillType::ULTIMATE_SLAY)) {
            ui.displayMessage("终极技能已可用！", UIManager::Color::MAGENTA);
        }
    }
}

// 装备重建辅助函数
Equipment* SaveLoadSystem::createEquipmentByName(const std::string& equipmentName) {
    if (equipmentName == "自由誓约・破枷之冠") {
        return new Equipment(equipmentName, EquipmentPart::HELMET, "抵抗精神控制", 15, 10, "免疫魅惑");
    }
    else if (equipmentName == "忠诚誓约・铁誓胸甲") {
        return new Equipment(equipmentName, EquipmentPart::CHESTPLATE, "坚固的胸甲", 20, 25, "减少背叛伤害");
    }
    else if (equipmentName == "真理誓约・明识之戒") {
        return new Equipment(equipmentName, EquipmentPart::RING, "看清真相", 10, 5, "识破幻象");
    }
    else if (equipmentName == "怜悯誓约・抚伤之链") {
        return new Equipment(equipmentName, EquipmentPart::NECKLACE, "治愈之链", 5, 15, "战斗回血");
    }
    else if (equipmentName == "希望誓约・晨曦披风") {
        return new Equipment(equipmentName, EquipmentPart::CAPE, "带来希望", 25, 20, "提升士气");
    }
    else if (equipmentName == "秩序誓约・创世战靴") {
        return new Equipment(equipmentName, EquipmentPart::BOOTS, "维护秩序", 30, 15, "移动加速");
    }
    return nullptr;
}