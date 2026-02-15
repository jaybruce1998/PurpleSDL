#pragma once

#include "Item.h"
#include "Move.h"
#include "Monster.h"
#include "SpriteManager.h"
#include <optional>

class OverworldGui;

class WorldObject {
public:
    static std::vector<std::string> ITEM_STRINGS;
    static std::vector<std::string> ENCOUNTER_STRINGS;
    static std::unordered_map<std::string, SDL_Texture*> ITEM_SPRITES;
    static int wid;
    static void buildWorldObjects();
    static void buildWorldObjects(class Player *p, const std::string &wobS);

    bool interacted = false;
    SDL_Texture *bi = nullptr;
    int x = 0;
    int y = 0;
    int level = 0;
    int id = 0;
    Move *move = nullptr;
    Item *item = nullptr;
    Monster *mon = nullptr;

    WorldObject() = default;
    explicit WorldObject(const std::string &s);
    virtual ~WorldObject() = default;
    virtual std::optional<bool> stepOn(OverworldGui *gui);

    class WorldEncounter;
    class SnorlaxEncounter;
};

class WorldObject::WorldEncounter : public WorldObject {
public:
    explicit WorldEncounter(const std::string &s);
    std::optional<bool> stepOn(OverworldGui *gui) override;
};

class WorldObject::SnorlaxEncounter : public WorldEncounter {
public:
    explicit SnorlaxEncounter(const std::string &s);
    std::optional<bool> stepOn(OverworldGui *gui) override;
};
