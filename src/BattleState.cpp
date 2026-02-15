#include "BattleState.h"
#include "Player.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include "OverworldGui.h"
#include "Types.h"
#include "Utils.h"

Move *BattleState::STRUGGLE = nullptr;
Move *BattleState::CONFUSION = nullptr;
Move *BattleState::RECHARGE = nullptr;
Move *BattleState::ROAR = nullptr;
Move *BattleState::TELEPORT = nullptr;
Move *BattleState::WHIRLWIND = nullptr;
Item *BattleState::LUCKY_EGG = nullptr;

std::vector<int> BattleState::EXPERIENCE_TABLE = {10000,64,141,208,65,142,209,66,143,210,53,72,160,52,71,159,55,113,172,57,116,58,162,62,147,82,122,93,163,59,117,194,60,118,195,68,129,63,178,76,
        109,54,171,78,132,184,70,128,75,138,81,153,69,148,80,174,74,149,91,213,77,131,185,73,145,186,88,146,193,84,151,191,105,205,86,134,177,152,192,99,164,89,169,94,96,158,
        100,176,90,157,97,203,95,126,190,108,102,165,115,206,103,150,98,212,87,124,139,140,127,114,173,135,204,255,166,175,83,155,111,170,106,207,136,187,137,156,167,200,211,
        20,214,219,61,92,196,197,198,130,120,199,119,201,202,154,215,216,217,67,144,218,220,154,1000};

std::vector<std::string> BattleState::ACTIONS = {"Fight", "Pokemon", "Item", "Run"};

static void initStatics() {
    if (!BattleState::STRUGGLE) {
        BattleState::STRUGGLE = Move::MOVE_MAP["Struggle"];
        BattleState::CONFUSION = Move::MOVE_MAP["CONFUSION"];
        BattleState::RECHARGE = Move::MOVE_MAP["RECHARGE"];
        BattleState::ROAR = Move::MOVE_MAP["Roar"];
        BattleState::TELEPORT = Move::MOVE_MAP["Teleport"];
        BattleState::WHIRLWIND = Move::MOVE_MAP["Whirlwind"];
        BattleState::LUCKY_EGG = Item::ITEM_MAP["Lucky Egg"];
    }
}

static int autoBattleMoveDex(BattleState *b) {
    if (!b) {
        return -1;
    }
    for (int i = 0; i < 4; i++) {
        if (b->moves[i] == nullptr) {
            break;
        }
        if (b->monster->pp[i] <= 0) {
            continue;
        }
        if (b->dTurns > 0 && b->disabled == i) {
            continue;
        }
        return i;
    }
    return -1;
}

int BattleState::getMoveDex(BattleState *b) {
    if (b->shouldStruggle()) {
        return -1;
    }
    auto refreshMoveMenu = [&]() {
        for (int i = 0; i < 4; i++) {
            OverworldGui::strArr[i] = b->moves[i] == nullptr ? "" : b->monster->moves[i]->name + " (" +
                                                                          std::to_string(b->monster->pp[i]) + "/" +
                                                                          std::to_string(b->monster->mpp[i]) + ")";
        }
    };

    refreshMoveMenu();
    while (true) {
        int moveDex = guiChoice(3);
        if (moveDex == -1) {
            return -2;
        }
        if (b->moves[moveDex] == nullptr) {
            OverworldGui::print("You don't know enough moves!");
            refreshMoveMenu();
        } else if (b->monster->pp[moveDex] == 0) {
            OverworldGui::print("You don't have any PP!");
            refreshMoveMenu();
        } else if (b->dTurns > 0 && b->disabled == moveDex) {
            OverworldGui::print("That move is disabled!");
            refreshMoveMenu();
        } else {
            return moveDex;
        }
    }
}

int BattleState::randomMoveDex(BattleState *b) {
    std::vector<int> l;
    for (int i = 0; i < 4; i++) {
        if (b->moves[i] == nullptr) {
            break;
        } else if ((b->dTurns == 0 || b->disabled != i) && b->monster->pp[i] > 0) {
            l.push_back(i);
        }
    }
    return l.empty() ? -1 : l[utils::randInt(0, static_cast<int>(l.size()))];
}

bool BattleState::blackedOut(const std::vector<Battler*> &monsters) {
    for (size_t i = 0; i < monsters.size(); i++) {
        if (monsters[i] == nullptr) {
            return true;
        } else if (monsters[i]->hp > 0) {
            return false;
        }
    }
    return true;
}

int BattleState::coinsPickup(BattleState *pState, BattleState *oState) {
    int coins = pState->coins + oState->coins;
    if (coins > 0) {
        OverworldGui::print("You picked up " + std::to_string(coins) + " scattered coins!");
    }
    return coins;
}

bool BattleState::canSwitch(const std::vector<Battler*> &monsters) {
    int r = 0;
    for (size_t i = 0; i < monsters.size(); i++) {
        if (monsters[i] == nullptr) {
            break;
        } else if (monsters[i]->hp > 0) {
            r++;
        }
    }
    return r > 1;
}

int BattleState::switchMon(std::vector<Battler*> &monsters, int i, bool mustSwitch) {
    OverworldGui::usableItems.clear();
    OverworldGui::longArr = Battler::partyStrings(monsters);
    OverworldGui::longListHeaderOverride = "Party";
    OverworldGui::checkingPokes = false;
    OverworldGui::checkingMoves = false;
    while (true) {
        OverworldGui::choosingFromLongList = true;
        int monDex = guiChoice(5);
        OverworldGui::choosingFromLongList = false;
        if (monDex < 0 || monDex >= static_cast<int>(monsters.size())) {
            if (mustSwitch) {
                OverworldGui::print("You must switch!");
                OverworldGui::longArr = Battler::partyStrings(monsters);
                OverworldGui::longListHeaderOverride = "Party";
                OverworldGui::choosingFromLongList = true;
                continue;
            }

            OverworldGui::longListHeaderOverride.clear();
            return -1;
        }
        if (monDex == i) {
            OverworldGui::print("No! You have to switch pokemon!");
            OverworldGui::longArr = Battler::partyStrings(monsters);
            OverworldGui::longListHeaderOverride = "Party";
            OverworldGui::choosingFromLongList = true;
        } else if (monsters[monDex] == nullptr) {
            OverworldGui::print("You don't have enough pokemon!");
            OverworldGui::longArr = Battler::partyStrings(monsters);
            OverworldGui::longListHeaderOverride = "Party";
            OverworldGui::choosingFromLongList = true;
        } else if (monsters[monDex]->hp == 0) {
            OverworldGui::print("No! That pokemon has already fainted!");
            OverworldGui::longArr = Battler::partyStrings(monsters);
            OverworldGui::longListHeaderOverride = "Party";
            OverworldGui::choosingFromLongList = true;
        } else {
            OverworldGui::longListHeaderOverride.clear();
            return monDex;
        }
    }
}

void BattleState::seed(BattleState *sucker, BattleState *sucked) {
    sucker->spdefTurns = std::max(0, sucker->spdefTurns - 1);
    sucker->defTurns = std::max(0, sucker->defTurns - 1);
    if (!sucked->seeded || sucker->monster->hp <= 0 || sucked->monster->hp <= 0) {
        return;
    }
    int d = std::max(1, std::min(sucked->monster->hp, sucked->monster->mhp / 16));
    sucked->monster->hp -= d;
    OverworldGui::print(sucker->monster->nickname + " sucked " + std::to_string(d) + " health from the seed!");
    sucker->monster->hp = std::min(sucker->monster->mhp, sucker->monster->hp + d);
}

