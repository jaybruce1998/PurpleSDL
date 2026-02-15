#include "BattleState.h"
#include "Blocker.h"
#include "Encounter.h"
#include "Evolution.h"
#include "FlyLocation.h"
#include "GameConfig.h"
#include "Giver.h"
#include "Item.h"
#include "LevelUpMove.h"
#include "Main.h"
#include "MartItem.h"
#include "Monster.h"
#include "Move.h"
#include "Npc.h"
#include "OverworldGui.h"
#include "PokeMap.h"
#include "Player.h"
#include "SpriteManager.h"
#include "TextRenderer.h"
#include "TmLearnsets.h"
#include "Trader.h"
#include "Trainer.h"
#include "Types.h"
#include "Warp.h"
#include "WorldObject.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <algorithm>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream>
#include <vector>

static bool initSdl(SDL_Window *&window, SDL_Renderer *&renderer) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        std::cerr << "IMG_Init failed: " << IMG_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Pokemon Purple", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              GameConfig::WINDOW_WIDTH, GameConfig::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        return false;
    }
    SpriteManager::setRenderer(renderer);
    return true;
}

static std::string findFontPath() {
    std::vector<std::string> candidates;
    candidates.push_back("assets/PKMN-RBYGSC.ttf");

    char *base = SDL_GetBasePath();
    if (base) {
        std::string basePath(base);
        SDL_free(base);
        candidates.push_back(basePath + "assets/PKMN-RBYGSC.ttf");
        candidates.push_back(basePath + "..\\assets\\PKMN-RBYGSC.ttf");
        candidates.push_back(basePath + "..\\..\\assets\\PKMN-RBYGSC.ttf");
    }

    for (const auto &path : candidates) {
        std::ifstream file(path);
        if (file.good()) {
            return path;
        }
    }
    return candidates.front();
}

static bool fileExists(const std::string &path) {
    return std::filesystem::exists(std::filesystem::u8path(path));
}

static void ensureWorkingDir() {
    namespace fs = std::filesystem;
    fs::path cwd = fs::current_path();
    if (fs::exists(cwd / "assets")) {
        return;
    }
    char *base = SDL_GetBasePath();
    if (!base) {
        return;
    }
    fs::path basePath(base);
    SDL_free(base);
    fs::path candidate = fs::weakly_canonical(basePath / ".." / "..");
    if (fs::exists(candidate / "assets")) {
        fs::current_path(candidate);
    }
}

static void logStartupInfo(const std::string &fontPath) {
    std::vector<std::string> assetChecks = {
        "assets/tiles/0.png",
        "assets/sprites/RED/0.png",
        "assets/sprites/SEEL/0.png",
        "assets/battlers/0.png",
        "assets/battlers_back/0.png",
        "assets/dance/0.png"
    };
    std::vector<std::string> dataChecks = {
        "data/maps/RedsHouse2F.txt",
        "data/maps/WorldMap.txt",
        "data/maps/Warps.txt"
    };
    (void)assetChecks;
    (void)dataChecks;
}

