#include "Types.h"

std::vector<std::string> Types::TYPES = {"Normal", "Fire", "Water", "Electric", "Grass", "Ice", "Fighting", "Poison", "Ground",
                                         "Flying", "Psychic", "Bug", "Rock", "Ghost", "Dragon"};

std::vector<std::string> Types::ATTACK_STRINGS = {
    "111111111111501", "155122111112515", "125151112111215", "112551110211115", "152151152515215",
    "115125112211112", "211112151555201", "111121155112551", "121251121015211", "111521211112511",
    "111111221151111", "151121521521151", "121112515212111", "011111111121121", "111111111111112"};

std::unordered_map<std::string, int> Types::TYPE_MAP;
std::vector<std::vector<double>> Types::TYPE_CHART;

double Types::damage(const Move &move, const std::vector<std::string> &types) {
    double d = 1.0;
    if (move.type == "???") {
        return d;
    }
    int i = TYPE_MAP[move.type];
    for (const auto &t : types) {
        d *= TYPE_CHART[i][TYPE_MAP[t]];
    }
    return d;
}

void Types::buildTypes() {
    TYPE_CHART.assign(TYPES.size(), std::vector<double>(TYPES.size(), 1.0));
    for (int i = 0; i < static_cast<int>(TYPES.size()); i++) {
        TYPE_MAP[TYPES[i]] = i;
        for (int j = 0; j < static_cast<int>(TYPES.size()); j++) {
            char c = ATTACK_STRINGS[i][j];
            switch (c) {
                case '5':
                    TYPE_CHART[i][j] = 0.5;
                    break;
                default:
                    TYPE_CHART[i][j] = static_cast<double>(c - '0');
            }
        }
    }
}
