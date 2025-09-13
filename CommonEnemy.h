#pragma once
#ifndef COMMONENEMY_H
#define COMMONENEMY_H
#include "Attribute.h"

/**
 * @enum EnemyType
 * @brief 小怪类型（含门禁/剧情相关）。
 */
enum class EnemyType {
    CORRUPT_WOLF,   // 蚀骨恶狼（裂隙废墟守卫）
    GOBLIN,         // 哥布林
    SLIME,          // 史莱姆
    MINOTAUR,       // 牛头人
    ZOMBIE,         // 僵尸
    SKELETON,       // 骷髅
	BOSS            // BOSS
};

/**
 * @class CommonEnemy
 * @brief 普通敌人：继承 Attribute，包含经验与金币奖励。
 */
class CommonEnemy : public Attribute {
private:
    EnemyType type;
    int expReward;   // 击败后经验奖励
    int goldReward;  // 击败后金币奖励

public:
    /** @brief 构造敌人并按类型/等级初始化属性。 */
    CommonEnemy(EnemyType type, int level);
    virtual ~CommonEnemy(); // 虚析构函数声明

    /** @brief 类型转名称工具。 */
    static std::string getEnemyName(EnemyType type);

    EnemyType getType() const;
    int getExpReward() const;
    int getGoldReward() const;
};

#endif // COMMONENEMY_H