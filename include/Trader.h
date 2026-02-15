#pragma once

#include "Giver.h"
#include "Monster.h"

class Trader : public Giver {
public:
    static std::vector<std::string> DEX_STRINGS;
    static std::vector<std::string> CLIPBOARD_STRINGS;
    static std::vector<std::string> MONSTER_STRINGS;
    static std::vector<std::string> FAIRY_STRINGS;
    static std::vector<std::string> BIRD_STRINGS;
    static std::vector<std::string> TRADER_STRINGS;

    static void buildTraders();

    std::string monA;
    Monster *monB = nullptr;

    Trader() = default;
    Trader(const std::string &s, const std::string &p);
    void interact(Player *p) override;
};
