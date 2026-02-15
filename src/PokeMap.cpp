#include "PokeMap.h"
#include "Encounter.h"
#include "Trainer.h"
#include "Warp.h"
#include "WorldObject.h"
#include "Giver.h"
#include "Blocker.h"
#include "Trader.h"
#include "Npc.h"
#include "MartItem.h"
#include "Battler.h"
#include "Player.h"
#include "NpcStrings.h"
#include <iostream>

std::vector<int> PokeMap::TILE_TYPES = {0, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 7, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 1, 1, 0, 1, 6, 2, 0, 0, 1, 0, 0, 0, 1, 0, 1, 2, 0, 7, 0, 7, 1, 0, 0, 0, 1, 0, 0, 0, 7, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7, 1, 1, 0, 0, 0, 0, 20, 0, 9, 0, 0, 0, 0, 0, 0, 0, 8, 7, 0, 0, 8, 0, 0, 8, 0, 4, 0, 0, 0, 7, 0, 0, 2, 0, 0, 0, 0, 0, 1, 1, 7, 1, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 2, 2, 4, 1, 4, 4, 0, 0, 0, 0, 1, 0, 7, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 1, 4, 4, 0, 
	0, 0, 0, 0, 10, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 7, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 7, 11, 0, 0, 0, 0, 3, 19, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 12, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 0, 0, 7, 0, 0, 0, 0, 0, 0, 13, 0, 1, 14, 16, 17, 15, 0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 18, 18, 0, 0, 0, 0, 0, 0, 0, 1, 4, 2, 2, 0, 7, 0, 0, 1, 0, 0, 12, 4, 4, 4, 4, 4, 0, 
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 2, 0, 0, 12, 1, 1, 12, 12, 12, 4, 4, 18, 4, 1, 4, 18, 18, 1, 18, 4, 4, 0, 0, 0, 0, 0, 0, 7, 0, 0, 3, 0, 0, 0, 9, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 
	2, 2, 7, 7, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 7, 0, 0, 0, 1, 0, 1, 0, 0, 0, 4, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 21, 8, 0, 0, 0, 0, 0, 0,
	2, 18, 18, 2, 18, 18, 2, 18, 18, 2, 2, 18, 18, 2, 6, 2, 18, 2, 2, 12};

std::vector<std::string> PokeMap::CONNECTION_STRINGS = {
    "CeladonCity west Route16 8", "CeladonCity east Route7 8", "CeruleanCity north Route24 10",
    "CeruleanCity south Route5 10", "CeruleanCity west Route4 8", "CeruleanCity east Route9 8",
    "CinnabarIsland north Route21 0", "CinnabarIsland east Route20 0", "FuchsiaCity south Route19 10",
    "FuchsiaCity west Route18 8", "FuchsiaCity east Route15 8", "IndigoPlateau south Route23 0",
    "LavenderTown north Route10 0", "LavenderTown south Route12 0", "LavenderTown west Route8 0",
    "PalletTown north Route1 0", "PalletTown south Route21 0", "PewterCity south Route2 10",
    "PewterCity east Route3 8", "Route1 north ViridianCity -10", "Route1 south PalletTown 0",
    "Route10 south LavenderTown 0", "Route10 west Route9 0", "Route11 west VermilionCity -8",
    "Route11 east Route12 -54", "Route12 north LavenderTown 0", "Route12 south Route13 -40",
    "Route12 west Route11 54", "Route15 west FuchsiaCity -8", "Route15 east Route14 -36",
    "Route13 north Route12 40", "Route13 west Route14 0", "Route14 west Route15 36",
    "Route14 east Route13 0", "Route16 south Route17 0", "Route16 east CeladonCity -8",
    "Route18 north Route17 0", "Route18 east FuchsiaCity -8", "Route17 north Route16 0",
    "Route17 south Route18 0", "Route19 north FuchsiaCity -10", "Route19 west Route20 36",
    "Route2 north PewterCity -10", "Route2 south ViridianCity -10", "Route20 west CinnabarIsland 0",
    "Route20 east Route19 -36", "Route22 north Route23 0", "Route22 east ViridianCity -8",
    "Route21 north PalletTown 0", "Route21 south CinnabarIsland 0", "Route23 north IndigoPlateau 0",
    "Route23 south Route22 0", "Route24 south CeruleanCity -10", "Route24 east Route25 0",
    "Route25 west Route24 0", "Route3 north Route4 50", "Route3 west PewterCity -8",
    "Route5 north CeruleanCity -10", "Route5 south SaffronCity -10", "Route4 south Route3 -50",
    "Route4 east CeruleanCity -8", "Route6 north SaffronCity -10", "Route6 south VermilionCity -10",
    "Route7 west CeladonCity -8", "Route7 east SaffronCity -8", "Route8 west SaffronCity -8",
    "Route8 east LavenderTown 0", "Route9 west CeruleanCity -8", "Route9 east Route10 0",
    "SaffronCity north Route5 10", "SaffronCity south Route6 10", "SaffronCity west Route7 8",
    "SaffronCity east Route8 8", "VermilionCity north Route6 10", "VermilionCity east Route11 8",
    "ViridianCity north Route2 10", "ViridianCity south Route1 10", "ViridianCity west Route22 8"};

