#pragma once

#include "Utils.h"

class Item {
public:
    static std::vector<std::string> ITEM_STRINGS;
    static std::unordered_map<std::string, Item*> ITEM_MAP;

    static void buildItems();

    std::string name;
    int price = 0;
    int quantity = 1;
    bool heal = false;
    bool battle = false;
    bool wild = false;
    bool world = false;

    Item() = default;
    explicit Item(const std::vector<std::string> &a);
    explicit Item(const Item &i);

    bool equals(const Item &o) const;
    std::string toString() const;
};
