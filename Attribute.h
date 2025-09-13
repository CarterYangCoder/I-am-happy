#pragma once
#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include <string>
#include "GameData.h"

/**
 * @file Attribute.h
 * @brief 角色属性基类：等级/生命/攻击/防御/速度/经验/金币/暴击/蓝量。
 */

/** @brief 最大等级常量。 */
const int MAX_LEVEL = 30;

/**
 * @class Attribute
 * @brief 可战斗实体的通用属性与成长逻辑。
 */
class Attribute {
protected:
    std::string name;
    int level;
    int hp;
    int maxHp;
    int atk;
    int def;
    int speed;
    int exp;
    int expToNextLevel;
    int gold;
    float critRate;
    int mp;
    int maxMp;

public:
    /** @brief 构造属性对象。 */
    Attribute(std::string name, int level = 1);
    virtual ~Attribute();

    /** @name 访问器 */
    ///@{
    std::string getName() const;
    int getLevel() const;
    int getHP() const;
    int getMaxHP() const;
    int getATK() const;
    int getDEF() const;
    int getSpeed() const;
    int getExp() const;
    int getExpToNextLevel() const;
    int getGold() const;
    float getCritRate() const;
    int getMP() const;
    int getMaxMP() const;
    std::string getExpBar() const;
    ///@}

    /** @name 修改器 */
    ///@{
    void setName(const std::string& Name);
    void setMaxHP(int value);
    void setHP(int value);
    void setATK(int value);
    void setDEF(int value);
    void setSpeed(int value);
    void addExp(int value);
    void addGold(int value);
    void setCritRate(float rate);
    void setExp(int value);
    void setGold(int value);
    void setExpToNextLevel(int value);
    void setLevel(int value);
    void setMP(int value);
    void setMaxMP(int value);
    ///@}

    /**
     * @brief 等级提升：属性成长、经验条调整与回血回蓝。
     * @return true 本次调用有发生升级
     */
    virtual bool levelUp();

    /** @brief 是否已到达满级。 */
    bool isMaxLevel() const;

    /** @brief 存活判定/受伤/治疗。 */
    bool isAlive() const;
    void takeDamage(int damage);
    void heal(int amount);
};

#endif // ATTRIBUTE_H