std::unordered_map<std::string, PokeMap*> PokeMap::POKEMAPS;

void PokeMap::buildPokeMaps() {
    std::vector<std::string> lines = utils::readLines("assets/tileIds.txt");
    for (const std::string &line : lines) {
        if (utils::trim(line).empty()) {
            continue;
        }
        size_t space = line.find(' ');
        if (space == std::string::npos) {
            throw std::runtime_error("Bad tileIds line (missing space): " + line);
        }
        std::string name = line.substr(0, space);
        std::string gridStr = line.substr(space + 1);
        POKEMAPS[name] = new PokeMap(name, gridStr, name.size() >= 10 && name.rfind("Pokecenter") == name.size() - 10 ? 3 : -2);
    }
    POKEMAPS["RedsHouse1F"]->setHeal(5);
    POKEMAPS["IndigoPlateauLobby"]->setHeal(7);
    for (const std::string &s : CONNECTION_STRINGS) {
        std::vector<std::string> a = utils::split(s, ' ');
        if (a.size() < 4) {
            throw std::runtime_error("Bad connection string: " + s);
        }
        if (!POKEMAPS.count(a[0]) || !POKEMAPS.count(a[2])) {
            throw std::runtime_error("Missing map in connection: " + s);
        }
        POKEMAPS[a[0]]->addConnection(a[1][0], POKEMAPS[a[2]], std::stoi(a[3]));
    }
}

PokeMap::PokeMap(const std::string &name, const std::string &s, int n) : name(name) {
    std::vector<std::string> a = utils::split(s, ';');
    if (a.empty()) {
        throw std::runtime_error("Empty map data for " + name);
    }
    std::vector<std::string> c = utils::split(a[0], ',');
    if (c.empty()) {
        throw std::runtime_error("Empty row in map " + name);
    }
    grid.assign(a.size(), std::vector<int>(c.size(), 0));
    int l = static_cast<int>(a.size()) - 1;
    for (int i = 0; i < l;) {
        if (c.size() != grid[i].size()) {
            throw std::runtime_error("Row width mismatch in map " + name + " at row " +
                                     std::to_string(i) + " (expected " + std::to_string(grid[i].size()) +
                                     ", got " + std::to_string(c.size()) + ")");
        }
        for (size_t j = 0; j < c.size(); j++) {
            grid[i][j] = std::stoi(c[j]);
        }
        c = utils::split(a[++i], ',');
    }
    if (c.size() != grid[l].size()) {
        throw std::runtime_error("Row width mismatch in map " + name + " at row " +
                                 std::to_string(l) + " (expected " + std::to_string(grid[l].size()) +
                                 ", got " + std::to_string(c.size()) + ")");
    }
    for (size_t j = 0; j < c.size(); j++) {
        grid[l][j] = std::stoi(c[j]);
    }
    types.assign(a.size(), std::vector<int>(c.size(), 0));
    sight.assign(a.size(), std::vector<int>(c.size(), 0));
    for (size_t i = 0; i < a.size(); i++) {
        for (size_t j = 0; j < c.size(); j++) {
            int tileId = grid[i][j];
            if (tileId < 0) {
                throw std::runtime_error("Tile id negative in map " + name + " at (" +
                                         std::to_string(i) + "," + std::to_string(j) + "): " +
                                         std::to_string(tileId));
            }
            if (tileId >= static_cast<int>(TILE_TYPES.size())) {
                size_t oldSize = TILE_TYPES.size();
                TILE_TYPES.resize(static_cast<size_t>(tileId) + 1, 0);
            }
            types[i][j] = TILE_TYPES[tileId];
        }
    }
    trainers.assign(a.size(), std::vector<Trainer*>(c.size(), nullptr));
    wob.assign(a.size(), std::vector<WorldObject*>(c.size(), nullptr));
    npcs.assign(a.size(), std::vector<Npc*>(c.size(), nullptr));
    healX = n;
    healY = n;
}

