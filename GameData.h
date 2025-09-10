#pragma once
#ifndef GAMEDATA_H
#define GAMEDATA_H

// 核心常量定义（基于剧本设定）
const float DEFAULT_CRIT_DAMAGE_MULTIPLE = 2.0f; // 默认暴击伤害为双倍
const float LEVEL_UP_ATTR_MULTIPLIER = 1.1f; // 等级提升属性倍率
const float LEVEL_UP_EXP_MULTIPLIER = 1.2f; // 等级提升经验条倍率

 
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
    ULTIMATE_SLAY           // 星闪流河圣龙飞升·神界湮灭斩·最终式（集齐神器解锁）
};

#endif // GAMEDATA_H