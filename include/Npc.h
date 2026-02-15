#pragma once

#include "SpriteManager.h"
#include "Move.h"

class Player;

class Npc {
public:
    static std::vector<std::string> NPC_STRINGS;
    static void buildNpcs();

    SDL_Texture *bi = nullptr;
    int x = 0;
    int y = 0;
    std::vector<std::string> quotes;
    bool dead = false;

    Npc() = default;
    Npc(const std::string &s, const std::string &q);
    virtual ~Npc() = default;

    virtual void interact(Player *p);
};
