#include "Map.h"
#include "BossWanEshuji.h"
#include "SaveLoadSystem.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <cctype>

// 辅助函数：判断字符串是否全为数字
static bool is_digits_game(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

// 构造函数
Map::Map() {
    initRooms();
    initNPCs(); 
    initBosses();
    initEnemies();
    currentRoomId = 1; // 初始位置：迷雾森林
}

// 初始化所有房间
void Map::initRooms() {
    // 初始区域
    Room forest(1, "迷雾森林", "古老的森林，参天大树遮天蔽日，藤蔓掩盖着路径",
        "东北部似乎有火光，或许是可以落脚的地方");
    forest.addExit("NE", 2, "铁匠铺");
    rooms.insert({ 1, forest });

    Room blacksmith(2, "铁砧铁匠铺", "炉火熊熊的铁匠铺，墙上挂着王室徽记",
        "东边有通往裂隙废墟的小径，北边是黑曜权枢殿");
    blacksmith.addExit("SW", 1, "迷雾森林");
    blacksmith.addExit("E", 3, "裂隙废墟");
    blacksmith.addExit("N", 4, "黑曜权枢殿");
    blacksmith.addNPC("杨思睿（白发铁匠）");
    blacksmith.addItem("破枷之冠蓝图");
    rooms.insert({ 2, blacksmith });

    Room rift(3, "裂隙废墟", "泛着幽蓝微光的古代废墟，地面散落着黑曜晶尘",
        "击败守卫的蚀骨恶狼后才能收集黑曜晶尘");
    rift.addExit("W", 2, "铁砧铁匠铺");
    rift.addExit("NE", 4, "黑曜权枢殿");
    // 初始不再直接放置黑曜晶尘，需击败狼后刷新
    rooms.insert({ 3, rift });

    Room obsidian(4, "黑曜权枢殿", "黑曜石建造的宏伟殿堂，中央有巨大王座",
        "击败厄休拉后可向东进入背契之坛");
    obsidian.addExit("S", 2, "铁砧铁匠铺");
    obsidian.addExit("SW", 3, "裂隙废墟");
    obsidian.addExit("E", 5, "背契之坛");
    rooms.insert({ 4, obsidian });

    // 背契区域
    Room betrayal(5, "背契之坛", "刻有红色咒文的圆形石坛，中央矗立着断柱",
        "晋津津守在这里，他或许知道卡莱恩的弱点");
    betrayal.addExit("W", 4, "黑曜权枢殿");
    betrayal.addExit("NE", 6, "凋誓之崖");
    betrayal.addNPC("晋津津（祭祀）");
    betrayal.addItem("铁誓胸甲");
    rooms.insert({ 5, betrayal });

    Room cliff(6, "凋誓之崖", "临海悬崖，海风呼啸，崖边有破碎的船骸",
        "东边断柱后隐藏着通往布道台的路径");
    cliff.addExit("SW", 5, "背契之坛");
    cliff.addExit("E", 7, "残垣断柱");
    rooms.insert({ 6, cliff });

    Room pillar(7, "残垣断柱", "刻有古老经文的巨大断柱，部分掩埋在沙土中",
        "张焜杰守在这里，他会指引你找到明识之戒");
    pillar.addExit("W", 6, "凋誓之崖");
    pillar.addExit("SE", 8, "残垣布道台");
    pillar.addNPC("张焜杰（守誓者）");
    pillar.addItem("明识之戒");
    rooms.insert({ 7, pillar });

    Room platform(8, "残垣布道台", "笼罩在粉色迷雾中的布道台，有虚假告示牌",
        "玛尔索的幻象在这里作祟，用明识之戒可破除");
    platform.addExit("NW", 7, "残垣断柱");
    platform.addExit("S", 9, "怜悯之城");
    rooms.insert({ 8, platform });

    // 怜悯区域
    Room mercy(9, "怜悯之城", "曾经繁华的城市，如今改为残酷的斗技场",
        "钟志炜城主知道怜悯之链的藏匿处");
    mercy.addExit("N", 8, "残垣布道台");
    mercy.addExit("SE", 10, "山脚下");
    mercy.addExit("E", 11, "棘刑斗技场");
    mercy.addNPC("钟志炜（城主）");
    rooms.insert({ 9, mercy });

    Room foot(10, "城外山脚下", "布满岩石的山坡，有隐蔽洞穴",
        "洞穴深处藏着怜悯之链");
    foot.addExit("NW", 9, "怜悯之城");
    foot.addItem("怜悯之链");
    rooms.insert({ 10, foot });

    Room arena(11, "棘刑斗技场", "环形斗技场，四周是带刺的铁丝网",
        "克鲁尔萨在这里进行残酷的角斗表演");
    arena.addExit("W", 9, "怜悯之城");
    arena.addExit("N", 12, "漠心城");
    rooms.insert({ 11, arena });

    // 漠心区域
    Room apathy(12, "漠心城", "灰白色调的城市，行人面无表情，目光空洞",
        "北边的静默尖塔是这座城市的地标");
    apathy.addExit("S", 11, "棘刑斗技场");
    apathy.addExit("N", 13, "静默尖塔");
    rooms.insert({ 12, apathy });

    Room spire(13, "静默尖塔", "无窗石塔，刻有'禁止喧哗'的符文",
        "塔内有上下两条路径，通往迷宫和塔顶");
    spire.addExit("S", 12, "漠心城");
    spire.addExit("D", 14, "塔底迷宫");
    spire.addExit("U", 15, "塔顶");
    rooms.insert({ 13, spire });

    Room maze(14, "塔底迷宫", "多岔路的石制迷宫，墙壁会轻微移动",
        "王浠珃被困在这里，她持有晨曦披风");
    maze.addExit("U", 13, "静默尖塔");
    maze.addNPC("王浠珃（三军将领）");
    maze.addItem("晨曦披风");
    rooms.insert({ 14, maze });

    Room top(15, "塔顶", "开阔平台，中央有黑色晶石，天空呈现灰白色",
        "灭欲将军在这里吸收着人们的情感");
    top.addExit("D", 13, "静默尖塔");
    top.addExit("E", 16, "虚无之城");
    rooms.insert({ 15, top });

    // 虚无区域
    Room voidCity(16, "虚无之城", "半坍塌的城市，街道覆盖着厚厚的灰尘",
        "东边的旧图书馆或许还保留着一些知识");
    voidCity.addExit("W", 15, "塔顶");
    voidCity.addExit("E", 17, "旧图书馆废墟");
    rooms.insert({ 16, voidCity });

    Room library(17, "旧图书馆废墟", "烧毁的图书馆，少数书页仍可辨认",
        "周洋迅在这里研究古代文献，他知道创世战靴的位置");
    library.addExit("W", 16, "虚无之城");
    library.addExit("N", 18, "亡念堡垒");
    library.addNPC("周洋迅（拾荒者）");
    library.addItem("创世战靴");
    rooms.insert({ 17, library });

    Room fortress(18, "亡念堡垒", "黑色石块砌成的堡垒，弥漫死亡气息",
        "尼赫尔将军的黑暗力量笼罩着这里");
    fortress.addExit("S", 17, "旧图书馆废墟");
    fortress.addExit("N", 19, "混沌之心");
    rooms.insert({ 18, fortress });

    // 最终区域
    Room chaos(19, "混沌之心", "虚空之地，漂浮着时空碎片，中央是能量核心",
        "万恶枢机就在核心处，准备最终决战吧");
    chaos.addExit("S", 18, "亡念堡垒");
    rooms.insert({ 19, chaos });
}

// 初始化NPC
void Map::initNPCs() {
    // 铁匠铺的NPC - 杨思睿
    roomNPCs[2] = std::make_unique<NPC>(
        "杨思睿", NPCType::BLACKSMITH,
        std::vector<std::string>{"欢迎来到铁砧铁匠铺！", "我可以为你修复装备。", "需要什么服务吗？"},
        "1"  // 发布任务ID为1的任务
    );

    // 背契之坛的NPC - 晋津津
    roomNPCs[5] = std::make_unique<NPC>(
        "晋津津", NPCType::PRIEST,
        std::vector<std::string>{"神明保佑你，勇者。", "背契之坛见证着古老的誓言。", "愿圣光指引你前行。"},
        "2"
    );

    // 残垣断柱的NPC - 张焜杰
    roomNPCs[7] = std::make_unique<NPC>(
        "张焜杰", NPCType::GUARDIAN,
        std::vector<std::string>{"我是这里的守护者。", "明识之戒就在这里。", "愿你能看清真相。"},
        ""
    );

    // 怜悯之城的NPC - 钟志炜
    roomNPCs[9] = std::make_unique<NPC>(
        "钟志炜", NPCType::MAYOR,
        std::vector<std::string>{"欢迎来到怜悯之城。", "这里曾经是繁华之地。", "怜悯之链藏在山脚下。"},
        ""
    );

    // 塔底迷宫的NPC - 王浠珃
    roomNPCs[14] = std::make_unique<NPC>(
        "王浠珃", NPCType::GENERAL,
        std::vector<std::string>{"我被困在这迷宫中。", "晨曦披风是我的珍藏。", "帮助我脱困吧。"},
        ""
    );

    // 图书馆的NPC - 周洋迅
    roomNPCs[17] = std::make_unique<NPC>(
        "周洋迅", NPCType::SCAVENGER,
        std::vector<std::string>{"我在这里研究古代文献。", "创世战靴的线索就在这些书中。", "知识就是力量。"},
        ""
    );
}

// 初始化BOSS
void Map::initBosses() {
    // 黑曜权枢殿 - 厄休拉
    roomBosses[4] = std::make_unique<EvilGeneral>("厄休拉", EvilType::POWER_HUNGRY, "黑曜权枢殿", 10);
    
    // 凋誓之崖 - 卡莱恩  
    roomBosses[6] = std::make_unique<EvilGeneral>("卡莱恩", EvilType::BETRAYAL, "凋誓之崖", 12);
    
    // 残垣布道台 - 玛尔索
    roomBosses[8] = std::make_unique<EvilGeneral>("玛尔索", EvilType::LIE, "残垣布道台", 14);
    
    // 棘刑斗技场 - 克鲁尔萨
    roomBosses[11] = std::make_unique<EvilGeneral>("克鲁尔萨", EvilType::CRUELTY, "棘刑斗技场", 16);
    
    // 塔顶 - 灭欲
    roomBosses[15] = std::make_unique<EvilGeneral>("灭欲", EvilType::DESIRELESS, "塔顶", 18);
    
    // 亡念堡垒 - 尼赫尔
    roomBosses[18] = std::make_unique<EvilGeneral>("尼赫尔", EvilType::DESTRUCTION, "亡念堡垒", 20);
    
    // 混沌之心 - 万恶枢机 (最终BOSS)
    roomBosses[19] = std::make_unique<BossWanEshuji>();
}

// 初始化普通敌人
void Map::initEnemies() {
    // 迷雾森林
    roomEnemies[1].push_back(std::make_unique<CommonEnemy>(EnemyType::GOBLIN, 1));
    roomEnemies[1].push_back(std::make_unique<CommonEnemy>(EnemyType::SLIME, 1));
    
    // 裂隙废墟 - 蚀骨恶狼守卫
    roomEnemies[3].push_back(std::make_unique<CommonEnemy>(EnemyType::CORRUPT_WOLF, 2));
    roomEnemies[3].push_back(std::make_unique<CommonEnemy>(EnemyType::CORRUPT_WOLF, 2));
    
    // 残垣断柱 - 新增试炼：两只牛头人，击败后才能接取任务3
    roomEnemies[7].push_back(std::make_unique<CommonEnemy>(EnemyType::MINOTAUR, 6));
    roomEnemies[7].push_back(std::make_unique<CommonEnemy>(EnemyType::MINOTAUR, 6));
    
    // 漠心城
    roomEnemies[12].push_back(std::make_unique<CommonEnemy>(EnemyType::ZOMBIE, 8));
    roomEnemies[12].push_back(std::make_unique<CommonEnemy>(EnemyType::SKELETON, 8));
    
    // 虚无之城
    roomEnemies[16].push_back(std::make_unique<CommonEnemy>(EnemyType::MINOTAUR, 10));
    roomEnemies[16].push_back(std::make_unique<CommonEnemy>(EnemyType::ZOMBIE, 10));
}

// 绘制全局地图
void Map::drawGlobalMap() const {
    std::cout << "\033[32m==== 恶念之界地图 ====\033[0m" << std::endl;
    std::cout << std::endl;
    std::cout << "     混沌之心[19]" << std::endl;
    std::cout << "         |" << std::endl;
    std::cout << "   亡念堡垒[18]" << std::endl;
    std::cout << "         |" << std::endl;
    std::cout << "   旧图书馆[17]──虚无之城[16]──塔顶[15]" << std::endl;
    std::cout << "                         |" << std::endl;
    std::cout << "                    静默尖塔[13]" << std::endl;
    std::cout << "                     |      |" << std::endl;
    std::cout << "                 迷宫[14]  漠心城[12]" << std::endl;
    std::cout << "                           |" << std::endl;
    std::cout << "                   棘刑斗技场[11]──怜悯之城[9]" << std::endl;
    std::cout << "                              |  \\     " << std::endl;
    std::cout << "                          山脚[10] 布道台[8]" << std::endl;
    std::cout << "                                     |" << std::endl;
    std::cout << "                              断柱[7]──凋誓崖[6]" << std::endl;
    std::cout << "                                     |" << std::endl;
    std::cout << "                          黑曜权枢殿[4]──背契坛[5]" << std::endl;
    std::cout << "                            |     |" << std::endl;
    std::cout << "                      铁匠铺[2]──裂隙废墟[3]" << std::endl;
    std::cout << "                         /" << std::endl;
    std::cout << "                   迷雾森林[1]" << std::endl;
    std::cout << std::endl;
}

// 绘制定位地图
void Map::drawLocationMap() const {
    std::cout << "\033[36m当前区域地图:\033[0m" << std::endl;
    auto it = rooms.find(currentRoomId);
    if (it != rooms.end()) {
        const Room& currentRoom = it->second;
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
        std::cout << "当前位置: " << currentRoom.getRoomName() << " [" << currentRoomId << "]" << std::endl;
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
        
        // 显示相邻房间
        const auto& exits = currentRoom.getExits();
        if (!exits.empty()) {
            std::cout << "可前往区域:" << std::endl;
            for (const auto& exit : exits) {
                std::cout << "  " << exit.first << "(" << Room::dirToNumber(exit.first) 
                         << ") -> " << exit.second.second << " [" << exit.second.first << "]" << std::endl;
            }
        }
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
    }
}

// 显示全局地图
void Map::showGlobalMap() {
    drawGlobalMap();
}

// 显示定位地图
void Map::showLocationMap() {
    drawLocationMap();
}

// 移动房间（支持方向文字或数字）- 1=北 2=东北 3=东 4=东南 5=南 6=西南 7=西 8=西北 9=上 0=下
bool Map::switchRoom(const std::string& input) {
    if (rooms.find(currentRoomId) == rooms.end()) {
        std::cout << "当前房间不存在！" << std::endl;
        return false;
    }

    const Room& currentRoom = rooms[currentRoomId];
    const auto& exits = currentRoom.getExits();
    
    std::string targetDir;
    
    // 检查是否为数字输入 (按UI显示的编号)
    if (input == "1") targetDir = "N";
    else if (input == "2") targetDir = "NE"; 
    else if (input == "3") targetDir = "E";
    else if (input == "4") targetDir = "SE";
    else if (input == "5") targetDir = "S";
    else if (input == "6") targetDir = "SW";
    else if (input == "7") targetDir = "W";
    else if (input == "8") targetDir = "NW";
    else if (input == "9") targetDir = "U";
    else if (input == "0") targetDir = "D";
    // 检查是否为中文方向
    else if (input == "北") targetDir = "N";
    else if (input == "东北") targetDir = "NE";
    else if (input == "东") targetDir = "E";
    else if (input == "东南") targetDir = "SE";
    else if (input == "南") targetDir = "S";
    else if (input == "西南") targetDir = "SW";
    else if (input == "西") targetDir = "W";
    else if (input == "西北") targetDir = "NW";
    else if (input == "上") targetDir = "U";
    else if (input == "下") targetDir = "D";
    // 检查是否为英文方向
    else {
        targetDir = input;
        std::transform(targetDir.begin(), targetDir.end(), targetDir.begin(), ::toupper);
    }

    auto it = exits.find(targetDir);
    if (it != exits.end()) {
        currentRoomId = it->second.first;
        std::cout << "\033[32m你移动到了新的区域。\033[0m" << std::endl;
        showCurrentRoom();
        return true;
    } else {
        std::cout << "无法朝 " << input << " 方向移动！" << std::endl;
        return false;
    }
}

bool Map::switchRoom(const std::string& input, Player* player, SaveLoadSystem* saveSystem, TaskSystem* taskSystem) {
    if (rooms.find(currentRoomId) == rooms.end()) {
        std::cout << "当前房间不存在！" << std::endl;
        return false;
    }

    const Room& currentRoom = rooms[currentRoomId];
    const auto& exits = currentRoom.getExits();
    
    std::string targetDir;
    
    // 检查是否为数字输入 (按UI显示的编号)
    if (input == "1") targetDir = "N";
    else if (input == "2") targetDir = "NE"; 
    else if (input == "3") targetDir = "E";
    else if (input == "4") targetDir = "SE";
    else if (input == "5") targetDir = "S";
    else if (input == "6") targetDir = "SW";
    else if (input == "7") targetDir = "W";
    else if (input == "8") targetDir = "NW";
    else if (input == "9") targetDir = "U";
    else if (input == "0") targetDir = "D";
    // 检查是否为中文方向
    else if (input == "北") targetDir = "N";
    else if (input == "东北") targetDir = "NE";
    else if (input == "东") targetDir = "E";
    else if (input == "东南") targetDir = "SE";
    else if (input == "南") targetDir = "S";
    else if (input == "西南") targetDir = "SW";
    else if (input == "西") targetDir = "W";
    else if (input == "西北") targetDir = "NW";
    else if (input == "上") targetDir = "U";
    else if (input == "下") targetDir = "D";
    // 检查是否为英文方向
    else {
        targetDir = input;
        std::transform(targetDir.begin(), targetDir.end(), targetDir.begin(), ::toupper);
    }

    auto it = exits.find(targetDir);
    if (it != exits.end()) {
        currentRoomId = it->second.first;
        
        // 同步玩家位置
        if (player) {
            player->setCurrentRoomId(currentRoomId);
        }
        
        // 检查是否有重要位置需要自动存档
        if (saveSystem && (currentRoomId == 4 || currentRoomId == 19)) { // BOSS房间自动存档
            saveSystem->autoSaveGame(*player, *taskSystem);
        }
        
        std::cout << "\033[32m你移动到了新的区域。\033[0m" << std::endl;
        showCurrentRoom();
        return true;
    } else {
        std::cout << "无法朝 " << input << " 方向移动！" << std::endl;
        return false;
    }
}

// 显示当前房间信息
void Map::showCurrentRoom() const {
    auto it = rooms.find(currentRoomId);
    if (it != rooms.end()) {
        it->second.showRoomInfo();
        
        // 显示NPC
        if (roomNPCs.find(currentRoomId) != roomNPCs.end()) {
            std::cout << "这里有NPC: " << roomNPCs.at(currentRoomId)->getName() << std::endl;
        }
        
        // 显示BOSS
        if (roomBosses.find(currentRoomId) != roomBosses.end()) {
            std::cout << "警告！这里有强敌: " << roomBosses.at(currentRoomId)->getName() << std::endl;
        }
        
        // 显示普通敌人
        if (roomEnemies.find(currentRoomId) != roomEnemies.end() && 
            !roomEnemies.at(currentRoomId).empty()) {
            std::cout << "附近有" << roomEnemies.at(currentRoomId).size() << "只敌人在游荡..." << std::endl;
        }
    }
}

// 显示初始房间信息
void Map::showInitialRoom() const {
    showCurrentRoom();
}

// 快速跳转房间
void Map::jumpToRoom(int roomId) {
    if (rooms.find(roomId) != rooms.end()) {
        currentRoomId = roomId;
        showCurrentRoom();
    } else {
        std::cout << "房间 " << roomId << " 不存在！" << std::endl;
    }
}

// 获取当前房间ID
int Map::getCurrentRoomId() const {
    return currentRoomId;
}

// 设置当前房间ID
void Map::setCurrentRoomId(int roomId) {
    if (rooms.find(roomId) != rooms.end()) {
        currentRoomId = roomId;
    }
}

// 获取当前房间的NPC
NPC* Map::getCurrentRoomNPC() const {
    auto it = roomNPCs.find(currentRoomId);
    return (it != roomNPCs.end()) ? it->second.get() : nullptr;
}

// 获取当前房间的BOSS
EvilGeneral* Map::getCurrentRoomBoss() const {
    auto it = roomBosses.find(currentRoomId);
    if (it != roomBosses.end()) {
        // 检查是否是万恶枢机（BossWanEshuji）
        if (currentRoomId == 19) {
            return nullptr; // 万恶枢机用专门的函数获取
        }
        return it->second.get();
    }
    return nullptr;
}

// 获取当前房间的万恶枢机
BossWanEshuji* Map::getCurrentRoomFinalBoss() const {
    if (currentRoomId == 19) {
        auto it = roomBosses.find(currentRoomId);
        if (it != roomBosses.end()) {
            return dynamic_cast<BossWanEshuji*>(it->second.get());
        }
    }
    return nullptr;
}

// 获取当前房间的随机敌人
CommonEnemy* Map::getRandomEnemy() const {
    auto it = roomEnemies.find(currentRoomId);
    if (it != roomEnemies.end() && !it->second.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, it->second.size() - 1);
        return it->second[dis(gen)].get();
    }
    return nullptr;
}

// 移除已击败的敌人
void Map::removeDefeatedEnemy(CommonEnemy* enemy) {
    auto it = roomEnemies.find(currentRoomId);
    if (it != roomEnemies.end()) {
        auto& enemies = it->second;
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ++enemyIt) {
            if (enemyIt->get() == enemy) {
                enemies.erase(enemyIt);
                break;
            }
        }
    }
}

// 移除已击败的BOSS
void Map::removeDefeatedBoss() {
    roomBosses.erase(currentRoomId);
}

bool Map::hasEnemyTypeInRoom(int roomId, EnemyType type) const {
    auto it = roomEnemies.find(roomId);
    if (it == roomEnemies.end()) return false;
    for (auto &ptr : it->second) {
        if (ptr && ptr->isAlive() && ptr->getType()==type) return true;
    }
    return false;
}