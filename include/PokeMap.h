#pragma once

#include "Utils.h"

class Warp;
class Encounter;
class Trainer;
class WorldObject;
class Npc;
class MartItem;
class Battler;
class Player;

class PokeMap {
public:
    static std::vector<int> TILE_TYPES;
    static std::vector<std::string> CONNECTION_STRINGS;
    static std::unordered_map<std::string, PokeMap*> POKEMAPS;

    static void buildPokeMaps();

    std::vector<std::vector<int>> grid;
    std::vector<std::vector<int>> types;
    PokeMap *north = nullptr;
    PokeMap *south = nullptr;
    PokeMap *east = nullptr;
    PokeMap *west = nullptr;
    int nOff = 0;
    int sOff = 0;
    int eOff = 0;
    int wOff = 0;
    int healX = 0;
    int healY = 0;
    std::string name;
    std::unordered_map<int, Warp*> warps;
    std::unordered_map<std::string, std::vector<Encounter>> encounters;
    std::vector<std::vector<Trainer*>> trainers;
    std::vector<std::vector<int>> sight;
    std::vector<std::vector<WorldObject*>> wob;
    std::vector<std::vector<Npc*>> npcs;
    std::unordered_map<int, std::vector<MartItem>> martMap;

    PokeMap(const std::string &name, const std::string &s, int n);

    Trainer* getTrainer(int x, int y, int i);
    void deleteTrainer(Trainer *t, int i);
    void addConnection(char d, PokeMap *pm, int off);
    void addWarp(int row, int col, Warp *w);
    Warp* getWarp(int row, int col);
    MartItem* getMartItems(int x, int y, int &outSize);
    Warp* getNearbyWarp(int row, int col);
    void addEncounters(const std::vector<std::string> &a);
    Battler* getRandomEncounter(const std::string &type);
    void addTrainer(const std::string &s, int i);
    void addTrainers(const std::vector<std::string> &a);
    void addLeader(int i, const std::vector<std::string> &a);
    void addGioRival(int i, const std::string &s);
    void addE4Trainer(int id, const std::string &s, const std::string &p);
    void addItem(const std::string &s);
    void addItems(const std::vector<std::string> &a);
    void addEncounter(const std::string &s);
    void addSnorlaxEncounter(const std::string &s);
    void addGiver(const std::string &s, const std::string &q);
    void addGiver(const std::string &s, int n, const std::string &q);
    void addGiver(const std::string &s, const std::string &i, const std::string &q);
    void addMartItems(const std::vector<std::string> &a);
    void addBlocker(const std::string &s, const std::string &q);
    void addBlocker(const std::string &s, const std::string &q, int n);
    void setHeal(int n);
    void addNpc(const std::string &s, const std::string &q);
    void addTrader(const std::string &s, const std::string &p);
    void stepOn(Player *p, int x, int y);

    std::string toString() const;
};
