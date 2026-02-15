#pragma once

#include "Monster.h"
#include "LevelUpMove.h"
#include "Evolution.h"

class Player;

class Battler : public Monster {
public:
    static std::vector<std::string> GROUPS;

    std::string nickname;
    std::string status;
    std::string group;
    int level = 0;
    int mhp = 0;
    int xp = 0;
    int mxp = 0;
    int atkDv = 0;
    int defDv = 0;
    int spatkDv = 0;
    int spdefDv = 0;
    int hpDv = 0;
    int spdDv = 0;
    int atkXp = 0;
    int defXp = 0;
    int spatkXp = 0;
    int spdefXp = 0;
    int hpXp = 0;
    int spdXp = 0;
    int bAtk = 0;
    int bDef = 0;
    int bSpatk = 0;
    int bSpdef = 0;
    int bHp = 0;
    int bSpd = 0;
    int lsi = 0;
    std::vector<Move*> moves;
    std::vector<int> pp;
    std::vector<int> mpp;
    bool shiny = false;
    std::vector<LevelUpMove> learnset;
    Evolution *evolution = nullptr;

    Battler() = default;
    Battler(int level, Monster *m);
    Battler(Battler *b);
    explicit Battler(const std::string &s);

    int xpNeeded(int level);
    void fullyHeal();
    std::vector<std::string> allInformation() const;
    std::vector<std::string> moveStrings() const;
    static std::vector<std::string> partyStrings(const std::vector<Battler*> &party);
    void append(std::string &out) const;
    std::string toString() const;

    void refreshStats();

    void gainXp(Player *p, const std::vector<Move*> &lastMoves, int xp, int atk, int def, int spatk, int spdef, int hp, int spd);
    bool newMove(Move *m);
    void learn(std::vector<Move*> &lastMoves, Move *m);
    bool useStone(const std::string &s);

private:
    int newStat(int base, int dv, int sxp, int c);
    void calculateStats();
    void become(Monster *m);
    void finishSetup();
    void learnLevelUpMoves(std::vector<Move*> &lastMoves);
};