Trainer* PokeMap::getTrainer(int x, int y, int i) {
    if (trainers[y][x] != nullptr) {
        return trainers[y][x];
    }
    for (int r = y - 1; r >= 0; r--) {
        if (sight[r][x] != i) {
            break;
        } else if (trainers[r][x] != nullptr) {
            return trainers[r][x];
        }
    }
    for (int r = y + 1; r < static_cast<int>(trainers.size()); r++) {
        if (sight[r][x] != i) {
            break;
        } else if (trainers[r][x] != nullptr) {
            return trainers[r][x];
        }
    }
    for (int c = x - 1; c >= 0; c--) {
        if (sight[y][c] != i) {
            break;
        } else if (trainers[y][c] != nullptr) {
            return trainers[y][c];
        }
    }
    for (int c = x + 1; c < static_cast<int>(trainers[y].size()); c++) {
        if (sight[y][c] != i) {
            break;
        } else if (trainers[y][c] != nullptr) {
            return trainers[y][c];
        }
    }
    sight[y][x] = 0;
    return nullptr;
}

void PokeMap::deleteTrainer(Trainer *t, int i) {
    int y = t->y;
    int x = t->x;
    trainers[y][x] = nullptr;
    sight[y][x] = 0;
    for (int r = y - 1; r >= 0; r--) {
        if (sight[r][x] != i) {
            break;
        } else {
            sight[r][x] = 0;
        }
    }
    for (int r = y + 1; r < static_cast<int>(trainers.size()); r++) {
        if (sight[r][x] != i) {
            break;
        } else {
            sight[r][x] = 0;
        }
    }
    for (int c = x - 1; c >= 0; c--) {
        if (sight[y][c] != i) {
            break;
        } else {
            sight[y][c] = 0;
        }
    }
    for (int c = x + 1; c < static_cast<int>(trainers[y].size()); c++) {
        if (sight[y][c] != i) {
            break;
        } else {
            sight[y][c] = 0;
        }
    }
}

void PokeMap::addConnection(char d, PokeMap *pm, int off) {
    switch (d) {
        case 'n':
            north = pm;
            nOff = off;
            break;
        case 's':
            south = pm;
            sOff = off;
            break;
        case 'e':
            east = pm;
            eOff = off;
            break;
        case 'w':
            west = pm;
            wOff = off;
            break;
        default:
            throw std::runtime_error("wtf is " + std::string(1, d) + "?!");
    }
}

void PokeMap::addWarp(int row, int col, Warp *w) {
    warps[row * static_cast<int>(grid[0].size()) + col] = w;
}

Warp* PokeMap::getWarp(int row, int col) {
    int key = row * static_cast<int>(grid[0].size()) + col;
    auto it = warps.find(key);
    return it == warps.end() ? nullptr : it->second;
}

MartItem* PokeMap::getMartItems(int x, int y, int &outSize) {
    int key = x * static_cast<int>(grid[0].size()) + y;
    auto it = martMap.find(key);
    if (it == martMap.end()) {
        outSize = 0;
        return nullptr;
    }
    outSize = static_cast<int>(it->second.size());
    return it->second.data();
}

Warp* PokeMap::getNearbyWarp(int row, int col) {
    Warp *w = getWarp(row - 1, col);
    if (w == nullptr) {
        w = getWarp(row + 1, col);
        if (w == nullptr) {
            w = getWarp(row, col - 1);
            if (w == nullptr) {
                w = getWarp(row, col + 1);
            }
        }
    }
    return w;
}