void BattleState::gainXp(Player *p, const std::unordered_set<Battler*> &participants, Battler *fainted, bool trainer, int lucky) {
    int l = static_cast<int>(participants.size());
    int xp = EXPERIENCE_TABLE[fainted->dexNum] * fainted->level / 7;
    int per = xp / l;
    int atk = fainted->bAtk / l;
    int def = fainted->bDef / l;
    int spatk = fainted->bSpatk / l;
    int spdef = fainted->bSpdef / l;
    int hp = fainted->bHp / l;
    int spd = fainted->bSpd / l;
    per *= lucky;
    if (trainer) {
        per *= 3;
        per /= 2;
    }
    for (Battler *b : participants) {
        b->gainXp(p, b == monster ? moves : b->moves, per, atk, def, spatk, spdef, hp, spd);
        if (b == monster) {
            moves = monster->moves;
        }
    }
}

int BattleState::guiChoice(int m) {
    return OverworldGui::waitForChoice(m);
}

BattleState::BattleState(Battler *monster) {
    initStatics();
    this->monster = monster;
    monsters.clear();
    monsters.push_back(monster);
    critMul = 1;
    spd = monster->spd;
    atk = monster->atk;
    def = monster->def;
    spatk = monster->spatk;
    spdef = monster->spdef;
    types = monster->types;
    if (monster->status == "SLEEPING" || monster->status == "FROZEN") {
        sTurns = 2 + utils::randInt(0, 4);
    }
    lastDamage[0] = 0;
    lastDamage[1] = 0;
    moves = monster->moves;
    canLower = true;
}

BattleState::BattleState(Battler *monster, BattleState *b) : BattleState(monster) {
    spdefTurns = b->spdefTurns;
    defTurns = b->defTurns;
}

double BattleState::effect(int stage) {
    return stage == 0 ? 1.0 : (stage < 0 ? 2.0 / (2 - stage) : (stage + 2) / 2.0);
}

double BattleState::speed() {
    return (monster->status == "PARALYZED" ? 0.5 : 1.0) * effect(spdStage);
}

double BattleState::accuracy(int stage) {
    return stage == 0 ? 1.0 : (stage < 0 ? 3.0 / (3 - stage) : (stage + 3) / 3.0);
}

bool BattleState::goBefore(Move *m, BattleState *b, Move *o) {
    int mp = m->priority();
    int op = o->priority();
    if (mp == op) {
        double s1 = spd * effect(spdStage);
        double s2 = b->spd * effect(b->spdStage);
        return s1 == s2 ? utils::rand01() < 0.5 : s1 > s2;
    }
    return mp > op;
}

double BattleState::stab(Move *m) {
    for (const std::string &t : types) {
        if (t == m->type) {
            return 1.5;
        }
    }
    return 1.0;
}

bool BattleState::hasType(const std::string &type) {
    for (const std::string &t : types) {
        if (t == type) {
            return true;
        }
    }
    return false;
}

bool BattleState::canMove() {
    if (sTurns == 0) {
        return true;
    }
    if (--sTurns == 0) {
        OverworldGui::print(monster->nickname + (monster->status == "SLEEPING" ? " woke up!" : "thawed out!"));
        monster->status = "";
        return true;
    }
    return false;
}

int BattleState::calcDamage(Move *m, BattleState *b) {
    double mul = Types::damage(*m, b->types);
    double d;
    if (mul == 0.0) {
        return 0;
    } else if (mul == 1.0) {
        es = "";
    } else if (mul == 2.0) {
        es = "It was super effective! ";
    } else if (mul == 4.0) {
        es = "It was crazy effective!! ";
    } else if (mul == 0.5) {
        es = "It was not very effective. ";
    } else if (mul == 0.25) {
        es = "It was very ineffective. ";
    } else {
        es = "I am so confused. ";
    }
    crit = utils::rand01() * 24 / critMul < m->critChance();
    if (m->atkType == "Physical") {
        if (crit) {
            d = ((2.0 * monster->level * 2 / 5 + 2) * m->power * atk * effect(std::max(0, atkStage)) / b->def /
                     effect(std::min(0, b->defStage)) / 50 +
                 2) *
                stab(m) * mul * (217 + utils::randInt(0, 39)) / 255;
        } else {
            d = ((2.0 * monster->level / 5 + 2) * m->power * atk * effect(atkStage) / b->def / effect(b->defStage) / 50 +
                 2) *
                stab(m) * mul * (217 + utils::randInt(0, 39)) / 255;
            if (b->defTurns > 0) {
                d /= 2;
            }
        }
        if (monster->status == "BURNED") {
            d /= 2;
        }
        return std::max(1, static_cast<int>(d));
    }
    if (m->atkType == "Special") {
        if (crit) {
            d = ((2.0 * monster->level * 2 / 5 + 2) * m->power * spatk * effect(std::max(0, spatkStage)) / b->spdef /
                     effect(std::min(0, b->spdefStage)) / 50 +
                 2) *
                stab(m) * mul * (217 + utils::randInt(0, 39)) / 255;
        } else {
            d = ((2.0 * monster->level / 5 + 2) * m->power * spatk * effect(spatkStage) / b->spdef / effect(b->spdefStage) / 50 +
                 2) *
                stab(m) * mul * (217 + utils::randInt(0, 39)) / 255;
            if (b->spdefTurns > 0) {
                d /= 2;
            }
        }
        return std::max(1, static_cast<int>(d));
    }
    if (m->atkType == "Status") {
        return 0;
    }
    throw std::runtime_error(m->atkType);
}

bool BattleState::setLastMove(Move *m, int d) {
    if (subHp > 0) {
        subHp -= d;
        if (subHp <= 0) {
            subHp = 0;
            OverworldGui::print("The substitute broke!");
        }
    } else {
        monster->hp -= d;
    }
    if (monster->hp <= 0) {
        return false;
    }
    if (raging && d > 0) {
        atkStage++;
        OverworldGui::print(monster->nickname + "'s rage is building...");
        if (atkStage == 7) {
            OverworldGui::print("But it can't get any angrier!");
        }
    }
    lastMove = m;
    lastDamage[0] = lastDamage[1];
    lastDamage[1] = d;
    wasPhysical = m != nullptr && m->atkType == "Physical";
    return true;
}

bool BattleState::endTurn() {
    int d;
    flinched = false;
    if (tTurns > 0) {
        tTurns--;
        d = std::max(monster->mhp / 16, 1);
        OverworldGui::print(monster->nickname + " was squeezed for " + std::to_string(d) + " damage!");
        monster->hp -= d;
    }
    if (monster->status == "BURNED") {
        d = std::max(monster->mhp / 16, 1);
        OverworldGui::print(monster->nickname + " took " + std::to_string(d) + " burn damage!");
        monster->hp -= d;
    } else if (monster->status == "POISONED") {
        d = std::max(monster->mhp / 16, 1);
        if (poisonDamage > 0) {
            d *= poisonDamage++;
        }
        OverworldGui::print(monster->nickname + " took " + std::to_string(d) + " poison damage!");
        monster->hp -= d;
    }
    monster->hp = std::max(0, monster->hp);
    if (monster->hp <= 0) {
        OverworldGui::print(monster->nickname + " fainted!");
        return true;
    }
    return false;
}

