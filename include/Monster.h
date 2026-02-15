#pragma once

#include "Move.h"

class Monster {
public:
    static std::vector<std::string> MONSTER_STRINGS;
    static std::vector<Monster> MONSTERS;
    static std::unordered_map<std::string, Monster*> MONSTER_MAP;

    static void buildMonsters();

    std::string name;
    std::vector<std::string> types;
    int hp = 0;
    int atk = 0;
    int def = 0;
    int spatk = 0;
    int spdef = 0;
    int spd = 0;
    int dexNum = 0;
    std::unordered_set<Move*> learnable;

    Monster();
    Monster(const std::string &s, int dexNum);
    void addMoves(const std::string &s);
    std::string toString() const;
};
