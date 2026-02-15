#pragma once

#include "Monster.h"

class Evolution {
public:
    static std::vector<std::string> EVOLUTION_STRINGS;
    static std::vector<Evolution*> EVOLUTIONS;

    static void buildEvolutions();

    std::string stone;
    int level = 0;
    int evo = 0;

    Evolution() = default;
    Evolution(const std::string &stone, int evo);
    Evolution(int level, int evo);
    std::string toString() const;
};