void PokeMap::addEncounters(const std::vector<std::string> &a) {
    for (size_t i = 1; i < a.size(); i++) {
        std::vector<std::string> es = utils::split(a[i], ',');
        std::vector<Encounter> e;
        for (size_t j = 1; j < es.size(); j++) {
            e.emplace_back(es[j]);
        }
        encounters[es[0]] = e;
    }
}

Battler* PokeMap::getRandomEncounter(const std::string &type) {
    auto it = encounters.find(type);
    if (it == encounters.end()) {
        return nullptr;
    }
    std::vector<Encounter> &a = it->second;
    int r = utils::randInt(0, 100);
    int l = 0;
    for (const Encounter &e : a) {
        int n = l + e.chance;
        if (r < n) {
            return utils::rand01() < 0.5 ? new Battler(e.rLevel, Monster::MONSTER_MAP[e.rMonster->name])
                                         : new Battler(e.bLevel, Monster::MONSTER_MAP[e.bMonster->name]);
        }
        l = n;
    }
    throw std::runtime_error("Could not find encounter");
}

static void fillSight(PokeMap *pm, int v, int y, int x, int dy, int dx) {
    for (int i = 0; i < 4; i++) {
        y += dy;
        if (y < 0 || y >= static_cast<int>(pm->grid.size())) {
            return;
        }
        x += dx;
        if (x < 0 || x >= static_cast<int>(pm->grid[0].size()) || pm->sight[y][x] != 0) {
            return;
        }
        switch (pm->types[y][x]) {
            case 0:
            case 3:
            case 5:
            case 10:
            case 11:
            case 19:
            case 20:
                return;
            default:
                pm->sight[y][x] = v;
        }
    }
}

void PokeMap::addTrainer(const std::string &s, int i) {
    Trainer *t = new Trainer(s);
    if (t->y < 0 || t->x < 0 || t->y >= static_cast<int>(trainers.size()) ||
        t->x >= static_cast<int>(trainers[0].size())) {
        throw std::runtime_error("Trainer out of bounds in map " + name + " at (" +
                                 std::to_string(t->y) + "," + std::to_string(t->x) + ")");
    }
    trainers[t->y][t->x] = t;
    sight[t->y][t->x] = i;
    switch (t->dir) {
        case 1:
            fillSight(this, i, t->y, t->x, 1, 0);
            break;
        case 4:
            fillSight(this, i, t->y, t->x, -1, 0);
            break;
        case 6:
            fillSight(this, i, t->y, t->x, 0, -1);
            break;
        case 8:
            fillSight(this, i, t->y, t->x, 0, 1);
            break;
        default:
            throw std::runtime_error("BRO WHAT?!");
    }
    t->id = Trainer::tid++;
    if (t->id < 0 || t->id >= static_cast<int>(NpcStrings::TRAINER_SPEECH_STRINGS.size())) {
        t->phrases = {"..."};
    } else {
        t->phrases = utils::split(NpcStrings::TRAINER_SPEECH_STRINGS[t->id], ';');
    }
}

void PokeMap::addTrainers(const std::vector<std::string> &a) {
    for (size_t i = 1; i < a.size(); i++) {
        addTrainer(a[i], static_cast<int>(i));
    }
}

void PokeMap::addLeader(int i, const std::vector<std::string> &a) {
    Trainer *t = new Trainer::Leader(i, a);
    if (t->y < 0 || t->x < 0 || t->y >= static_cast<int>(trainers.size()) ||
        t->x >= static_cast<int>(trainers[0].size())) {
        throw std::runtime_error("Leader out of bounds in map " + name + " at (" +
                                 std::to_string(t->y) + "," + std::to_string(t->x) + ")");
    }
    trainers[t->y][t->x] = t;
    sight[t->y][t->x] = -2;
    t->phrases = utils::split(NpcStrings::LEADER_SPEECH_STRINGS[i], ';');
}

