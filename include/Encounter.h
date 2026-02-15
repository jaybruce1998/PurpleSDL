#pragma once

#include "Monster.h"

class Encounter {
public:
    static std::vector<std::string> ENCOUNTER_STRINGS;
    static void buildEncounterRates();

    Monster *rMonster = nullptr;
    Monster *bMonster = nullptr;
    int chance = 0;
    int rLevel = 0;
    int bLevel = 0;

    Encounter() = default;
    explicit Encounter(const std::string &s);
    std::string toString() const;
};
