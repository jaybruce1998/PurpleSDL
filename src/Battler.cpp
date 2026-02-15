#include "Battler.h"
#include "Main.h"
#include "OverworldGui.h"
#include "BattleState.h"
#include "Player.h"
#include "Utils.h"

std::vector<std::string> Battler::GROUPS = {
    "Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Slow","Medium Slow","Medium Slow","Medium Fast",
    "Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Fast","Fast","Medium Fast","Medium Fast","Fast","Fast","Medium Fast","Medium Fast",
    "Medium Slow","Medium Slow","Medium Slow","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Slow","Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow",
    "Medium Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Medium Slow","Slow","Slow","Medium Slow","Medium Slow","Medium Slow","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast",
    "Medium Fast","Medium Fast","Medium Fast","Slow","Slow","Medium Slow","Medium Slow","Medium Slow","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Slow","Slow","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast",
    "Medium Fast","Slow","Slow","Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Slow","Slow","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Slow","Slow","Slow","Slow","Slow","Medium Fast","Medium Fast","Medium Fast","Medium Fast",
    "Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Medium Fast","Slow","Slow","Slow","Slow","Slow","Slow","Slow","Slow","Slow","Medium Slow","Slow"};

int Battler::xpNeeded(int level) {
    if (group == "Medium Slow") {
        return static_cast<int>(6 * std::pow(level, 3) / 5 - 15 * level * level + 100 * level - 140);
    }
    if (group == "Medium Fast") {
        return level * level * level;
    }
    if (group == "Fast") {
        return static_cast<int>(4 * (std::pow(level, 3)) / 5);
    }
    if (group == "Slow") {
        return static_cast<int>(5 * (std::pow(level, 3)) / 4);
    }
    throw std::runtime_error(group);
}

int Battler::newStat(int base, int dv, int sxp, int c) {
    return static_cast<int>(((base + dv) * 2 + std::sqrt(sxp) / 4) * level / 100 + c);
}

void Battler::calculateStats() {
    hp++;
    mhp = newStat(bHp, hpDv, hpXp, level + 10);
    spd = newStat(bSpd, spdDv, spdXp, 5);
    atk = newStat(bAtk, atkDv, atkXp, 5);
    def = newStat(bDef, defDv, defXp, 5);
    spatk = newStat(bSpatk, spatkDv, spatkXp, 5);
    spdef = newStat(bSpdef, spdefDv, spdefXp, 5);
}

void Battler::become(Monster *m) {
    if (name == nickname) {
        nickname = m->name;
    }
    dexNum = m->dexNum;
    learnset = LevelUpMove::LEVEL_UP_MOVES[dexNum];
    evolution = Evolution::EVOLUTIONS[dexNum];
    name = m->name;
    types = m->types;
    bAtk = m->atk;
    bDef = m->def;
    bSpatk = m->spatk;
    bSpdef = m->spdef;
    bHp = m->hp;
    bSpd = m->spd;
    lsi = 0;
}

void Battler::finishSetup() {
    nickname = name;
    status = "";
    group = GROUPS[dexNum];
    xp = xpNeeded(level);
    mxp = xpNeeded(level + 1);
    calculateStats();
    hp = mhp;
    moves.assign(4, nullptr);
    pp.assign(4, 0);
    mpp.assign(4, 0);
    for (int md = 0; lsi < static_cast<int>(learnset.size()); lsi++) {
        if (learnset[lsi].level > level) {
            break;
        }
        moves[md] = learnset[lsi].move;
        md = (md + 1) % static_cast<int>(moves.size());
    }
    for (size_t i = 0; i < moves.size(); i++) {
        if (moves[i] == nullptr) {
            break;
        }
        pp[i] = moves[i]->pp;
        mpp[i] = pp[i];
    }
}

Battler::Battler(int level, Monster *m) {
    become(m);
    this->level = level;
    atkDv = utils::randInt(0, 16);
    defDv = utils::randInt(0, 16);
    spatkDv = utils::randInt(0, 16);
    spdefDv = utils::randInt(0, 16);
    hpDv = utils::randInt(0, 16);
    spdDv = utils::randInt(0, 16);
    shiny = utils::randInt(0, Main::SHINY_CHANCE) == 0;
    finishSetup();
}

