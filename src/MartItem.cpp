#include "MartItem.h"
#include "PokeMap.h"
#include "Utils.h"

static std::vector<std::string> splitLinesMart(const std::string &s) {
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

std::vector<std::string> MartItem::MART_ITEM_STRINGS = splitLinesMart(R"(ViridianMart,2 5,PokeBall,Antidote,Paralyze Heal,Burn Heal,Move=Ice Beam 5000
ViridianMart,0 7,PokeBall,Antidote,Paralyze Heal,Burn Heal,Move=Absorb 500
PewterMart,2 5,PokeBall,Potion,Escape Rope,Antidote,Burn Heal,Awakening,Paralyze Heal,Repel,Move=Flamethrower 5000
PewterMart,0 7,PokeBall,Potion,Escape Rope,Antidote,Burn Heal,Awakening,Paralyze Heal,Repel,Move=Acid 750
CeruleanMart,2 5,PokeBall,Potion,Repel,Antidote,Burn Heal,Awakening,Paralyze Heal,Move=Confusion 500
CeruleanMart,0 7,PokeBall,Potion,Repel,Antidote,Burn Heal,Awakening,Paralyze Heal,Move=Acid Armor 2500
VermilionMart,2 5,PokeBall,Super Potion,Ice Heal,Awakening,Paralyze Heal,Repel,Move=Hydro Pump 5000
VermilionMart,0 7,PokeBall,Super Potion,Ice Heal,Awakening,Paralyze Heal,Repel,Move=Agility 2500
LavenderMart,2 5,Great Ball,Super Potion,Revive,Escape Rope,Super Repel,Antidote,Burn Heal,Ice Heal,Paralyze Heal,Move=Amnesia 2500
LavenderMart,0 7,Great Ball,Super Potion,Revive,Escape Rope,Super Repel,Antidote,Burn Heal,Ice Heal,Paralyze Heal,Move=Aurora Beam 2500
CeladonMart2F,3 3,Great Ball,Super Potion,Revive,Super Repel,Antidote,Burn Heal,Ice Heal,Awakening,Paralyze Heal,Move=Barrier 2500
CeladonMart2F,5 5,Great Ball,Super Potion,Revive,Super Repel,Antidote,Burn Heal,Ice Heal,Awakening,Paralyze Heal,Move=Barrage 500
CeladonMart2F,6 5,Move=Double Team 1000,Move=Reflect 1000,Move=Razor Wind 2000,Move=Horn Drill 2000,Move=Egg Bomb 2000,Move=Mega Punch 3000,Move=Mega Kick 3000,Move=Take Down 3000,Move=Submission 3000
CeladonMart2F,8 3,Move=Double Team 1000,Move=Reflect 1000,Move=Razor Wind 2000,Move=Horn Drill 2000,Move=Egg Bomb 2000,Move=Mega Punch 3000,Move=Mega Kick 3000,Move=Take Down 3000,Move=Submission 3000
CeladonMart4F,5 5,Pokedoll,Fire Stone,Thunder Stone,Water Stone,Leaf Stone,Moon Stone,Move=Bind 500
CeladonMart5F,3 3,X Accuracy,Guard Spec.,Dire Hit,X Attack,X Defend,X Speed,X Special Attack,X Special Defend,Move=Bite 1500
CeladonMart5F,5 5,X Accuracy,Guard Spec.,Dire Hit,X Attack,X Defend,X Speed,X Special Attack,X Special Defend,Move=Bone Club 1500
CeladonMart5F,6 5,HP Up,Protein,Iron,Carbos,Calcium,Zinc,Move=Bonemerang 2500
CeladonMart5F,8 3,HP Up,Protein,Iron,Carbos,Calcium,Zinc,Move=Bubble 500
CeladonMartRoof,10 2,Fresh Water,Lemonade,Soda Pop,Move=Clamp 500
CeladonMartRoof,11 2,Fresh Water,Lemonade,Soda Pop,Move=Comet Punch 750
CeladonMartRoof,12 3,Fresh Water,Lemonade,Soda Pop,Move=Confuse Ray 2000
GameCornerPrizeRoom,2 3,Pokemon=Abra 4600,Pokemon=Vulpix 20000,Pokemon=Wigglytuff 53600,Move=Rock Rampage 5000
GameCornerPrizeRoom,4 3,Pokemon=Scyther 130000,Pokemon=Pinsir 130000,Pokemon=Porygon 200000,Move=Disable 500
GameCornerPrizeRoom,6 3,Move=Solar Beam 66000,Move=Hyper Beam 110000,Move=Substitute 154000,Move=Defense Curl 250
SaffronMart,2 5,Great Ball,Hyper Potion,Max Repel,Escape Rope,Full Heal,Revive,Move=Ember 250
SaffronMart,0 7,Great Ball,Hyper Potion,Max Repel,Escape Rope,Full Heal,Revive,Move=Drill Peck 3000
FuchsiaMart,2 5,Ultra Ball,Great Ball,Super Potion,Revive,Full Heal,Super Repel,Move=Big Money 2500
FuchsiaMart,0 7,Ultra Ball,Great Ball,Super Potion,Revive,Full Heal,Super Repel,Move=Big Money 2500
CinnabarMart,2 5,Ultra Ball,Great Ball,Hyper Potion,Max Repel,Escape Rope,Full Heal,Revive,Move=Glare 3000
CinnabarMart,0 7,Ultra Ball,Great Ball,Hyper Potion,Max Repel,Escape Rope,Full Heal,Revive,Move=Twineedle 2500
IndigoPlateauLobby,2 5,Ultra Ball,Great Ball,Full Restore,Max Potion,Full Heal,Revive,Max Repel,PP Up,Move=Vine Whip 5000
IndigoPlateauLobby,0 7,Ultra Ball,Great Ball,Full Restore,Max Potion,Full Heal,Revive,Max Repel,Rare Candy,Move=Waterfall 3200)");

void MartItem::buildMartItems() {
    for (const std::string &s : MART_ITEM_STRINGS) {
        std::vector<std::string> a = utils::split(s, ',');
        PokeMap::POKEMAPS[a[0]]->addMartItems(a);
    }
}

MartItem::MartItem(const std::string &s) {
    if (s.rfind("Move", 0) == 0) {
        size_t i = s.find_last_of(' ');
        move = Move::MOVE_MAP[s.substr(5, i - 5)];
        price = std::stoi(s.substr(i + 1));
    } else if (s.rfind("Pokem", 0) == 0) {
        size_t i = s.find_last_of(' ');
        mon = Monster::MONSTER_MAP[s.substr(8, i - 8)];
        price = std::stoi(s.substr(i + 1));
    } else {
        item = Item::ITEM_MAP[s];
        price = item->price;
    }
}

std::string MartItem::toString() const {
    std::string n = move ? move->name : (item ? item->name : (mon ? mon->name : ""));
    return n + "   $" + std::to_string(price);
}
