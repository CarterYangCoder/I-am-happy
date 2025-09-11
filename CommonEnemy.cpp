#include "CommonEnemy.h"
// 构造函数：根据敌人类型和等级初始化属性
CommonEnemy::CommonEnemy(EnemyType type, int level)
    : Attribute(getEnemyName(type), level),  // 调用基类构造函数（传入名称和等级）
    type(type)
{
    // 根据敌人类型设置属性（使用基类的 Setter 方法，方法名必须完全匹配
    switch (type) {
    case EnemyType::CORRUPT_WOLF:  // 蚀骨恶狼
        setMaxHP(25 * level);      // 最大生命值 = 25 * 等级
        setHP(getMaxHP());         // 当前生命值初始化为最大值
        setATK(5 * level);        // 攻击力 = 5 * 等级
        setDEF(2 * level);         // 防御力 = 2 * 等级
        setSpeed(10 * level);      // 速度 = 10 * 等级
        setCritRate(0.02f);         // 暴击率 = 2%（0-1范围）
        expReward = 60 * level;    // 经验奖励 = 60 * 等级
        goldReward = 50 * level;   // 金币奖励 = 50 * 等级
        break;

    case EnemyType::GOBLIN:  // 哥布林
        setMaxHP(20 * level);
        setHP(getMaxHP());
        setATK(4 * level);
        setDEF(2 * level);
        setSpeed(5 * level);
        setCritRate(0.02f);
        expReward = 70 * level;
        goldReward = 30 * level;
        break;

    case EnemyType::SLIME:// 史莱姆
        setMaxHP(25 * level);
        setHP(getMaxHP());
        setATK(3 * level);
        setDEF(5 * level);
        setSpeed(3 * level);
        setCritRate(0.02f);
        expReward = 70 * level;
        goldReward = 30 * level;
        break;

    case EnemyType::MINOTAUR:// 牛头人
        setMaxHP(30 * level);
        setHP(getMaxHP());
        setATK(6 * level);
        setDEF(6 * level);
        setSpeed(8 * level);
        setCritRate(0.12f);
        expReward = 120 * level;
        goldReward = 100 * level;
        break;

    case EnemyType::ZOMBIE:// 僵尸
        setMaxHP(35 * level);
        setHP(getMaxHP());
        setATK(8 * level);
        setDEF(8 * level);
        setSpeed(4 * level);
        setCritRate(0.07f);
        expReward = 120 * level;
        goldReward = 100 * level;
        break;

    case EnemyType::SKELETON:// 骷髅
        setMaxHP(40 * level);
        setHP(getMaxHP());
        setATK(10 * level);
        setDEF(8 * level);
        setSpeed(6 * level);
        setCritRate(0.09f);
        expReward = 140 * level;
        goldReward = 120 * level;
        break;

    default:  // 神秘敌人
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
    default: return "神秘敌人";
    }
}

EnemyType CommonEnemy::getType() const { return type; }
int CommonEnemy::getExpReward() const { return expReward; }
int CommonEnemy::getGoldReward() const { return goldReward; }