Battler::Battler(Battler *b) {
    dexNum = b->dexNum - 1;
    if (dexNum > 132 && dexNum < 136) {
        dexNum = 133;
    } else if (dexNum < 1 || Evolution::EVOLUTIONS[dexNum] == nullptr) {
        dexNum++;
    } else {
        dexNum--;
        if (Evolution::EVOLUTIONS[dexNum] == nullptr) {
            dexNum++;
        }
    }
    become(&Monster::MONSTERS[dexNum]);
    level = 1;
    int n = 0;
    int s = Main::SHINY_CHANCE / 2;
    atkDv = b->atkDv == 15 ? 15 : utils::randInt(0, 16);
    if (atkDv == 15) n++;
    defDv = b->defDv == 15 ? 15 : utils::randInt(0, 16);
    if (defDv == 15) n++;
    spatkDv = b->spatkDv == 15 ? 15 : utils::randInt(0, 16);
    if (spatkDv == 15) n++;
    spdefDv = b->spdefDv == 15 ? 15 : utils::randInt(0, 16);
    if (spdefDv == 15) n++;
    hpDv = b->hpDv == 15 ? 15 : utils::randInt(0, 16);
    if (hpDv == 15) n++;
    spdDv = b->spdDv == 15 ? 15 : utils::randInt(0, 16);
    if (spdDv == 15) n++;
    shiny = utils::randInt(0, b->shiny ? static_cast<int>(std::sqrt(s)) : s) == 0;
    finishSetup();
    OverworldGui::print(std::to_string(n) + " perfect DVs! " +
                        (shiny ? "And it's shiny!!" : (n == 0 ? " Wow that sucks!" : "Not too shabby...")));
}

Battler::Battler(const std::string &s) {
    std::vector<std::string> a = utils::split(s, '/');
    std::vector<std::string> i = utils::split(a[0], ',');
    name = i[2];
    become(&Monster::MONSTERS[std::stoi(i[0])]);
    level = std::stoi(i[1]);
    nickname = i[2];
    status = i[3];
    xp = std::stoi(i[4]);
    hp = std::stoi(i[5]) - 1;
    lsi = std::stoi(i[6]);
    atkDv = std::stoi(i[7]);
    defDv = std::stoi(i[8]);
    spatkDv = std::stoi(i[9]);
    spdefDv = std::stoi(i[10]);
    hpDv = std::stoi(i[11]);
    spdDv = std::stoi(i[12]);
    shiny = i[13] == "1";
    atkXp = std::stoi(i[14]);
    defXp = std::stoi(i[15]);
    spatkXp = std::stoi(i[16]);
    spdefXp = std::stoi(i[17]);
    hpXp = std::stoi(i[18]);
    spdXp = std::stoi(i[19]);
    group = GROUPS[dexNum];
    mxp = xpNeeded(level + 1);
    calculateStats();
    moves.assign(4, nullptr);
    pp.assign(4, 0);
    mpp.assign(4, 0);
    for (int j = static_cast<int>(a.size()) - 2; j >= 0; j--) {
        i = utils::split(a[j + 1], ',');
        moves[j] = Move::MOVE_MAP[i[0]];
        std::vector<std::string> q = utils::split(i[1], 'x');
        pp[j] = std::stoi(q[0]);
        if (q.size() == 2) {
            mpp[j] = std::stoi(q[1]);
        } else {
            mpp[j] = moves[j]->pp;
        }
    }
}

