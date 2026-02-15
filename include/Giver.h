#pragma once

#include "Npc.h"
#include "Item.h"
#include "Move.h"

class Giver : public Npc {
public:
    static std::vector<std::string> GIVER_STRINGS;
    static std::vector<std::string> GIVER_QUOTE_STRINGS;
    static int gid;

    static void buildGivers();

    Move *move = nullptr;
    Item *item = nullptr;
    int id = 0;

    Giver() = default;
    Giver(const std::string &s, const std::string &q);
    void interact(Player *p) override;

    class Aide;
    class IfGiver;
};

class Giver::Aide : public Giver {
public:
    int dexNum = 0;
    Aide(const std::string &s, int dexNum, const std::string &q);
    void interact(Player *p) override;
};

class Giver::IfGiver : public Giver {
public:
    Item *preReq = nullptr;
    IfGiver(const std::string &s, const std::string &i, const std::string &q);
    void interact(Player *p) override;
};
