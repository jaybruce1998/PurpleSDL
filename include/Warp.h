#pragma once

#include "PokeMap.h"

class Warp {
public:
    static std::vector<std::string> WARP_STRINGS;
    static void buildWarps();

    PokeMap *pm = nullptr;
    int row = 0;
    int col = 0;

    Warp() = default;
    Warp(PokeMap *pm, int row, int col);
    std::string toString() const;
};