bool BattleState::disable(Move *m, int a, int b) {
    for (disabled = 0; disabled < static_cast<int>(moves.size()); disabled++) {
        if (moves[disabled] == m) {
            dTurns = a + utils::randInt(0, b);
            return true;
        }
    }
    return false;
}

bool BattleState::shouldStruggle() {
    for (size_t i = 0; i < monster->pp.size(); i++) {
        if ((dTurns == 0 || disabled != static_cast<int>(i)) && monster->pp[i] > 0) {
            return false;
        }
    }
    return true;
}

void BattleState::resetStats() {
    accStage = 0;
    evsnStage = 0;
    spdStage = 0;
    atkStage = 0;
    defStage = 0;
    spatkStage = 0;
    spdefStage = 0;
    critMul = 1;
    canLower = true;
}

void BattleState::processMiss(Move *m) {
    if (m->effects.empty()) {
        return;
    }
    const Effect &e = m->effects[0];
    if (e.effect == "MISS") {
        if (e.stat == "max_hp") {
            monster->hp = std::max(0, monster->hp - static_cast<int>(e.modifier * monster->mhp));
            OverworldGui::print("It fell and lost " + std::to_string(e.modifier * 100) + " percent of its max health!");
        } else {
            throw std::runtime_error(e.stat);
        }
    } else if (e.effect == "hp") {
        if (e.me && e.stat.empty()) {
            monster->hp = std::max(0, monster->hp - e.amount);
            OverworldGui::print("But " + monster->nickname + " still exploded!");
        }
    }
}

