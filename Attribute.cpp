#include "Attribute.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// 小幅度固定加成（在原倍率基础上再叠加）
static constexpr int LEVEL_UP_FLAT_HP    = 15;   // 每级额外生命
static constexpr int LEVEL_UP_FLAT_MP    = 6;    // 每级额外蓝量
static constexpr int LEVEL_UP_FLAT_ATK   = 3;    // 每级额外攻击
static constexpr int LEVEL_UP_FLAT_DEF   = 2;    // 每级额外防御
// 速度不直接每级+1，改为每2级+1，避免后期过快
static constexpr int LEVEL_UP_SPEED_STEP = 2;    // 每满2级额外+1速度

Attribute::Attribute(std::string name, int level)
    : name(name), level(level), hp(100), maxHp(100), mp(50), maxMp(50), atk(10), def(5), speed(5),
    exp(0), expToNextLevel(100), gold(0), critRate(0.05f) {
    // 初始化随机数种子（用于暴击判定）
    srand(time(0));
}

// 虚析构函数实现
Attribute::~Attribute() {
    // 基类析构函数，子类可以重写
}

// 属性获取
std::string Attribute::getName() const { return name; }
int Attribute::getLevel() const { return level; }
int Attribute::getHP() const { return hp; }
int Attribute::getMaxHP() const { return maxHp; }
int Attribute::getATK() const { return atk; }
int Attribute::getDEF() const { return def; }
int Attribute::getSpeed() const { return speed; }
int Attribute::getExp() const { return exp; }
int Attribute::getExpToNextLevel() const { return expToNextLevel; }
int Attribute::getGold() const { return gold; }
float Attribute::getCritRate() const { return critRate; }
int Attribute::getMP() const { return mp; }
int Attribute::getMaxMP() const { return maxMp; }
std::string Attribute::getExpBar() const {
    return "EXP: " + std::to_string(exp) + "/" + std::to_string(expToNextLevel);
}

// 属性修改

void Attribute::setMaxHP(int value) {
    // 可以添加一些合理性检查，确保生命值为非负值
    if (value >= 0) {
        maxHp = value;
    }
    else {
        // 可以根据需要处理无效值，这里简单设为0
        maxHp = 0;
    }
}
void Attribute::setName(const std::string& Name) { name = Name; }
void Attribute::setHP(int value) { hp = std::max(0, std::min(value, maxHp)); }
void Attribute::setATK(int value) { atk = std::max(1, value); }
void Attribute::setDEF(int value) { def = std::max(0, value); }
void Attribute::setSpeed(int value) { speed = std::max(1, value); }
void Attribute::addExp(int value) { exp += value; }
void Attribute::addGold(int value) { gold = std::max(0, gold + value); }
void Attribute::setCritRate(float rate) { critRate = std::max(0.0f, std::min(1.0f, rate)); }
void Attribute::setGold(int value) { gold = value; }
void Attribute::setExp(int value) { exp = value; }
void Attribute::setLevel(int value) { level = value; }
void Attribute::setExpToNextLevel(int value) { expToNextLevel = value; }
void Attribute::setMP(int value) { mp = std::max(0, std::min(value, maxMp)); }
void Attribute::setMaxMP(int value) { maxMp = std::max(0, value); if (mp > maxMp) mp = maxMp; }
// 等级提升
bool Attribute::levelUp() {
    if (exp < expToNextLevel || level >= MAX_LEVEL) return false;
    
    bool hasLeveledUp = false;
    
    while (exp >= expToNextLevel && level < MAX_LEVEL) {
        exp -= expToNextLevel;
        level++;
        hasLeveledUp = true;
        
        // 记录升级前的当前生命值和蓝量，用于计算30%回复
        int oldHp = hp;
        int oldMp = mp;
        
        // 基础属性倍率成长 + 固定加成
        maxHp = static_cast<int>(maxHp * LEVEL_UP_ATTR_MULTIPLIER) + LEVEL_UP_FLAT_HP;
        maxMp = static_cast<int>(maxMp * LEVEL_UP_ATTR_MULTIPLIER) + LEVEL_UP_FLAT_MP;
        atk = static_cast<int>(atk * LEVEL_UP_ATTR_MULTIPLIER) + LEVEL_UP_FLAT_ATK;
        def = static_cast<int>(def * LEVEL_UP_ATTR_MULTIPLIER) + LEVEL_UP_FLAT_DEF;
        
        // 速度：倍率后每满2级额外+1（例如2,4,6...）
        speed = static_cast<int>(speed * LEVEL_UP_ATTR_MULTIPLIER);
        if (level % LEVEL_UP_SPEED_STEP == 0) speed += 1;
        if (speed < 1) speed = 1;
        
        // 设定：升级后回复当前生命值和蓝量的30%
        int hpRecover = static_cast<int>(oldHp * 0.3f);
        int mpRecover = static_cast<int>(oldMp * 0.3f);
        hp = std::min(maxHp, oldHp + hpRecover);
        mp = std::min(maxMp, oldMp + mpRecover);
        
        // 检查是否达到满级
        if (level >= MAX_LEVEL) {
            std::cout << "\033[33m════════════════════════════════\033[0m" << std::endl;
            std::cout << "\033[32m🎉 恭喜！你已达到最高等级 " << MAX_LEVEL << " 级！🎉\033[0m" << std::endl;
            std::cout << "\033[36m✨ 你的力量已臻于完美，无需再积累经验！ ✨\033[0m" << std::endl;
            std::cout << "\033[37m神明：\"你已成为真正的传奇英雄！这份力量足以拯救世界！\"\033[0m" << std::endl;
            std::cout << "\033[35m🌟 满级奖励：全属性最终强化！🌟\033[0m" << std::endl;
            std::cout << "\033[33m════════════════════════════════\033[0m" << std::endl;
            
            // 满级特别奖励：额外属性加成
            maxHp += 100;
            maxMp += 50;
            atk += 20;
            def += 15;
            speed += 5;
            hp = maxHp; // 满级时回满状态
            mp = maxMp;
            break; // 达到满级后停止升级
        } else {
            // 普通升级提示
            std::cout << "\033[33m════════════════════════════════\033[0m" << std::endl;
            std::cout << "\033[32m🎉 恭喜！等级提升到 " << getLevel() << " 级！🎉\033[0m" << std::endl;
            std::cout << "\033[36m你的属性得到了提升，生命值和蓝量也恢复了一些。\033[0m" << std::endl;
            std::cout << "\033[37m神明：\"感受到力量的成长了吗，小子！\"\033[0m" << std::endl;
            std::cout << "\033[33m════════════════════════════════\033[0m" << std::endl;
            
            // 下一等级经验需求
            expToNextLevel = static_cast<int>(expToNextLevel * LEVEL_UP_EXP_MULTIPLIER);
        }
    }
    
    return hasLeveledUp;
}

// 新增：检查是否已达到满级
bool Attribute::isMaxLevel() const {
    return level >= MAX_LEVEL;
}

bool Attribute::isAlive() const { return hp > 0; }

void Attribute::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

void Attribute::heal(int amount) {
    hp += amount;
    if (hp > maxHp) hp = maxHp;
}