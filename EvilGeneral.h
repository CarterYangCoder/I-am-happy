#pragma once
#ifndef EVILGENERAL_H
#define EVILGENERAL_H
#include "CommonEnemy.h"
#include "GameData.h"
#include "Item.h"
#include <vector>
 
class EvilGeneral : public CommonEnemy {
private:
    EvilType evilType;         // 恶念类型
    std::string territory;     // 领地（如黑曜权枢殿）
    std::vector<Item*> drops;  // 掉落物品

public:
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

	// 对话系统
	virtual std::string getPreBattleDialogue() const;
	virtual std::string getDefeatDialogue() const;
	virtual std::string getVictoryDialogue() const;

};



#endif // EVILGENERAL_H