bool BattleState::doMove(Move *m, BattleState *b) {
    if (dTurns > 0 && --dTurns == 0) {
        OverworldGui::print(monster->nickname + " is no longer disabled!");
    }
    nextMove = nullptr;
    immune = false;
    raging = false;
    if (b->monster->hp <= 0) {
        return false;
    }
    if (flinched) {
        flinched = false;
        OverworldGui::print(monster->nickname + " flinched and cannot move!");
        return true;
    }
    if (sTurns > 0) {
        OverworldGui::print(monster->nickname + (monster->status == "SLEEPING" ? " is fast asleep!" : " is frozen solid!"));
        return true;
    }
    if (monster->status == "PARALYZED" && utils::rand01() < 0.25) {
        OverworldGui::print(monster->nickname + " is fully paralyzed!");
        return true;
    }
    if (cTurns > 0) {
        cTurns--;
        if (utils::rand01() < 0.5) {
            int damage = calcDamage(CONFUSION, this);
            monster->hp = std::max(0, monster->hp - damage);
            OverworldGui::print(monster->nickname + " hit itself in confusion for " + std::to_string(damage) + " damage!");
            return true;
        }
    }
    OverworldGui::print(monster->nickname + " used " + m->name + "!");
    if (b->immune || utils::rand01() * 100 >= m->acc * accuracy(accStage) / accuracy(b->evsnStage)) {
        OverworldGui::print("The attack missed!");
        processMiss(m);
        return true;
    }
    int damage = calcDamage(m, b);
    if (damage == 0 && m->atkType != "Status") {
        OverworldGui::print("It had no effect!");
        processMiss(m);
        return true;
    }
    if (m->shouldPrintDamage()) {
        if (crit) {
            OverworldGui::print("Critical hit!");
        }
        OverworldGui::print(es + "It did " + std::to_string(damage) + " damage!");
    }
    for (size_t i = 0; i < m->effects.size(); i++) {
        const Effect &e = m->effects[i];
        if (utils::rand01() < e.acc) {
            if (e.effect == "hp") {
                int r;
                if (e.stat == "damage") {
                    r = std::max(1, static_cast<int>(std::min(damage, b->monster->hp) * e.modifier));
                    if (e.up) {
                        monster->hp = std::min(monster->mhp, monster->hp + r);
                        OverworldGui::print(monster->nickname + " gained " + std::to_string(r) + " health!");
                    } else {
                        monster->hp = std::max(0, monster->hp - r);
                        OverworldGui::print(monster->nickname + " took " + std::to_string(r) + " damage in recoil!");
                    }
                } else if (e.stat == "physical_damage") {
                    if (wasPhysical) {
                        damage = lastDamage[1] * 2;
                        OverworldGui::print(monster->nickname + " countered for " + std::to_string(damage) + " damage!");
                    } else {
                        damage = 0;
                        OverworldGui::print("But " + monster->nickname + " wasn't hit by a physical attack!");
                    }
                } else if (e.stat == "max_hp") {
                    if (monster->hp == monster->mhp) {
                        OverworldGui::print(monster->nickname + " already has full health!");
                    } else {
                        r = monster->mhp / 2;
                        monster->hp = std::min(monster->mhp, monster->hp + r);
                        OverworldGui::print(monster->nickname + " recovered " + std::to_string(r) + " health!");
                    }
                } else if (e.stat == "oppHp") {
                    damage = b->monster->hp / 2;
                    if (damage == 0) {
                        OverworldGui::print("But dingus, you can't KO a pokemon with Super Fang!");
                    }
                } else if (e.stat == "level") {
                    damage = monster->level;
                    OverworldGui::print(monster->nickname + "'s level determined the damage!");
                } else if (e.stat.empty()) {
                    if (e.up) {
                        if (monster->hp == monster->mhp) {
                            OverworldGui::print(monster->nickname + " already has full health!");
                            return true;
                        }
                        r = monster->mhp - monster->hp;
                        monster->hp = monster->mhp;
                        OverworldGui::print(monster->nickname + " recovered " + std::to_string(r) + " health!");
                    } else if (e.me) {
                        r = std::min(monster->mhp, e.amount);
                        monster->hp -= r;
                        OverworldGui::print(monster->nickname + " lost " + std::to_string(e.amount) + " health!");
                    } else {
                        damage = e.amount;
                        OverworldGui::print(b->monster->nickname + " took " + std::to_string(e.amount) + " damage!");
                    }
                } else {
                    throw std::runtime_error(e.stat);
                }
            } else if (e.effect == "spdef") {
                if (e.me) {
                    if (e.up) {
                        spdefStage += e.amount;
                        OverworldGui::print(monster->nickname + "'s special defense increased by " + std::to_string(e.amount) + " stages!");
                        if (spdefStage > 6) {
                            spdefStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else {
                        spdefStage -= e.amount;
                        OverworldGui::print(monster->nickname + "'s special defense dropped by " + std::to_string(e.amount) + " stages!");
                        if (spdefStage < -6) {
                            spdefStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    }
                } else {
                    if (b->subHp > 0) {
                        OverworldGui::print("But " + b->monster->nickname + " has a substitute!");
                    } else if (e.up) {
                        b->spdefStage += e.amount;
                        OverworldGui::print(b->monster->nickname + "'s special defense increased by " + std::to_string(e.amount) + " stages!");
                        if (b->spdefStage > 6) {
                            b->spdefStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else if (b->canLower) {
                        b->spdefStage -= e.amount;
                        OverworldGui::print(b->monster->nickname + "'s special defense dropped by " + std::to_string(e.amount) + " stages!");
                        if (b->spdefStage < -6) {
                            b->spdefStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    } else {
                        OverworldGui::print(b->monster->nickname + "'s stats cannot be lowered!");
                    }
                }
            } else if (e.effect == "def") {
                if (e.me) {
                    if (e.up) {
                        defStage += e.amount;
                        OverworldGui::print(monster->nickname + "'s defense increased by " + std::to_string(e.amount) + " stages!");
                        if (defStage > 6) {
                            defStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else {
                        defStage -= e.amount;
                        OverworldGui::print(monster->nickname + "'s defense dropped by " + std::to_string(e.amount) + " stages!");
                        if (defStage < -6) {
                            defStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    }
                } else {
                    if (b->subHp > 0) {
                        OverworldGui::print("But " + b->monster->nickname + " has a substitute!");
                    } else if (e.up) {
                        b->defStage += e.amount;
                        OverworldGui::print(b->monster->nickname + "'s defense increased by " + std::to_string(e.amount) + " stages!");
                        if (b->defStage > 6) {
                            b->defStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else if (b->canLower) {
                        b->defStage -= e.amount;
                        OverworldGui::print(b->monster->nickname + "'s defense dropped by " + std::to_string(e.amount) + " stages!");
                        if (b->defStage < -6) {
                            b->defStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    } else {
                        OverworldGui::print(b->monster->nickname + "'s stats cannot be lowered!");
                    }
                }
            } else if (e.effect == "spd") {
                if (e.me) {
                    if (e.up) {
                        spdStage += e.amount;
                        OverworldGui::print(monster->nickname + "'s speed increased by " + std::to_string(e.amount) + " stages!");
                        if (spdStage > 6) {
                            spdStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else {
                        spdStage -= e.amount;
                        OverworldGui::print(monster->nickname + "'s speed dropped by " + std::to_string(e.amount) + " stages!");
                        if (spdStage < -6) {
                            spdStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    }
                } else {
                    if (b->subHp > 0) {
                        OverworldGui::print("But " + b->monster->nickname + " has a substitute!");
                    } else if (e.up) {
                        b->spdStage += e.amount;
                        OverworldGui::print(b->monster->nickname + "'s speed increased by " + std::to_string(e.amount) + " stages!");
                        if (b->spdStage > 6) {
                            b->spdStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else if (b->canLower) {
                        b->spdStage -= e.amount;
                        OverworldGui::print(b->monster->nickname + "'s speed dropped by " + std::to_string(e.amount) + " stages!");
                        if (b->spdStage < -6) {
                            b->spdStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    } else {
                        OverworldGui::print(b->monster->nickname + "'s stats cannot be lowered!");
                    }
                }
            } else if (e.effect == "atk") {
                if (e.me) {
                    if (e.up) {
                        atkStage += e.amount;
                        OverworldGui::print(monster->nickname + "'s attack increased by " + std::to_string(e.amount) + " stages!");
                        if (atkStage > 6) {
                            atkStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else {
                        atkStage -= e.amount;
                        OverworldGui::print(monster->nickname + "'s attack dropped by " + std::to_string(e.amount) + " stages!");
                        if (atkStage < -6) {
                            atkStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    }
                } else {
                    if (b->subHp > 0) {
                        OverworldGui::print("But " + b->monster->nickname + " has a substitute!");
                    } else if (e.up) {
                        b->atkStage += e.amount;
                        OverworldGui::print(b->monster->nickname + "'s attack increased by " + std::to_string(e.amount) + " stages!");
                        if (b->atkStage > 6) {
                            b->atkStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else if (b->canLower) {
                        b->atkStage -= e.amount;
                        OverworldGui::print(b->monster->nickname + "'s attack dropped by " + std::to_string(e.amount) + " stages!");
                        if (b->atkStage < -6) {
                            b->atkStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    } else {
                        OverworldGui::print(b->monster->nickname + "'s stats cannot be lowered!");
                    }
                }
            } else if (e.effect == "spatk") {
                if (e.me) {
                    if (e.up) {
                        spatkStage += e.amount;
                        OverworldGui::print(monster->nickname + "'s special attack increased by " + std::to_string(e.amount) + " stages!");
                        if (spatkStage > 6) {
                            spatkStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else {
                        spatkStage -= e.amount;
                        OverworldGui::print(monster->nickname + "'s special attack dropped by " + std::to_string(e.amount) + " stages!");
                        if (spatkStage < -6) {
                            spatkStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    }
                } else {
                    if (b->subHp > 0) {
                        OverworldGui::print("But " + b->monster->nickname + " has a substitute!");
                    } else if (e.up) {
                        b->spatkStage += e.amount;
                        OverworldGui::print(b->monster->nickname + "'s special attack increased by " + std::to_string(e.amount) + " stages!");
                        if (b->spatkStage > 6) {
                            b->spatkStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else if (b->canLower) {
                        b->spatkStage -= e.amount;
                        OverworldGui::print(b->monster->nickname + "'s special attack dropped by " + std::to_string(e.amount) + " stages!");
                        if (b->spatkStage < -6) {
                            b->spatkStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    } else {
                        OverworldGui::print(b->monster->nickname + "'s stats cannot be lowered!");
                    }
                }
            } else if (e.effect == "evasion") {
                if (e.me) {
                    if (e.up) {
                        evsnStage += e.amount;
                        OverworldGui::print(monster->nickname + "'s evasion increased by " + std::to_string(e.amount) + " stages!");
                        if (evsnStage > 6) {
                            evsnStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else {
                        evsnStage -= e.amount;
                        OverworldGui::print(monster->nickname + "'s evasion dropped by " + std::to_string(e.amount) + " stages!");
                        if (evsnStage < -6) {
                            evsnStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    }
                } else {
                    if (b->subHp > 0) {
                        OverworldGui::print("But " + b->monster->nickname + " has a substitute!");
                    } else if (e.up) {
                        b->evsnStage += e.amount;
                        OverworldGui::print(b->monster->nickname + "'s evasion increased by " + std::to_string(e.amount) + " stages!");
                        if (b->evsnStage > 6) {
                            b->evsnStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else if (b->canLower) {
                        b->evsnStage -= e.amount;
                        OverworldGui::print(b->monster->nickname + "'s evasion dropped by " + std::to_string(e.amount) + " stages!");
                        if (b->evsnStage < -6) {
                            b->evsnStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    } else {
                        OverworldGui::print(b->monster->nickname + "'s stats cannot be lowered!");
                    }
                }
            } else if (e.effect == "accuracy") {
                if (e.me) {
                    if (e.up) {
                        accStage += e.amount;
                        OverworldGui::print(monster->nickname + "'s accuracy increased by " + std::to_string(e.amount) + " stages!");
                        if (accStage > 6) {
                            accStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else {
                        accStage -= e.amount;
                        OverworldGui::print(monster->nickname + "'s accuracy dropped by " + std::to_string(e.amount) + " stages!");
                        if (accStage < -6) {
                            accStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    }
                } else {
                    if (b->subHp > 0) {
                        OverworldGui::print("But " + b->monster->nickname + " has a substitute!");
                    } else if (e.up) {
                        b->accStage += e.amount;
                        OverworldGui::print(b->monster->nickname + "'s accuracy increased by " + std::to_string(e.amount) + " stages!");
                        if (b->accStage > 6) {
                            b->accStage = 6;
                            OverworldGui::print("Although it maxed out at 6!");
                        }
                    } else if (b->canLower) {
                        b->accStage -= e.amount;
                        OverworldGui::print(b->monster->nickname + "'s accuracy dropped by " + std::to_string(e.amount) + " stages!");
                        if (b->accStage < -6) {
                            b->accStage = -6;
                            OverworldGui::print("Although it bottomed out at -6!");
                        }
                    } else {
                        OverworldGui::print(b->monster->nickname + "'s stats cannot be lowered!");
                    }
                }
            } else if (e.effect == "crit_chance") {
                critMul *= e.amount;
                OverworldGui::print(b->monster->nickname + " is now " + std::to_string(e.amount) + " times more likely to get a critical hit!");
            } else if (e.effect == "POISON") {
                if (b->monster->status.empty() && !b->hasType("Poison") && b->subHp == 0) {
                    b->monster->status = "POISONED";
                    OverworldGui::print(b->monster->nickname + " was poisoned!");
                } else {
                    OverworldGui::print(b->monster->nickname + " cannot be poisoned!");
                }
            } else if (e.effect == "TOXIC") {
                if (b->monster->status.empty() && !b->hasType("Poison") && b->subHp == 0) {
                    b->monster->status = "POISONED";
                    b->poisonDamage = 1;
                    OverworldGui::print(b->monster->nickname + " was poisoned!");
                } else {
                    OverworldGui::print(b->monster->nickname + " cannot be poisoned!");
                }
            } else if (e.effect == "BURN") {
                if (b->monster->status.empty() && !b->hasType("Fire") && b->subHp == 0) {
                    b->monster->status = "BURNED";
                    OverworldGui::print(b->monster->nickname + " was burned!");
                } else {
                    OverworldGui::print(b->monster->nickname + " cannot be burned!");
                }
            } else if (e.effect == "PARALYZE") {
                if (b->monster->status.empty() && !b->hasType("Electric") && b->subHp == 0) {
                    b->monster->status = "PARALYZED";
                    OverworldGui::print(b->monster->nickname + " was paralyzed!");
                } else {
                    OverworldGui::print(b->monster->nickname + " cannot be paralyzed!");
                }
            } else if (e.effect == "SLEEP") {
                if (b->monster->status.empty() && b->subHp == 0) {
                    b->sTurns = 2 + utils::randInt(0, 4);
                    b->monster->status = "SLEEPING";
                    OverworldGui::print(b->monster->nickname + " fell asleep!");
                } else {
                    OverworldGui::print(b->monster->nickname + " cannot fall asleep!");
                }
            } else if (e.effect == "FLINCH") {
                b->flinched = true;
            } else if (e.effect == "FREEZE") {
                if (b->monster->status.empty() && b->subHp == 0) {
                    b->sTurns = 2 + utils::randInt(0, 4);
                    b->monster->status = "FROZEN";
                    OverworldGui::print(b->monster->nickname + " froze!");
                } else {
                    OverworldGui::print(b->monster->nickname + " cannot be frozen!");
                }
            } else if (e.effect == "CONFUSE") {
                if (b->cTurns == 0 && b->subHp == 0) {
                    b->cTurns = 2 + utils::randInt(0, 4);
                    OverworldGui::print(b->monster->nickname + " is confused!");
                } else {
                    OverworldGui::print(b->monster->nickname + " cannot become confused!");
                }
            } else if (e.effect == "HIT") {
                int hits = e.amount + utils::randInt(0, e.variation);
                if (b->subHp > 0 && damage > b->subHp) {
                    OverworldGui::print(b->monster->nickname + "'s substitute broke!");
                    b->subHp = 0;
                    damage = 0;
                }
                for (int j = 1; j < hits; j++) {
                    int d = calcDamage(m, b);
                    if (crit) {
                        OverworldGui::print("Critical hit!");
                    }
                    OverworldGui::print("It did " + std::to_string(d) + " damage!");
                    damage += d;
                    if (b->subHp > 0 && damage > b->subHp) {
                        OverworldGui::print(b->monster->nickname + "'s substitute broke!");
                        b->subHp = 0;
                        damage = 0;
                    }
                }
            } else if (e.effect == "CHARGE") {
                if (m == RECHARGE) {
                    nextMove = nullptr;
                } else {
                    nextMove = new Move(*m, static_cast<int>(i + 1));
                    OverworldGui::print("But " + monster->nickname + " has to charge, so it did no damage...");
                }
                return true;
            } else if (e.effect == "BIDE") {
                damage = (lastDamage[0] + lastDamage[1]) * 2;
                OverworldGui::print(monster->nickname + " unleashed " + std::to_string(damage) + " points of damage!");
            } else if (e.effect == "END_DAMAGE") {
                if (e.stat == "TRAP") {
                    if (b->subHp == 0 && b->tTurns == 0) {
                        OverworldGui::print(b->monster->nickname + " has been trapped!");
                        b->tTurns = e.amount + utils::randInt(0, e.variation);
                    }
                } else if (e.stat == "LEECH_SEED") {
                    if (b->seeded) {
                        OverworldGui::print(b->monster->nickname + " is already seeded!");
                    } else if (b->subHp > 0) {
                        OverworldGui::print("But " + b->monster->nickname + " has a substitute!");
                    } else {
                        b->seeded = true;
                        OverworldGui::print(b->monster->nickname + " was seeded!");
                    }
                } else {
                    throw std::runtime_error(e.stat);
                }
            } else if (e.effect == "CONVERT") {
                types = b->types;
                std::string typeStr = types.empty() ? "" : types[0];
                for (size_t j = 1; j < types.size(); j++) {
                    typeStr += "/" + types[j];
                }
                OverworldGui::print(monster->nickname + " turned into a " + typeStr + " type pokemon!");
            } else if (e.effect == "IMMUNE") {
                immune = true;
                nextMove = new Move(*m, static_cast<int>(i + 1));
                OverworldGui::print(monster->nickname + " became invincible!");
                return true;
            } else if (e.effect == "DISABLE") {
                if (lastMove == nullptr) {
                    OverworldGui::print(b->monster->nickname + " hasn't attacked you!");
                } else if (b->subHp > 0) {
                    OverworldGui::print("But " + b->monster->nickname + " has a substitute!");
                } else if (b->dTurns > 0) {
                    OverworldGui::print(b->monster->nickname + " is still disabled!");
                } else if (b->disable(lastMove, e.amount, e.variation)) {
                    OverworldGui::print(monster->nickname + " disabled " + lastMove->name + "!");
                } else {
                    OverworldGui::print("But it failed!");
                }
            } else if (e.effect == "SLEEPING") {
                if (b->monster->status == "SLEEPING") {
                    if (e.stat == "damage") {
                        int r = std::max(1, static_cast<int>(e.modifier * damage));
                        monster->hp = std::min(monster->mhp, monster->hp + r);
                        OverworldGui::print(monster->nickname + " ate " + std::to_string(r) + " health!");
                    } else {
                        throw std::runtime_error(e.stat);
                    }
                } else {
                    OverworldGui::print("But your opponent isn't sleeping, so you did nothing!");
                    return true;
                }
            } else if (e.effect == "RESET_STATS") {
                resetStats();
                b->resetStats();
                OverworldGui::print("All stat changes were reset!");
            } else if (e.effect == "RECHARGE") {
                nextMove = RECHARGE;
            } else if (e.effect == "SPDEF") {
                if (spdefTurns > 0) {
                    OverworldGui::print(monster->nickname + " already has a light screen up!");
                } else {
                    spdefTurns = e.variation;
                    OverworldGui::print(monster->nickname + " doubled its team's special defense for " + std::to_string(spdefTurns) + " turns!");
                }
            } else if (e.effect == "DEF") {
                if (defTurns > 0) {
                    OverworldGui::print(monster->nickname + " already has reflect up!");
                } else {
                    defTurns = e.variation;
                    OverworldGui::print(monster->nickname + " doubled its team's defense for " + std::to_string(defTurns) + " turns!");
                }
            } else if (e.effect == "RANDOM_MOVE") {
                auto isAllUpper = [](const std::string &s) {
                    bool anyLetter = false;
                    for (unsigned char c : s) {
                        if (std::isalpha(c)) {
                            anyLetter = true;
                            if (!std::isupper(c)) {
                                return false;
                            }
                        }
                    }
                    return anyLetter;
                };
                Move *nm = nullptr;
                do {
                    nm = &Move::MOVES[utils::randInt(0, static_cast<int>(Move::MOVES.size()))];
                } while (isAllUpper(nm->name));
                doMove(nm, b);
            } else if (e.effect == "LAST_OPPONENT_MOVE") {
                if (lastMove == nullptr) {
                    OverworldGui::print("But " + monster->nickname + " hasn't seen any attacks used yet!");
                } else {
                    int r = 0;
                    for (; moves[r] != m; r++) {
                    }
                    moves[r] = lastMove;
                    monster->moves[r] = lastMove;
                    OverworldGui::print(monster->nickname + " learned " + lastMove->name + "!");
                }
            } else if (e.effect == "DISABLE_STAT_CHANGES") {
                if (canLower) {
                    canLower = false;
                    OverworldGui::print(monster->nickname + "'s stats can no longer be lowered!");
                } else {
                    OverworldGui::print("But " + monster->nickname + " already has mist!");
                }
            } else if (e.effect == "SCATTER_COINS") {
                coins += monster->level * 2;
                OverworldGui::print(monster->nickname + " scattered coins everywhere!");
            } else if (e.effect == "CONTINUE") {
                if (e.amount == 0) {
                    break;
                }
                nextMove = new Move(*m);
                return b->setLastMove(m, damage);
            } else if (e.effect == "CONFUSE_ME") {
                if (cTurns <= 0) {
                    cTurns = 2 + utils::randInt(0, 4);
                    OverworldGui::print(monster->nickname + " became confused!");
                }
            } else if (e.effect == "RAGING") {
                raging = true;
            } else if (e.effect == "SLEEP_FOR") {
                monster->status = "SLEEPING";
                poisonDamage = 0;
                sTurns = 2;
                OverworldGui::print(monster->nickname + " fell asleep!");
            } else if (e.effect == "MAKE_SUBSTITUTE") {
                if (subHp > 0) {
                    OverworldGui::print("But " + monster->nickname + " already has a substitute!");
                } else {
                    int r = monster->mhp / 4;
                    if (r >= monster->hp) {
                        OverworldGui::print("But " + monster->nickname + " is too weak to create one!");
                    } else {
                        monster->hp -= r;
                        subHp = r;
                        OverworldGui::print(monster->nickname + " created a substitute!");
                    }
                }
            } else if (e.effect == "TRANSFORM_INTO_OPPONENT") {
                accStage = b->accStage;
                evsnStage = b->evsnStage;
                spdStage = b->spdStage;
                atkStage = b->atkStage;
                defStage = b->defStage;
                spatkStage = b->spatkStage;
                spdefStage = b->spdefStage;
                critMul = b->critMul;
                spd = b->spd;
                atk = b->atk;
                def = b->def;
                spatk = b->spatk;
                spdef = b->spdef;
                types = b->types;
                for (size_t j = 0; j < moves.size(); j++) {
                    moves[j] = b->moves[j];
                    monster->pp[j] = std::max(monster->pp[j], 5);
                }
                OverworldGui::print(monster->nickname + " transformed into " + b->monster->nickname + "!");
            } else if (e.effect == "SWITCH" || e.effect == "SWITCH_OPPONENT") {
                OverworldGui::print("But it failed!");
            } else if (e.effect == "ATTACK_FIRST" || e.effect == "ATTACK_SECOND" || e.effect == "CRIT_CHANCE" || e.effect == "MISS") {
                // no-op here
            } else {
                throw std::runtime_error(e.effect);
            }
        }
    }
    return b->setLastMove(m, damage);
}

int BattleState::wildBattle(std::vector<Battler*> &monsters, Battler *wildMon, OverworldGui *gui) {
    initStatics();
    int monDex = 0;
    for (; monDex < static_cast<int>(monsters.size()); monDex++) {
        if (monsters[monDex]->hp > 0) break;
    }
    int escapeAttempts = 0;
    int moveDex = 0;
    int wMoveDex = 0;
    int lucky = gui->player->hasItem(LUCKY_EGG) ? 2 : 1;
    Move *myMove = STRUGGLE;
    Move *yourMove = STRUGGLE;
    BattleState *pState = new BattleState(monsters[monDex]);
    BattleState *wState = new BattleState(wildMon);
    OverworldGui::setBattleStates(pState, wState);
    std::unordered_set<Battler*> participants;
    participants.insert(monsters[monDex]);
    wildMon->nickname = "Wild " + wildMon->name + (gui->player->pokedex[wildMon->dexNum] ? "🔴" : "");
    OverworldGui::print("A wild " + wildMon->name + " appeared!");
    OverworldGui::print("Go! " + monsters[monDex]->nickname + "!");
    while (true) {
        OverworldGui::autoAdvanceBattleText();
        bool didSomething = true;
        myMove = STRUGGLE;
        yourMove = STRUGGLE;
        for (size_t i = 0; i < ACTIONS.size(); i++) {
            OverworldGui::strArr[i] = ACTIONS[i];
        }
        bool useAuto = gui && OverworldGui::autoBattle;
        switch (useAuto ? 0 : guiChoice(3)) {
            case 0:
                if (pState->nextMove == nullptr) {
                    if (useAuto) {
                        moveDex = autoBattleMoveDex(pState);
                        if (moveDex == -1) {
                            OverworldGui::autoBattle = false;
                        }
                    } else {
                        moveDex = getMoveDex(pState);
                    }
                    if (moveDex == -2) {
                        didSomething = false;
                        break;
                    }
                    if (pState->canMove() && moveDex >= 0) {
                        myMove = pState->moves[moveDex];
                        monsters[monDex]->pp[moveDex]--;
                    }
                } else if (pState->canMove()) {
                    myMove = pState->nextMove;
                }
                if (wState->nextMove == nullptr) {
                    wMoveDex = randomMoveDex(wState);
                    if (wState->canMove() && wMoveDex >= 0) {
                        yourMove = wState->moves[wMoveDex];
                        wildMon->pp[wMoveDex]--;
                    }
                } else if (wState->canMove()) {
                    yourMove = wState->nextMove;
                }
                if (myMove == ROAR) {
                    OverworldGui::print("You scared away the wild " + wildMon->name + "!");
                    return coinsPickup(pState, wState);
                } else if (yourMove == ROAR) {
                    OverworldGui::print(wildMon->name + " scared you away!");
                    return coinsPickup(pState, wState);
                } else if (myMove == TELEPORT) {
                    OverworldGui::print("You teleported away!");
                    return coinsPickup(pState, wState);
                } else if (yourMove == TELEPORT) {
                    OverworldGui::print(wildMon->name + " teleported away!");
                    return coinsPickup(pState, wState);
                } else if (myMove == WHIRLWIND) {
                    OverworldGui::print("A strong gust blew wild " + wildMon->name + " away!");
                    return coinsPickup(pState, wState);
                } else if (yourMove == WHIRLWIND) {
                    OverworldGui::print("A strong gust blew you away!");
                    return coinsPickup(pState, wState);
                }
                if (pState->goBefore(myMove, wState, yourMove)) {
                    if (pState->doMove(myMove, wState)) {
                        if (!wState->doMove(yourMove, pState) && blackedOut(monsters)) {
                            OverworldGui::print("All of your pokemon fainted!");
                            OverworldGui::autoBattle = false;
                            return -1;
                        }
                    } else {
                        OverworldGui::print("The wild " + wildMon->name + " fainted!");
                        pState->gainXp(gui->player, participants, wildMon, false, lucky);
                        return coinsPickup(pState, wState);
                    }
                } else if (wState->doMove(yourMove, pState) && !pState->doMove(myMove, wState)) {
                    OverworldGui::print("The wild " + wildMon->name + " fainted!");
                    pState->gainXp(gui->player, participants, wildMon, false, lucky);
                    return coinsPickup(pState, wState);
                }
                break;
            case 1:
                if (pState->tTurns == 0 && pState->nextMove == nullptr) {
                    moveDex = switchMon(monsters, monDex, false);
                    if (moveDex == -1) {
                        didSomething = false;
                        break;
                    }
                    monDex = moveDex;
                    pState = new BattleState(monsters[monDex], pState);
                    OverworldGui::playerState = pState;
                    participants.insert(monsters[monDex]);
                    OverworldGui::print("Go! " + monsters[monDex]->nickname + "!");
                    if (wState->nextMove == nullptr) {
                        wMoveDex = randomMoveDex(wState);
                        if (wState->canMove() && wMoveDex >= 0) {
                            yourMove = wState->moves[wMoveDex];
                            wildMon->pp[wMoveDex]--;
                        }
                    } else if (wState->canMove()) {
                        yourMove = wState->nextMove;
                    }
                    if (!wState->doMove(yourMove, pState) && blackedOut(monsters)) {
                        OverworldGui::print("All of your pokemon fainted!");
                        OverworldGui::autoBattle = false;
                        return -1;
                    }
                } else {
                    didSomething = false;
                    OverworldGui::print("You are trapped!");
                }
                break;
            case 2:
                gui->setWildItems();
                while (OverworldGui::usingBattleItem) {
                    SDL_Event event;
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            OverworldGui::usingBattleItem = false;
                            OverworldGui::choosingFromLongList = false;
                            break;
                        }
                        gui->handleEvent(event);
                    }
                    gui->pumpEvents();
                    gui->update();
                    gui->render();
                    SDL_Delay(16);
                }
                OverworldGui::choosingFromLongList = false;
                if (!OverworldGui::battling) {
                    return coinsPickup(pState, wState);
                }
                if (!OverworldGui::busedItem) {
                    didSomething = false;
                    break;
                }
                if (pState->monster->status.empty()) {
                    pState->sTurns = 0;
                }
                if (wState->nextMove == nullptr) {
                    wMoveDex = randomMoveDex(wState);
                    if (wState->canMove() && wMoveDex >= 0) {
                        yourMove = wState->moves[wMoveDex];
                        wildMon->pp[wMoveDex]--;
                    }
                } else if (wState->canMove()) {
                    yourMove = wState->nextMove;
                }
                if (!wState->doMove(yourMove, pState) && blackedOut(monsters)) {
                    OverworldGui::print("All of your pokemon fainted!");
                    OverworldGui::autoBattle = false;
                    return -1;
                }
                break;
            case 3:
                if (monsters[monDex]->spd * 128.0 / wildMon->spd + 30 * ++escapeAttempts > utils::rand01() * 256) {
                    OverworldGui::print("You got away safely!");
                    return coinsPickup(pState, wState);
                } else {
                    OverworldGui::print("You couldn't escape!");
                    if (wState->nextMove == nullptr) {
                        wMoveDex = randomMoveDex(wState);
                        if (wState->canMove() && wMoveDex >= 0) {
                            yourMove = wState->moves[wMoveDex];
                            wildMon->pp[wMoveDex]--;
                        }
                    } else if (wState->canMove()) {
                        yourMove = wState->nextMove;
                    }
                    if (!wState->doMove(yourMove, pState) && blackedOut(monsters)) {
                        OverworldGui::print("All of your pokemon fainted!");
                        OverworldGui::autoBattle = false;
                        return -1;
                    }
                }
        }
        if (didSomething) {
            seed(pState, wState);
            seed(wState, pState);
            if (pState->endTurn()) {
                if (blackedOut(monsters)) {
                    OverworldGui::print("All of your pokemon fainted!");
                    OverworldGui::autoBattle = false;
                    return -1;
                } else {
                    OverworldGui::autoBattle = false;
                    monDex = switchMon(monsters, monDex, true);
                    pState = new BattleState(monsters[monDex], pState);
                    participants.insert(pState->monster);
                    OverworldGui::playerState = pState;
                }
            }
            if (wState->endTurn()) {
                OverworldGui::print("The wild " + wildMon->name + " fainted!");
                pState->gainXp(gui->player, participants, wildMon, false, lucky);
                return coinsPickup(pState, wState);
            }
        }
    }
}

int BattleState::trainerBattle(std::vector<Battler*> &monsters, const std::string &tName, std::vector<Battler*> &tMonsters,
                               OverworldGui *gui) {
    initStatics();
    int monDex = 0;
    int tMonDex = 0;
    for (; monDex < static_cast<int>(monsters.size()); monDex++) {
        if (monsters[monDex]->hp > 0) break;
    }
    int moveDex = 0;
    int wMoveDex = 0;
    int lucky = gui->player->hasItem(LUCKY_EGG) ? 2 : 1;
    Move *myMove = STRUGGLE;
    Move *yourMove = STRUGGLE;
    BattleState *pState = new BattleState(monsters[monDex]);
    BattleState *wState = new BattleState(tMonsters[0]);
    OverworldGui::setBattleStates(pState, wState);
    std::unordered_set<Battler*> participants;
    participants.insert(monsters[monDex]);
    tMonsters[tMonDex]->nickname = "Enemy " + tMonsters[0]->name;
    OverworldGui::print(tName + " would like to battle!");
    OverworldGui::print(tName + " sent out " + tMonsters[0]->name + "!");
    OverworldGui::print("Go! " + monsters[monDex]->nickname + "!");
    while (true) {
        OverworldGui::autoAdvanceBattleText();
        bool didSomething = true;
        myMove = STRUGGLE;
        yourMove = STRUGGLE;
        for (size_t i = 0; i < ACTIONS.size(); i++) {
            OverworldGui::strArr[i] = ACTIONS[i];
        }
        bool useAuto = gui && OverworldGui::autoBattle;
        switch (useAuto ? 0 : guiChoice(3)) {
            case 0:
                if (pState->nextMove == nullptr) {
                    if (useAuto) {
                        moveDex = autoBattleMoveDex(pState);
                        if (moveDex == -1) {
                            OverworldGui::autoBattle = false;
                        }
                    } else {
                        moveDex = getMoveDex(pState);
                    }
                    if (moveDex == -2) {
                        didSomething = false;
                        break;
                    }
                    if (pState->canMove() && moveDex >= 0) {
                        myMove = pState->moves[moveDex];
                        monsters[monDex]->pp[moveDex]--;
                    }
                } else if (pState->canMove()) {
                    myMove = pState->nextMove;
                }
                if (wState->nextMove == nullptr) {
                    wMoveDex = randomMoveDex(wState);
                    if (wState->canMove() && wMoveDex >= 0) {
                        yourMove = wState->moves[wMoveDex];
                        tMonsters[tMonDex]->pp[wMoveDex]--;
                    }
                } else if (wState->canMove()) {
                    yourMove = wState->nextMove;
                }
                if (pState->goBefore(myMove, wState, yourMove)) {
                    if (pState->doMove(myMove, wState)) {
                        if (!wState->doMove(yourMove, pState) && blackedOut(monsters)) {
                            OverworldGui::print("All of your pokemon fainted!");
                            OverworldGui::autoBattle = false;
                            return -1;
                        }
                    } else {
                        pState->tTurns = 0;
                        OverworldGui::print(tMonsters[tMonDex]->nickname + " fainted!");
                        pState->gainXp(gui->player, participants, tMonsters[tMonDex], true, lucky);
                        if (blackedOut(tMonsters)) {
                            OverworldGui::print("You won the battle!");
                            return coinsPickup(pState, wState);
                        } else {
                            wState = new BattleState(tMonsters[++tMonDex], wState);
                            OverworldGui::enemyState = wState;
                            OverworldGui::print(tName + " sent out " + tMonsters[tMonDex]->name + "!");
                            participants.clear();
                            participants.insert(pState->monster);
                        }
                    }
                } else if (!wState->doMove(yourMove, pState) && blackedOut(monsters)) {
                    OverworldGui::print("All of your pokemon fainted!");
                    OverworldGui::autoBattle = false;
                    return -1;
                } else if (pState->monster->hp > 0 && !pState->doMove(myMove, wState)) {
                    pState->tTurns = 0;
                    OverworldGui::print(tMonsters[tMonDex]->nickname + " fainted!");
                    pState->gainXp(gui->player, participants, tMonsters[tMonDex], true, lucky);
                    if (blackedOut(tMonsters)) {
                        OverworldGui::print("You won the battle!");
                        return coinsPickup(pState, wState);
                    } else {
                        wState = new BattleState(tMonsters[++tMonDex], wState);
                        OverworldGui::enemyState = wState;
                        OverworldGui::print(tName + " sent out " + tMonsters[tMonDex]->name + "!");
                        participants.clear();
                        participants.insert(pState->monster);
                    }
                }
                break;
            case 1:
                if (pState->tTurns == 0 && pState->nextMove == nullptr) {
                    moveDex = switchMon(monsters, monDex, false);
                    if (moveDex == -1) {
                        didSomething = false;
                        break;
                    }
                    monDex = moveDex;
                    pState = new BattleState(monsters[monDex], pState);
                    OverworldGui::playerState = pState;
                    participants.insert(monsters[monDex]);
                    OverworldGui::print("Go! " + monsters[monDex]->nickname + "!");
                    if (wState->nextMove == nullptr) {
                        wMoveDex = randomMoveDex(wState);
                        if (wState->canMove() && wMoveDex >= 0) {
                            yourMove = wState->moves[wMoveDex];
                            tMonsters[tMonDex]->pp[wMoveDex]--;
                        }
                    } else if (wState->canMove()) {
                        yourMove = wState->nextMove;
                    }
                    if (!wState->doMove(yourMove, pState) && blackedOut(monsters)) {
                        OverworldGui::print("All of your pokemon fainted!");
                        OverworldGui::autoBattle = false;
                        return -1;
                    }
                } else {
                    didSomething = false;
                    OverworldGui::print("You are trapped!");
                }
                break;
            case 2:
                gui->setTrainerItems();
                while (OverworldGui::usingBattleItem) {
                    SDL_Event event;
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            OverworldGui::usingBattleItem = false;
                            OverworldGui::choosingFromLongList = false;
                            break;
                        }
                        gui->handleEvent(event);
                    }
                    gui->pumpEvents();
                    gui->update();
                    gui->render();
                    SDL_Delay(16);
                }
                OverworldGui::choosingFromLongList = false;
                if (!OverworldGui::battling) {
                    return coinsPickup(pState, wState);
                }
                if (!OverworldGui::busedItem) {
                    didSomething = false;
                    break;
                }
                if (pState->monster->status.empty()) {
                    pState->sTurns = 0;
                }
                if (wState->nextMove == nullptr) {
                    wMoveDex = randomMoveDex(wState);
                    if (wState->canMove() && wMoveDex >= 0) {
                        yourMove = wState->moves[wMoveDex];
                        tMonsters[tMonDex]->pp[wMoveDex]--;
                    }
                } else if (wState->canMove()) {
                    yourMove = wState->nextMove;
                }
                if (!wState->doMove(yourMove, pState) && blackedOut(monsters)) {
                    OverworldGui::print("All of your pokemon fainted!");
                    OverworldGui::autoBattle = false;
                    return -1;
                }
                break;
            case 3:
                didSomething = false;
                OverworldGui::print("Aww come on man, don't give up!");
                break;
        }
        if (didSomething) {
            seed(pState, wState);
            seed(wState, pState);
            if (pState->endTurn()) {
                if (blackedOut(monsters)) {
                    OverworldGui::print("All of your pokemon fainted!");
                    OverworldGui::autoBattle = false;
                    return -1;
                } else {
                    OverworldGui::autoBattle = false;
                    monDex = switchMon(monsters, monDex, true);
                    pState = new BattleState(monsters[monDex], pState);
                    participants.insert(pState->monster);
                    OverworldGui::playerState = pState;
                }
            }
            if (wState->endTurn()) {
                pState->tTurns = 0;
                OverworldGui::print(tMonsters[tMonDex]->nickname + " fainted!");
                pState->gainXp(gui->player, participants, tMonsters[tMonDex], true, lucky);
                if (blackedOut(tMonsters)) {
                    OverworldGui::print("You won the battle!");
                    return coinsPickup(pState, wState);
                } else {
                    wState = new BattleState(tMonsters[++tMonDex], wState);
                    OverworldGui::enemyState = wState;
                    OverworldGui::print(tName + " sent out " + tMonsters[tMonDex]->name + "!");
                    participants.clear();
                    participants.insert(pState->monster);
                }
            }
        }
    }
}
