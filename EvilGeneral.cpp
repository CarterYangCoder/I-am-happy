#include "EvilGeneral.h"
#include <iostream>
#include <cstdlib>

EvilGeneral::EvilGeneral(std::string name, EvilType type, std::string territory, int level)
    :CommonEnemy (EnemyType::BOSS,level), evilType(type), territory(territory) {
    setName(name);  // 设置BOSS的名称
    maxHp = 100 + level * 20; // 增加合理的最大生命值
    setHP(maxHp);
    setHP(getMaxHP()); 
    setATK(15 + level * 0.5);
    setDEF(8 + level * 0.5);
    setSpeed(10 + level * 0.5);
    setCritRate(0.05f + level * 0.01f);
}

// 虚析构函数实现
EvilGeneral::~EvilGeneral() {
    // 清理资源
}

// 对话系统实现
std::string EvilGeneral::getPreBattleDialogue() const {
    switch (evilType) {
    case EvilType::POWER_HUNGRY:
        return "厄休拉：\"落魄王子也敢闯我的领地？\" 她掌心翻出黑焰，语气满是轻蔑，\"人类本就是供我驱使的蝼蚁，这片土地的规则由我定！敢挑战我的权威，就先死在这些影子手里！\"";
    case EvilType::BETRAYAL:
        return "卡莱恩：\"知道吗？每次挥剑时我都能听见誓约碎裂的声音——真是悦耳极了。\" 他举起染血的长剑，眼中满是对过往忠诚的嘲讽。";
    case EvilType::LIE:
        return "玛尔索：\"孩子听句劝，你那神灵指引就是毒鸡汤，再硬撑早晚翻车！\" 他指尖闪着廉价荧光，\"当年首都破防，全是这王子跑路坑的！我这'救世预言'可是经过三重认证，绝对保真！\"";
    case EvilType::CRUELTY:
        return "克鲁尔萨：\"听见他们骨头断裂的脆响了吗？比你神剑的嗡鸣更美妙。\" 他抛接着染血怀表，表盘内侧映着一个个头颅，\"你的统治在今天到此为止了！\"";
    case EvilType::DESIRELESS:
        return "灭欲：\"欢迎来到终极宁静的门槛。情感是噪音，希望是幻觉，爱是缺陷。我正在拯救他们，也将拯救你。\" 它以绝对的平静注视着你，仿佛看透了一切。";
    case EvilType::DESTRUCTION:
        return "尼赫尔：\"终结，从未到来。你我，不过是短暂的幻影。而我的存在，只是为了将所有幻影，送回它们应有的归宿。\" 他缓缓抬手，碎裂的石块开始悬浮、扭曲，\"让我看看，你这最后的挣扎，能掀起多大的波澜。\"";
    default:
        return "神秘将军：\"你...不应该来到这里...\"";
    }
}

std::string EvilGeneral::getDefeatDialogue() const {
    switch (evilType) {
    case EvilType::POWER_HUNGRY:
        return "厄休拉：\"不可能！我才是这片土地的主宰，权力怎么会抛弃我！\" 她眼中满是不甘与疯狂，\"就算我输了，还有其他将军！人类的软弱永远改不了，权力终会找到新的主人...\"";
    case EvilType::BETRAYAL:
        return "卡莱恩：\"安特国的星空下，连风都在唾弃你的名字...\" 他咳着血大笑，\"至少...我终于真正自由了...\"";
    case EvilType::LIE:
        return "玛尔索：\"忽悠不如卖菜大妈，翻车了！\" 他嘶吼着试图改写现实，\"我改剧本！\" 但真理誓约・明识之戒的光芒控住了他，\"现在，该让真实怼碎你的诳言了。\"";
    case EvilType::CRUELTY:
        return "克鲁尔萨：\"你的'乐章'，该用镣铐来陪葬了。\" 他单膝跪地，望着散落一地的棘刺笑到呛血，\"你的颤抖...才是我最得意的杰作。\"";
    case EvilType::DESIRELESS:
        return "灭欲：\"你的希望...战胜了我的虚无...\" 它的心楔破碎，被禁锢的情感爆发，\"或许...情感确实不是缺陷...\"";
    case EvilType::DESTRUCTION:
        return "尼赫尔：\"你赢了，王子...\" 他轻声说，声音如同风中碎语，没有痛苦，只有解脱，\"也许...虚无并不是唯一的归宿...\"";
    default:
        return "神秘将军：\"也许...这就是命运...\"";
    }
}

std::string EvilGeneral::getVictoryDialogue() const {
    switch (evilType) {
    case EvilType::POWER_HUNGRY:
        return "厄休拉：\"看看你这副狼狈样，还想终结我的暴政？真是可笑。\" 她掌心黑焰凑近你的脸颊，\"我早说过，人类是供我驱使的蝼蚁，你这落魄王子也不例外。今天留你一命，是想让你亲眼看看，我如何用权力掌控整个世界！\"";
    case EvilType::BETRAYAL:
        return "卡莱恩：\"看啊，这就是忠诚的重量——轻得像缕灰烬。\" 他踩着你的断剑，冷笑道，\"背叛才是这个世界的真理，弱者注定被抛弃！\"";
    case EvilType::LIE:
        return "玛尔索：\"神灵指引是画饼，你偏信？\" 他挥杖让污蔑告示发光，幻象斥声四起，\"大家都嫌你菜！逃兵王子，认怂吧！\"";
    case EvilType::CRUELTY:
        return "克鲁尔萨：\"猜猜看，你的骨头能撑住多少声惨叫？\" 镣铐穿透你的手腕锁在石柱上，棘刺扎进皮肉渗出幽蓝毒雾，\"你的'艺术'...终会被钉进坟墓。\"";
    case EvilType::DESIRELESS:
        return "灭欲：\"你的挣扎被心楔吸收...你的希望，已被证伪，你的存在，已被抹除，现在，归于宁静吧。\" 绝对的虚无将你包围。";
    case EvilType::DESTRUCTION:
        return "尼赫尔：\"你所守护的，不过是沙上城堡...看，它化为乌有。而你的爱与希望，也随着它一同消逝了。现在，你和我一样，一无所有。\" 他伸出手，邀请你一同拥抱那最终的虚无。";
    default:
        return "神秘将军：\"胜利...但这真的是我想要的吗？\"";
    }
}
