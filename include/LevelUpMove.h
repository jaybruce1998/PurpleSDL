#pragma once

#include "Move.h"

class LevelUpMove {
public:
    static std::vector<std::string> LEVEL_UP_STRINGS;
    static std::vector<std::vector<LevelUpMove>> LEVEL_UP_MOVES;

    static void buildLevelUpMoves();

    int level = 0;
    Move *move = nullptr;

    LevelUpMove() = default;
    explicit LevelUpMove(const std::string &s);
    std::string toString() const;
};
