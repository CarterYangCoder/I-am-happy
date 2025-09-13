#pragma once
#ifndef EVILGENERAL_H
#define EVILGENERAL_H
#include "CommonEnemy.h"
#include "GameData.h"
#include "Item.h"
#include <vector>

/**
 * @class EvilGeneral
 * @brief 恶念将军（BOSS 基类），拥有领地与掉落与对话。
 */
class EvilGeneral : public CommonEnemy {
private:
    EvilType evilType;         // 恶念类型
    std::string territory;     // 领地（如黑曜权枢殿）
    std::vector<Item*> drops;  // 掉落物品

public:
    /**
     * @brief 构造将军。
     * @param name 名称
     * @param type 恶念类型
     * @param territory 领地名
     * @param level 等级
     */
    EvilGeneral(std::string name, EvilType type, std::string territory, int level);
    virtual ~EvilGeneral(); // 虚析构函数声明

	EvilType getEvilType() const { return evilType; }
	std::string getTerritory() const { return territory; }
	static std::string getEnemyName(EvilType type) {
		switch (type) {
		case EvilType::POWER_HUNGRY: return "厄休拉";
		case EvilType::BETRAYAL: return "卡莱恩";
		case EvilType::LIE: return "玛尔索";
		case EvilType::CRUELTY: return "克鲁尔萨";
		case EvilType::DESIRELESS: return "灭欲";
		case EvilType::DESTRUCTION: return "尼赫尔";
		default: return "神秘将军";
		}
	}
	const std::vector<Item*>& getDropItems() const { return drops; }

	/** @brief 战前对话。 */
	virtual std::string getPreBattleDialogue() const;
	/** @brief 战败台词（敌方被击败）。 */
	virtual std::string getDefeatDialogue() const;
	/** @brief 胜利台词（敌方获胜）。 */
	virtual std::string getVictoryDialogue() const;
};



#endif // EVILGENERAL_H