#pragma once

#include "Giver.h"

class Blocker : public Giver {
public:
    static std::vector<std::string> BLOCKER_STRINGS;
    static std::vector<std::string> BADGE_BLOCKER_STRINGS;
    static std::vector<std::string> BOULDER_STRINGS;
    static std::vector<std::string> BLOCKER_QUOTE_STRINGS;
    static std::vector<std::string> BADGE_QUOTE_STRINGS;

    static void buildBlockers();

    int numBadges = -1;

    Blocker() = default;
    Blocker(const std::string &s, const std::string &q);
    Blocker(const std::string &s, const std::string &q, int n);
    void interact(Player *p) override;
};