void Battler::learn(std::vector<Move*> &lastMoves, Move *m) {
    if (m == nullptr) {
        return;
    }
    std::vector<Move*> lastMovesCopy = moves;
    OverworldGui::print(nickname + " is trying to learn " + m->name + "! Select a move to replace it with.");

    OverworldGui::longArr = moveStrings();
    OverworldGui::longListHeaderOverride = "Replace which move with " + m->name + "?";
    OverworldGui::choosingFromLongList = true;
    OverworldGui::inMenu = true;
    int v = BattleState::guiChoice(3);
    OverworldGui::choosingFromLongList = false;
    OverworldGui::inMenu = false;
    OverworldGui::longArr.clear();
    OverworldGui::longListHeaderOverride.clear();

    if (v == -1) {
        OverworldGui::print(nickname + " did not learn " + m->name + ".");
        return;
    }
    OverworldGui::print(nickname + " forgot how to use " + lastMovesCopy[v]->name + "...");
    lastMovesCopy[v] = m;
    moves[v] = m;
    pp[v] = std::min(pp[v], m->pp);
    mpp[v] = m->pp;
    OverworldGui::print(" and learned how to use " + m->name + "!");
}

bool Battler::newMove(Move *m) {
    for (auto *mv : moves) {
        if (mv == m) {
            return false;
        }
    }
    return true;
}

void Battler::learnLevelUpMoves(std::vector<Move*> &lastMoves) {
    while (lsi < static_cast<int>(learnset.size())) {
        if (learnset[lsi].level <= level) {
            if (newMove(learnset[lsi].move)) {
                if (lastMoves[3] == nullptr) {
                    for (int i = 2; true; i--) {
                        if (lastMoves[i] != nullptr) {
                            moves[++i] = learnset[lsi].move;
                            lastMoves[i] = moves[i];
                            pp[i] = moves[i]->pp;
                            mpp[i] = pp[i];
                            OverworldGui::print(nickname + " learned " + moves[i]->name + "!");
                            break;
                        }
                    }
                } else {
                    learn(lastMoves, learnset[lsi].move);
                }
            }
            lsi++;
        } else {
            break;
        }
    }
}

void Battler::gainXp(Player *p, const std::vector<Move*> &lastMoves, int xp, int atk, int def, int spatk, int spdef, int hp,
                     int spd) {
    this->xp += xp;
    atkXp += atk;
    defXp += def;
    spatkXp += spatk;
    spdefXp += spdef;
    hpXp += hp;
    spdXp += spd;
    OverworldGui::print(nickname + " gained " + std::to_string(xp) + " experience points!");
    while (this->xp >= mxp) {
        level++;
        OverworldGui::print(nickname + " grew to level " + std::to_string(level) + "!");
        mxp = xpNeeded(level + 1);
        std::vector<Move*> tempMoves = lastMoves;
        learnLevelUpMoves(tempMoves);
        if (evolution != nullptr && evolution->stone.empty() && evolution->level <= level) {
            std::string n = nickname;
            become(&Monster::MONSTERS[evolution->evo]);
            OverworldGui::print(n + " evolved into " + name + "!");
            p->registerBattler(this);
            std::vector<Move*> mv = moves;
            learnLevelUpMoves(mv);
        }
        calculateStats();
    }
}

bool Battler::useStone(const std::string &s) {
    if (dexNum == 133) {
        if (s == "Water Stone") {
            evolution = new Evolution(s, 134);
        } else if (s == "Thunder Stone") {
            evolution = new Evolution(s, 135);
        } else if (s == "Fire Stone") {
            evolution = new Evolution(s, 136);
        } else {
            OverworldGui::print("Neat idea, but nope, sorry! Try a different stone...");
            return false;
        }
    } else if (evolution == nullptr || s != evolution->stone) {
        OverworldGui::print("Quit messing around!");
        return false;
    }
    std::string n = nickname;
    become(&Monster::MONSTERS[evolution->evo]);
    OverworldGui::print(n + " evolved into " + name + "!");
    return true;
}

std::vector<std::string> Battler::moveStrings() const {
    std::vector<std::string> a(moves.size());
    for (size_t i = 0; i < moves.size(); i++) {
        if (moves[i] == nullptr) {
            a[i] = "";
        } else {
            a[i] = moves[i]->name + " (" + std::to_string(pp[i]) + "/" + std::to_string(mpp[i]) + ")";
        }
    }
    return a;
}

