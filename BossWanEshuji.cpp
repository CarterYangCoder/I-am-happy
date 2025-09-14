#include "BossWanEshuji.h"
#include <algorithm> // 新增：确保 std::min 可用

BossWanEshuji::BossWanEshuji()
    : EvilGeneral("万恶枢机", EvilType::BOSS, "混沌之心", 30), phase(1) {
    // 调整：初始面板更合理（仍为终极BOSS）
    setMaxHP(1800);
    setHP(maxHp);
    setATK(120);
    setDEF(60);
    setSpeed(22);
    setCritRate(0.20f);
}

// 进入下一阶段（生命值低于33%进入阶段2，低于10%进入阶段3）
void BossWanEshuji::enterNextPhase() {
    if (phase >= 3) return;
    phase++;
    std::cout << "万恶枢机进入第 " << phase << " 阶段！" << std::endl;
    
    // 调整：阶段强化幅度略降，避免不可打
    switch (phase) {
        case 2:
            setATK(static_cast<int>(getATK() * 1.20));
            setSpeed(static_cast<int>(getSpeed() * 1.15));
            setDEF(static_cast<int>(getDEF() * 1.12));
            setCritRate(std::min(0.35f, getCritRate() + 0.08f));
            std::cout << "万恶枢机的混沌之力觉醒！攻击与速度提升，获得新技能！" << std::endl;
            break;
        case 3:
            setATK(static_cast<int>(getATK() * 1.25));
            setSpeed(static_cast<int>(getSpeed() * 1.15));
            setDEF(static_cast<int>(getDEF() * 1.12));
            setCritRate(std::min(0.45f, getCritRate() + 0.10f));
            std::cout << "万恶枢机进入最终形态！力量攀升至顶点！" << std::endl;
            break;
    }
    phaseTransitionEffect(); // 触发阶段转换特效
}

int BossWanEshuji::getPhase() const { return phase; }

// 特殊技能：混沌风暴（只有第三阶段才能使用）
bool BossWanEshuji::canUseChaosStorm() const {
    return phase >= 3;
}

// 特殊技能：暗影冲击（第二阶段开始可用）
bool BossWanEshuji::canUseShadowStrike() const {
    return phase >= 2;
}

// 获取当前阶段的攻击描述
std::string BossWanEshuji::getPhaseAttackDescription() const {
    switch (phase) {
        case 1:
            return "用混沌之力";
        case 2:
            return "混沌之力已觉醒，力量更加强大";
        case 3:
            return "最终形态的恐怖力量";
        default:
            return "未知的邪恶力量";
    }
}

// 阶段转换时的特殊效果（回血比例略降，更公平）
void BossWanEshuji::phaseTransitionEffect() {
    switch (phase) {
        case 2:
            heal(static_cast<int>(getMaxHP() * 0.10)); // 调整：10%
            std::cout << "万恶枢机从混沌之力中汲取力量，恢复了部分生命！" << std::endl;
            break;
        case 3:
            heal(static_cast<int>(getMaxHP() * 0.15)); // 调整：15%
            std::cout << "万恶枢机释放原始混沌力量，恢复生命！" << std::endl;
            break;
    }
}

// 检查是否应该进入下一阶段
bool BossWanEshuji::shouldEnterNextPhase() const {
    double hpPercentage = static_cast<double>(getHP()) / getMaxHP();
    
    if (phase == 1 && hpPercentage <= 0.33) {
        return true;
    }
    if (phase == 2 && hpPercentage <= 0.10) {
        return true;
    }
    return false;
}

// 对话系统实现
std::string BossWanEshuji::getPreBattleDialogue() const {
    return
        "你：\"只要信任、希望与爱尚存，你的规则注定崩坏。\"\n"
        "万恶枢机：\"欢迎，安特国的王子。\" 它发出六位将军交织的重叠声音，散发着令人窒息的压迫感，"
        "你以为战胜我的分身便是胜利？他们不过是我播撒的种子，而你——你的愤怒、你的复仇、你的痛苦，"
        "正是滋养我的温床。只要人类心存欲望，我的规则便永不消亡！";
}

std::string BossWanEshuji::getDefeatDialogue() const {
    switch (phase) {
    case 1:
        return "你：\"你的第一重外壳，已经裂开。\"\n万恶枢机：\"第一阶段的失利...不过是开始...我还有更强大的形态...你这点力量...还远远不够...\"";
    case 2:
        return "你：\"觉醒的混沌，也挡不住希望之刃。\"\n万恶枢机：\"连觉醒的混沌力量也...无法阻止你吗？你的意志...确实强大...但这还不是...终结...\"";
    case 3:
        return "你：\"规则，应由信任、希望与爱书写。\"\n万恶枢机：\"不...我才是规则！\" 它在刺目白光中崩解，发出不甘的嘶吼。";
    default:
        return "你：\"落幕。\"";
    }
}

std::string BossWanEshuji::getVictoryDialogue() const {
    switch (phase) {
    case 1:
        return "万恶枢机：\"第一阶段的胜利只是开始...你还没有见识到真正的绝望...准备迎接更强大的混沌力量吧！\"\n你：\"我会踏破每一层黑暗。\"";
    case 2:
        return "万恶枢机：\"觉醒状态下的力量让你倒下了...但这还不是我的极限...最终形态将彻底碾碎你的希望！\"\n你：\"希望不灭。\"";
    case 3:
        return "万恶枢机：\"看，你所守护的，终归虚无。\" 以虚无之力将你抛回废墟，天空被吞噬。\n你：\"只要一人仍怀希望……我便永不屈服。\"";
    default:
        return "万恶枢机：\"胜利属于混沌...\"";
    }
}

