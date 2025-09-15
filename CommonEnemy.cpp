#include "CommonEnemy.h"
// 构造函数：根据敌人类型和等级初始化属性
CommonEnemy::CommonEnemy(EnemyType type, int level)
    : Attribute(getEnemyName(type), level),  // 调用基类构造函数（传入名称和等级）
    type(type)
{
    switch (type) {
    case EnemyType::CORRUPT_WOLF:  // 蚀骨恶狼（关键早期门怪）
        setMaxHP(22 * level);      // 调整：更低HP
        setHP(getMaxHP());
        setATK(4 * level);         // 调整：更低攻击
        setDEF(2 * level);
        setSpeed(6 * level);       // 调整：降低速度
        setCritRate(0.02f);
        expReward = 35 * level;    // 调整：降低奖励，避免早期暴涨
        goldReward = 55 * level;
        break;

    case EnemyType::GOBLIN:  // 哥布林（新手练级点）
        setMaxHP(18 * level);
        setHP(getMaxHP());
        setATK(4 * level);
        setDEF(2 * level);
        setSpeed(5 * level);
        setCritRate(0.02f);
        expReward = 28 * level;
        goldReward = 40 * level;
        break;

    case EnemyType::SLIME: // 史莱姆（肉一点）
        setMaxHP(22 * level);
        setHP(getMaxHP());
        setATK(3 * level);
        setDEF(4 * level);
        setSpeed(3 * level);
        setCritRate(0.02f);
        expReward = 25 * level;
        goldReward = 35 * level;
        break;

    case EnemyType::MINOTAUR: // 牛头人（试炼怪，强一点但可打）
        setMaxHP(35 * level);
        setHP(getMaxHP());
        setATK(7 * level);
        setDEF(6 * level);
        setSpeed(6 * level);
        setCritRate(0.10f);
        expReward = 70 * level;
        goldReward = 90 * level;
        break;

    case EnemyType::ZOMBIE: // 僵尸（慢而硬）
        setMaxHP(42 * level);
        setHP(getMaxHP());
        setATK(7 * level);
        setDEF(7 * level);
        setSpeed(3 * level);
        setCritRate(0.07f);
        expReward = 85 * level;
        goldReward = 90 * level;
        break;

    case EnemyType::SKELETON: // 骷髅（快一些，输出高）
        setMaxHP(36 * level);
        setHP(getMaxHP());
        setATK(9 * level);
        setDEF(7 * level);
        setSpeed(6 * level);
        setCritRate(0.09f);
        expReward = 90 * level;
        goldReward = 95 * level;
        break;
    
    case EnemyType::BOSS: // 通用BOSS奖励（将军/终极BOSS通过子类覆盖面板）
        setMaxHP(70 * level);
        setHP(getMaxHP());
        setATK(11 * level);
        setDEF(10 * level);
        setSpeed(8 * level);
        setCritRate(0.15f);
        expReward = 80 * level;    // 调整：显著降低BOSS一次性经验
        goldReward = 120 * level;
        break;

    default:
        setMaxHP(10 * level);
        setHP(getMaxHP());
        setATK(1 * level);
        setDEF(0 * level);
        setSpeed(1 * level);
        setCritRate(0.01f);
        expReward = 10 * level;
        goldReward = 5 * level;
        break;
    }
}

// 虚析构函数实现
CommonEnemy::~CommonEnemy() {
    // 清理资源
}

// 辅助函数：根据 EnemyType 获取敌人名称（避免构造函数初始化列表中直接写复杂逻辑）
std::string CommonEnemy::getEnemyName(EnemyType type) {
    switch (type) {
    case EnemyType::CORRUPT_WOLF: return "蚀骨恶狼";
    case EnemyType::GOBLIN: return "哥布林";
    case EnemyType::SLIME: return "史莱姆";
    case EnemyType::MINOTAUR: return "牛头人";
    case EnemyType::ZOMBIE: return "僵尸";
    case EnemyType::SKELETON: return "骷髅";
    case EnemyType::BOSS: return "恶念将军";
    default: return "神秘敌人";
    }
}

EnemyType CommonEnemy::getType() const { return type; }
int CommonEnemy::getExpReward() const { return expReward; }
int CommonEnemy::getGoldReward() const { return goldReward; }

