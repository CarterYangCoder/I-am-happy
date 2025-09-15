#pragma once
#ifndef GAMEDATA_H
#define GAMEDATA_H

// 核心常量定义
const float DEFAULT_CRIT_DAMAGE_MULTIPLE = 2.5f; // 暴击伤害为2.5倍
const float LEVEL_UP_ATTR_MULTIPLIER = 1.15f; // 等级提升属性倍率
const float LEVEL_UP_EXP_MULTIPLIER = 1.25f; // 调整：升级经验增长更快（原1.1）

// 新增：技能默认每级成长系数（线性成长，按解锁等级起算）
const float DEFAULT_SKILL_GROWTH_PER_LEVEL = 0.10f; // 调整：略降技能成长（原0.12）

// 新增：技能MP消耗配置（基础值 + 每级成长系数）
const int   DEFAULT_MP_COST_PHYSICAL = 6;
const int   DEFAULT_MP_COST_BUFF     = 10; // 包含 BUFF/HOLY_MARK_SPEED/STAR_ARMOR
const int   DEFAULT_MP_COST_MAGICAL  = 14;
const float DEFAULT_SKILL_MP_GROWTH_PER_LEVEL = 0.03f; // MP消耗随等级线性增长

// 恶念将军类型（对应六大恶念）
enum class EvilType {
    POWER_HUNGRY,   // 嗜权（厄休拉）
    BETRAYAL,       // 背信（卡莱恩）
    LIE,            // 诳言（玛尔索）
    CRUELTY,        // 残虐（克鲁尔萨）
    DESIRELESS,     // 灭欲（灭欲）
    DESTRUCTION,    // 亡念（尼赫尔）
    BOSS            // BOSS
};

// 装备部位（对应六誓圣辉救赎套装）
enum class EquipmentPart {
    SWORD,      // 圣剑（六圣裁恶神剑，唯一神器）
    HELMET,     // 头盔（自由誓约・破枷之冠）
    CHESTPLATE, // 胸甲（忠诚誓约・铁誓胸甲）
    RING,       // 戒指（真理誓约・明识之戒）
    NECKLACE,   // 项链（怜悯誓约・抚伤之链）
    CAPE,       // 披风（希望誓约・晨曦披风）
    BOOTS       // 战靴（秩序誓约・创世战靴）
};

// 技能类型（神灵赐予的六个技能）
enum class SkillType {
    HOLY_RIFT_SLASH,        // 圣界裂隙斩（物理伤害）
    GOLDEN_TREE_VOW,        // 黄金树之誓（增益加血加攻）
    HOLY_PRISON_JUDGMENT,   // 圣狱裁决（魔法伤害）
    STAR_ARMOR,             // 星辰圣铠（防御）
    HOLY_MARK_SPEED,        // 圣痕疾影步（速度up）
    ULTIMATE_SLAY           // 星闪流河圣龙飞升·神界湮灭斩·最终式（集齐套装解锁）
};

#endif // GAMEDATA_H