#include "WorldObject.h"
#include "Battler.h"
#include "PokeMap.h"
#include "OverworldGui.h"
#include "Player.h"
#include "Utils.h"
#include "GameConfig.h"

static std::vector<std::string> splitLinesWorld(const std::string &s) {
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

std::vector<std::string> WorldObject::ITEM_STRINGS = splitLinesWorld(R"(Route2;POKE_BALL 13 54,HP Up;POKE_BALL 13 45,Moon Stone
Route4;POKE_BALL 57 3,Move=Whirlwind
Route9;POKE_BALL 10 15,Move=Teleport
Route12;POKE_BALL 14 35,Move=Pay Day;POKE_BALL 5 89,Iron
Route24;POKE_BALL 10 5,Move=Thunder Wave
CeruleanCave2F;POKE_BALL 29 9,Max Elixir;POKE_BALL 4 15,Nugget;POKE_BALL 13 6,Full Restore
CeruleanCave1F;POKE_BALL 7 13,PP Up;POKE_BALL 19 3,Full Restore;POKE_BALL 5 0,Ultra Ball
MtMoon1F;POKE_BALL 2 20,Potion;POKE_BALL 2 2,Potion;POKE_BALL 35 31,Rare Candy;POKE_BALL 36 23,Escape Rope;POKE_BALL 20 33,Moon Stone;POKE_BALL 5 32,Move=Water Gun
Museum1F;OLD_AMBER 16 2,Old Amber
PokemonMansion1F;POKE_BALL 14 3,Escape Rope;POKE_BALL 18 21,Carbos
PokemonTower3F;POKE_BALL 12 1,Escape Rope
PowerPlant;POKE_BALL 9 20,Carbos;POKE_BALL 32 18,Move=Reflect;POKE_BALL 21 25,Move=Thunder;POKE_BALL 25 18,Rare Candy;POKE_BALL 23 34,HP Up
RocketHideoutB1F;POKE_BALL 11 14,Escape Rope;POKE_BALL 9 17,Hyper Potion;60 3 24,Rare Candy;60 3 25,Rare Candy;60 3 26,Rare Candy;60 3 27,Rare Candy;60 3 12,Max Revive;60 3 7,Max Elixir
SafariZoneCenter;POKE_BALL 14 10,Nugget
SSAnne1FRooms;POKE_BALL 7 2,Move=Body Slam
VictoryRoad1F;POKE_BALL 11 0,Move=Sky Attack;POKE_BALL 9 2,Rare Candy
ViridianForest;POKE_BALL 25 11,PokeBall;POKE_BALL 12 29,Antidote;POKE_BALL 1 31,Potion
CeladonMansionRoofHouse;POKE_BALL 4 3,Pokemon=25 Eevee
CeruleanCaveB1F;POKE_BALL 16 9,Ultra Ball;POKE_BALL 18 1,Max Revive
FightingDojo;POKE_BALL 4 1,Pokemon=30 Hitmonlee;POKE_BALL 5 1,Pokemon=30 Hitmonchan
MtMoonB2F;FOSSIL 12 6,Helix Fossil;FOSSIL 13 6,Dome Fossil;POKE_BALL 25 21,Move=Mega Punch;POKE_BALL 29 5,Ether
PokemonMansion2F;POKE_BALL 28 7,Calcium
PokemonMansion3F;POKE_BALL 1 16,Iron;POKE_BALL 25 5,Max Potion
PokemonMansionB1F;POKE_BALL 10 2,Rare Candy;POKE_BALL 1 22,Move=Solar Beam;POKE_BALL 19 25,Full Restore;POKE_BALL 5 4,Secret Key;POKE_BALL 5 13,Move=Blizzard
PokemonTower4F;POKE_BALL 12 10,Awakening;POKE_BALL 9 10,Elixir;POKE_BALL 12 16,HP Up
PokemonTower6F;POKE_BALL 6 8,X Accuracy;POKE_BALL 14 14,Rare Candy
PokemonTower5F;POKE_BALL 6 14,Nugget
RocketHideoutB2F;POKE_BALL 1 11,Nugget;POKE_BALL 16 8,Move=Horn Drill;POKE_BALL 6 12,Moon Stone;POKE_BALL 3 21,Super Potion
RocketHideoutB3F;POKE_BALL 26 17,Rare Candy;POKE_BALL 20 14,Move=Double-Edge
RocketHideoutB4F;POKE_BALL 10 12,Great Ball;POKE_BALL 9 4,Move=Razor Wind;POKE_BALL 12 20,HP Up;POKE_BALL 25 2,Silph Scope;POKE_BALL 10 2,Lift Key
Route25;POKE_BALL 22 2,Move=Seismic Toss
SSAnne2FRooms;POKE_BALL 7 1,Max Ether;POKE_BALL 15 2,Rare Candy
SSAnneB1FRooms;POKE_BALL 10 2,Move=Rest;POKE_BALL 5 2,Ether;POKE_BALL 22 1,Max Potion
SafariZoneEast;POKE_BALL 21 10,Move=Egg Bomb;POKE_BALL 3 7,Max Potion;POKE_BALL 20 13,Full Restore;POKE_BALL 15 12,Carbos
SafariZoneNorth;POKE_BALL 25 1,Move=Skull Bash;POKE_BALL 19 7,Protein
SafariZoneWest;POKE_BALL 8 20,Max Revive;POKE_BALL 9 7,Max Potion;POKE_BALL 18 18,Gold Teeth;POKE_BALL 19 7,Move=Double Team
SilphCo3F;POKE_BALL 8 5,Hyper Potion
SilphCo4F;POKE_BALL 3 9,Escape Rope;POKE_BALL 4 7,Max Revive;POKE_BALL 5 8,Full Heal
SilphCo5F;POKE_BALL 2 13,Move=Take Down;POKE_BALL 4 6,Protein;POKE_BALL 21 16,Card Key
SilphCo6F;POKE_BALL 3 12,HP Up;POKE_BALL 2 15,X Accuracy
SilphCo7F;POKE_BALL 1 9,Calcium;POKE_BALL 24 11,Move=Swords Dance
SilphCo10F;POKE_BALL 2 12,Move=Earthquake;POKE_BALL 4 14,Rare Candy;POKE_BALL 5 11,Carbos
VictoryRoad2F;POKE_BALL 27 5,Guard Spec.;POKE_BALL 18 9,Move=Mega Kick;POKE_BALL 9 11,Move=Submission;POKE_BALL 11 0,Full Heal
VictoryRoad3F;POKE_BALL 26 5,Move=Explosion;POKE_BALL 7 7,Max Revive
WardensHouse;POKE_BALL 8 3 DOWN,Rare Candy
ViridianGym;POKE_BALL 16 9 DOWN,Revive
Route21;POKE_BALL 3 0 DOWN,Max Revive)");

std::vector<std::string> WorldObject::ENCOUNTER_STRINGS = splitLinesWorld(R"(Route12;SNORLAX 10 62,Pokemon=30 Snorlax
Route16;SNORLAX 26 10,Pokemon=30 Snorlax
PowerPlant;BIRD 4 9,Pokemon=50 Zapdos
PowerPlant;POKE_BALL 26 28,Pokemon=10 Voltorb
PowerPlant;POKE_BALL 21 14,Pokemon=20 Voltorb
PowerPlant;POKE_BALL 37 32,Pokemon=35 Electrode
SeafoamIslandsB4F;BIRD 6 1,Pokemon=50 Articuno
VictoryRoad2F;BIRD 11 5,Pokemon=60 Moltres
CeruleanCaveB1F;MONSTER 27 13,Pokemon=70 Mewtwo
RocketHideoutB1F;60 3 1,Pokemon=200 Missingno.)");

std::unordered_map<std::string, SDL_Texture*> WorldObject::ITEM_SPRITES;
int WorldObject::wid = 0;

void WorldObject::buildWorldObjects() {
    for (const std::string &s : ITEM_STRINGS) {
        std::vector<std::string> a = utils::split(s, ';');
        if (a.empty()) {
            continue;
        }
        if (!PokeMap::POKEMAPS.count(a[0])) {
            throw std::runtime_error("WorldObject item map missing: " + a[0]);
        }
        PokeMap::POKEMAPS[a[0]]->addItems(a);
    }
    for (int i = 0; i < 2; i++) {
        std::vector<std::string> a = utils::split(ENCOUNTER_STRINGS[i], ';');
        if (a.size() < 2) {
            throw std::runtime_error("WorldObject encounter string malformed");
        }
        if (!PokeMap::POKEMAPS.count(a[0])) {
            throw std::runtime_error("WorldObject encounter map missing: " + a[0]);
        }
        PokeMap::POKEMAPS[a[0]]->addSnorlaxEncounter(a[1]);
    }
    for (size_t i = 2; i < ENCOUNTER_STRINGS.size(); i++) {
        std::vector<std::string> a = utils::split(ENCOUNTER_STRINGS[i], ';');
        if (a.size() < 2) {
            throw std::runtime_error("WorldObject encounter string malformed");
        }
        if (!PokeMap::POKEMAPS.count(a[0])) {
            throw std::runtime_error("WorldObject encounter map missing: " + a[0]);
        }
        PokeMap::POKEMAPS[a[0]]->addEncounter(a[1]);
    }
}

void WorldObject::buildWorldObjects(Player *p, const std::string &wobS) {
    p->objectsCollected.assign(wobS.size(), false);
    for (const std::string &s : ITEM_STRINGS) {
        std::vector<std::string> a = utils::split(s, ';');
        PokeMap *pm = PokeMap::POKEMAPS[a[0]];
        for (size_t i = 1; i < a.size(); i++) {
            if (wobS[wid] == '1') {
                p->objectsCollected[wid++] = true;
            } else {
                pm->addItem(a[i]);
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        std::vector<std::string> a = utils::split(ENCOUNTER_STRINGS[i], ';');
        if (wobS[wid] == '1') {
            p->objectsCollected[wid++] = true;
        } else {
            PokeMap::POKEMAPS[a[0]]->addSnorlaxEncounter(a[1]);
        }
    }
    for (size_t i = 2; i < ENCOUNTER_STRINGS.size(); i++) {
        std::vector<std::string> a = utils::split(ENCOUNTER_STRINGS[i], ';');
        if (wobS[wid] == '1') {
            p->objectsCollected[wid++] = true;
        } else {
            PokeMap::POKEMAPS[a[0]]->addEncounter(a[1]);
        }
    }
}

WorldObject::WorldObject(const std::string &s) {
    id = wid++;
    std::vector<std::string> a = utils::split(s, ',');
    std::vector<std::string> f = utils::split(a[0], ' ');
    auto it = ITEM_SPRITES.find(f[0]);
    if (it == ITEM_SPRITES.end()) {
        std::string path = "assets/sprites/" + f[0] + "/0.png";
        SDL_Texture *tex = SpriteManager::loadScaledTexture(path, GameConfig::TILE_SIZE, GameConfig::TILE_SIZE);
        ITEM_SPRITES[f[0]] = tex;
        bi = tex;
    } else {
        bi = it->second;
    }
    x = std::stoi(f[1]);
    y = std::stoi(f[2]);
    if (a[1].rfind("Move", 0) == 0) {
        move = Move::MOVE_MAP[a[1].substr(5)];
    } else if (a[1].rfind("Pokem", 0) == 0) {
        std::vector<std::string> p = utils::split(a[1].substr(8), ' ');
        level = std::stoi(p[0]);
        mon = Monster::MONSTER_MAP[p[1]];
    } else {
        item = Item::ITEM_MAP[a[1]];
    }
}

std::optional<bool> WorldObject::stepOn(OverworldGui *gui) {
    Player *p = gui->player;
    if (move != nullptr) {
        p->give(move);
    } else if (item != nullptr) {
        p->give(item);
    } else {
        p->give(new Battler(level, mon));
    }
    interacted = true;
    return true;
}

WorldObject::WorldEncounter::WorldEncounter(const std::string &s) : WorldObject(s) {}

std::optional<bool> WorldObject::WorldEncounter::stepOn(OverworldGui *gui) {
    return std::nullopt;
}

static Item *POKEFLUTE = nullptr;

WorldObject::SnorlaxEncounter::SnorlaxEncounter(const std::string &s) : WorldEncounter(s) {}

std::optional<bool> WorldObject::SnorlaxEncounter::stepOn(OverworldGui *gui) {
    if (!POKEFLUTE) {
        POKEFLUTE = Item::ITEM_MAP["Pokeflute"];
    }
    Player *p = gui->player;
    if (p->hasItem(POKEFLUTE)) {
        return std::nullopt;
    }
    gui->print("It's fast asleep...");
    return false;
}