static void shutdownSdl(SDL_Window *window, SDL_Renderer *renderer) {
    SpriteManager::clear();
    TextRenderer::shutdown();
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

static void buildGameData() {
    auto runStep = [](const char *name, const std::function<void()> &fn) {
        (void)name;
        fn();
    };
    runStep("Types", [] { Types::buildTypes(); });
    runStep("Monsters", [] { Monster::buildMonsters(); });
    runStep("Moves", [] { Move::buildMoves(); });
    runStep("Learnsets", [] { LevelUpMove::buildLevelUpMoves(); });
    runStep("Evolutions", [] { Evolution::buildEvolutions(); });
    runStep("PokeMaps", [] { PokeMap::buildPokeMaps(); });
    runStep("WorldMap", [] { FlyLocation::buildWorldMap(); });
    runStep("Warps", [] { Warp::buildWarps(); });
    runStep("TmLearnsets", [] { TmLearnsets::buildTmLearnsets(); });
    runStep("Encounters", [] { Encounter::buildEncounterRates(); });
    runStep("Items", [] { Item::buildItems(); });
    runStep("MartItems", [] { MartItem::buildMartItems(); });
    runStep("Givers", [] { Giver::buildGivers(); });
    runStep("Blockers", [] { Blocker::buildBlockers(); });
    runStep("Traders", [] { Trader::buildTraders(); });
    runStep("Npcs", [] { Npc::buildNpcs(); });
}

int main(int argc, char **argv) {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    if (!initSdl(window, renderer)) {
        shutdownSdl(window, renderer);
        return 1;
    }
    ensureWorkingDir();
    std::string fontPath = findFontPath();
    logStartupInfo(fontPath);
    if (!TextRenderer::init(fontPath, 24)) {
        std::cerr << "Failed to load font " << fontPath << std::endl;
    }

    try {
        buildGameData();
    } catch (const std::exception &e) {
        std::cerr << "buildGameData failed: " << e.what() << std::endl;
        shutdownSdl(window, renderer);
        return 1;
    } catch (...) {
        std::cerr << "buildGameData failed: unknown error" << std::endl;
        shutdownSdl(window, renderer);
        return 1;
    }

    try {
        Player *player = nullptr;
        std::ifstream saveFile("save.txt");
        if (saveFile.good()) {
            std::string pInfo;
            std::string pcS;
            std::string partyS;
            std::string trainS;
            std::string leadS;
            std::string gioRS;
            std::string wobS;
            std::string dexS;
            std::string itemS;
            std::string tmS;
            std::string fLoc;
            std::getline(saveFile, pInfo);
            std::getline(saveFile, pcS);
            std::getline(saveFile, partyS);
            std::getline(saveFile, trainS);
            std::getline(saveFile, leadS);
            std::getline(saveFile, gioRS);
            std::getline(saveFile, wobS);
            std::getline(saveFile, dexS);
            std::getline(saveFile, itemS);
            std::getline(saveFile, tmS);
            std::getline(saveFile, fLoc);
            player = new Player(pcS, partyS, dexS, itemS, tmS);
            for (size_t i = 0; i < fLoc.size() && i + 1 < FlyLocation::INDEX_MEANINGS.size(); i++) {
                if (fLoc[i] == '1') {
                    FlyLocation::FLY_LOCATIONS[FlyLocation::INDEX_MEANINGS[i + 1]]->visited = true;
                }
            }
            Trainer::buildTrainers(player, trainS, leadS, gioRS);
            WorldObject::buildWorldObjects(player, wobS);
            if (player->hasItem(Item::ITEM_MAP["Shiny Charm"])) {
                Main::SHINY_CHANCE = 256;
            }
            OverworldGui gui(player, renderer, pInfo);
            bool running = true;
            while (running) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        running = false;
                    }
                    gui.handleEvent(event);
                }
                gui.update();
                gui.render();
                SDL_Delay(16);
            }
        } else {
            Trainer::buildTrainers();
            WorldObject::buildWorldObjects();
            player = new Player("Purple");
            OverworldGui gui(player, renderer);
            std::string name = OverworldGui::promptText(
                "Welcome to Pokemon Purple! Controls are in the README.\nWhat is your name?");
            if (!name.empty()) {
                name.erase(std::remove(name.begin(), name.end(), ','), name.end());
                if (name.empty()) {
                    player->name = "Purple";
                } else {
                    player->name = name.size() < 11 ? name : name.substr(0, 10);
                }
            }
            OverworldGui::pickingStarter = true;
            bool running = true;
            while (running) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        running = false;
                    }
                    gui.handleEvent(event);
                }
                gui.update();
                gui.render();
                SDL_Delay(16);
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        shutdownSdl(window, renderer);
        return 1;
    } catch (...) {
        std::cerr << "Runtime error: unknown error" << std::endl;
        shutdownSdl(window, renderer);
        return 1;
    }
    shutdownSdl(window, renderer);
    return 0;
}
#include "Main.h"

int Main::SHINY_CHANCE = 8192;
