#include "Player.h"
#include "OverworldGui.h"
#include "Trainer.h"
#include "WorldObject.h"
#include "Utils.h"

#include <algorithm>

static Item *MASTER_BALL = nullptr;

Player::Player(const std::string &name) : name(name) {
    if (!MASTER_BALL) {
        MASTER_BALL = Item::ITEM_MAP["Master Ball"];
    }
    tmHms.clear();
    items.clear();
    trainersBeaten.assign(Trainer::tid, false);
    leadersBeaten.assign(9, false);
    gioRivalsBeaten.assign(9, false);
    objectsCollected.assign(WorldObject::wid, false);
    pokedex.assign(152, false);
    team.assign(6, nullptr);
    pc.clear();
    seenItems.clear();
    ballin = true;
}

Player::Player(const std::string &pcS, const std::string &partyS, const std::string &dexS, const std::string &itemS,
               const std::string &tmS) {
    pc.clear();
    if (!pcS.empty()) {
        for (const std::string &p : utils::split(pcS, ';')) {
            pc.push_back(new Battler(p));
        }
    }
    team.assign(6, nullptr);
    std::vector<std::string> a = utils::split(partyS, ';');
    for (size_t i = 0; i < a.size(); i++) {
        team[i] = new Battler(a[i]);
    }
    pokedex.assign(dexS.size(), false);
    for (size_t i = 0; i < pokedex.size(); i++) {
        if (dexS[i] == '1') {
            pokedex[i] = true;
        }
    }
    items.clear();
    seenItems.clear();
    if (!itemS.empty()) {
        for (const std::string &s : utils::split(itemS, ';')) {
            a = utils::split(s, ',');
            Item *i = Item::ITEM_MAP[a[0]];
            seenItems.insert(i);
            Item *copy = new Item(*i);
            copy->quantity = std::stoi(a[1]);
            items.push_back(copy);
        }
    }
    tmHms.clear();
    a = utils::split(tmS, ',');
    for (size_t i = 1; i < a.size(); i++) {
        tmHms.insert(Move::MOVE_MAP[a[i]]);
    }
    if (!MASTER_BALL) {
        MASTER_BALL = Item::ITEM_MAP["Master Ball"];
    }
}

bool Player::give(Move *move) {
    if (tmHms.insert(move).second) {
        OverworldGui::print("You got " + move->name + "!");
        return true;
    }
    OverworldGui::print("You already have " + move->name + "!");
    return false;
}

void Player::give(Item *item) {
    if (item == MASTER_BALL) {
        ballin = false;
    }
    for (Item *i : items) {
        if (i->name == item->name) {
            i->quantity += item->quantity;
            OverworldGui::print("You got " + item->name + "!");
            return;
        }
    }
    items.push_back(new Item(*item));
    seenItems.insert(item);
    OverworldGui::print("You got " + item->name + "!");
}

void Player::give(Item *item, int n) {
    for (Item *i : items) {
        if (i->name == item->name) {
            i->quantity += n;
            return;
        }
    }
    Item *copy = new Item(*item);
    copy->quantity = n;
    items.push_back(copy);
    seenItems.insert(item);
}

void Player::registerBattler(Battler *battler) {
    if (!pokedex[battler->dexNum]) {
        pokedex[battler->dexNum] = true;
        numCaught++;
        OverworldGui::print("That's a new species!");
    }
}

void Player::give(Battler *battler) {
    std::string nick = OverworldGui::promptText("What is " + battler->name + "'s new nickname?");
    if (!nick.empty()) {
        nick.erase(std::remove(nick.begin(), nick.end(), ','), nick.end());
        if (nick.empty()) {
            battler->nickname = battler->name;
        } else {
            battler->nickname = nick.size() < 11 ? nick : nick.substr(0, 10);
        }
    }
    OverworldGui::print("You got " + battler->nickname + "!");
    if (team[5] != nullptr) {
        pc.push_back(battler);
        OverworldGui::print("It was sent to the PC!");
    } else {
        for (size_t i = 0; i < team.size(); i++) {
            if (team[i] == nullptr) {
                team[i] = battler;
                break;
            }
        }
    }
    registerBattler(battler);
}

bool Player::hasItem(Item *item) const {
    return seenItems.find(item) != seenItems.end();
}

bool Player::hasMove(Move *move) const {
    return tmHms.find(move) != tmHms.end();
}

void Player::healTeam() {
    for (size_t i = 0; i < team.size(); i++) {
        if (team[i] == nullptr) {
            return;
        }
        team[i]->fullyHeal();
    }
}

void Player::sell(Item *item, int q) {
    q = std::min(q, item->quantity);
    money += item->price / 2 * q;
    item->quantity -= q;
    if (item->quantity == 0) {
        items.erase(std::remove(items.begin(), items.end(), item), items.end());
    }
}

void Player::use(Item *item) {
    if (--item->quantity <= 0) {
        items.erase(std::remove(items.begin(), items.end(), item), items.end());
        Item *canon = Item::ITEM_MAP[item->name];
        if (canon) {
            seenItems.erase(canon);
        }
    }
}
