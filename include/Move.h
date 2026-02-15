#pragma once

#include "Effect.h"

class Move {
public:
    static std::vector<std::string> MOVE_STRINGS;
    static std::vector<Move> MOVES;
    static std::unordered_map<std::string, Move*> MOVE_MAP;

    static void buildMoves();

    std::string name;
    std::string type;
    std::string atkType;
    int power = 0;
    int acc = 0;
    int pp = 0;
    std::vector<Effect> effects;

    Move() = default;
    explicit Move(const std::string &s);
    Move(const Move &m, int i);
    explicit Move(const Move &m);

    int priority() const;
    int critChance() const;
    bool shouldPrintDamage() const;
    std::string toString() const;
};
