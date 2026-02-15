#pragma once

#include "Item.h"
#include "Move.h"
#include "Monster.h"

class MartItem {
public:
    static std::vector<std::string> MART_ITEM_STRINGS;
    static void buildMartItems();

    Move *move = nullptr;
    Item *item = nullptr;
    Monster *mon = nullptr;
    int price = 0;

    MartItem() = default;
    explicit MartItem(const std::string &s);
    std::string toString() const;
};
