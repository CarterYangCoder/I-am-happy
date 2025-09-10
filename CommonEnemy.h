#pragma once
#ifndef COMMONENEMY_H
#define COMMONENEMY_H
#include "Attribute.h"

// 小怪类型（基于剧本中的恶兽）
enum class EnemyType {
    CORRUPT_WOLF,   // 蚀骨恶狼（裂隙废墟守卫）
    GOBLIN,         // 哥布林
    SLIME,          // 史莱姆
    MINOTAUR,       // 牛头人
    ZOMBIE,         // 僵尸
    SKELETON,       // 骷髅
	BOSS            // BOSS
};

class CommonEnemy : public Attribute {
private:
    EnemyType type;
    int expReward;   // 击败后经验奖励
    int goldReward;  // 击败后金币奖励

public:
    CommonEnemy(EnemyType type, int level);
    virtual ~CommonEnemy(); // 虚析构函数声明
    static std::string getEnemyName(EnemyType type);
    EnemyType getType() const;
    int getExpReward() const;
    int getGoldReward() const;
    
};

#endif // COMMONENEMY_H