std::vector<std::string> Battler::partyStrings(const std::vector<Battler*> &party) {
    std::vector<std::string> r(party.size());
    for (size_t i = 0; i < party.size(); i++) {
        if (party[i] == nullptr) {
            r[i] = " ";
        } else {
            r[i] = party[i]->toString();
        }
    }
    return r;
}

void Battler::fullyHeal() {
    hp = mhp;
    status.clear();
    for (size_t i = 0; i < moves.size(); i++) {
        if (moves[i] == nullptr) {
            return;
        }
        pp[i] = mpp[i];
    }
}

void Battler::refreshStats() {
    int oldMhp = mhp;
    calculateStats();
    if (oldMhp > 0 && mhp > 0) {
        hp = std::min(hp, mhp);
    }
}

std::vector<std::string> Battler::allInformation() const {
    std::vector<std::string> r;
    r.push_back(nickname + "/" + name);
    r.push_back((status.empty() ? "healthy" : status) + "/" + (shiny ? "SHINY" : "happy"));
    std::string typeStr = types.empty() ? "" : types[0];
    for (size_t i = 1; i < types.size(); i++) {
        typeStr += "/" + types[i];
    }
    r.push_back("Level: " + std::to_string(level) + ", " + typeStr);
    r.push_back("XP: " + std::to_string(xp) + "/" + std::to_string(mxp));
    r.emplace_back("");
    r.push_back("HP: " + std::to_string(hp) + "/" + std::to_string(mhp) + " DV: (" + std::to_string(hpDv) + ")");
    r.push_back("Speed: " + std::to_string(spd) + " DV: (" + std::to_string(spdDv) + ")");
    r.push_back("Attack: " + std::to_string(atk) + " DV: (" + std::to_string(atkDv) + ")");
    r.push_back("Defense: " + std::to_string(def) + " DV: (" + std::to_string(defDv) + ")");
    r.push_back("Special Attack: " + std::to_string(spatk) + " DV: (" + std::to_string(spatkDv) + ")");
    r.push_back("Special Defense: " + std::to_string(spdef) + " DV: (" + std::to_string(spdefDv) + ")");
    r.emplace_back("");
    r.push_back("Moves:");
    r.push_back(moves[0]->name + " " + std::to_string(pp[0]) + "/" + std::to_string(mpp[0]));
    r.push_back(moves[1] == nullptr ? "" : moves[1]->name + " " + std::to_string(pp[1]) + "/" + std::to_string(mpp[1]));
    r.push_back(moves[2] == nullptr ? "" : moves[2]->name + " " + std::to_string(pp[2]) + "/" + std::to_string(mpp[2]));
    r.push_back(moves[3] == nullptr ? "" : moves[3]->name + " " + std::to_string(pp[3]) + "/" + std::to_string(mpp[3]));
    return r;
}

void Battler::append(std::string &out) const {
    std::ostringstream sb;
    sb << dexNum << "," << level << "," << nickname << "," << status << "," << xp << "," << hp << "," << lsi << ","
       << atkDv << "," << defDv << "," << spatkDv << "," << spdefDv << "," << hpDv << "," << spdDv << ","
       << (shiny ? '1' : '0') << "," << atkXp << "," << defXp << "," << spatkXp << "," << spdefXp << "," << hpXp << ","
       << spdXp;
    for (size_t i = 0; i < moves.size(); i++) {
        if (moves[i] == nullptr) {
            break;
        }
        sb << "/" << moves[i]->name << "," << pp[i] << "x" << mpp[i];
    }
    out += sb.str();
}

std::string Battler::toString() const {
    std::string s = nickname + " L" + std::to_string(level) + ", HP: " + std::to_string(hp) + "/" + std::to_string(mhp);
    if (status == "POISONED") {
        s += " PSN";
    } else if (status == "PARALYZED") {
        s += " PRZ";
    } else if (status == "SLEEPING") {
        s += " SLP";
    } else if (status == "FROZEN") {
        s += " FZN";
    } else if (status == "BURNED") {
        s += " BRN";
    }
    return s;
}