void PokeMap::addGioRival(int i, const std::string &s) {
    Trainer *t = new Trainer::GioRival(i, s);
    if (t->y < 0 || t->x < 0 || t->y >= static_cast<int>(trainers.size()) ||
        t->x >= static_cast<int>(trainers[0].size())) {
        throw std::runtime_error("GioRival out of bounds in map " + name + " at (" +
                                 std::to_string(t->y) + "," + std::to_string(t->x) + ")");
    }
    trainers[t->y][t->x] = t;
    sight[t->y][t->x] = -2;
    t->phrases = utils::split(NpcStrings::GIO_RIVAL_SPEECH_STRINGS[i], ';');
}

void PokeMap::addE4Trainer(int id, const std::string &s, const std::string &p) {
    Trainer *t = new Trainer::E4Trainer(id, s);
    if (t->y < 0 || t->x < 0 || t->y >= static_cast<int>(trainers.size()) ||
        t->x >= static_cast<int>(trainers[0].size())) {
        throw std::runtime_error("E4 trainer out of bounds in map " + name + " at (" +
                                 std::to_string(t->y) + "," + std::to_string(t->x) + ")");
    }
    trainers[t->y][t->x] = t;
    sight[t->y][t->x] = -2;
    t->phrases = utils::split(p, ';');
}

void PokeMap::addItem(const std::string &s) {
    WorldObject *wi = new WorldObject(s);
    wob[wi->y][wi->x] = wi;
}

void PokeMap::addItems(const std::vector<std::string> &a) {
    for (size_t i = 1; i < a.size(); i++) {
        addItem(a[i]);
    }
}

void PokeMap::addEncounter(const std::string &s) {
    WorldObject *we = new WorldObject::WorldEncounter(s);
    wob[we->y][we->x] = we;
}

void PokeMap::addSnorlaxEncounter(const std::string &s) {
    WorldObject *se = new WorldObject::SnorlaxEncounter(s);
    wob[se->y][se->x] = se;
}

void PokeMap::addGiver(const std::string &s, const std::string &q) {
    Giver *g = new Giver(s, q);
    npcs[g->y][g->x] = g;
}

void PokeMap::addGiver(const std::string &s, int n, const std::string &q) {
    Giver *g = new Giver::Aide(s, n, q);
    npcs[g->y][g->x] = g;
}

void PokeMap::addGiver(const std::string &s, const std::string &i, const std::string &q) {
    Giver *g = new Giver::IfGiver(s, i, q);
    npcs[g->y][g->x] = g;
}

void PokeMap::addMartItems(const std::vector<std::string> &a) {
    std::vector<MartItem> mi;
    for (size_t i = 2; i < a.size(); i++) {
        mi.emplace_back(a[i]);
    }
    std::vector<std::string> c = utils::split(a[1], ' ');
    int key = std::stoi(c[0]) * static_cast<int>(grid[0].size()) + std::stoi(c[1]);
    martMap[key] = mi;
}

void PokeMap::addBlocker(const std::string &s, const std::string &q) {
    Blocker *b = new Blocker(s, q);
    npcs[b->y][b->x] = b;
}

void PokeMap::addBlocker(const std::string &s, const std::string &q, int n) {
    Blocker *b = new Blocker(s, q, n);
    npcs[b->y][b->x] = b;
}

void PokeMap::setHeal(int n) {
    healX = n;
    healY = n;
}

void PokeMap::addNpc(const std::string &s, const std::string &q) {
    Npc *n = new Npc(s, q);
    npcs[n->y][n->x] = n;
}

void PokeMap::addTrader(const std::string &s, const std::string &p) {
    Trader *t = new Trader(s, p);
    npcs[t->y][t->x] = t;
}

void PokeMap::stepOn(Player *p, int x, int y) {
    p->objectsCollected[wob[y][x]->id] = true;
    wob[y][x] = nullptr;
    if (wob[y][x - 1] != nullptr) {
        p->objectsCollected[wob[y][x - 1]->id] = true;
        wob[y][x - 1] = nullptr;
    }
    if (wob[y][x + 1] != nullptr) {
        p->objectsCollected[wob[y][x + 1]->id] = true;
        wob[y][x + 1] = nullptr;
    }
}

std::string PokeMap::toString() const {
    return std::to_string(grid.size()) + "," + std::to_string(grid[0].size());
}
