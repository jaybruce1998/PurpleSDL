#include "Item.h"

static std::vector<std::string> splitLinesItem(const std::string &s) {
    std::vector<std::string> lines;
    std::stringstream ss(s);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::string> Item::ITEM_STRINGS = splitLinesItem(R"(Antidote,100
Awakening,250
Bicycle,-1
Bike Voucher,-1
Burn Heal,250
Calcium,9800
Carbos,9800
Card Key,-1
Coin,50
Coin Case,-1
Dire Hit,1000
Dome Fossil,-1
Elixir,10000
Escape Rope,550
Ether,2500
Exp. All,-1
Fire Stone,2100
Fresh Water,200
Full Heal,600
Full Restore,3000
Gold Teeth,-1
Good Rod,-1
Great Ball,600
Guard Spec.,700
Helix Fossil,-1
HP Up,9800
Hyper Potion,1500
Ice Heal,250
Iron,9800
Item Finder,-1
Leaf Stone,2100
Lemonade,350
Lift Key,-1
Master Ball,10000
Max Elixir,20000
Max Ether,5000
Max Potion,2500
Max Repel,700
Max Revive,10000
Moon Stone,8400
Nugget,10000
Oak's Parcel,-1
Old Amber,-1
Old Rod,-1
Paralyze Heal,200
PokeBall,200
Pokedex,-1
Pokedoll,1000
Pokeflute,-1
Potion,300
PP Up,100000
Protein,9800
Rare Candy,100000
Repel,350
Revive,1500
SS Ticket,-1
Safari Ball (30),1000
Secret Key,-1
Silph Scope,-1
Soda Pop,300
Super Potion,700
Super Repel,500
Super Rod,-1
Thunder Stone,2100
Town Map,-1
Ultra Ball,1200
Water Stone,2100
X Accuracy,950
X Attack,500
X Defend,550
X Special Attack,350
X Special Defend,650
X Speed,350
Amulet Coin,-1
Lucky Egg,-1
Shiny Charm,-1
Fake ID,-1
...Secret Sauce,-1
Key to Darkness,-1
Zinc,9800)");

std::unordered_map<std::string, Item*> Item::ITEM_MAP;

Item::Item(const std::vector<std::string> &a) {
    name = a[0];
    switch (name[0] ? name[0] : ' ') {
        default:
            break;
    }
    if (name == "Antidote" || name == "Awakening" || name == "Fresh Water" || name == "Lemonade" || name == "Soda Pop") {
        heal = true;
    } else {
        heal = name.size() >= 4 &&
               (name.rfind("Heal") == name.size() - 4 || name.rfind("Ether") == name.size() - 5 ||
                name.rfind("Restore") == name.size() - 7 || name.rfind("Elixir") == name.size() - 6 ||
                name.rfind("Potion") == name.size() - 6 || name.rfind("Revive") == name.size() - 6);
    }
    if (name == "Full Restore") {
        heal = true;
    }
    battle = heal || name.rfind("X", 0) == 0 || name == "Dire Hit" || name == "Guard Spec." || name == "Poke Doll";
    wild = battle || (name.size() >= 4 && name.rfind("Ball") == name.size() - 4);
    if (name == "Full Restore") {
        battle = true;
        wild = true;
        world = true;
    }
    if (name == "Calcium" || name == "Carbos" || name == "Escape Rope" || name == "Iron" || name == "Item Finder" ||
        name == "Protein" || name == "Town Map" || name == "Rare Candy") {
        world = true;
    } else {
        world = heal || (name.size() >= 5 && name.rfind("Stone") == name.size() - 5) ||
                (name.size() >= 3 && name.rfind("Rod") == name.size() - 3) ||
                (name.size() >= 5 && name.rfind("Repel") == name.size() - 5) ||
                (name.size() >= 2 && name.rfind("Up") == name.size() - 2);
    }
    price = std::stoi(a[1]);
    quantity = 1;
}

Item::Item(const Item &i) {
    name = i.name;
    price = i.price;
    quantity = i.quantity;
    heal = i.heal;
    battle = i.battle;
    wild = i.wild;
    world = i.world;
}

bool Item::equals(const Item &o) const {
    return name == o.name;
}

std::string Item::toString() const {
    return name + " x" + std::to_string(quantity);
}

void Item::buildItems() {
    for (const std::string &i : ITEM_STRINGS) {
        std::vector<std::string> a = utils::split(i, ',');
        ITEM_MAP[a[0]] = new Item(a);
    }
}
