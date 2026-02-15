#include "FlyLocation.h"
#include "PokeMap.h"

std::vector<std::vector<int>> FlyLocation::WORLD_MAP = {
    {5, 0, 0, 0, 0, 0, 0, 27, 38, 38, 0},  {26, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0},
    {26, 0, 8, 28, 28, 12, 29, 2, 34, 34, 13}, {26, 0, 22, 0, 0, 0, 0, 30, 0, 0, 15},
    {25, 25, 11, 0, 19, 1, 32, 9, 33, 33, 6}, {0, 0, 14, 0, 37, 0, 0, 31, 0, 0, 17},
    {0, 0, 7, 0, 37, 0, 0, 10, 16, 16, 17}, {0, 0, 24, 0, 37, 0, 0, 0, 0, 0, 17},
    {0, 0, 24, 0, 20, 20, 4, 18, 35, 36, 17}, {0, 0, 24, 0, 0, 0, 21, 0, 0, 0, 0},
    {0, 0, 3, 23, 23, 21, 21, 0, 0, 0, 0},
};

std::vector<std::string> FlyLocation::INDEX_MEANINGS = {
    "", "CeladonCity", "CeruleanCity", "CinnabarIsland", "FuchsiaCity", "IndigoPlateau", "LavenderTown", "PalletTown",
    "PewterCity", "SaffronCity", "VermilionCity", "ViridianCity", "MtMoon", "RockTunnel", "Route1", "Route10", "Route11",
    "Route12", "Route15", "Route16", "Route18", "Route19", "Route2", "Route20", "Route21", "Route22", "Route23", "Route24",
    "Route3", "Route4", "Route5", "Route6", "Route7", "Route8", "Route9", "Route14", "Route13", "Route17", "Route25"};

std::vector<std::vector<std::string>> FlyLocation::NAME_MAP;
std::unordered_map<std::string, FlyLocation*> FlyLocation::FLY_LOCATIONS;
SDL_Texture *FlyLocation::MAP_TEXTURE = nullptr;

FlyLocation::FlyLocation(PokeMap *dest, int x, int y) : dest(dest), x(x), y(y) {}

bool FlyLocation::loadMapTexture(SDL_Renderer *renderer, const std::string &path) {
    if (MAP_TEXTURE != nullptr) {
        SDL_DestroyTexture(MAP_TEXTURE);
        MAP_TEXTURE = nullptr;
    }
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface) {
        return false;
    }
    MAP_TEXTURE = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return MAP_TEXTURE != nullptr;
}

void FlyLocation::buildWorldMap() {
    NAME_MAP.assign(11, std::vector<std::string>(11, ""));
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            NAME_MAP[i][j] = INDEX_MEANINGS[WORLD_MAP[i][j]];
        }
    }
    FLY_LOCATIONS.clear();
    FlyLocation *p = new FlyLocation(PokeMap::POKEMAPS["PalletTown"], 5, 6);
    p->visited = true;
    FLY_LOCATIONS["PalletTown"] = p;
    FLY_LOCATIONS["IndigoPlateau"] = new FlyLocation(PokeMap::POKEMAPS["IndigoPlateau"], 10, 6);
}

std::string FlyLocation::normalize(const std::string &s) {
    std::string n = s.substr(0, s.size() - 10);
    if (n == "Cinnabar") {
        return "CinnabarIsland";
    }
    if (n == "Lavender") {
        return "LavenderTown";
    }
    if (n == "MtMoon" || n == "RockTunnel") {
        return n;
    }
    if (n == "IndigoPl") {
        return "IndigoPlateau";
    }
    return n + "City";
}

bool FlyLocation::isRed(int i, int j) {
    auto it = FLY_LOCATIONS.find(NAME_MAP[i][j]);
    if (it == FLY_LOCATIONS.end()) {
        return false;
    }
    return !it->second->visited;
}

void FlyLocation::visit(const std::string &name) {
    std::string key = normalize(name);
    auto it = FLY_LOCATIONS.find(key);
    if (it != FLY_LOCATIONS.end()) {
        it->second->visited = true;
    }
}
