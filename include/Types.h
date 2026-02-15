#pragma once

#include "Move.h"

class Types {
public:
    static std::vector<std::string> TYPES;
    static std::vector<std::string> ATTACK_STRINGS;
    static std::unordered_map<std::string, int> TYPE_MAP;
    static std::vector<std::vector<double>> TYPE_CHART;

    static void buildTypes();
    static double damage(const Move &move, const std::vector<std::string> &types);
};
