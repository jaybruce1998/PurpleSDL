#pragma once

#include "Utils.h"

class PokeMap;

class FlyLocation {
public:
    static std::vector<std::vector<int>> WORLD_MAP;
    static std::vector<std::string> INDEX_MEANINGS;
    static std::vector<std::vector<std::string>> NAME_MAP;
    static std::unordered_map<std::string, FlyLocation*> FLY_LOCATIONS;
    static SDL_Texture *MAP_TEXTURE;

    static void buildWorldMap();
    static bool isRed(int i, int j);
    static void visit(const std::string &name);
    static std::string normalize(const std::string &s);
    static bool loadMapTexture(SDL_Renderer *renderer, const std::string &path);

    bool visited = false;
    PokeMap *dest = nullptr;
    int x = 0;
    int y = 0;

    FlyLocation() = default;
    FlyLocation(PokeMap *dest, int x, int y);
};
