#pragma once

#include "Item.h"
#include "Move.h"
#include "Battler.h"

class Player {
public:
    std::unordered_set<Move*> tmHms;
    std::vector<Item*> items;
    int numCaught = 0;
    int money = 0;
    std::vector<bool> trainersBeaten;
    std::vector<bool> leadersBeaten;
    std::vector<bool> gioRivalsBeaten;
    std::vector<bool> objectsCollected;
    std::vector<bool> pokedex;
    std::unordered_set<Item*> seenItems;
    std::vector<Battler*> team;
    std::vector<Battler*> pc;
    std::string name;
    bool ballin = true;

    Player() = default;
    explicit Player(const std::string &name);
    Player(const std::string &pcS, const std::string &partyS, const std::string &dexS, const std::string &itemS,
           const std::string &tmS);

    bool give(Move *move);
    void give(Item *item);
    void give(Item *item, int n);
    void registerBattler(Battler *battler);
    void give(Battler *battler);
    bool hasItem(Item *item) const;
    bool hasMove(Move *move) const;
    void healTeam();
    void sell(Item *item, int q);
    void use(Item *item);
};
