#include "EvilGeneral.h"
#include <iostream>
#include <cstdlib>

EvilGeneral::EvilGeneral(std::string name, EvilType type, std::string territory, int level)
    :CommonEnemy (EnemyType::BOSS,level), evilType(type), territory(territory) {
    setName(name);
    // 调整：更合理的将军面板（强于小怪，但可用技能打穿）
    setMaxHP(600 + level * 50);  // HP随等级递增
    setHP(getMaxHP());
    setATK(25 + level * 2);      // 攻击力中等偏高
    setDEF(12 + static_cast<int>(level * 1.5)); // 防御较高但可被技能穿透
    setSpeed(10 + static_cast<int>(level * 0.5));
    setCritRate(0.10f + level * 0.005f); // 略随等级提升
}

// 虚析构函数实现
EvilGeneral::~EvilGeneral() {
    // 清理资源
}

// 对话系统实现（加入主角台词穿插）
std::string EvilGeneral::getPreBattleDialogue() const {
    switch (evilType) {
    case EvilType::POWER_HUNGRY:
        return
            "你：\"你的暴政到此为止，厄休拉。\"\n"
            "厄休拉：\"落魄王子也敢闯我的领地？\" 她掌心翻出黑焰，语气满是轻蔑，\"人类本就是供我驱使的蝼蚁，这片土地的规则由我定！敢挑战我的权威，就先死在这些影子手里！\"";
        
    case EvilType::BETRAYAL:
        return
            "卡莱恩：\"知道吗？每次挥剑时我都能听见誓约碎裂的声音——真是悦耳极了。\"\n"
            "你：\"那今晚你会听到新的声音——背叛者骨头断裂的脆响。\"";
            
    case EvilType::LIE:
        return
            "玛尔索：\"家人们谁懂啊！当年首都破防，全是这王子跑路坑的！我这'救世预言'可是经过三重认证，绝对保真！\"\n"
            "你：\"编瞎话都不卡bug？今天就拆了你这忽悠局！\"";
            
    case EvilType::CRUELTY:
        return
            "克鲁尔萨：（抛接着染血怀表）\"听见他们骨头断裂的脆响了吗？比你神剑的嗡鸣更美妙。\"\n"
            "你：\"你的统治在今天到此为止！\"";
            
    case EvilType::DESIRELESS:
        return
            "灭欲：\"欢迎来到终极宁静的门槛。情感是噪音，希望是幻觉，爱是缺陷。我正在拯救他们，也将拯救你。\"\n"
            "你：\"希望不是幻觉，它是我前进的方向。\"";
            
    case EvilType::DESTRUCTION:
        return
            "你：\"终结不是毁灭，尼赫尔。\"\n"
            "尼赫尔：\"终结，从未到来。你我，不过是短暂的幻影。而我的存在，只是为了将所有幻影，送回它们应有的归宿。\" 他缓缓抬手，碎裂的石块开始悬浮、扭曲，\"让我看看，你这最后的挣扎，能掀起多大的波澜。\"";
            
    default:
        return "你：\"无论你是谁，都将止步于此。\"";
    }
}

std::string EvilGeneral::getDefeatDialogue() const {
    switch (evilType) {
    case EvilType::POWER_HUNGRY:
        return
            "你：\"你的权力建立在掠夺与压迫上，本就该覆灭。\"\n"
            "厄休拉：\"不可能！我才是这片土地的主宰，权力怎么会抛弃我！\" 她眼中满是不甘与疯狂，\"就算我输了，还有其他将军！人类的软弱永远改不了，权力终会找到新的主人……\"";
            
    case EvilType::BETRAYAL:
        return
            "你：\"安特国的星空下，连风都在唾弃你的名字。\"\n"
            "卡莱恩：（咳着血大笑）\"至少...我终于真正自由了...\"";
            
    case EvilType::LIE:
        return
            "你：\"现在，该让真实怼碎你的诳言了。\"\n"
            "玛尔索：（徽章碎裂，卷轴坠地）\"我改剧本！我改——\" 真理之戒的光芒束缚了他的谎言。";
            
    case EvilType::CRUELTY:
        return
            "你：\"你的'乐章'，该用镣铐来陪葬了。\"\n"
            "克鲁尔萨：（单膝跪地，带笑咳血）\"你的颤抖…… 才是我最得意的杰作。\"";
            
    case EvilType::DESIRELESS:
        return
            "你：\"希望，终将点亮他们的眼睛。\"\n"
            "灭欲：\"你的希望...战胜了我的虚无...\" 被禁锢的情感从它体内溢出。";
            
    case EvilType::DESTRUCTION:
        return
            "你：\"回归尘土吧，虚无不是答案。\"\n"
            "尼赫尔：\"你赢了，王子...\" 他轻声说，声音如同风中碎语，没有痛苦，只有解脱。";
            
    default:
        return "你：\"落幕。\"";
    }
}

std::string EvilGeneral::getVictoryDialogue() const {
    switch (evilType) {
    case EvilType::POWER_HUNGRY:
        return
            "厄休拉：\"看看你这副狼狈样，还想终结我的暴政？真是可笑。\" 她掌心黑焰逼近，\"今天留你一命，只为让你亲眼看着，我如何用权力掌控整个世界！\"\n"
            "你：\"我会回来。那时，黑焰将熄。\"";
            
    case EvilType::BETRAYAL:
        return
            "卡莱恩：（踩着你的断剑）\"看啊，这就是忠诚的重量——轻得像缕灰烬。\"\n"
            "你：（紧抓胸甲）\"背叛，会比死亡更沉重。\"";
            
    case EvilType::LIE:
        return
            "玛尔索：\"神灵指引是画饼，你偏信？\" 他挥杖让污蔑告示发光，幻象斥声四起，\"大家都嫌你菜！逃兵王子，认怂吧！\"\n"
            "你：\"我的回答，会在下次挥剑时给出。\"";
            
    case EvilType::CRUELTY:
        return
            "克鲁尔萨：（镣铐穿透你的手腕）\"猜猜看，你的骨头能撑住多少声惨叫？你的'艺术'... 终会被钉进坟墓。\"\n"
            "你：\"怜悯之链，不会被毒雾吞噬。\"";
            
    case EvilType::DESIRELESS:
        return
            "灭欲：\"你的挣扎被心楔吸收……你的希望，已被证伪，你的存在，已被抹除，现在，归于宁静吧。\"\n"
            "你：\"只要一人仍怀希望，我便不屈。\"";
            
    case EvilType::DESTRUCTION:
        return
            "尼赫尔：\"你所守护的，不过是沙上城堡……看，它化为乌有。而你的爱与希望，也随着它一同消逝了。现在，你和我一样，一无所有。\"\n"
            "你：\"希望不死，城堡可再建。\"";
            
    default:
        return "你：\"我会再来。\"";
    }
}
