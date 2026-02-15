#include "OverworldGui.h"
#include "BattleState.h"
#include "BlackjackDialog.h"
#include "Blocker.h"
#include "FlyLocation.h"
#include "GameConfig.h"
#include "Item.h"
#include "MartItem.h"
#include "Monster.h"
#include "Move.h"
#include "Npc.h"
#include "NpcStrings.h"
#include "PokeMap.h"
#include "Player.h"
#include "SpriteManager.h"
#include "TextRenderer.h"
#include "Trainer.h"
#include "Warp.h"
#include "WorldObject.h"
#include "Utils.h"
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cctype>
#include <cmath>
#include <optional>
#include <sstream>
#include <unordered_map>

static Item *FAKE_ID = nullptr;
static Item *LIFT_KEY = nullptr;
static Item *AMULET_COIN = nullptr;
static Move *CUT = nullptr;
static Move *FLY = nullptr;
static Move *SURF = nullptr;
static Move *STRENGTH = nullptr;

static const int NUM_STEP_FRAMES = 8;
static const int BUMP_STEP_FRAMES = NUM_STEP_FRAMES * 2;

static bool s_inTextPrompt = false;

static const std::vector<int> HIDEOUT_Y = {0, 18, 18, 0, 14};
static const std::vector<int> SILPH_X = {0, 20, 20, 20, 20, 20, 18, 18, 18, 18, 12, 13};
static const std::vector<int> CATCH_RATES = {
    1,  45, 45, 45, 45, 45, 45, 45, 45, 45, 255, 120, 45, 255, 120, 45, 255, 120, 45, 255, 127, 255, 90, 255, 90,
    190, 75, 255, 90, 235, 120, 45, 235, 120, 45, 150, 25, 190, 75, 170, 50, 255, 90, 255, 120, 45, 190, 75, 190, 75,
    255, 50, 255, 90, 190, 75, 190, 75, 190, 75, 255, 120, 45, 200, 100, 50, 180, 90, 45, 255, 120, 45, 190, 60, 255, 120,
    45, 190, 60, 190, 75, 190, 60, 45, 190, 45, 190, 75, 190, 75, 190, 60, 190, 90, 45, 45, 190, 75, 225, 60, 190, 60, 90,
    45, 190, 75, 45, 45, 45, 190, 60, 120, 60, 30, 45, 45, 225, 75, 225, 60, 225, 60, 45, 45, 45, 45, 45, 45, 45, 255, 45,
    45, 35, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 25, 3,  3,  3,  45, 27, 9,  3,  2};

bool OverworldGui::showingText = false;
bool OverworldGui::battling = false;
bool OverworldGui::canMap = true;
bool OverworldGui::choosingFromLongList = false;
bool OverworldGui::usingBattleItem = false;
bool OverworldGui::busedItem = false;
bool OverworldGui::rightClicked = false;
bool OverworldGui::inMenu = false;
bool OverworldGui::buySell = false;
bool OverworldGui::buying = false;
bool OverworldGui::selling = false;
bool OverworldGui::checkingPokes = false;
bool OverworldGui::checkingMoves = false;
bool OverworldGui::checkingTms = false;
bool OverworldGui::teachingMove = false;
bool OverworldGui::replacingMove = false;
bool OverworldGui::depWith = false;
bool OverworldGui::depositing = false;
bool OverworldGui::withdraw = false;
bool OverworldGui::flying = false;
bool OverworldGui::inside = false;
bool OverworldGui::showingDex = false;
bool OverworldGui::rareCandy = false;
bool OverworldGui::pickingStarter = false;
bool OverworldGui::surfing = false;
bool OverworldGui::spacebar = false;
bool OverworldGui::autoBattle = false;

int OverworldGui::clickedChoice = -1;
std::string OverworldGui::currentText;
std::string OverworldGui::currentLoc;
std::string OverworldGui::longListHeaderOverride;
std::vector<std::string> OverworldGui::strArr = {"", "", "", ""};
std::vector<std::string> OverworldGui::longArr;
std::vector<Item*> OverworldGui::usableItems;
bool OverworldGui::usingItemList = false;
BattleState *OverworldGui::playerState = nullptr;
BattleState *OverworldGui::enemyState = nullptr;
OverworldGui *OverworldGui::instance = nullptr;

static SDL_Texture *getMissingnoTexture(SDL_Renderer *renderer) {
    static SDL_Texture *tex = nullptr;
    if (!renderer) {
        return nullptr;
    }
    if (tex) {
        return tex;
    }
    tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 256, 256);
    if (!tex) {
        return nullptr;
    }
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    void *pixels = nullptr;
    int pitch = 0;
    if (SDL_LockTexture(tex, nullptr, &pixels, &pitch) == 0) {
        for (int y = 0; y < 256; y++) {
            auto *row = reinterpret_cast<uint32_t *>(static_cast<uint8_t *>(pixels) + y * pitch);
            for (int x = 0; x < 256; x++) {
                uint8_t r = static_cast<uint8_t>(utils::randInt(0, 256));
                uint8_t g = static_cast<uint8_t>(utils::randInt(0, 256));
                uint8_t b = static_cast<uint8_t>(utils::randInt(0, 256));
                row[x] = (0xFFu << 24) | (static_cast<uint32_t>(b) << 16) | (static_cast<uint32_t>(g) << 8) | r;
            }
        }
        SDL_UnlockTexture(tex);
    }
    return tex;
}

static void mutateMissingnoTexture(SDL_Renderer *renderer) {
    SDL_Texture *tex = getMissingnoTexture(renderer);
    if (!tex) {
        return;
    }
    void *pixels = nullptr;
    int pitch = 0;
    if (SDL_LockTexture(tex, nullptr, &pixels, &pitch) != 0) {
        return;
    }
    int y = utils::randInt(0, 256);
    auto *row = reinterpret_cast<uint32_t *>(static_cast<uint8_t *>(pixels) + y * pitch);
    for (int x = 0; x < 256; x++) {
        uint8_t r = static_cast<uint8_t>(utils::randInt(0, 256));
        uint8_t g = static_cast<uint8_t>(utils::randInt(0, 256));
        uint8_t b = static_cast<uint8_t>(utils::randInt(0, 256));
        row[x] = (0xFFu << 24) | (static_cast<uint32_t>(b) << 16) | (static_cast<uint32_t>(g) << 8) | r;
    }
    SDL_UnlockTexture(tex);
}

void OverworldGui::advanceText() {
    if (showingText) {
        showingText = false;
        currentText.clear();
    }
}

void OverworldGui::autoAdvanceBattleText() {
    if (!battling || !autoBattle) {
        return;
    }
    if (!showingText) {
        return;
    }
    if (choosingFromLongList || usingBattleItem || s_inTextPrompt) {
        return;
    }

    static Uint32 lastAdvance = 0;
    Uint32 now = SDL_GetTicks();
    if (now - lastAdvance < 100) {
        return;
    }
    lastAdvance = now;
    advanceText();
}

void OverworldGui::stopMovement() {
    pressedKeys.clear();
    hasHeldDirection = false;
    stepPhase = StepPhase::NONE;
    phaseFrame = 0;
    offsetX = 0;
    offsetY = 0;
}

static int menuChoiceFromY(int y) {
    int menuBoxY = GameConfig::WINDOW_HEIGHT - GameConfig::MENU_BOX_HEIGHT - GameConfig::MENU_BOX_MARGIN;
    int menuStartY = menuBoxY + GameConfig::MENU_OPTION_START_OFFSET;
    int menuEndY = menuStartY + GameConfig::MENU_OPTION_COUNT * GameConfig::MENU_OPTION_HEIGHT;
    if (y < menuStartY || y >= menuEndY) {
        return -1;
    }
    return (y - menuStartY) / GameConfig::MENU_OPTION_HEIGHT;
}

static int longListChoiceFromY(int y) {
    int startY = GameConfig::WORLD_HEIGHT + GameConfig::LONG_LIST_START_Y;
    if (y < startY) {
        return -1;
    }
    return (y - startY) / GameConfig::MENU_OPTION_HEIGHT;
}

static int clampPageStart(int start, int total, int pageSize) {
    if (pageSize <= 0) {
        return 0;
    }
    if (total <= pageSize) {
        return 0;
    }
    int maxStart = ((total - 1) / pageSize) * pageSize;
    start = std::max(0, std::min(start, maxStart));
    start = (start / pageSize) * pageSize;
    return start;
}

OverworldGui::OverworldGui(Player *player) : player(player) {
    instance = this;
    setup();
}

OverworldGui::OverworldGui(Player *player, SDL_Renderer *renderer) : player(player), renderer(renderer) {
    instance = this;
    setup();
}

OverworldGui::OverworldGui(Player *player, SDL_Renderer *renderer, const std::string &saveInfo)
    : player(player), renderer(renderer) {
    instance = this;
    applySaveInfo(saveInfo);
    setup();
}

void OverworldGui::applySaveInfo(const std::string &saveInfo) {
    if (saveInfo.empty()) {
        return;
    }
    std::vector<std::string> parts = utils::split(saveInfo, ',');
    if (parts.size() < 10) {
        return;
    }
    mapName = parts[0];
    try {
        playerX = std::stoi(parts[1]);
        playerY = std::stoi(parts[2]);
        facing = static_cast<OverworldGui::Direction>(std::stoi(parts[3]));
        repelSteps = std::stoi(parts[4]);
    } catch (...) {
    }
    if (parts.size() >= 6) {
        auto it = PokeMap::POKEMAPS.find(parts[5]);
        if (it != PokeMap::POKEMAPS.end()) {
            lastHeal = it->second;
        }
    }
    if (player) {
        if (parts.size() >= 7) {
            player->ballin = (parts[6] == "1");
        }
        if (parts.size() >= 8) {
            try {
                player->numCaught = std::stoi(parts[7]);
            } catch (...) {
            }
        }
        if (parts.size() >= 9) {
            try {
                player->money = std::stoi(parts[8]);
            } catch (...) {
            }
        }
        if (parts.size() >= 10) {
            player->name = parts[9];
        }
    }
}

void OverworldGui::setup() {
    if (!FAKE_ID) {
        FAKE_ID = Item::ITEM_MAP["Fake ID"];
        LIFT_KEY = Item::ITEM_MAP["Lift Key"];
        AMULET_COIN = Item::ITEM_MAP["Amulet Coin"];
        CUT = Move::MOVE_MAP["Cut"];
        FLY = Move::MOVE_MAP["Fly"];
        SURF = Move::MOVE_MAP["Surf"];
        STRENGTH = Move::MOVE_MAP["Strength"];
    }
    pm = PokeMap::POKEMAPS[mapName];
    if (!lastHeal) {
        lastHeal = PokeMap::POKEMAPS["RedsHouse1F"];
    }
    loadTileImages();
    loadPlayerSprites();
    loadMap(pm);
    danceFrames.resize(10, nullptr);
    oakFrames.resize(10, nullptr);
    monsterFrames.resize(3, nullptr);

    if (renderer) {
        FlyLocation::loadMapTexture(renderer, "assets/WORLD_MAP.png");
        for (int i = 0; i < 10; i++) {
            danceFrames[i] = SpriteManager::loadScaledTexture("assets/sprites/COOLTRAINER_F/" + std::to_string(i) + ".png",
                                                             GameConfig::TILE_SIZE, GameConfig::TILE_SIZE);
            oakFrames[i] = SpriteManager::loadScaledTexture("assets/sprites/OAK/" + std::to_string(i) + ".png",
                                                           GameConfig::TILE_SIZE, GameConfig::TILE_SIZE);
        }
        for (int i = 0; i < 3; i++) {
            monsterFrames[i] = SpriteManager::loadScaledTexture("assets/sprites/MONSTER/" + std::to_string(i) + ".png",
                                                               GameConfig::TILE_SIZE, GameConfig::TILE_SIZE);
        }
        mattNpc = PokeMap::POKEMAPS["Route24"]->npcs[0][19];
        oakNpc = PokeMap::POKEMAPS["FuchsiaCity"]->npcs[6][13];
        monsterTrainer = nullptr;
        auto it = PokeMap::POKEMAPS.find("FuchsiaCity");
        if (it != PokeMap::POKEMAPS.end() && it->second) {
            PokeMap *fm = it->second;
            if (fm->trainers.size() > 5 && fm->trainers[5].size() > 13) {
                monsterTrainer = fm->trainers[5][13];
            }
        }
    }

    if (player) {
        for (auto &kv : PokeMap::POKEMAPS) {
            PokeMap *m = kv.second;
            if (!m) {
                continue;
            }
            for (size_t y = 0; y < m->npcs.size(); y++) {
                for (size_t x = 0; x < m->npcs[y].size(); x++) {
                    Blocker *b = dynamic_cast<Blocker *>(m->npcs[y][x]);
                    if (!b) {
                        continue;
                    }
                    bool missingItem = false;
                    if (b->item != nullptr) {
                        if (b->item->name == "Master Ball") {
                            missingItem = player->ballin;
                        } else {
                            missingItem = !player->hasItem(b->item);
                        }
                    }
                    bool missingMove = (b->move != nullptr && !player->hasMove(b->move));
                    bool missingBadge = (b->numBadges >= 0 && (b->numBadges >= static_cast<int>(player->leadersBeaten.size()) ||
                                                             !player->leadersBeaten[b->numBadges]));

                    if (!missingItem && !missingMove && !missingBadge) {
                        m->npcs[y][x] = nullptr;
                        delete b;
                    }
                }
            }
        }
    }
}

void OverworldGui::print(const std::string &s) {
    if (instance && instance->renderer) {
        instance->stopMovement();
        for (int i = 0; i < 4 && i < static_cast<int>(strArr.size()); i++) {
            strArr[i].clear();
        }
        longArr.clear();
        usableItems.clear();
        choosingFromLongList = false;
        usingItemList = false;
        usingBattleItem = false;
        busedItem = false;
        clickedChoice = -1;
        instance->psi = 0;

        showingText = true;
        currentText = s;
        std::cout << s << std::endl;

        bool waiting = true;
        Uint32 startTicks = SDL_GetTicks();
        while (waiting) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    waiting = false;
                    break;
                }
                if (!(battling && autoBattle)) {
                    if (event.type == SDL_MOUSEBUTTONDOWN) {
                        waiting = false;
                        break;
                    }
                    if (event.type == SDL_KEYDOWN && !event.key.repeat) {
                        waiting = false;
                        break;
                    }
                }
            }

            if (battling && autoBattle) {
                Uint32 now = SDL_GetTicks();
                if (now - startTicks >= 100) {
                    waiting = false;
                }
            }
            instance->update();
            instance->render();
            SDL_Delay(16);
        }

        SDL_Event flush;
        while (SDL_PollEvent(&flush)) {
        }

        showingText = false;
        currentText.clear();
        return;
    }

    std::cout << s << std::endl;
}

std::string OverworldGui::promptText(const std::string &prompt) {
    if (instance && instance->renderer) {
        std::string input;
        bool oldShowing = showingText;
        std::string oldText = currentText;
        bool oldCanMap = canMap;

        showingText = true;
        currentText = prompt;
        canMap = false;
        s_inTextPrompt = true;
        SDL_StartTextInput();

        bool running = true;
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    input.clear();
                    break;
                }
                if (event.type == SDL_TEXTINPUT) {
                    input += event.text.text;
                    if (input.size() > 10) {
                        input.resize(10);
                    }
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (!input.empty()) {
                            input.pop_back();
                        }
                    } else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                        running = false;
                    } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        input.clear();
                        running = false;
                    }
                }
            }

            currentText = prompt + "\n" + input;
            instance->update();
            instance->render();
            SDL_Delay(16);
        }

        SDL_StopTextInput();
        s_inTextPrompt = false;
        showingText = oldShowing;
        currentText = oldText;
        canMap = oldCanMap;
        return input;
    }

    std::cout << prompt << std::endl;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int OverworldGui::waitForChoice(int maxChoice) {
    while (true) {
        autoAdvanceBattleText();
        if (battling && autoBattle && !showingText && !choosingFromLongList && maxChoice == 3 && !strArr.empty() && strArr[0] == "Fight") {
            return 0;
        }
        if (clickedChoice >= 0 && clickedChoice <= maxChoice) {
            int choice = clickedChoice;
            clickedChoice = -1;
            if (rightClicked) {
                rightClicked = false;
                if (battling) {
                    for (int i = 0; i < 4 && i < static_cast<int>(strArr.size()); i++) {
                        strArr[i].clear();
                    }
                }
                return -1;
            }
            if (battling) {
                for (int i = 0; i < 4 && i < static_cast<int>(strArr.size()); i++) {
                    strArr[i].clear();
                }
            }
            return choice;
        }
        if (instance && instance->renderer) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    return -1;
                }
                instance->handleEvent(event);
            }
            instance->update();
            instance->render();
            SDL_Delay(16);
            continue;
        }
        if (!std::cin.good()) {
            return -1;
        }
        std::cout << "Choose [0-" << maxChoice << "], or 'q' to cancel: ";
        std::string line;
        if (!std::getline(std::cin, line)) {
            return -1;
        }
        if (line == "q" || line == "Q") {
            return -1;
        }
        try {
            int v = std::stoi(line);
            if (v >= 0 && v <= maxChoice) {
                return v;
            }
        } catch (...) {
        }
    }
}

void OverworldGui::setBattleStates(BattleState *a, BattleState *b) {
    playerState = a;
    enemyState = b;
    battling = true;
}

void OverworldGui::runGui() {
    // Placeholder for future CLI launch behavior.
}

void OverworldGui::loadMap(PokeMap *map) {
    if (!map) {
        return;
    }
    pm = map;
    currentMap = pm->grid;
    tileTypes = pm->types;
    connections[0] = pm->north;
    connOffsets[0] = pm->nOff;
    connections[1] = pm->south;
    connOffsets[1] = pm->sOff;
    connections[2] = pm->west;
    connOffsets[2] = pm->wOff;
    connections[3] = pm->east;
    connOffsets[3] = pm->eOff;
}

void OverworldGui::loadTileImages() {
    tileImages.assign(801, nullptr);
    for (int i = 0; i < 781; i++) {
        tileImages[i] = SpriteManager::loadScaledTexture("assets/tiles/" + std::to_string(i) + ".png",
                                                         GameConfig::TILE_SIZE, GameConfig::TILE_SIZE);
        if (!tileImages[i]) {
            std::cerr << "Failed to load tile: " << i << std::endl;
        }
    }
    int weird[] = {642, 69, 73, 263, 79, 94, 102, 263, 262, 742, 599, 7, 619, 1, 136, 263, 313, 203, 436, 148};
    for (int i = 0; i < 20; i++) {
        tileImages[781 + i] = tileImages[weird[i]];
    }
}

void OverworldGui::loadPlayerSprites() {
    playerFrames.assign(10, nullptr);
    seelFrames.assign(10, nullptr);
    for (int i = 0; i < 10; i++) {
        playerFrames[i] = SpriteManager::loadScaledTexture("assets/sprites/RED/" + std::to_string(i) + ".png",
                                                           GameConfig::TILE_SIZE, GameConfig::TILE_SIZE);
        seelFrames[i] = SpriteManager::loadScaledTexture("assets/sprites/SEEL/" + std::to_string(i) + ".png",
                                                         GameConfig::TILE_SIZE, GameConfig::TILE_SIZE);
    }
    const int bsize = 256;
    pBattlers.assign(153, nullptr);
    eBattlers.assign(153, nullptr);
    if (!renderer) {
        return;
    }
    for (int i = 1; i < 153; i++) {
        SDL_Surface *surface = IMG_Load(("assets/battlers/" + std::to_string(i) + ".png").c_str());
        if (!surface) {
            continue;
        }
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!tex) {
            continue;
        }
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        SDL_Texture *scaled = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, bsize, bsize);
        SDL_Texture *prev = SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer, scaled);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_Rect dst{0, 0, bsize, bsize};
        SDL_RenderCopy(renderer, tex, nullptr, &dst);
        SDL_SetRenderTarget(renderer, prev);
        SDL_DestroyTexture(tex);
        SDL_SetTextureBlendMode(scaled, SDL_BLENDMODE_BLEND);
        eBattlers[i] = scaled;
        SDL_Texture *flipped = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, bsize, bsize);
        prev = SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer, flipped);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, scaled, nullptr, &dst, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
        SDL_SetRenderTarget(renderer, prev);
        SDL_SetTextureBlendMode(flipped, SDL_BLENDMODE_BLEND);
        pBattlers[i] = flipped;
    }
}

void OverworldGui::closeMenus() {
    inMenu = false;
    buySell = false;
    buying = false;
    selling = false;
    checkingPokes = false;
    checkingMoves = false;
    checkingTms = false;
    teachingMove = false;
    replacingMove = false;
    longListHeaderOverride.clear();
    depWith = false;
    depositing = false;
    withdraw = false;
    choosingFromLongList = false;
    showingDex = false;
    usingItemList = false;
}

void OverworldGui::clickMouse(int mouseX, int mouseY, bool rightClick) {
    if (showingText) {
        advanceText();
        return;
    }

    if (blackjackShowingResults) {
        longArr.clear();
        choosingFromLongList = false;
        blackjackShowingResults = false;
        inMenu = false;
        buySell = false;
        buying = false;
        selling = false;
        closeMenus();
        clickedChoice = 0;
        return;
    }

    if (blackjackActive) {
        if (blackjackFinished) {
            return;
        }

        static std::mt19937 s_blackjackRng(std::random_device{}());

        auto cardValue = [](int c) {
            int r = c % 13;
            if (r == 0) {
                return 11;
            }
            if (r >= 9) {
                return 10;
            }
            return r + 1;
        };
        auto handValue = [&](const std::vector<int> &hand) {
            int sum = 0;
            int aces = 0;
            for (int c : hand) {
                int v = cardValue(c);
                sum += v;
                if (c % 13 == 0) {
                    aces++;
                }
            }
            while (sum > 21 && aces-- > 0) {
                sum -= 10;
            }
            return sum;
        };
        auto isSoft = [&](const std::vector<int> &hand) {
            int sum = 0;
            int aces = 0;
            for (int c : hand) {
                sum += cardValue(c);
                if (c % 13 == 0) {
                    aces++;
                }
            }
            return aces > 0 && sum <= 21;
        };
        auto dealerShouldHit = [&](const std::vector<int> &hand) {
            int v = handValue(hand);
            return v < 17 || (v == 17 && isSoft(hand));
        };
        auto drawCard = [&]() {
            if (blackjackDeck.empty()) {
                blackjackDeck.clear();
                blackjackDeck.reserve(52);
                for (int i = 0; i < 52; i++) {
                    blackjackDeck.push_back(i);
                }
                std::shuffle(blackjackDeck.begin(), blackjackDeck.end(), s_blackjackRng);
            }
            int c = blackjackDeck.back();
            blackjackDeck.pop_back();
            return c;
        };

        if (!rightClick) {
            blackjackPlayer.push_back(drawCard());
            if (handValue(blackjackPlayer) > 21) {
                blackjackFinished = true;
                blackjackPlayerWon = false;
            }
        } else {
            blackjackHideFirst = false;
            while (dealerShouldHit(blackjackDealer)) {
                blackjackDealer.push_back(drawCard());
            }
            int p = handValue(blackjackPlayer);
            int d = handValue(blackjackDealer);
            if (p > 21) {
                blackjackFinished = true;
                blackjackPlayerWon = false;
            } else if (d > 21) {
                blackjackFinished = true;
                blackjackPlayerWon = true;
            } else if (p > d) {
                blackjackFinished = true;
                blackjackPlayerWon = true;
            } else if (p < d) {
                blackjackFinished = true;
                blackjackPlayerWon = false;
            } else {
                blackjackTieFrames = 60;
            }
        }

        if (blackjackFinished) {
            blackjackHideFirst = false;

            auto fmtCard = [](int c) {
                static const char *ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
                static const char *suits[] = {"H", "D", "C", "S"};
                int r = c % 13;
                int s = c / 13;
                std::string out = ranks[r];
                out += suits[s];
                return out;
            };
            auto fmtHand = [&](const std::vector<int> &hand) {
                std::string out;
                for (size_t i = 0; i < hand.size(); i++) {
                    if (!out.empty()) {
                        out += " ";
                    }
                    out += fmtCard(hand[i]);
                }
                return out;
            };

            int p = handValue(blackjackPlayer);
            int d = handValue(blackjackDealer);

            std::string result;
            result += blackjackPlayerWon ? "YOU WIN!\n\n" : "YOU LOSE\n\n";
            result += "Dealer: " + fmtHand(blackjackDealer) + " (" + std::to_string(d) + ")\n";
            result += "Player: " + fmtHand(blackjackPlayer) + " (" + std::to_string(p) + ")\n\n";

            std::string moneyLine;

            if (player) {
                if (blackjackPlayerWon) {
                    player->money += blackjackBet;
                    moneyLine = "You won $" + std::to_string(blackjackBet) + "!";
                } else {
                    player->money -= blackjackBet;
                    moneyLine = "You lost $" + std::to_string(blackjackBet) + "!";
                }
                bet = std::max(1, blackjackBet * 2);
            }

            longArr.clear();
            longArr.push_back(blackjackPlayerWon ? "YOU WIN!" : "YOU LOSE");
            longArr.push_back("");
            longArr.push_back("Dealer:");
            longArr.push_back(fmtHand(blackjackDealer) + " (" + std::to_string(d) + ")");
            longArr.push_back("");
            longArr.push_back("Player:");
            longArr.push_back(fmtHand(blackjackPlayer) + " (" + std::to_string(p) + ")");
            longArr.push_back("");
            if (!moneyLine.empty()) {
                longArr.push_back(moneyLine);
                longArr.push_back("");
            }
            psi = 0;
            choosingFromLongList = true;
            showingText = false;
            blackjackShowingResults = true;
            blackjackActive = false;
            blackjackBet = 0;
        }
        return;
    }

    if (rightClick) {
        if (battling) {
            if (usingBattleItem || usingItemList) {
                usedItem = nullptr;
                chosenMon = nullptr;
                tm = nullptr;
                tms.clear();
                longArr.clear();
                usableItems.clear();
                choosingFromLongList = false;
                usingItemList = false;
                busedItem = false;
                usingBattleItem = false;
                inMenu = false;
                checkingPokes = false;
                checkingMoves = false;
                checkingTms = false;
                teachingMove = false;
                replacingMove = false;
                closeMenus();
                advanceText();
                rightClicked = true;
                clickedChoice = 0;
                return;
            }

            if (!choosingFromLongList) {
                clickedChoice = 3;
                return;
            }

            rightClicked = true;
            clickedChoice = 0;
            return;
        }

        if (buySell || buying || selling) {
            canMap = true;
        }
        usedItem = nullptr;
        chosenMon = nullptr;
        tm = nullptr;
        tms.clear();
        longArr.clear();
        choosingFromLongList = false;
        usingItemList = false;
        busedItem = false;
        usingBattleItem = false;
        inMenu = false;
        buySell = false;
        buying = false;
        selling = false;
        checkingPokes = false;
        checkingMoves = false;
        checkingTms = false;
        teachingMove = false;
        replacingMove = false;
        closeMenus();
        advanceText();
        rightClicked = true;
        clickedChoice = 0;
        return;
    }
    if (buySell) {
        int choice = menuChoiceFromY(mouseY);
        if (choice == 0) {
            buying = true;
            buySell = false;
            canMap = false;
            choosingFromLongList = true;
            usableItems.clear();
            longArr.clear();
            for (const MartItem &m : martItems) {
                longArr.push_back(m.toString());
            }
            strArr[0] = "Money: $" + std::to_string(player ? player->money : 0);
            psi = 0;
        } else if (choice == 1) {
            selling = true;
            buySell = false;
            canMap = false;
            choosingFromLongList = true;
            usableItems.clear();
            longArr.clear();
            for (Item *it : player->items) {
                if (it && it->price > 0) {
                    usableItems.push_back(it);
                    longArr.push_back(it->name + " x" + std::to_string(it->quantity) + "   $" + std::to_string(it->price / 2));
                }
            }
            strArr[0] = "Sell";
            psi = 0;
        }
        return;
    }
    if (buying && choosingFromLongList) {
        int lineIdx = longListChoiceFromY(mouseY);
        if (lineIdx < 0) {
            return;
        }
        int idx = psi + lineIdx;
        if (idx < 0 || idx >= static_cast<int>(martItems.size())) {
            return;
        }
        const MartItem &m = martItems[idx];
        if (m.price <= 0) {
            return;
        }

        if (m.move) {
            if (player && player->hasMove(m.move)) {
                print("You already have that TM!");
            } else {
                if (!player || player->money < m.price) {
                    print("You can't afford that!");
                    buying = true;
                    buySell = false;
                    canMap = false;
                    choosingFromLongList = true;
                    inMenu = true;
                    usableItems.clear();
                    longArr.clear();
                    for (const MartItem &mi : martItems) {
                        longArr.push_back(mi.toString());
                    }
                    strArr[0] = "Money: $" + std::to_string(player ? player->money : 0);
                    psi = 0;
                    return;
                }
                player->money -= m.price;
                player->give(m.move);
                print("Thank you!");
            }
        } else {
            std::string qStr = promptText("How many?");
            int q = 0;
            try {
                long long parsed = std::stoll(qStr);
                if (parsed <= 0 || parsed > std::numeric_limits<int>::max()) {
                    print("Keep those obscenities to yourself, boy!");
                    buying = true;
                    buySell = false;
                    canMap = false;
                    choosingFromLongList = true;
                    inMenu = true;
                    usableItems.clear();
                    longArr.clear();
                    for (const MartItem &mi : martItems) {
                        longArr.push_back(mi.toString());
                    }
                    strArr[0] = "Money: $" + std::to_string(player ? player->money : 0);
                    psi = 0;
                    return;
                }
                q = static_cast<int>(parsed);
            } catch (...) {
                print("Keep those obscenities to yourself, boy!");
                buying = true;
                buySell = false;
                canMap = false;
                choosingFromLongList = true;
                inMenu = true;
                usableItems.clear();
                longArr.clear();
                for (const MartItem &mi : martItems) {
                    longArr.push_back(mi.toString());
                }
                strArr[0] = "Money: $" + std::to_string(player ? player->money : 0);
                psi = 0;
                return;
            }
            long long cost = static_cast<long long>(m.price) * q;
            if (!player || player->money < cost) {
                print("You can't afford that!");
                buying = true;
                buySell = false;
                canMap = false;
                choosingFromLongList = true;
                inMenu = true;
                usableItems.clear();
                longArr.clear();
                for (const MartItem &mi : martItems) {
                    longArr.push_back(mi.toString());
                }
                strArr[0] = "Money: $" + std::to_string(player ? player->money : 0);
                psi = 0;
                return;
            }
            player->money -= static_cast<int>(cost);
            if (m.item) {
                player->give(m.item, q);
            } else if (m.mon) {
                for (int i = 0; i < q; i++) {
                    player->give(new Battler(5, m.mon));
                }
            }
            print("Thank you!");
        }
        buying = false;
        buySell = true;
        canMap = false;
        choosingFromLongList = false;
        inMenu = true;
        strArr[0] = "Buy";
        strArr[1] = "Sell";
        strArr[2].clear();
        strArr[3].clear();
        return;
    }
    if (selling && choosingFromLongList) {
        int lineIdx = longListChoiceFromY(mouseY);
        if (lineIdx < 0) {
            return;
        }
        int idx = psi + lineIdx;
        if (idx < 0 || idx >= static_cast<int>(usableItems.size())) {
            return;
        }
        Item *it = usableItems[idx];
        if (!it) {
            return;
        }
        std::string qStr = promptText("How many?");
        int q = 0;
        try {
            long long parsed = std::stoll(qStr);
            if (parsed <= 0 || parsed > std::numeric_limits<int>::max()) {
                print("Keep those obscenities to yourself, boy!");
                selling = true;
                buySell = false;
                canMap = false;
                choosingFromLongList = true;
                inMenu = true;
                usableItems.clear();
                longArr.clear();
                for (Item *si : player->items) {
                    if (si && si->price > 0) {
                        usableItems.push_back(si);
                        longArr.push_back(si->name + " x" + std::to_string(si->quantity) + "   $" + std::to_string(si->price / 2));
                    }
                }
                strArr[0] = "Sell";
                psi = 0;
                return;
            }
            q = static_cast<int>(parsed);
        } catch (...) {
            print("Keep those obscenities to yourself, boy!");
            selling = true;
            buySell = false;
            canMap = false;
            choosingFromLongList = true;
            inMenu = true;
            usableItems.clear();
            longArr.clear();
            for (Item *si : player->items) {
                if (si && si->price > 0) {
                    usableItems.push_back(si);
                    longArr.push_back(si->name + " x" + std::to_string(si->quantity) + "   $" + std::to_string(si->price / 2));
                }
            }
            strArr[0] = "Sell";
            psi = 0;
            return;
        }
        if (it->quantity <= 0) {
            return;
        }
        q = std::min(q, it->quantity);
        player->sell(it, q);
        print("Thank you!");
        selling = false;
        buySell = true;
        canMap = false;
        choosingFromLongList = false;
        inMenu = true;
        strArr[0] = "Buy";
        strArr[1] = "Sell";
        strArr[2].clear();
        strArr[3].clear();
        return;
    }
    if (pickingStarter) {
        pickingStarter = false;
        if (mouseY >= GameConfig::WORLD_HEIGHT) {
            if (mouseX < GameConfig::WINDOW_WIDTH / 3) {
                player->give(new Battler(1, Monster::MONSTER_MAP["Bulbasaur"]));
            } else if (mouseX > 2 * GameConfig::WINDOW_WIDTH / 3) {
                player->give(new Battler(1, Monster::MONSTER_MAP["Squirtle"]));
            } else {
                player->give(new Battler(1, Monster::MONSTER_MAP["Charmander"]));
            }
            return;
        }
        pickingStarter = true;
        return;
    }
    if (flying) {
        if (rightClick) {
            flying = false;
            inMenu = false;
            inside = false;
            currentLoc.clear();
            return;
        }
        if (mouseY < GameConfig::WORLD_HEIGHT) {
            return;
        }
        int mapX = mouseX;
        int mapY = mouseY - GameConfig::WORLD_HEIGHT;
        int cellW = GameConfig::WINDOW_WIDTH / 11;
        int cellH = GameConfig::UI_HEIGHT / 11;
        int j = mapX / std::max(1, cellW);
        int i = mapY / std::max(1, cellH);
        if (i < 0 || j < 0 || i >= 11 || j >= 11) {
            return;
        }
        std::string n = FlyLocation::NAME_MAP[i][j];
        if (n.empty()) {
            return;
        }
        std::string p = printable(n);
        if (inside || FlyLocation::isRed(i, j)) {
            currentLoc = p;
            return;
        }
        auto it = FlyLocation::FLY_LOCATIONS.find(n);
        if (it == FlyLocation::FLY_LOCATIONS.end() || !it->second || !it->second->visited) {
            return;
        }
        FlyLocation *f = it->second;
        loadMap(f->dest);
        playerX = f->x;
        playerY = f->y;
        offsetX = 0;
        offsetY = 0;
        inMenu = false;
        flying = false;
        return;
    }
    if (showingText) {
        advanceText();
        return;
    }

    if (depWith) {
        int choice = menuChoiceFromY(mouseY);
        if (choice == 0) {
            depositing = true;
            depWith = false;
            longArr = Battler::partyStrings(player->team);
            choosingFromLongList = true;
            inMenu = true;
            psi = 0;
            return;
        }
        if (choice == 1) {
            withdraw = true;
            depWith = false;
            choosingFromLongList = true;
            inMenu = true;
            psi = 0;
            boxNum = 1;
            longArr.clear();
            for (int i = 0; i < 20; i++) {
                int ind = psi + i;
                longArr.push_back((player && ind < static_cast<int>(player->pc.size()) && player->pc[ind]) ? player->pc[ind]->toString() : "");
            }
            return;
        }
    }

    if (depositing) {
        int lineIdx = longListChoiceFromY(mouseY);
        if (lineIdx < 0) {
            return;
        }
        int idx = psi + lineIdx;
        if (!player || idx < 0 || idx >= static_cast<int>(player->team.size()) || player->team[idx] == nullptr) {
            return;
        }
        if (player->team.size() > 1 && player->team[1] == nullptr) {
            print("No, you'll never survive in this world without pokemon!");
            depositing = false;
            withdraw = false;
            depWith = true;
            inMenu = true;
            choosingFromLongList = false;
            psi = 0;
            strArr[0] = "Deposit";
            strArr[1] = "Withdraw";
            strArr[2].clear();
            strArr[3].clear();
            return;
        }
        print("You deposited " + player->team[idx]->nickname + "!");
        player->pc.push_back(player->team[idx]);
        for (size_t i = static_cast<size_t>(idx) + 1; i < player->team.size(); i++) {
            player->team[i - 1] = player->team[i];
        }
        if (!player->team.empty()) {
            player->team[player->team.size() - 1] = nullptr;
        }
        longArr = Battler::partyStrings(player->team);
        inMenu = true;
        choosingFromLongList = true;
        return;
    }

    if (withdraw) {
        int lineIdx = longListChoiceFromY(mouseY);
        if (lineIdx < 0 || lineIdx > 19) {
            return;
        }
        int ind = psi + lineIdx;
        if (!player || ind < 0 || ind >= static_cast<int>(player->pc.size())) {
            return;
        }
        for (size_t i = 1; i < player->team.size(); i++) {
            if (player->team[i] == nullptr) {
                player->team[i] = player->pc[ind];
                player->pc.erase(player->pc.begin() + ind);
                std::string nick = player->team[i]->nickname;
                print("You withdrew " + nick + "!");
                inMenu = true;
                withdraw = true;
                depositing = false;
                depWith = false;
                choosingFromLongList = true;
                usingItemList = false;
                psi = clampPageStart(psi, static_cast<int>(player->pc.size()), 20);
                longArr.clear();
                for (int j = 0; j < 20; j++) {
                    int refillInd = psi + j;
                    longArr.push_back((refillInd < static_cast<int>(player->pc.size()) && player->pc[refillInd]) ? player->pc[refillInd]->toString() : "");
                }
                return;
            }
        }
        print("Hey, you already have 6 pokemon with you! Don't be greedy.");
        withdraw = false;
        depositing = false;
        depWith = true;
        inMenu = true;
        choosingFromLongList = false;
        psi = 0;
        strArr[0] = "Deposit";
        strArr[1] = "Withdraw";
        strArr[2].clear();
        strArr[3].clear();
        return;
    }

    if (checkingPokes) {
        int lineIdx = longListChoiceFromY(mouseY);
        if (lineIdx < 0) {
            return;
        }
        int idx = psi + lineIdx;
        if (!player || idx < 0 || idx >= static_cast<int>(player->team.size())) {
            return;
        }
        if (player->team[idx] != nullptr) {
            clickedChoice = idx;
            longArr = player->team[idx]->allInformation();
            checkingPokes = false;
            checkingMoves = true;
        }
        return;
    }

    if (checkingMoves) {
        longArr = Battler::partyStrings(player->team);
        checkingPokes = true;
        checkingMoves = false;
        return;
    }

    if (checkingTms) {
        int lineIdx = longListChoiceFromY(mouseY);
        if (lineIdx < 0) {
            return;
        }
        int idx = psi + lineIdx;
        if (idx < 0 || idx >= static_cast<int>(tms.size())) {
            return;
        }
        tm = tms[idx];
        if (!tm) {
            return;
        }
        checkingTms = false;
        teachingMove = true;
        longArr = Battler::partyStrings(player->team);
        return;
    }

    if (teachingMove) {
        int lineIdx = longListChoiceFromY(mouseY);
        if (lineIdx < 0) {
            return;
        }
        int idx = psi + lineIdx;
        if (!player || idx < 0 || idx >= static_cast<int>(player->team.size()) || player->team[idx] == nullptr) {
            return;
        }
        chosenMon = player->team[idx];
        if (!chosenMon || !tm) {
            return;
        }

        bool learnable = false;
        if (chosenMon->dexNum >= 0 && chosenMon->dexNum < static_cast<int>(Monster::MONSTERS.size())) {
            const Monster &mon = Monster::MONSTERS[chosenMon->dexNum];
            learnable = std::find(mon.learnable.begin(), mon.learnable.end(), tm) != mon.learnable.end();
        }
        if (!learnable) {
            print(chosenMon->nickname + " cannot learn " + tm->name + ".");
            chosenMon = nullptr;
            return;
        }
        for (size_t i = 0; i < chosenMon->moves.size(); i++) {
            if (chosenMon->moves[i] == nullptr) {
                chosenMon->moves[i] = tm;
                chosenMon->pp[i] = tm->pp;
                chosenMon->mpp[i] = tm->pp;
                print(chosenMon->nickname + " learned " + tm->name + "!");
                chosenMon = nullptr;
                tm = nullptr;
                longArr.clear();
                choosingFromLongList = false;
                psi = 0;
                closeMenus();
                return;
            }
            if (chosenMon->moves[i] == tm) {
                print(chosenMon->nickname + " already knows " + tm->name + "!");
                chosenMon = nullptr;
                tm = nullptr;
                longArr.clear();
                choosingFromLongList = false;
                psi = 0;
                closeMenus();
                return;
            }
        }

        print(chosenMon->nickname + " is trying to learn " + tm->name + "! Select a move to replace it with.");
        longArr = chosenMon->moveStrings();
        psi = 0;
        choosingFromLongList = true;
        inMenu = true;
        teachingMove = false;
        replacingMove = true;
        return;
    }

    if (replacingMove) {
        int lineIdx = longListChoiceFromY(mouseY);
        if (lineIdx < 0) {
            return;
        }
        int idx = psi + lineIdx;
        if (!chosenMon || !tm || idx < 0 || idx >= static_cast<int>(chosenMon->moves.size())) {
            return;
        }
        if (chosenMon->moves[idx] != nullptr) {
            print(chosenMon->nickname + " forgot how to use " + chosenMon->moves[idx]->name + ", but...");
        }
        chosenMon->moves[idx] = tm;
        chosenMon->pp[idx] = std::min(chosenMon->pp[idx], tm->pp);
        chosenMon->mpp[idx] = tm->pp;
        print(chosenMon->nickname + " learned " + tm->name + "!");
        tm = nullptr;
        chosenMon = nullptr;
        replacingMove = false;
        longArr.clear();
        choosingFromLongList = false;
        psi = 0;
        closeMenus();
        return;
    }

    if (choosingFromLongList && usingItemList && !usingBattleItem) {
        int lineIdx = longListChoiceFromY(mouseY);
        if (lineIdx < 0) {
            return;
        }
        int idx = psi + lineIdx;
        if (idx < 0 || idx >= static_cast<int>(usableItems.size())) {
            return;
        }

        if (usedItem == nullptr) {
            usedItem = usableItems[idx];
            if (!usedItem) {
                return;
            }

            if (usedItem->name == "Rare Candy") {
                rareCandy = true;
                longArr = Battler::partyStrings(player->team);
                chosenMon = nullptr;
                strArr[0] = "Pokemon";
                psi = 0;
                choosingFromLongList = true;
                return;
            }

            if (usedItem->name == "Repel") {
                useRepel(101);
                return;
            }
            if (usedItem->name == "Super Repel") {
                useRepel(201);
                return;
            }
            if (usedItem->name == "Max Repel") {
                useRepel(251);
                return;
            }

            if (usedItem->name == "Old Rod" || usedItem->name == "Good Rod" || usedItem->name == "Super Rod") {
                inMenu = true;
                std::string n = usedItem->name;
                usedItem = nullptr;

                int nx = playerX;
                int ny = playerY;
                switch (facing) {
                    case Direction::SOUTH:
                        ny++;
                        break;
                    case Direction::NORTH:
                        ny--;
                        break;
                    case Direction::WEST:
                        nx--;
                        break;
                    case Direction::EAST:
                        nx++;
                        break;
                }
                if (nx < 0 || ny < 0 || ny >= static_cast<int>(tileTypes.size()) || nx >= static_cast<int>(tileTypes[0].size())) {
                    closeMenus();
                    choosingFromLongList = false;
                    usingItemList = false;
                    inMenu = false;
                    print("What exactly do you expect to find in the void?");
                    return;
                }
                if (tileTypes[ny][nx] != 4) {
                    closeMenus();
                    choosingFromLongList = false;
                    usingItemList = false;
                    inMenu = false;
                    print("Maybe I should fish in the water...");
                    return;
                }
                wildMon = pm->getRandomEncounter(n);
                if (wildMon == nullptr) {
                    closeMenus();
                    choosingFromLongList = false;
                    usingItemList = false;
                    inMenu = false;
                    print("Huh, no fish... I should try somewhere else!");
                    return;
                }
                closeMenus();
                int result = BattleState::wildBattle(player->team, wildMon, this);
                if (result < 0) {
                    blackout();
                } else {
                    player->money += result * (player->hasItem(AMULET_COIN) ? 2 : 1);
                }
                battling = false;
                wildMon = nullptr;
                offsetX = 0;
                offsetY = 0;
                return;
            }

            if (usedItem->name == "Escape Rope") {
                loadMap(lastHeal);
                playerX = lastHeal->healX;
                playerY = lastHeal->healY;
                closeMenus();
                print("You climbed back to the last heal spot!");
                player->use(usedItem);
                usedItem = nullptr;
                return;
            }

            if (!usedItem->world && !usedItem->wild && !usedItem->battle && !usedItem->heal) {
                print("This can't be used now.");
                notifyUseItem(false);
                return;
            }

            longArr = Battler::partyStrings(player->team);
            chosenMon = nullptr;
            strArr[0] = "Pokemon";
            psi = 0;
            choosingFromLongList = true;
            return;
        }

        if (chosenMon == nullptr) {
            if (!player || idx < 0 || idx >= static_cast<int>(player->team.size())) {
                return;
            }
            Battler *b = player->team[idx];
            if (!b) {
                return;
            }

            choosingFromLongList = false;
            inMenu = false;

            if (usedItem->name == "Antidote") {
                healStatus(b, "POISONED");
            } else if (usedItem->name == "Awakening") {
                healStatus(b, "SLEEPING");
            } else if (usedItem->name == "Burn Heal") {
                healStatus(b, "BURNED");
            } else if (usedItem->name == "Ice Heal") {
                healStatus(b, "FROZEN");
            } else if (usedItem->name == "Paralyze Heal") {
                healStatus(b, "PARALYZED");
            } else if (usedItem->name == "Full Heal") {
                if (b->status.empty()) {
                    print("Quit messing around!");
                    notifyUseItem(false);
                } else {
                    if (playerState && playerState->monster == b) {
                        playerState->poisonDamage = 0;
                    }
                    b->status.clear();
                    print(b->nickname + "'s status was healed!");
                    notifyUseItem(true);
                }
            } else if (usedItem->name == "Full Restore") {
                if (b->hp == 0) {
                    print("No, you'll need something stronger to heal this one...");
                    notifyUseItem(false);
                } else if (b->hp == b->mhp && b->status.empty()) {
                    print("Quit messing around!");
                    notifyUseItem(false);
                } else {
                    b->hp = b->mhp;
                    b->status.clear();
                    print(b->nickname + " was fully restored!");
                    notifyUseItem(true);
                }
            } else if (usedItem->name == "Elixir") {
                useElixir(b, 10);
            } else if (usedItem->name == "Max Elixir") {
                useElixir(b, 99);
            } else if (usedItem->name == "Potion") {
                healHp(b, 20);
            } else if (usedItem->name == "Fresh Water") {
                healHp(b, 50);
            } else if (usedItem->name == "Super Potion") {
                healHp(b, 50);
            } else if (usedItem->name == "Soda Pop") {
                healHp(b, 60);
            } else if (usedItem->name == "Hyper Potion") {
                healHp(b, 200);
            } else if (usedItem->name == "Lemonade") {
                healHp(b, 80);
            } else if (usedItem->name == "Max Potion") {
                healHp(b, 999);
            } else if (usedItem->name == "Revive") {
                useRevive(b, b->mhp / 2);
            } else if (usedItem->name == "Max Revive") {
                useRevive(b, b->mhp);
            } else if (usedItem->name == "Ether" || usedItem->name == "Max Ether" || usedItem->name == "PP Up") {
                chosenMon = b;
                longArr = chosenMon->moveStrings();
                choosingFromLongList = true;
                inMenu = true;
                return;
            } else if (rareCandy && usedItem && usedItem->name == "Rare Candy") {
                player->use(usedItem);
                usedItem = nullptr;
                int delta = std::max(0, b->mxp - b->xp);
                b->gainXp(player, b->moves, delta, 0, 0, 0, 0, 0, 0);
                rareCandy = false;
                choosingFromLongList = false;
                inMenu = false;
                return;
            } else if (usedItem->name == "Moon Stone" || usedItem->name == "Fire Stone" || usedItem->name == "Water Stone" ||
                       usedItem->name == "Thunder Stone" || usedItem->name == "Leaf Stone") {
                bool ok = b->useStone(usedItem->name);
                if (ok && player) {
                    player->registerBattler(b);
                }
                notifyUseItem(ok);
            } else if (usedItem->name == "Calcium") {
                b->spatkXp += 10;
                b->refreshStats();
                print(b->nickname + " gained special attack experience!");
                notifyUseItem(true);
            } else if (usedItem->name == "Carbos") {
                b->spdXp += 10;
                b->refreshStats();
                print(b->nickname + " gained speed experience!");
                notifyUseItem(true);
            } else if (usedItem->name == "HP Up") {
                b->hpXp += 10;
                b->refreshStats();
                print(b->nickname + " gained HP experience!");
                notifyUseItem(true);
            } else if (usedItem->name == "Iron") {
                b->defXp += 10;
                b->refreshStats();
                print(b->nickname + " gained defense experience!");
                notifyUseItem(true);
            } else if (usedItem->name == "Protein") {
                b->atkXp += 10;
                b->refreshStats();
                print(b->nickname + " gained attack experience!");
                notifyUseItem(true);
            } else if (usedItem->name == "Zinc") {
                b->spdefXp += 10;
                b->refreshStats();
                print(b->nickname + " gained special defense experience!");
                notifyUseItem(true);
            } else {
                print("This can't be used now.");
                notifyUseItem(false);
            }
            return;
        }

        if (usedItem != nullptr && chosenMon != nullptr) {
            if (idx < 0 || chosenMon->moves.empty() || idx >= static_cast<int>(chosenMon->moves.size())) {
                return;
            }
            Move *move = chosenMon->moves[idx];
            if (!move) {
                return;
            }
            if (usedItem->name == "Ether") {
                if (chosenMon->pp[idx] == chosenMon->mpp[idx]) {
                    print("DO NOT WASTE THAT!");
                    notifyUseItem(false);
                } else {
                    chosenMon->pp[idx] = std::min(chosenMon->mpp[idx], chosenMon->pp[idx] + 10);
                    print(move->name + "'s PP was restored by 10!");
                    notifyUseItem(true);
                }
            } else if (usedItem->name == "Max Ether") {
                if (chosenMon->pp[idx] == chosenMon->mpp[idx]) {
                    print("DO NOT WASTE THAT!");
                    notifyUseItem(false);
                } else {
                    chosenMon->pp[idx] = chosenMon->mpp[idx];
                    print(move->name + "'s PP was fully restored!");
                    notifyUseItem(true);
                }
            } else if (usedItem->name == "PP Up") {
                chosenMon->mpp[idx] = static_cast<int>(chosenMon->mpp[idx] * 1.2);
                print(move->name + "'s PP was permanently increased!");
                player->use(usedItem);
                usedItem = nullptr;
                choosingFromLongList = false;
                busedItem = true;
                usingBattleItem = false;
            }
            chosenMon = nullptr;
            inMenu = false;
            choosingFromLongList = false;
            return;
        }
    }
    if (choosingFromLongList && usingBattleItem && usingItemList) {
        int lineIdx = longListChoiceFromY(mouseY);
        if (lineIdx < 0) {
            return;
        }
        int idx = psi + lineIdx;

        if (usedItem == nullptr) {
            if (idx < 0 || idx >= static_cast<int>(usableItems.size())) {
                return;
            }
            usedItem = usableItems[idx];
            if (!usedItem) {
                notifyUseItem(false);
                return;
            }
            if (!usedItem->battle && !usedItem->wild) {
                print("This can't be used now.");
                notifyUseItem(false);
                return;
            }

            if (usedItem->name == "X Accuracy") {
            playerState->accStage++;
            print(playerState->monster->nickname + "'s accuracy rose by 1 stage!");
            if (playerState->accStage == 7) {
                playerState->accStage = 6;
                print("Although it maxed out at 6!");
            }
            notifyUseItem(true);
            } else if (usedItem->name == "X Attack") {
            playerState->atkStage++;
            print(playerState->monster->nickname + "'s attack rose by 1 stage!");
            if (playerState->atkStage == 7) {
                playerState->atkStage = 6;
                print("Although it maxed out at 6!");
            }
            notifyUseItem(true);
            } else if (usedItem->name == "X Defend") {
            playerState->defStage++;
            print(playerState->monster->nickname + "'s defense rose by 1 stage!");
            if (playerState->defStage == 7) {
                playerState->defStage = 6;
                print("Although it maxed out at 6!");
            }
            notifyUseItem(true);
            } else if (usedItem->name == "X Special Attack") {
            playerState->spatkStage++;
            print(playerState->monster->nickname + "'s special attack rose by 1 stage!");
            if (playerState->spatkStage == 7) {
                playerState->spatkStage = 6;
                print("Although it maxed out at 6!");
            }
            notifyUseItem(true);
            } else if (usedItem->name == "X Special Defend") {
            playerState->spdefStage++;
            print(playerState->monster->nickname + "'s special defense rose by 1 stage!");
            if (playerState->spdefStage == 7) {
                playerState->spdefStage = 6;
                print("Although it maxed out at 6!");
            }
            notifyUseItem(true);
            } else if (usedItem->name == "X Speed") {
            playerState->spdStage++;
            print(playerState->monster->nickname + "'s speed rose by 1 stage!");
            if (playerState->spdStage == 7) {
                playerState->spdStage = 6;
                print("Although it maxed out at 6!");
            }
            notifyUseItem(true);
            } else if (usedItem->name == "Dire Hit") {
            playerState->critMul *= 4;
            print(playerState->monster->nickname + " is now 4 times more likely to get a critical hit!");
            notifyUseItem(true);
            } else if (usedItem->name == "Guard Spec.") {
            if (playerState->canLower) {
                playerState->canLower = false;
                print(playerState->monster->nickname + "'s stats cannot be lowered!");
            } else {
                print("Although they already could not be lowered!");
            }
            notifyUseItem(true);
            } else if (usedItem->name == "Poke Doll") {
            battling = false;
            print("You got away successfully!");
            notifyUseItem(true);
            } else if (usedItem->name == "PokeBall") {
            if (!wildMon) {
                print("This can't be used now.");
                notifyUseItem(false);
                return;
            }
            if (enemyState && enemyState->monster && enemyState->monster->nickname.rfind("Enemy ", 0) == 0) {
                print("You can't catch a trainer's pokemon!");
                notifyUseItem(false);
                return;
            }
            catchMon(1.0);
            } else if (usedItem->name == "Great Ball") {
            if (!wildMon) {
                print("This can't be used now.");
                notifyUseItem(false);
                return;
            }
            if (enemyState && enemyState->monster && enemyState->monster->nickname.rfind("Enemy ", 0) == 0) {
                print("You can't catch a trainer's pokemon!");
                notifyUseItem(false);
                return;
            }
            catchMon(1.5);
            } else if (usedItem->name == "Ultra Ball") {
            if (!wildMon) {
                print("This can't be used now.");
                notifyUseItem(false);
                return;
            }
            if (enemyState && enemyState->monster && enemyState->monster->nickname.rfind("Enemy ", 0) == 0) {
                print("You can't catch a trainer's pokemon!");
                notifyUseItem(false);
                return;
            }
            catchMon(2.0);
            } else if (usedItem->name == "Master Ball") {
            if (!wildMon) {
                print("This can't be used now.");
                notifyUseItem(false);
                return;
            }
            if (enemyState && enemyState->monster && enemyState->monster->nickname.rfind("Enemy ", 0) == 0) {
                print("You can't catch a trainer's pokemon!");
                notifyUseItem(false);
                return;
            }
            catchMon(100000);
            } else if (usedItem->name == "Repel") {
            useRepel(101);
            } else if (usedItem->name == "Super Repel") {
            useRepel(201);
            } else if (usedItem->name == "Max Repel") {
            useRepel(251);
            } else {
                longArr = Battler::partyStrings(player->team);
                psi = 0;
                chosenMon = nullptr;
            }
            return;
        }

        if (chosenMon == nullptr) {
            if (!player || idx < 0 || idx >= static_cast<int>(player->team.size()) || player->team[idx] == nullptr) {
                return;
            }
            Battler *b = player->team[idx];
            if (!b) {
                return;
            }
            if (usedItem->name == "Ether" || usedItem->name == "Max Ether" || usedItem->name == "PP Up") {
                chosenMon = b;
                longArr = chosenMon->moveStrings();
                psi = 0;
                return;
            }

            if (usedItem->name == "Antidote") {
                healStatus(b, "POISONED");
            } else if (usedItem->name == "Awakening") {
                healStatus(b, "SLEEPING");
            } else if (usedItem->name == "Burn Heal") {
                healStatus(b, "BURNED");
            } else if (usedItem->name == "Ice Heal") {
                healStatus(b, "FROZEN");
            } else if (usedItem->name == "Paralyze Heal") {
                healStatus(b, "PARALYZED");
            } else if (usedItem->name == "Full Heal") {
                if (b->status.empty()) {
                    print("Quit messing around!");
                    notifyUseItem(false);
                } else {
                    if (playerState && playerState->monster == b) {
                        playerState->poisonDamage = 0;
                    }
                    b->status.clear();
                    print(b->nickname + "'s status was healed!");
                    notifyUseItem(true);
                }
            } else if (usedItem->name == "Full Restore") {
                if (b->hp == 0) {
                    print("No, you'll need something stronger to heal this one...");
                    notifyUseItem(false);
                } else if (b->hp == b->mhp && b->status.empty()) {
                    print("Quit messing around!");
                    notifyUseItem(false);
                } else {
                    b->hp = b->mhp;
                    b->status.clear();
                    print(b->nickname + " was fully restored!");
                    notifyUseItem(true);
                }
            } else if (usedItem->name == "Elixir") {
                useElixir(b, 10);
            } else if (usedItem->name == "Max Elixir") {
                useElixir(b, 99);
            } else if (usedItem->name == "Potion") {
                healHp(b, 20);
            } else if (usedItem->name == "Fresh Water") {
                healHp(b, 50);
            } else if (usedItem->name == "Super Potion") {
                healHp(b, 50);
            } else if (usedItem->name == "Soda Pop") {
                healHp(b, 60);
            } else if (usedItem->name == "Hyper Potion") {
                healHp(b, 200);
            } else if (usedItem->name == "Lemonade") {
                healHp(b, 80);
            } else if (usedItem->name == "Max Potion") {
                healHp(b, 999);
            } else if (usedItem->name == "Revive") {
                useRevive(b, b->mhp / 2);
            } else if (usedItem->name == "Max Revive") {
                useRevive(b, b->mhp);
            } else {
                print("This can't be used now.");
                notifyUseItem(false);
            }
            return;
        }

        if (chosenMon != nullptr) {
            if (idx < 0 || idx >= static_cast<int>(chosenMon->moves.size()) || chosenMon->moves[idx] == nullptr) {
                return;
            }
            Move *move = chosenMon->moves[idx];
            if (usedItem->name == "Ether") {
                if (chosenMon->pp[idx] == chosenMon->mpp[idx]) {
                    print("DO NOT WASTE THAT!");
                    notifyUseItem(false);
                } else {
                    chosenMon->pp[idx] = std::min(chosenMon->mpp[idx], chosenMon->pp[idx] + 10);
                    print(move->name + "'s PP was restored by 10!");
                    notifyUseItem(true);
                }
            } else if (usedItem->name == "Max Ether") {
                if (chosenMon->pp[idx] == chosenMon->mpp[idx]) {
                    print("DO NOT WASTE THAT!");
                    notifyUseItem(false);
                } else {
                    chosenMon->pp[idx] = chosenMon->mpp[idx];
                    print(move->name + "'s PP was fully restored!");
                    notifyUseItem(true);
                }
            } else if (usedItem->name == "PP Up") {
                chosenMon->mpp[idx] = static_cast<int>(chosenMon->mpp[idx] * 1.2);
                print(move->name + "'s PP was permanently increased!");
                notifyUseItem(true);
            } else {
                notifyUseItem(false);
            }
            chosenMon = nullptr;
            return;
        }
    }
    if (choosingFromLongList) {
        int lineIdx = longListChoiceFromY(mouseY);
        if (lineIdx < 0) {
            return;
        }
        clickedChoice = lineIdx;
        return;
    }
    clickedChoice = menuChoiceFromY(mouseY);
}

void OverworldGui::setWildItems() {
    if (!player) {
        return;
    }
    strArr[0] = "Items";
    usableItems.clear();
    longArr.clear();
    for (Item *item : player->items) {
        usableItems.push_back(item);
        longArr.push_back(item ? item->toString() : "");
    }
    psi = 0;
    usingBattleItem = true;
    busedItem = false;
    choosingFromLongList = true;
    usingItemList = true;
    if (player->items.empty()) {
        print("No items!");
        usingBattleItem = false;
        choosingFromLongList = false;
        usingItemList = false;
    }
}

void OverworldGui::setTrainerItems() {
    if (!player) {
        return;
    }
    strArr[0] = "Items";
    usableItems.clear();
    longArr.clear();
    for (Item *item : player->items) {
        usableItems.push_back(item);
        longArr.push_back(item ? item->toString() : "");
    }
    psi = 0;
    usingBattleItem = true;
    busedItem = false;
    choosingFromLongList = true;
    usingItemList = true;
    if (player->items.empty()) {
        print("No items!");
        usingBattleItem = false;
        choosingFromLongList = false;
        usingItemList = false;
    }
}

void OverworldGui::pumpEvents() {
}

void OverworldGui::handleEvent(const SDL_Event &event) {
    if (s_inTextPrompt) {
        return;
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !event.key.repeat) {
        autoBattle = !autoBattle;
        spacebar = false;
        if (showingText) {
            advanceText();
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        clickMouse(event.button.x, event.button.y, event.button.button == SDL_BUTTON_RIGHT);
        return;
    }
    auto pressDirection = [&](Direction dir, bool pressed) {
        auto it = std::find(pressedKeys.begin(), pressedKeys.end(), dir);
        if (pressed) {
            if (it != pressedKeys.end()) {
                pressedKeys.erase(it);
            }
            pressedKeys.push_back(dir);
            heldDirection = dir;
            hasHeldDirection = true;
        } else {
            if (it != pressedKeys.end()) {
                pressedKeys.erase(it);
            }
            if (hasHeldDirection && heldDirection == dir) {
                if (pressedKeys.empty()) {
                    hasHeldDirection = false;
                } else {
                    heldDirection = pressedKeys.back();
                }
            }
        }
    };
    if (event.type == SDL_KEYDOWN && !event.key.repeat) {
        if (event.key.keysym.sym == SDLK_p || event.key.keysym.sym == SDLK_l || event.key.keysym.sym == SDLK_i || event.key.keysym.sym == SDLK_c ||
            event.key.keysym.sym == SDLK_t || event.key.keysym.sym == SDLK_y || event.key.keysym.sym == SDLK_x || event.key.keysym.sym == SDLK_q ||
            event.key.keysym.sym == SDLK_m) {
            if (inMenu || choosingFromLongList || buySell || flying || usingItemList || usingBattleItem || depWith || depositing || withdraw ||
                checkingPokes || checkingMoves || checkingTms || teachingMove || replacingMove) {
                clickMouse(0, 0, true);
                return;
            }
        }
        switch (event.key.keysym.sym) {
            case SDLK_w:
            case SDLK_COMMA:
                if (!pickingStarter) {
                    pressDirection(Direction::NORTH, true);
                }
                break;
            case SDLK_s:
            case SDLK_o:
                if (!pickingStarter) {
                    pressDirection(Direction::SOUTH, true);
                }
                break;
            case SDLK_a:
                if (!pickingStarter) {
                    pressDirection(Direction::WEST, true);
                }
                break;
            case SDLK_d:
            case SDLK_e:
                if (!pickingStarter) {
                    pressDirection(Direction::EAST, true);
                }
                break;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                if (!showingText && !battling && !pickingStarter) {
                    save();
                }
                break;
            case SDLK_m:
                if (!showingText && !battling && !pickingStarter) {
                    if (flying) {
                        flying = false;
                        inMenu = false;
                        inside = false;
                        currentLoc.clear();
                    } else {
                        startFlying();
                    }
                }
                break;
            case SDLK_p:
            case SDLK_l:
                if (battling && !showingText) {
                    clickedChoice = 1;
                } else if (!showingText && !pickingStarter) {
                    if (inMenu || choosingFromLongList || buySell) {
                        closeMenus();
                    }
                    usableItems.clear();
                    longArr = Battler::partyStrings(player->team);
                    strArr[0] = "Pokemon";
                    psi = 0;
                    choosingFromLongList = true;
                    inMenu = true;
                    checkingPokes = true;
                }
                break;
            case SDLK_i:
            case SDLK_c:
                if (battling && !showingText) {
                    clickedChoice = 2;
                } else if (!showingText && !pickingStarter) {
                    if (inMenu || choosingFromLongList || buySell) {
                        closeMenus();
                    }
                    usableItems.clear();
                    longArr.clear();
                    for (Item *it : player->items) {
                        usableItems.push_back(it);
                        longArr.push_back(it ? it->toString() : "");
                    }
                    strArr[0] = "Items";
                    psi = 0;
                    choosingFromLongList = true;
                    inMenu = true;
                    usingItemList = true;
                }
                break;
            case SDLK_t:
            case SDLK_y:
                if (!showingText && !pickingStarter && !battling) {
                    if (inMenu || choosingFromLongList || buySell) {
                        closeMenus();
                    }
                    longArr.clear();
                    tms.clear();
                    for (Move *m : player->tmHms) {
                        if (!m) {
                            continue;
                        }
                        tms.push_back(m);
                        longArr.push_back(m->name);
                    }
                    strArr[0] = "TMs";
                    psi = 0;
                    choosingFromLongList = true;
                    inMenu = true;
                    checkingTms = true;
                }
                break;
            case SDLK_x:
            case SDLK_q:
                if (!showingText && !pickingStarter && !battling) {
                    if (inMenu) {
                        closeMenus();
                        break;
                    }
                    if (choosingFromLongList || buySell) {
                        break;
                    }
                    closeMenus();
                    strArr[0] = "Pokedex (Caught: " + std::to_string(player->numCaught) + ")";
                    int dexSize = static_cast<int>(Monster::MONSTERS.size());
                    longArr.assign(std::max(20, dexSize), "");
                    for (int i = 0; i < static_cast<int>(longArr.size()); i++) {
                        std::string n = std::to_string(i);
                        std::string num = std::string(3 - static_cast<int>(n.size()), '0') + n;
                        std::string monName = "?";
                        if (i < static_cast<int>(player->pokedex.size()) && player->pokedex[i] && i < static_cast<int>(Monster::MONSTERS.size())) {
                            monName = Monster::MONSTERS[i].name;
                        }
                        if (i == 0) {
                            monName = (!player->pokedex.empty() && player->pokedex[0]) ? "Missingo" : "";
                        }
                        if ((i == 151 || i == 152) && (i >= static_cast<int>(player->pokedex.size()) || !player->pokedex[i])) {
                            monName.clear();
                        }
                        longArr[i] = monName.empty() ? "" : (num + ": " + monName);
                    }
                    psi = 0;
                    choosingFromLongList = true;
                    showingDex = true;
                    inMenu = true;
                }
                break;
            case SDLK_UP:
                if (inMenu) {
                    if (checkingPokes || depositing) {
                        int n = 0;
                        while (n < static_cast<int>(player->team.size()) && player->team[n] != nullptr) {
                            n++;
                        }
                        if (n >= 2) {
                            Battler *b = player->team[0];
                            for (int i = 1; i < n; i++) {
                                player->team[i - 1] = player->team[i];
                                if (i - 1 < static_cast<int>(longArr.size()) && i < static_cast<int>(longArr.size())) {
                                    longArr[i - 1] = longArr[i];
                                }
                            }
                            player->team[n - 1] = b;
                            if (n - 1 < static_cast<int>(longArr.size())) {
                                longArr[n - 1] = b ? b->toString() : "";
                            }
                        }
                    } else if (checkingTms && !tms.empty()) {
                        if (tms.size() >= 2 && !longArr.empty()) {
                            Move *m = tms[0];
                            for (size_t i = 1; i < tms.size(); i++) {
                                tms[i - 1] = tms[i];
                                if (i - 1 < longArr.size() && i < longArr.size()) {
                                    longArr[i - 1] = longArr[i];
                                }
                            }
                            tms[tms.size() - 1] = m;
                            if (tms.size() - 1 < longArr.size()) {
                                longArr[tms.size() - 1] = m ? m->name : "";
                            }
                        }
                    } else if (checkingMoves && player && clickedChoice >= 0 && clickedChoice < static_cast<int>(player->team.size()) && player->team[clickedChoice]) {
                        Battler *b = player->team[clickedChoice];
                        for (int i = 1; i < 4; i++) {
                            if (b->moves[i] == nullptr) {
                                continue;
                            }
                            std::swap(b->moves[i - 1], b->moves[i]);
                            std::swap(b->pp[i - 1], b->pp[i]);
                            std::swap(b->mpp[i - 1], b->mpp[i]);
                        }
                        longArr = b->allInformation();
                    } else if (withdraw) {
                        if (!player->pc.empty()) {
                            psi = clampPageStart(psi, static_cast<int>(player->pc.size()), 20);
                            if (psi < static_cast<int>(player->pc.size())) {
                                player->pc.push_back(player->pc[psi]);
                                player->pc.erase(player->pc.begin() + psi);
                                for (int i = 0; i < 20 && i < static_cast<int>(longArr.size()); i++) {
                                    longArr[i] = (psi + i < static_cast<int>(player->pc.size()) && player->pc[psi + i]) ? player->pc[psi + i]->toString() : "";
                                }
                            }
                        }
                    } else if (usingItemList && !usableItems.empty()) {
                        if (usableItems.size() >= 2 && !longArr.empty()) {
                            Item *it = usableItems[0];
                            for (size_t i = 1; i < usableItems.size(); i++) {
                                usableItems[i - 1] = usableItems[i];
                                if (i - 1 < longArr.size() && i < longArr.size()) {
                                    longArr[i - 1] = longArr[i];
                                }
                            }
                            usableItems[usableItems.size() - 1] = it;
                            if (!longArr.empty()) {
                                longArr[longArr.size() - 1] = it ? it->toString() : "";
                            }

                            if (player && player->items.size() == usableItems.size()) {
                                Item *pit = player->items[0];
                                for (size_t i = 1; i < player->items.size(); i++) {
                                    player->items[i - 1] = player->items[i];
                                }
                                player->items[player->items.size() - 1] = pit;
                            }
                        }
                    }
                }
                break;
            case SDLK_DOWN:
                if (inMenu) {
                    if ((checkingPokes || depositing) && player && player->team.size() >= 2 && player->team[0] && player->team[1]) {
                        std::swap(player->team[0], player->team[1]);
                        if (longArr.size() >= 2) {
                            longArr[0] = player->team[0]->toString();
                            longArr[1] = player->team[1]->toString();
                        }
                    } else if (checkingMoves && player && clickedChoice >= 0 && clickedChoice < static_cast<int>(player->team.size()) && player->team[clickedChoice]) {
                        Battler *b = player->team[clickedChoice];
                        for (int i = 2; i >= 0; i--) {
                            if (b->moves[i] == nullptr || b->moves[i + 1] == nullptr) {
                                continue;
                            }
                            std::swap(b->moves[i], b->moves[i + 1]);
                            std::swap(b->pp[i], b->pp[i + 1]);
                            std::swap(b->mpp[i], b->mpp[i + 1]);
                        }
                        longArr = b->allInformation();
                    } else if (checkingTms && tms.size() >= 2) {
                        std::swap(tms[0], tms[1]);
                        if (longArr.size() >= 2) {
                            longArr[0] = tms[0] ? tms[0]->name : "";
                            longArr[1] = tms[1] ? tms[1]->name : "";
                        }
                    } else if (usingItemList && usableItems.size() >= 2) {
                        std::swap(usableItems[0], usableItems[1]);
                        if (longArr.size() >= 2) {
                            longArr[0] = usableItems[0] ? usableItems[0]->toString() : "";
                            longArr[1] = usableItems[1] ? usableItems[1]->toString() : "";
                        }

                        if (player && player->items.size() == usableItems.size()) {
                            std::swap(player->items[0], player->items[1]);
                        }
                    }
                }
                break;
            case SDLK_LEFT:
                if (withdraw) {
                    if (psi == 0) {
                        break;
                    }
                    boxNum = std::max(1, boxNum - 1);
                    psi = std::max(0, psi - 20);
                    longArr.clear();
                    for (int i = 0; i < 20; i++) {
                        int ind = psi + i;
                        longArr.push_back((player && ind < static_cast<int>(player->pc.size()) && player->pc[ind]) ? player->pc[ind]->toString() : "");
                    }
                    break;
                }
                if (choosingFromLongList) {
                    psi = clampPageStart(psi - 20, static_cast<int>(longArr.size()), 20);
                }
                break;
            case SDLK_RIGHT:
                if (withdraw) {
                    if (!player || psi + 20 >= static_cast<int>(player->pc.size())) {
                        break;
                    }
                    boxNum++;
                    psi += 20;
                    longArr.clear();
                    for (int i = 0; i < 20; i++) {
                        int ind = psi + i;
                        longArr.push_back((player && ind < static_cast<int>(player->pc.size()) && player->pc[ind]) ? player->pc[ind]->toString() : "");
                    }
                    break;
                }
                if (choosingFromLongList) {
                    psi = clampPageStart(psi + 20, static_cast<int>(longArr.size()), 20);
                }
                break;
            case SDLK_1:
            case SDLK_2:
            case SDLK_3:
            case SDLK_4:
            case SDLK_5:
            case SDLK_6:
            case SDLK_7:
            case SDLK_8:
            case SDLK_9:
            case SDLK_0: {
                if (showingText) {
                    advanceText();
                    break;
                }
                int v = (event.key.keysym.sym == SDLK_0) ? 9 : (static_cast<int>(event.key.keysym.sym - SDLK_1));

                if (choosingFromLongList && v >= 0 && v < 20) {
                    int startY = GameConfig::WORLD_HEIGHT + GameConfig::LONG_LIST_START_Y;
                    int clickY = startY + v * GameConfig::MENU_OPTION_HEIGHT + 1;
                    clickMouse(80, clickY, false);
                    return;
                }

                if (depWith) {
                    int boxHeight = GameConfig::MENU_BOX_HEIGHT;
                    int boxY = GameConfig::WINDOW_HEIGHT - boxHeight - GameConfig::MENU_BOX_MARGIN;
                    int startY = boxY + GameConfig::MENU_OPTION_START_OFFSET;
                    int clickY = startY + v * GameConfig::MENU_OPTION_HEIGHT + 1;
                    clickMouse(80, clickY, false);
                    return;
                }

                if (buySell && !choosingFromLongList) {
                    if (v == 0) {
                        buying = true;
                        buySell = false;
                        choosingFromLongList = true;
                        usableItems.clear();
                        longArr.clear();
                        for (const MartItem &m : martItems) {
                            longArr.push_back(m.toString());
                        }
                        strArr[0] = "Money: $" + std::to_string(player ? player->money : 0);
                        psi = 0;
                        return;
                    }
                    if (v == 1) {
                        selling = true;
                        buySell = false;
                        choosingFromLongList = true;
                        usableItems.clear();
                        longArr.clear();
                        for (Item *it : player->items) {
                            if (it && it->price > 0) {
                                usableItems.push_back(it);
                                longArr.push_back(it->name + " x" + std::to_string(it->quantity) + "  " + std::to_string(it->price / 2));
                            }
                        }
                        strArr[0] = "Sell";
                        psi = 0;
                        return;
                    }
                }
                if ((buying || selling) && choosingFromLongList) {
                    clickedChoice = v;
                    return;
                }
                clickedChoice = v;
                break;
            }
            default:
                break;
        }
    } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
            case SDLK_COMMA:
                if (!pickingStarter) {
                    pressDirection(Direction::NORTH, false);
                }
                break;
            case SDLK_s:
            case SDLK_o:
                if (!pickingStarter) {
                    pressDirection(Direction::SOUTH, false);
                }
                break;
            case SDLK_a:
                if (!pickingStarter) {
                    pressDirection(Direction::WEST, false);
                }
                break;
            case SDLK_d:
            case SDLK_e:
                if (!pickingStarter) {
                    pressDirection(Direction::EAST, false);
                }
                break;
            default:
                break;
        }
    }
}

void OverworldGui::update() {
    if (showingText || battling || buySell || buying || selling || inMenu || pickingStarter) {
        return;
    }

    static std::mt19937 s_blackjackRng(std::random_device{}());

    frames++;
    if (frames % 6 == 0) {
        if (spacebar) {
            clickMouse(0, 0, false);
        }
        if (frames % 30 == 0) {
            if (mattNpc && !danceFrames.empty()) {
                mattNpc->bi = danceFrames[utils::randInt(0, static_cast<int>(danceFrames.size()) - 1)];
            }
        }
        if (monsterTrainer != nullptr) {
            if (oakNpc && !oakFrames.empty()) {
                oakNpc->bi = oakFrames[utils::randInt(0, static_cast<int>(oakFrames.size()) - 1)];
            }
            if (!monsterFrames.empty()) {
                if (frames == 24) {
                    monsterTrainer->bi = monsterFrames[0];
                } else if (frames == 48 && monsterFrames.size() > 2) {
                    monsterTrainer->bi = monsterFrames[2];
                } else if (monsterFrames.size() > 1) {
                    frames %= 60;
                    monsterTrainer->bi = monsterFrames[1];
                }
            }
        }
    }

    if (blackjackActive) {
        if (blackjackTieFrames > 0) {
            blackjackTieFrames--;
            if (blackjackTieFrames == 0) {
                blackjackPlayer.clear();
                blackjackDealer.clear();
                blackjackHideFirst = true;
                blackjackFinished = false;
                blackjackPlayerWon = false;

                if (blackjackDeck.size() < 10) {
                    blackjackDeck.clear();
                    blackjackDeck.reserve(52);
                    for (int i = 0; i < 52; i++) {
                        blackjackDeck.push_back(i);
                    }
                    std::shuffle(blackjackDeck.begin(), blackjackDeck.end(), s_blackjackRng);
                }
                auto drawCard = [&]() {
                    int c = blackjackDeck.back();
                    blackjackDeck.pop_back();
                    return c;
                };
                blackjackPlayer.push_back(drawCard());
                blackjackPlayer.push_back(drawCard());
                blackjackDealer.push_back(drawCard());
                blackjackDealer.push_back(drawCard());
            }
        }
        return;
    }

    if (stepPhase == StepPhase::NONE && hasHeldDirection) {
        facing = heldDirection;
        currentStepFrames = canMove(heldDirection) ? NUM_STEP_FRAMES : BUMP_STEP_FRAMES;
        stepPhase = StepPhase::MOVING;
        phaseFrame = 0;
    }
    if (stepPhase != StepPhase::NONE) {
        advanceStep();
    }
}

void OverworldGui::render() {
    if (!renderer) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int viewW = GameConfig::WINDOW_WIDTH;
    int viewH = GameConfig::WORLD_HEIGHT;
    int camX = static_cast<int>(playerX * GameConfig::TILE_SIZE + offsetX - viewW / 2 + GameConfig::TILE_SIZE / 2);
    int camY = static_cast<int>(playerY * GameConfig::TILE_SIZE + offsetY - viewH / 2 + GameConfig::TILE_SIZE / 2);

    int startX = std::max(0, camX / GameConfig::TILE_SIZE - 1);
    int startY = std::max(0, camY / GameConfig::TILE_SIZE - 1);
    int endX = std::min(static_cast<int>(currentMap[0].size()), startX + viewW / GameConfig::TILE_SIZE + 3);
    int endY = std::min(static_cast<int>(currentMap.size()), startY + viewH / GameConfig::TILE_SIZE + 3);

    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            SDL_Texture *tile = tileImages[currentMap[y][x]];
            SDL_Rect dst{ x * GameConfig::TILE_SIZE - camX,
                          y * GameConfig::TILE_SIZE - camY,
                          GameConfig::TILE_SIZE, GameConfig::TILE_SIZE };
            if (tile) {
                SDL_RenderCopy(renderer, tile, nullptr, &dst);
            }
            Trainer *t = pm->trainers[y][x];
            if (t) {
                SDL_RenderCopy(renderer, t->bi, nullptr, &dst);
            } else if (pm->wob[y][x]) {
                SDL_RenderCopy(renderer, pm->wob[y][x]->bi, nullptr, &dst);
            } else if (pm->npcs[y][x]) {
                SDL_RenderCopy(renderer, pm->npcs[y][x]->bi, nullptr, &dst);
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        if (connections[i]) {
            drawConnection(i, connections[i], connOffsets[i], camX, camY);
        }
    }

    int drawX = viewW / 2 - GameConfig::TILE_SIZE / 2;
    int drawY = viewH / 2 - GameConfig::TILE_SIZE / 2;
    SDL_Texture *playerTex = surfing ? seelFrames[getCurrentFrame()] : playerFrames[getCurrentFrame()];
    if (playerTex) {
        SDL_Rect dst{drawX, drawY, GameConfig::TILE_SIZE, GameConfig::TILE_SIZE};
        SDL_RenderCopy(renderer, playerTex, nullptr, &dst);
    }
    if (battling && playerState && enemyState) {
        auto hsvToRgb = [](float h, float s, float v) {
            h = std::fmod(h, 360.0f);
            if (h < 0) {
                h += 360.0f;
            }
            float c = v * s;
            float x = c * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
            float m = v - c;
            float r = 0.0f;
            float g = 0.0f;
            float b = 0.0f;

            if (h < 60.0f) {
                r = c;
                g = x;
            } else if (h < 120.0f) {
                r = x;
                g = c;
            } else if (h < 180.0f) {
                g = c;
                b = x;
            } else if (h < 240.0f) {
                g = x;
                b = c;
            } else if (h < 300.0f) {
                r = x;
                b = c;
            } else {
                r = c;
                b = x;
            }

            SDL_Color out;
            out.r = static_cast<Uint8>(std::round((r + m) * 255.0f));
            out.g = static_cast<Uint8>(std::round((g + m) * 255.0f));
            out.b = static_cast<Uint8>(std::round((b + m) * 255.0f));
            out.a = 255;
            return out;
        };

        auto drawFilledRainbowStar = [&](int cx, int cy, int outerRadius, int points) {
            if (points < 2) {
                return;
            }
            int innerRadius = std::max(1, outerRadius * 2 / 5);
            std::vector<SDL_Point> outline;
            outline.reserve(points * 2 + 1);
            for (int i = 0; i < points * 2; i++) {
                double angle = (static_cast<double>(i) * M_PI / static_cast<double>(points)) - (M_PI / 2.0);
                int r = (i % 2 == 0) ? outerRadius : innerRadius;
                int x = cx + static_cast<int>(std::round(std::cos(angle) * r));
                int y = cy + static_cast<int>(std::round(std::sin(angle) * r));
                outline.push_back(SDL_Point{x, y});
            }
            outline.push_back(outline.front());

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

            const int steps = std::max(48, outerRadius * 3);
            for (int i = 0; i < steps; i++) {
                float t = steps <= 1 ? 0.0f : (static_cast<float>(i) / static_cast<float>(steps - 1));
                int seg = static_cast<int>(t * static_cast<float>(outline.size() - 1));
                seg = std::max(0, std::min(seg, static_cast<int>(outline.size()) - 2));
                float local = (t * static_cast<float>(outline.size() - 1)) - static_cast<float>(seg);
                int x = static_cast<int>(std::round(outline[seg].x + (outline[seg + 1].x - outline[seg].x) * local));
                int y = static_cast<int>(std::round(outline[seg].y + (outline[seg + 1].y - outline[seg].y) * local));

                SDL_Color c = hsvToRgb(360.0f * t, 1.0f, 1.0f);
                SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 220);
                SDL_RenderDrawLine(renderer, cx, cy, x, y);
            }

            for (int o = 0; o < 2; o++) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawLines(renderer, outline.data(), static_cast<int>(outline.size()));
            }
        };

        auto drawRainbowText = [&](int x, int y, const std::string &text) {
            std::string t = text;
            size_t p = 0;
            while ((p = t.find("🔴", p)) != std::string::npos) {
                t.erase(p, std::string("🔴").size());
            }

            int penX = x;
            int idx = 0;
            for (unsigned char ch : t) {
                if (ch == 0) {
                    continue;
                }
                std::string s(1, static_cast<char>(ch));
                SDL_Color c = hsvToRgb(static_cast<float>((idx * 25) % 360), 1.0f, 1.0f);
                SDL_Texture *tex = TextRenderer::renderText(renderer, s, c, 0);
                if (tex) {
                    int tw = 0;
                    int th = 0;
                    SDL_QueryTexture(tex, nullptr, nullptr, &tw, &th);
                    SDL_Rect dst{penX, y, tw, th};
                    SDL_RenderCopy(renderer, tex, nullptr, &dst);
                    SDL_DestroyTexture(tex);
                    penX += tw;
                }
                idx++;
            }
        };

        auto drawBattlerPanel = [&](int x, int y, Battler *b) {
            if (!b) {
                return;
            }

            SDL_Rect outer{x, y, 360, 124};
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &outer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &outer);

            SDL_Color white{255, 255, 255, 255};
            if (b->shiny) {
                drawRainbowText(x + 10, y + 8, b->nickname);
            } else {
                SDL_Texture *t1 = TextRenderer::renderText(renderer, b->nickname, white, 0);
                if (t1) {
                    int tw = 0;
                    int th = 0;
                    SDL_QueryTexture(t1, nullptr, nullptr, &tw, &th);
                    SDL_Rect dst{x + 10, y + 8, tw, th};
                    SDL_RenderCopy(renderer, t1, nullptr, &dst);
                    SDL_DestroyTexture(t1);
                }
            }

            SDL_Texture *tLvl = TextRenderer::renderText(renderer, "Level " + std::to_string(b->level), white, 0);
            if (tLvl) {
                int tw = 0;
                int th = 0;
                SDL_QueryTexture(tLvl, nullptr, nullptr, &tw, &th);
                SDL_Rect dst{x + 10, y + 32, tw, th};
                SDL_RenderCopy(renderer, tLvl, nullptr, &dst);
                SDL_DestroyTexture(tLvl);
            }

            if (!b->status.empty()) {
                SDL_Texture *tSt = TextRenderer::renderText(renderer, b->status, white, 0);
                if (tSt) {
                    int tw = 0;
                    int th = 0;
                    SDL_QueryTexture(tSt, nullptr, nullptr, &tw, &th);
                    SDL_Rect dst{x + 10, y + 52, tw, th};
                    SDL_RenderCopy(renderer, tSt, nullptr, &dst);
                    SDL_DestroyTexture(tSt);
                }
            }

            std::string hpText = std::to_string(b->hp) + " / " + std::to_string(b->mhp);
            SDL_Texture *t2 = TextRenderer::renderText(renderer, "HP: " + hpText, white, 0);
            if (t2) {
                int tw = 0;
                int th = 0;
                SDL_QueryTexture(t2, nullptr, nullptr, &tw, &th);
                SDL_Rect dst{x + 10, y + 69, tw, th};
                SDL_RenderCopy(renderer, t2, nullptr, &dst);
                SDL_DestroyTexture(t2);
            }

            int percent = b->mhp <= 0 ? 0 : static_cast<int>((b->hp / static_cast<float>(b->mhp)) * 100.0f);
            percent = std::max(0, std::min(100, percent));
            SDL_Rect barBg{x + 10, y + 96, 330, 16};
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderFillRect(renderer, &barBg);
            SDL_Rect barFg{x + 10, y + 96, 330 * percent / 100, 16};
            if (percent <= 33) {
                SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
            } else if (percent <= 66) {
                SDL_SetRenderDrawColor(renderer, 200, 200, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
            }
            SDL_RenderFillRect(renderer, &barFg);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &barBg);
        };

        drawBattlerPanel(20, 20, playerState->monster);
        drawBattlerPanel(GameConfig::WINDOW_WIDTH - 20 - 360, 20, enemyState->monster);

        int pX = 64;
        int eX = GameConfig::WINDOW_WIDTH - 64 - 256;
        int y = 128;
        SDL_Texture *pTex = (playerState->monster->dexNum >= 0 && playerState->monster->dexNum < static_cast<int>(pBattlers.size()))
                                 ? pBattlers[playerState->monster->dexNum]
                                 : nullptr;
        SDL_Texture *eTex = (enemyState->monster->dexNum >= 0 && enemyState->monster->dexNum < static_cast<int>(eBattlers.size()))
                                 ? eBattlers[enemyState->monster->dexNum]
                                 : nullptr;
        if (!pTex && playerState->monster->dexNum == 0) {
            pTex = getMissingnoTexture(renderer);
            mutateMissingnoTexture(renderer);
        }
        if (!eTex && enemyState->monster->dexNum == 0) {
            eTex = getMissingnoTexture(renderer);
            mutateMissingnoTexture(renderer);
        }
        if (pTex) {
            SDL_Rect dst{pX, y, 256, 256};
            SDL_RenderCopy(renderer, pTex, nullptr, &dst);
            if (playerState->monster && playerState->monster->shiny) {
                drawFilledRainbowStar(pX + 32, y + 32, 44, 5);
                drawFilledRainbowStar(pX + 96, y + 32, 36, 5);
            }
        }
        if (eTex) {
            SDL_Rect dst{eX, y, 256, 256};
            SDL_RenderCopy(renderer, eTex, nullptr, &dst);
            if (enemyState->monster && enemyState->monster->shiny) {
                drawFilledRainbowStar(eX + 96, y + 32, 44, 5);
                drawFilledRainbowStar(eX + 160, y + 32, 36, 5);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_Rect uiRect{0, GameConfig::WORLD_HEIGHT, GameConfig::WINDOW_WIDTH, GameConfig::UI_HEIGHT};
    SDL_RenderFillRect(renderer, &uiRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, 0, GameConfig::WORLD_HEIGHT, GameConfig::WINDOW_WIDTH, GameConfig::WORLD_HEIGHT);

    bool canDrawMap = !showingText && !battling && !choosingFromLongList && !buySell && !pickingStarter && !blackjackActive && FlyLocation::MAP_TEXTURE;
    if (canDrawMap) {
        canDrawMap = flying || (canMap && !inMenu);
    }

    if (canDrawMap) {
        SDL_Rect dst{0, GameConfig::WORLD_HEIGHT, GameConfig::WINDOW_WIDTH, GameConfig::UI_HEIGHT};
        SDL_RenderCopy(renderer, FlyLocation::MAP_TEXTURE, nullptr, &dst);

        int cellW = GameConfig::WINDOW_WIDTH / 11;
        int cellH = GameConfig::UI_HEIGHT / 11;

        if (flying) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            for (int i = 0; i < 11; i++) {
                for (int j = 0; j < 11; j++) {
                    if (FlyLocation::isRed(i, j)) {
                        SDL_Rect r{j * cellW, GameConfig::WORLD_HEIGHT + i * cellH, cellW, cellH};
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 120);
                        SDL_RenderFillRect(renderer, &r);
                    }
                }
            }
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }

        SDL_Texture *playerTex = surfing ? seelFrames[getCurrentFrame()] : playerFrames[getCurrentFrame()];
        if (playerTex) {
            int px = 10 * cellW;
            int py = GameConfig::WORLD_HEIGHT + 10 * cellH;
            if (!inside) {
                for (int i = 0; i < 11; i++) {
                    for (int j = 0; j < 11; j++) {
                        if (pm && pm->name == FlyLocation::NAME_MAP[i][j]) {
                            px = j * cellW;
                            py = GameConfig::WORLD_HEIGHT + i * cellH;
                            i = 11;
                            break;
                        }
                    }
                }
            }
            px += (cellW - GameConfig::TILE_SIZE) / 2;
            py += (cellH - GameConfig::TILE_SIZE) / 2;
            SDL_Rect pDst{px, py, GameConfig::TILE_SIZE, GameConfig::TILE_SIZE};
            SDL_RenderCopy(renderer, playerTex, nullptr, &pDst);
        }

        if (flying && !currentLoc.empty()) {
            SDL_Color green{0, 160, 0, 255};
            SDL_Texture *tex = TextRenderer::renderText(renderer, currentLoc, green, 600);
            if (tex) {
                int w = 0;
                int h = 0;
                SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
                SDL_Rect tDst{80, GameConfig::WORLD_HEIGHT + 20, w, h};
                SDL_RenderCopy(renderer, tex, nullptr, &tDst);
                SDL_DestroyTexture(tex);
            }
        }
    }

    if (blackjackActive) {
        auto cardValue = [](int c) {
            int r = c % 13;
            if (r == 0) {
                return 11;
            }
            if (r >= 9) {
                return 10;
            }
            return r + 1;
        };

        auto handValue = [&](const std::vector<int> &hand) {
            int sum = 0;
            int aces = 0;
            for (int c : hand) {
                sum += cardValue(c);
                if (c % 13 == 0) {
                    aces++;
                }
            }
            while (sum > 21 && aces-- > 0) {
                sum -= 10;
            }
            return sum;
        };

        auto fmtCard = [](int c) {
            static const char *ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
            static const char *suits[] = {"H", "D", "C", "S"};
            int r = c % 13;
            int s = c / 13;
            std::string out = ranks[r];
            out += suits[s];
            return out;
        };

        auto fmtHand = [&](const std::vector<int> &hand, bool hideFirst) {
            std::string out;
            for (size_t i = 0; i < hand.size(); i++) {
                if (!out.empty()) {
                    out += " ";
                }
                if (i == 0 && hideFirst) {
                    out += "??";
                } else {
                    out += fmtCard(hand[i]);
                }
            }
            return out;
        };

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect bg{0, GameConfig::WORLD_HEIGHT, GameConfig::WINDOW_WIDTH, GameConfig::UI_HEIGHT};
        SDL_RenderFillRect(renderer, &bg);

        {
            SDL_Color color{0, 0, 0, 255};
            SDL_Texture *tex = TextRenderer::renderText(renderer, "Blackjack", color, GameConfig::WINDOW_WIDTH - 120);
            if (tex) {
                int w = 0;
                int h = 0;
                SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
                int headerY = GameConfig::WORLD_HEIGHT + 30;
                SDL_Rect dst{(GameConfig::WINDOW_WIDTH - w) / 2, headerY, w, h};
                SDL_RenderCopy(renderer, tex, nullptr, &dst);
                SDL_DestroyTexture(tex);
            }
        }

        std::vector<std::string> lines;
        lines.push_back("");
        lines.push_back("Dealer:");
        lines.push_back(fmtHand(blackjackDealer, blackjackHideFirst));
        lines.push_back("");
        lines.push_back("Player:");
        lines.push_back(fmtHand(blackjackPlayer, false));
        lines.push_back("Points: " + std::to_string(handValue(blackjackPlayer)));
        lines.push_back("");
        if (blackjackTieFrames > 0) {
            lines.push_back("TIE! DEALING AGAIN");
        } else {
            lines.push_back("Left click: Hit");
            lines.push_back("Right click: Stand");
        }

        int x = 60;
        int y = GameConfig::WORLD_HEIGHT + GameConfig::LONG_LIST_START_Y;
        int maxWidth = GameConfig::WINDOW_WIDTH - 120;
        int maxLines = 20;
        for (int lineIndex = 0; lineIndex < maxLines; lineIndex++) {
            if (lineIndex >= static_cast<int>(lines.size())) {
                break;
            }
            drawWrappedText(lines[lineIndex], x, y + lineIndex * GameConfig::MENU_OPTION_HEIGHT, maxWidth);
        }
    } else if (showingText) {
        drawTextBox(currentText);
    } else if (choosingFromLongList) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect bg{0, GameConfig::WORLD_HEIGHT, GameConfig::WINDOW_WIDTH, GameConfig::UI_HEIGHT};
        SDL_RenderFillRect(renderer, &bg);

        auto headerText = [&]() -> std::string {
            if (!longListHeaderOverride.empty()) {
                return longListHeaderOverride;
            }
            if (blackjackShowingResults) {
                return "Blackjack";
            }
            if (teachingMove && tm) {
                return "Teach " + tm->name + " to which pokemon?";
            }
            if (replacingMove && tm) {
                return "Replace which move with " + tm->name + "?";
            }
            if (buying || selling) {
                return "Money: $" + std::to_string(player ? player->money : 0);
            }
            if (checkingPokes || depositing) {
                return "Party";
            }
            if (withdraw) {
                return "Box " + std::to_string(boxNum);
            }
            if (checkingTms) {
                return "TM/HMs";
            }
            if (checkingMoves) {
                return "Stats";
            }
            if (showingDex) {
                return "Number Caught: " + std::to_string(player ? player->numCaught : 0);
            }
            return "Inventory";
        };

        std::string header = headerText();
        int listTopY = GameConfig::WORLD_HEIGHT + GameConfig::LONG_LIST_START_Y;
        int headerH = 0;
        if (!header.empty()) {
            SDL_Color color{0, 0, 0, 255};
            SDL_Texture *tex = TextRenderer::renderText(renderer, header, color, GameConfig::WINDOW_WIDTH - 120);
            if (tex) {
                int w = 0;
                int h = 0;
                SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
                headerH = h;
                int headerY = GameConfig::WORLD_HEIGHT + 30;
                SDL_Rect dst{(GameConfig::WINDOW_WIDTH - w) / 2, headerY, w, h};
                SDL_RenderCopy(renderer, tex, nullptr, &dst);
                SDL_DestroyTexture(tex);
            }
        }

        int x = 60;
        int y = std::max(listTopY, GameConfig::WORLD_HEIGHT + 30 + headerH + 10);
        int maxWidth = GameConfig::WINDOW_WIDTH - 120;
        int maxLines = 20;
        int drawStart = 0;
        if (!withdraw) {
            psi = clampPageStart(psi, static_cast<int>(longArr.size()), maxLines);
            drawStart = psi;
        }
        for (int lineIndex = 0; lineIndex < maxLines; lineIndex++) {
            int i = drawStart + lineIndex;
            if (i < 0 || i >= static_cast<int>(longArr.size())) {
                break;
            }
            drawWrappedText(longArr[i], x, y + lineIndex * GameConfig::MENU_OPTION_HEIGHT, maxWidth);
        }
    } else if (battling) {
        bool hasBottomUi = false;
        for (int i = 0; i < 4; i++) {
            if (!strArr[i].empty()) {
                hasBottomUi = true;
                break;
            }
        }

        if (!hasBottomUi) {
            drawTextBox("");
        } else {
            drawTextBox(strArr[0]);
            int boxHeight = GameConfig::MENU_BOX_HEIGHT;
            int boxY = GameConfig::WINDOW_HEIGHT - boxHeight - GameConfig::MENU_BOX_MARGIN;
            int startY = boxY + GameConfig::MENU_OPTION_START_OFFSET;
            drawWrappedText(strArr[1], 60, startY + GameConfig::MENU_OPTION_HEIGHT * 1, GameConfig::WINDOW_WIDTH - 120);
            drawWrappedText(strArr[2], 60, startY + GameConfig::MENU_OPTION_HEIGHT * 2, GameConfig::WINDOW_WIDTH - 120);
            drawWrappedText(strArr[3], 60, startY + GameConfig::MENU_OPTION_HEIGHT * 3, GameConfig::WINDOW_WIDTH - 120);
        }
    } else if (pickingStarter) {
        drawTextBox("Which starter will you choose?");
        int y = GameConfig::WORLD_HEIGHT + std::max(0, (GameConfig::UI_HEIGHT - 256) / 2);
        SDL_Texture *b1 = pBattlers.size() > 1 ? pBattlers[1] : nullptr;
        SDL_Texture *b4 = pBattlers.size() > 4 ? pBattlers[4] : nullptr;
        SDL_Texture *b7 = pBattlers.size() > 7 ? pBattlers[7] : nullptr;
        if (b1) {
            SDL_Rect dst{-20, y, 256, 256};
            SDL_RenderCopy(renderer, b1, nullptr, &dst);
        }
        if (b4) {
            SDL_Rect dst{GameConfig::WINDOW_WIDTH / 2 - 128, y, 256, 256};
            SDL_RenderCopy(renderer, b4, nullptr, &dst);
        }
        if (b7) {
            SDL_Rect dst{GameConfig::WINDOW_WIDTH - 256 + 20, y, 256, 256};
            SDL_RenderCopy(renderer, b7, nullptr, &dst);
        }
    } else if (flying) {
    } else if (inMenu) {
        if (choosingFromLongList) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Rect bg{0, GameConfig::WORLD_HEIGHT, GameConfig::WINDOW_WIDTH, GameConfig::UI_HEIGHT};
            SDL_RenderFillRect(renderer, &bg);
        }
        if (choosingFromLongList) {
            int x = 60;
            int y = GameConfig::WORLD_HEIGHT + GameConfig::LONG_LIST_START_Y;
            int maxWidth = GameConfig::WINDOW_WIDTH - 120;
            int maxLines = 20;
            psi = clampPageStart(psi, static_cast<int>(longArr.size()), maxLines);
            for (int lineIndex = 0; lineIndex < maxLines; lineIndex++) {
                int i = psi + lineIndex;
                if (i < 0 || i >= static_cast<int>(longArr.size())) {
                    break;
                }
                drawWrappedText(longArr[i], x, y + lineIndex * GameConfig::MENU_OPTION_HEIGHT, maxWidth);
            }
        } else {
            drawTextBox(strArr[0]);
            int boxHeight = GameConfig::MENU_BOX_HEIGHT;
            int boxY = GameConfig::WINDOW_HEIGHT - boxHeight - GameConfig::MENU_BOX_MARGIN;
            int startY = boxY + GameConfig::MENU_OPTION_START_OFFSET;
            drawWrappedText(strArr[1], 60, startY + GameConfig::MENU_OPTION_HEIGHT * 1, GameConfig::WINDOW_WIDTH - 120);
            drawWrappedText(strArr[2], 60, startY + GameConfig::MENU_OPTION_HEIGHT * 2, GameConfig::WINDOW_WIDTH - 120);
            drawWrappedText(strArr[3], 60, startY + GameConfig::MENU_OPTION_HEIGHT * 3, GameConfig::WINDOW_WIDTH - 120);
        }
    }
    SDL_RenderPresent(renderer);
}

void OverworldGui::drawTextBox(const std::string &text) {
    int boxWidth = GameConfig::WINDOW_WIDTH - 80;
    int boxHeight = GameConfig::MENU_BOX_HEIGHT;
    int x = 40;
    int y = GameConfig::WINDOW_HEIGHT - boxHeight - GameConfig::MENU_BOX_MARGIN;
    SDL_Rect rect{x, y, boxWidth, boxHeight};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
    drawWrappedText(text, x + 20, y + 20, boxWidth - 40);
}

void OverworldGui::drawWrappedText(const std::string &text, int x, int y, int maxWidth) {
    SDL_Color color{0, 0, 0, 255};
    SDL_Texture *tex = TextRenderer::renderText(renderer, text, color, maxWidth);
    if (!tex) {
        return;
    }
    int w = 0;
    int h = 0;
    SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
    SDL_Rect dst{x, y, w, h};
    SDL_RenderCopy(renderer, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
}

void OverworldGui::drawConnection(int dirIndex, PokeMap *conn, int offset, int camX, int camY) {
    int baseX = 0;
    int baseY = 0;
    switch (dirIndex) {
        case 0:
            baseX = offset * GameConfig::TILE_SIZE;
            baseY = -static_cast<int>(conn->grid.size()) * GameConfig::TILE_SIZE;
            break;
        case 1:
            baseX = offset * GameConfig::TILE_SIZE;
            baseY = static_cast<int>(currentMap.size()) * GameConfig::TILE_SIZE;
            break;
        case 2:
            baseX = -static_cast<int>(conn->grid[0].size()) * GameConfig::TILE_SIZE;
            baseY = offset * GameConfig::TILE_SIZE;
            break;
        case 3:
            baseX = static_cast<int>(currentMap[0].size()) * GameConfig::TILE_SIZE;
            baseY = offset * GameConfig::TILE_SIZE;
            break;
    }
    int px = baseX - camX;
    int py = baseY - camY;

    auto floorDiv = [](int a, int b) {
        int q = a / b;
        int r = a % b;
        if (r != 0 && ((r < 0) != (b < 0))) {
            q--;
        }
        return q;
    };

    int tilesW = GameConfig::WINDOW_WIDTH / GameConfig::TILE_SIZE + 3;
    int tilesH = GameConfig::WORLD_HEIGHT / GameConfig::TILE_SIZE + 3;

    int sx = floorDiv(camX - baseX, GameConfig::TILE_SIZE) - 1;
    int sy = floorDiv(camY - baseY, GameConfig::TILE_SIZE) - 1;
    int ex = std::min(sx + tilesW, static_cast<int>(conn->grid[0].size()));
    int ey = std::min(sy + tilesH, static_cast<int>(conn->grid.size()));
    sx = std::max(sx, 0);
    sy = std::max(sy, 0);
    for (int y = sy; y < ey; y++) {
        for (int x = sx; x < ex; x++) {
            SDL_Texture *tile = tileImages[conn->grid[y][x]];
            SDL_Rect dst{x * GameConfig::TILE_SIZE + px, y * GameConfig::TILE_SIZE + py, GameConfig::TILE_SIZE, GameConfig::TILE_SIZE};
            if (tile) {
                SDL_RenderCopy(renderer, tile, nullptr, &dst);
            }
            Trainer *t = conn->trainers[y][x];
            if (t) {
                SDL_RenderCopy(renderer, t->bi, nullptr, &dst);
            } else if (conn->wob[y][x]) {
                SDL_RenderCopy(renderer, conn->wob[y][x]->bi, nullptr, &dst);
            } else if (conn->npcs[y][x]) {
                SDL_RenderCopy(renderer, conn->npcs[y][x]->bi, nullptr, &dst);
            }
        }
    }
}

int OverworldGui::getCurrentFrame() const {
    bool moving = stepPhase == StepPhase::MOVING || (stepPhase == StepPhase::LANDING && phaseFrame < currentStepFrames / 2);
    switch (facing) {
        case Direction::SOUTH:
            return moving ? 0 + timesMoved : 1;
        case Direction::NORTH:
            return moving ? 3 + timesMoved : 4;
        case Direction::WEST:
            return moving ? 7 : 6;
        case Direction::EAST:
            return moving ? 9 : 8;
    }
    return 1;
}

bool OverworldGui::canMove(Direction d) {
    if (switchingMaps) {
        return true;
    }
    if (!currentMap.empty() && !currentMap[0].empty() && playerX == static_cast<int>(currentMap[0].size())) {
        print("You beat the game! Now fly out of here and save... and maybe talk to the professor, too!");
        playerX = 1;
        startFlying();
        return true;
    }
    int nx = playerX;
    int ny = playerY;
    switch (d) {
        case Direction::SOUTH:
            ny++;
            break;
        case Direction::NORTH:
            ny--;
            break;
        case Direction::WEST:
            nx--;
            break;
        case Direction::EAST:
            nx++;
            break;
    }
    if (nx < 0 || ny < 0 || ny >= static_cast<int>(currentMap.size()) || nx >= static_cast<int>(currentMap[0].size())) {
        return false;
    }
    if (pm && pm->npcs[ny][nx] != nullptr) {
        Blocker *b = dynamic_cast<Blocker *>(pm->npcs[ny][nx]);
        if (b) {
            if ((b->move != nullptr && !player->hasMove(b->move)) || (b->numBadges >= 0 && !player->leadersBeaten[b->numBadges]) ||
                (b->item != nullptr && !player->hasItem(b->item))) {
                b->interact(player);
                return false;
            }
        }
    }
    switch (tileTypes[playerY][playerX]) {
        case 5:
            return d == Direction::SOUTH;
        case 9:
            return d != Direction::NORTH && tileTypes[ny][nx] != 0;
        case 10:
            return d == Direction::WEST;
        case 11:
            return d == Direction::EAST;
    }
    switch (tileTypes[ny][nx]) {
        case 0:
            return false;
        case 3:
            print(printable(pm->name) + "... I think? Man, I wish I could read!");
            return false;
        case 4:
            return player->leadersBeaten[4] && player->hasMove(SURF);
        case 5:
            return d == Direction::SOUTH;
        case 6:
            return player->leadersBeaten[1] && player->hasMove(CUT);
        case 9:
            return d != Direction::SOUTH;
        case 10:
            return d == Direction::WEST;
        case 11:
            return d == Direction::EAST;
        case 19:
            return player->leadersBeaten[3] && player->hasMove(STRENGTH);
        case 20:
            print("Ew, that stinks!");
            if (utils::rand01() < 0.1) {
                if (utils::rand01() < 0.01) {
                    print("Whoay! Who would throw this away?!");
                    player->give(Item::ITEM_MAP["Rare Candy"]);
                } else {
                    print("Oh no, I'll have to wash my hands...");
                    player->give(Item::ITEM_MAP["...Secret Sauce"]);
                }
            }
            return false;
    }
    return true;
}

std::string OverworldGui::printable(const std::string &s) {
    std::string out;
    out.reserve(s.size() + 8);
    for (size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        if (i > 0) {
            char prev = s[i - 1];
            if (std::isdigit(static_cast<unsigned char>(c)) && std::isalpha(static_cast<unsigned char>(prev))) {
                out.push_back(' ');
            } else if (std::isupper(static_cast<unsigned char>(c)) &&
                       std::islower(static_cast<unsigned char>(prev))) {
                out.push_back(' ');
            }
        }
        out.push_back(c);
    }
    return out;
}

int OverworldGui::elevate(const std::string &s, bool B, int m) {
    std::string prompt = (pm && pm->name == "ViridianBunglerHouse") ? "..." : "Enter the floor you would like to go to:";
    stopMovement();

    if (B && player && LIFT_KEY && !player->hasItem(LIFT_KEY)) {
        print("Weird, nothing is happening.");
        return 0;
    }

    std::string input = promptText(prompt);
    if (input.empty()) {
        return 0;
    }
    if (pm && pm->name == "ViridianBunglerHouse") {
        if (input == "DEATH" && player && player->pokedex.size() > 150 && player->pokedex[150]) {
            loadMap(PokeMap::POKEMAPS["RocketHideoutB1F"]);
            return 1;
        }
        return 0;
    }
    std::string u = input;
    std::transform(u.begin(), u.end(), u.begin(), ::toupper);
    if (B && !u.empty() && u[0] == 'B') {
        u = u.substr(1);
    }
    if (!u.empty() && u.back() == 'F') {
        u.pop_back();
    }
    int q = 0;
    try {
        q = std::stoi(u);
    } catch (...) {
        print(input + ", huh? Yeah, okay buddy.");
        return 0;
    }
    if (q <= 0 || q > m) {
        print("Uhh, you know there are only " + std::to_string(m) + " floors, right?");
        return 0;
    }
    if (B && q == 3) {
        print("...no.");
        return 0;
    }
    loadMap(PokeMap::POKEMAPS[s + (B ? "B" + std::to_string(q) : std::to_string(q)) + "F"]);
    print("Wee!");
    return q;
}

char OverworldGui::randomSpooky() {
    return static_cast<char>(32 + utils::randInt(0, 95));
}

void OverworldGui::printSpooky() {
    std::string s;
    for (int i = 0; i < 10; i++) {
        s.push_back(randomSpooky());
    }
    s += "DEATH";
    for (int i = 0; i < 10; i++) {
        s.push_back(randomSpooky());
    }
    print(s);
}

void OverworldGui::advanceStep() {
    float delta = static_cast<float>(GameConfig::TILE_SIZE) / static_cast<float>(currentStepFrames);
    int dx = 0;
    int dy = 0;
    PokeMap *newMap = nullptr;
    switch (facing) {
        case Direction::SOUTH:
            dy = 1;
            if (playerY == static_cast<int>(currentMap.size()) - 1 && connections[1] != nullptr) {
                newMap = connections[1];
                playerX -= connOffsets[1];
                playerY = -1;
            }
            break;
        case Direction::NORTH:
            dy = -1;
            if (playerY == 0 && connections[0] != nullptr) {
                newMap = connections[0];
                playerX -= connOffsets[0];
                playerY = static_cast<int>(newMap->grid.size());
            }
            break;
        case Direction::WEST:
            dx = -1;
            if (playerX == 0 && connections[2] != nullptr) {
                newMap = connections[2];
                playerX = static_cast<int>(newMap->grid[0].size());
                playerY -= connOffsets[2];
            }
            break;
        case Direction::EAST:
            dx = 1;
            if (playerX == static_cast<int>(currentMap[0].size()) - 1 && connections[3] != nullptr) {
                newMap = connections[3];
                playerX = -1;
                playerY -= connOffsets[3];
            }
            break;
    }
    if (newMap != nullptr) {
        mapName = newMap->name;
        loadMap(newMap);
        switchingMaps = true;
        currentStepFrames /= 2;
    }
    bool isBump = currentStepFrames == BUMP_STEP_FRAMES;
    int nextX = playerX + dx;
    int nextY = playerY + dy;
    if (isBump) {
        bool b = nextX >= 0 && nextY >= 0 && nextY < static_cast<int>(currentMap.size()) && nextX < static_cast<int>(currentMap[0].size());

        if (b && (pm->grid[nextY][nextX] == 582 || pm->grid[nextY][nextX] == 583)) {
            stopMovement();
            phaseFrame = currentStepFrames;
            if (player && player->team[0]) {
                int r = utils::randInt(1, 100);
                print("Honestly, this one doesn't really look like " + player->team[0]->nickname + ", although it says it's supposed to be... " + Monster::MONSTERS[r].name + "?");
            } else {
                print("Honestly, this one doesn't really look like that pokemon...");
            }
            return;
        }

        if (tileTypes[playerY][playerX] == 2 || tileTypes[playerY][playerX] == 18) {
            Warp *w = pm->getWarp(playerY, playerX);
            if (w == nullptr) {
                w = pm->getNearbyWarp(playerY, playerX);
                if (w == nullptr) {
                    inMenu = true;
                    int n = elevate("RocketHideout", true, 4);
                    if (n > 0) {
                        playerY = HIDEOUT_Y[n];
                    } else {
                        playerY--;
                    }
                    inMenu = false;
                    return;
                }
            }
            stepPhase = StepPhase::NONE;
            loadMap(w->pm);
            playerX = w->col;
            playerY = w->row;
            offsetX = 0;
            offsetY = 0;
            currentStepFrames /= 2;
            return;
        }

        if (pm->healX == playerX && pm->healY == playerY) {
            stopMovement();
            player->healTeam();
            phaseFrame = currentStepFrames;
            lastHeal = pm;
            FlyLocation::visit(pm->name);
            print("Your team was fully healed!");
            return;
        }

        if (b) {
            WorldObject *wo = pm->wob[nextY][nextX];
            if (wo) {
                stopMovement();
                std::optional<bool> res = wo->stepOn(this);
                if (res.has_value() && res.value()) {
                    pm->stepOn(player, nextX, nextY);
                } else if (res.has_value() && !res.value()) {
                    playerX -= dx;
                    playerY -= dy;
                    offsetX = 0;
                    offsetY = 0;
                }
                phaseFrame = currentStepFrames;
                return;
            }
            if (pm->npcs[nextY][nextX] != nullptr) {
                stopMovement();
                phaseFrame = currentStepFrames;
                bool oldCanMap = canMap;
                canMap = false;
                pm->npcs[nextY][nextX]->interact(player);

                if (pm && pm->name == "Daycare" && player && !player->team.empty() && player->team[0]) {
                    int p = 5000;
                    int d = player->team[0]->dexNum;
                    if (d == 0) {
                        p = 50000;
                    } else if (d > 132) {
                        p = 25000;
                    }
                    if (p > player->money) {
                        print("Wait, you're sooo poor! You need at least $" + std::to_string(p) +
                              " for this child, I guess I'm keeping this one...");
                    } else {
                        player->money -= p;
                        print("You spent $" + std::to_string(p) + "...");
                        player->give(new Battler(player->team[0]));
                    }
                }

                canMap = oldCanMap;
                return;
            }
            if (pm->grid[playerY][playerX] == 122 && pm->name == "GameCorner") {
                stopMovement();
                phaseFrame = currentStepFrames;
                if (player->hasItem(FAKE_ID)) {
                    if (player->money < bet) {
                        print("No! I want to bet $" + std::to_string(bet) + " if I play, but I'm broke!");
                    } else {
                        static std::mt19937 s_blackjackRng(std::random_device{}());

                        blackjackActive = true;
                        blackjackBet = bet;
                        blackjackFinished = false;
                        blackjackTieFrames = 0;
                        blackjackHideFirst = true;

                        blackjackDeck.clear();
                        blackjackDeck.reserve(52);
                        for (int i = 0; i < 52; i++) {
                            blackjackDeck.push_back(i);
                        }
                        std::shuffle(blackjackDeck.begin(), blackjackDeck.end(), s_blackjackRng);
                        auto drawCard = [&]() {
                            int c = blackjackDeck.back();
                            blackjackDeck.pop_back();
                            return c;
                        };

                        blackjackPlayer.clear();
                        blackjackDealer.clear();
                        blackjackPlayer.push_back(drawCard());
                        blackjackPlayer.push_back(drawCard());
                        blackjackDealer.push_back(drawCard());
                        blackjackDealer.push_back(drawCard());

                        auto cardValue = [](int c) {
                            int r = c % 13;
                            if (r == 0) {
                                return 11;
                            }
                            if (r >= 9) {
                                return 10;
                            }
                            return r + 1;
                        };
                        auto handValue = [&](const std::vector<int> &hand) {
                            int sum = 0;
                            int aces = 0;
                            for (int c : hand) {
                                sum += cardValue(c);
                                if (c % 13 == 0) {
                                    aces++;
                                }
                            }
                            while (sum > 21 && aces-- > 0) {
                                sum -= 10;
                            }
                            return sum;
                        };

                        if (handValue(blackjackPlayer) == 21) {
                            blackjackHideFirst = false;
                            blackjackFinished = true;
                            blackjackPlayerWon = true;
                            player->money += blackjackBet;
                            blackjackActive = false;
                            blackjackBet = 0;
                            bet *= 2;
                            print("You won $" + std::to_string(bet / 2) + "!");
                        }
                    }
                } else {
                    print("Crap, it wants me to scan an ID!!");
                }
                return;
            }
            switch (pm->grid[nextY][nextX]) {
                case 143:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Hey baby, come sit on my lap!");
                    print("(Eek, I'd better get out of here!)");
                    heldDirection = Direction::EAST;
                    facing = Direction::EAST;
                    hasHeldDirection = true;
                    return;
                case 35:
                case 220:
                case 388:
                case 420:
                    stopMovement();
                    inMenu = true;
                    depWith = true;
                    strArr[0] = "Deposit";
                    strArr[1] = "Withdraw";
                    strArr[2].clear();
                    strArr[3].clear();
                    phaseFrame = currentStepFrames;
                    return;
                case 51:
                case 52:
                case 65:
                case 66:
                case 139:
                case 141:
                case 326:
                case 329:
                case 534:
                case 535:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("No " + player->name + ", you told yourself you'd stop eating plants!");
                    return;
                case 3:
                case 4:
                case 56:
                case 57:
                case 317:
                case 328:
                case 435:
                case 438:
                case 496:
                case 668:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    if (player && !player->team.empty() && player->team[0]) {
                        print("Hot dog, that looks just like " + player->team[0]->nickname + "!");
                    } else {
                        print("Hot dog, that looks familiar!");
                    }
                    return;
                case 189:
                case 190:
                case 192:
                case 193:
                case 279:
                case 280:
                case 281:
                case 282:
                case 284:
                case 285:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Gah, I can never find what I need! May as well speak to a worker...");
                    return;
                case 2:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Here lies... huh? I wish I could read!");
                    return;
                case 42:
                case 378:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Golly gee, I could stand here all day looking at this view!");
                    return;
                case 44:
                case 45:
                case 61:
                case 161:
                case 350:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Books, huh? Boooring!");
                    return;
                case 9:
                case 13:
                case 14:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Whoa, this one's a beauty! But I'll never afford it.");
                    return;
                case 20:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Aw great, now my hands are covered in oil! I hate getting lubed up.");
                    return;
                case 36:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("How the heck do I work this crazy thing?!");
                    return;
                case 31:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Nope, DEFINITELY not going in there!");
                    return;
                case 41:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Gasp! Who would hang this in their house?!");
                    return;
                case 62:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("A trophy for comedy! How pathetic.");
                    return;
                case 177:
                case 178:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    heldDirection = Direction::SOUTH;
                    hasHeldDirection = true;
                    print("NO, I HATE SCHOOL!");
                    return;
                case 179:
                case 180:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    hasHeldDirection = false;
                    print("Nope, " + player->name + " is NOT a nerd!");
                    return;
                case 185:
                case 188:
                case 333:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Hehehe look at all those squiggles... I wonder what they mean!");
                    return;
                case 194:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Limited time sale on EVERYTHING?! I'd better act quick!");
                    return;
                case 226:
                case 614:
                case 633:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("It'd probably be rude to drink this... slurp!");
                    return;
                case 302:
                case 303:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Well, I suppose a nibble couldn't hurt... CRUNCH!");
                    return;
                case 314:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Wait, is there somebody INSIDE there? I'd better keep my distance...");
                    return;
                case 347:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Blah blah words blah, LAME!");
                    return;
                case 391:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("A bachelor's degree in... computer science? Well that's worthless!");
                    return;
                case 206:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Are those... pokeballs? Maybe I should just snatch one... eh, seems risky!");
                    return;
                case 455:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("This must be what the region looked like thousands of years ago, neat!");
                    return;
                case 464:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("What? It just says *ROCKS*, no duh!!");
                    return;
                case 575:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Hehe, ohhh yeah, that's the good stuff!");
                    return;
                case 627:
                case 629:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("It says here the ship is... sinking?! I'd better get out of here!");
                    return;
                case 625:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Yikes, this thing could tip over at any second!");
                    return;
                case 631:
                case 632:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Is that... slowpoke tail? My my, how SCRUMPTIOUS indeed!");
                    return;
                case 164:
                case 165:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("No time to sleep, I've got pokemon to catch!");
                    return;
                case 195:
                case 379:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Whoa, I don't think I'm old enough to look at this...");
                    return;
                case 196:
                case 390:
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Crud, he hit me with Rat Gambit again! I suck at this game.");
                    return;
                default:
                    break;
            }

            int martSize = 0;
            MartItem *mi = pm->getMartItems(playerX, playerY, martSize);
            if (mi == nullptr) {
                if (pm->grid[nextY][nextX] == 187) {
                    stopMovement();
                    phaseFrame = currentStepFrames;
                    print("Hm, better not touch that...");
                    return;
                }
            } else {
                stopMovement();
                martItems.clear();
                for (int i = 0; i < martSize; i++) {
                    martItems.push_back(mi[i]);
                }
                phaseFrame = currentStepFrames;
                canMap = false;
                print("Welcome to the mart!");
                buySell = true;
                inMenu = true;
                choosingFromLongList = false;
                psi = 0;
                strArr[0] = "Buy";
                strArr[1] = "Sell";
                strArr[2].clear();
                strArr[3].clear();
                return;
            }
        }
    } else if (isBump) {
        float t = static_cast<float>(phaseFrame) / static_cast<float>(std::max(1, currentStepFrames - 1));
        float push = (t < 0.5f) ? (t * 2.0f) : ((1.0f - t) * 2.0f);
        float bumpDelta = delta * 0.35f;
        offsetX += dx * bumpDelta * push;
        offsetY += dy * bumpDelta * push;
    } else if (canMove(facing)) {
        offsetX += dx * delta;
        offsetY += dy * delta;
    }
    phaseFrame++;
    if (phaseFrame >= currentStepFrames) {
        switchingMaps = false;
        phaseFrame = 0;
        stepPhase = StepPhase::NONE;
        offsetX = 0;
        offsetY = 0;
        timesMoved = timesMoved == 0 ? 2 : 0;
        if (!isBump) {
            int prevType = -1;
            if (playerX >= 0 && playerY >= 0 && playerY < static_cast<int>(tileTypes.size()) &&
                playerX < static_cast<int>(tileTypes[0].size())) {
                prevType = tileTypes[playerY][playerX];
            }
            playerX = nextX;
            playerY = nextY;
            if (playerX < 0 || playerY < 0 || playerY >= static_cast<int>(currentMap.size()) ||
                playerX >= static_cast<int>(currentMap[0].size())) {
                return;
            }

            auto startForcedStep = [&](Direction d) {
                if (!canMove(d)) {
                    return;
                }
                facing = d;
                currentStepFrames = NUM_STEP_FRAMES;
                stepPhase = StepPhase::MOVING;
                phaseFrame = 0;
            };

            int curType = tileTypes[playerY][playerX];
            if (pm && player && curType == 13 && prevType != 13 && pm->name.rfind("PokemonTower", 0) == 0) {
                player->healTeam();
                print("You feel power flow through you. Your pokemon have been healed!");
            }
            if (curType == 4) {
                surfing = true;
            } else if (curType == 1 || curType == 8 || curType == 12) {
                surfing = false;
            }

            if (curType == 5) {
                startForcedStep(Direction::SOUTH);
                return;
            }
            if (curType == 10) {
                startForcedStep(Direction::WEST);
                return;
            }
            if (curType == 11) {
                startForcedStep(Direction::EAST);
                return;
            }

            if (curType == 14) {
                spinning = true;
                startForcedStep(Direction::WEST);
                return;
            }
            if (curType == 15) {
                spinning = true;
                startForcedStep(Direction::EAST);
                return;
            }
            if (curType == 16) {
                spinning = true;
                startForcedStep(Direction::NORTH);
                return;
            }
            if (curType == 17) {
                spinning = true;
                startForcedStep(Direction::SOUTH);
                return;
            }

            if (spinning && pm && pm->grid[playerY][playerX] == 545) {
                spinning = false;
            }

            if (spinning) {
                startForcedStep(facing);
                return;
            }

            if (curType == 7) {
                if (spinning) {
                    spinning = false;
                }
                if (pm->grid[playerY][playerX] == 182) {
                    inMenu = true;
                    elevate("CeladonMart", false, 5);
                    inMenu = false;
                    return;
                }
                if (pm->grid[playerY][playerX] == 697) {
                    inMenu = true;
                    int f = elevate("SilphCo", false, 11);
                    if (f > 0) {
                        playerX = SILPH_X[f];
                    }
                    inMenu = false;
                    return;
                }
                Warp *w = pm->getWarp(playerY, playerX);
                if (w) {
                    PokeMap *from = pm;
                    loadMap(w->pm);
                    playerX = w->col;
                    playerY = w->row;
                    offsetX = 0;
                    offsetY = 0;
                    if (playerX == 0 && playerY == 6 && from && from->name == "RedsHouse2F") {
                        player->ballin = true;
                        Trainer::addEliteFour();
                        print("What? How did I get back here??");
                    }
                    return;
                }
            }
            if (pm->healX == playerX && pm->healY == playerY) {
                if (spinning) {
                    spinning = false;
                }
                player->healTeam();
                lastHeal = pm;
                FlyLocation::visit(pm->name);
                print("Your team was fully healed!");
            }
            WorldObject *wo = pm->wob[playerY][playerX];
            if (wo) {
                if (spinning) {
                    spinning = false;
                }
                std::optional<bool> res = wo->stepOn(this);
                if (!res.has_value()) {
                    wildMon = new Battler(wo->level, wo->mon);
                    bool oldCanMap = canMap;
                    canMap = false;
                    int result = BattleState::wildBattle(player->team, wildMon, this);
                    if (result < 0) {
                        blackout();
                    } else {
                        player->money += result * (player->hasItem(AMULET_COIN) ? 2 : 1);
                        pm->stepOn(player, playerX, playerY);
                    }
                    battling = false;
                    wildMon = nullptr;
                    canMap = oldCanMap;
                    offsetX = 0;
                    offsetY = 0;
                    return;
                }
                if (res.value()) {
                    pm->stepOn(player, playerX, playerY);
                } else {
                    playerX -= dx;
                    playerY -= dy;
                    offsetX = 0;
                    offsetY = 0;
                }
            }
            Npc *n = pm->npcs[playerY][playerX];
            if (n) {
                if (spinning) {
                    spinning = false;
                }
                stopMovement();
                bool oldCanMap = canMap;
                canMap = false;
                n->interact(player);

                if (pm && pm->name == "Daycare" && player && !player->team.empty() && player->team[0]) {
                    int p = 5000;
                    int d = player->team[0]->dexNum;
                    if (d == 0) {
                        p = 50000;
                    } else if (d > 132) {
                        p = 25000;
                    }
                    if (p > player->money) {
                        print("Wait, you're sooo poor! You need at least $" + std::to_string(p) +
                              " for this child, I guess I'm keeping this one...");
                    } else {
                        player->money -= p;
                        print("You spent $" + std::to_string(p) + "...");
                        player->give(new Battler(player->team[0]));
                    }
                }

                canMap = oldCanMap;

                if (n->dead) {
                    pm->npcs[playerY][playerX] = nullptr;
                } else {
                    playerX -= dx;
                    playerY -= dy;
                }
                offsetX = 0;
                offsetY = 0;
                return;
            }
            if (pm->sight[playerY][playerX] != 0) {
                if (spinning) {
                    spinning = false;
                }
                Trainer *t = pm->getTrainer(playerX, playerY, pm->sight[playerY][playerX]);
                if (t) {
                    //fixes map display bug
                    bool oldCanMap = canMap;
                    canMap = false;
                    print(t->phrases[0]);
                    battling = true;
                    wildMon = nullptr;
                    int result = BattleState::trainerBattle(player->team, t->type, t->team, this);
                    battling = false;
                    if (result < 0) {
                        blackout();
                        for (Battler *b : t->team) {
                            b->fullyHeal();
                        }
                    } else {
                        int r = (result + t->reward) * (player->hasItem(AMULET_COIN) ? 2 : 1);
                        player->money += r;
                        print("You got $" + std::to_string(r) + " for winning!");
                        print(t->phrases[1]);
                        t->beat(player);
                        pm->deleteTrainer(t, pm->sight[playerY][playerX]);
                        Trainer::E4Trainer *e4 = dynamic_cast<Trainer::E4Trainer*>(t);
                        if (e4 && e4->id == 8) {
                            Trainer::addEliteFour();
                        }
                    }
                    offsetX = 0;
                    offsetY = 0;
                    canMap = oldCanMap;
                    wildMon = nullptr;
                    return;
                }
            }
            if (--repelSteps == 0) {
                print("Your repel ran out!");
            } else {
                repelSteps = std::max(repelSteps, 0);
            }
            int type = tileTypes[playerY][playerX];
            if (type == 12 || type == 8 || type == 4) {
                if (utils::rand01() < 0.1) {
                    std::string encounterType = type == 12 ? "Tall Grass" : (type == 8 ? "Cave" : "Surfing");
                    bool oldCanMap = canMap;
                    canMap = false;
                    wildMon = pm->getRandomEncounter(encounterType);
                    if (wildMon && (repelSteps == 0 || (player->team[0] && wildMon->level >= player->team[0]->level))) {
                        int result = BattleState::wildBattle(player->team, wildMon, this);
                        if (result < 0) {
                            blackout();
                        } else {
                            player->money += result * (player->hasItem(AMULET_COIN) ? 2 : 1);
                        }
                        battling = false;
                        wildMon = nullptr;
                        offsetX = 0;
                        offsetY = 0;
                        canMap = oldCanMap;
                        return;
                    }
                    canMap = oldCanMap;
                }
            }
        }
        offsetX = 0;
        offsetY = 0;
    }
}

void OverworldGui::save() {
    if (!pm) {
        return;
    }
    if (inMenu || battling || choosingFromLongList || buySell) {
        return;
    }
    if (pm->name == "RocketHideoutB1F" && playerX == 3) {
        printSpooky();
        printSpooky();
        printSpooky();
        printSpooky();
        print("You will not be saved.");
        return;
    }
    if (pm->name == "ChampionsRoom" || pm->name == "LancesRoom" || pm->name == "AgathasRoom" || pm->name == "BrunosRoom" ||
        pm->name == "LoreleisRoom") {
        printSpooky();
        print("You will not be saved.");
        return;
    }
    std::string out;
    out += pm->name + "," + std::to_string(playerX) + "," + std::to_string(playerY) + "," +
           std::to_string(static_cast<int>(facing)) + "," + std::to_string(repelSteps) + "," + lastHeal->name + "," +
           (player->ballin ? "1" : "0") + "," + std::to_string(player->numCaught) + "," + std::to_string(player->money) +
           "," + player->name + "\n";
    if (!player->pc.empty()) {
        player->pc[0]->append(out);
        for (size_t i = 1; i < player->pc.size(); i++) {
            out.push_back(';');
            player->pc[i]->append(out);
        }
    }
    out.push_back('\n');
    if (player->team[0]) {
        player->team[0]->append(out);
        for (size_t i = 1; i < player->team.size(); i++) {
            if (!player->team[i]) {
                break;
            }
            out.push_back(';');
            player->team[i]->append(out);
        }
    }
    auto appendBoolLine = [&](const std::vector<bool> &v) {
        out.push_back('\n');
        for (bool b : v) {
            out.push_back(b ? '1' : '0');
        }
    };
    appendBoolLine(player->trainersBeaten);
    appendBoolLine(player->leadersBeaten);
    appendBoolLine(player->gioRivalsBeaten);
    appendBoolLine(player->objectsCollected);
    appendBoolLine(player->pokedex);
    out.push_back('\n');
    if (!player->items.empty()) {
        Item *i = player->items[0];
        out += i->name + "," + std::to_string(i->quantity);
        for (size_t j = 1; j < player->items.size(); j++) {
            i = player->items[j];
            out += ";" + i->name + "," + std::to_string(i->quantity);
        }
    }
    out.push_back('\n');
    for (Move *m : player->tmHms) {
        out += "," + m->name;
    }
    out.push_back('\n');
    for (size_t i = 1; i < 14 && i < FlyLocation::INDEX_MEANINGS.size(); i++) {
        auto it = FlyLocation::FLY_LOCATIONS.find(FlyLocation::INDEX_MEANINGS[i]);
        out.push_back(it != FlyLocation::FLY_LOCATIONS.end() && it->second->visited ? '1' : '0');
    }
    std::ofstream file("save.txt", std::ios::binary);
    file << out;
    file.close();
    print("Saved!");
}

void OverworldGui::startFlying() {
    if (!player || !player->hasMove(FLY) || player->leadersBeaten.size() <= 2 || !player->leadersBeaten[2]) {
        print("You can't fly right now!");
        return;
    }
    flying = true;
    inMenu = true;
    currentLoc = printable(pm->name);
    inside = true;
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            if (pm->name == FlyLocation::NAME_MAP[i][j]) {
                inside = false;
                return;
            }
        }
    }
}

void OverworldGui::useRevive(Battler *b, int n) {
    if (b->hp > 0) {
        print("Quit messing around!");
        notifyUseItem(false);
    } else {
        b->hp += n;
        print(b->nickname + "'s health was restored by " + std::to_string(n) + " points!");
        notifyUseItem(true);
    }
}

void OverworldGui::healHp(Battler *b, int n) {
    if (b->hp == 0) {
        print("No, you'll need something stronger to heal this one...");
        notifyUseItem(false);
    } else if (b->hp == b->mhp) {
        print("Quit messing around!");
        notifyUseItem(false);
    } else {
        b->hp = std::min(b->hp + n, b->mhp);
        print(b->nickname + "'s health was restored by " + std::to_string(n) + " points!");
        notifyUseItem(true);
    }
}

void OverworldGui::useElixir(Battler *b, int n) {
    bool used = false;
    for (size_t i = 0; i < b->moves.size(); i++) {
        if (b->moves[i] == nullptr) {
            break;
        }
        if (b->pp[i] < b->mpp[i]) {
            b->pp[i] = std::min(b->pp[i] + n, b->mpp[i]);
            used = true;
        }
    }
    if (used) {
        print(b->nickname + "'s PP was restored!");
        notifyUseItem(true);
    } else {
        print("DO NOT WASTE THAT!");
        notifyUseItem(false);
    }
}

void OverworldGui::healStatus(Battler *b, const std::string &s) {
    if (b->status == s) {
        if (playerState && playerState->monster == b) {
            playerState->poisonDamage = 0;
        }
        b->status.clear();
        print(b->nickname + "'s status was healed!");
        notifyUseItem(true);
    } else {
        print("That wouldn't do anything!");
        notifyUseItem(false);
    }
}

void OverworldGui::useRepel(int steps) {
    repelSteps = std::max(repelSteps, 0) + steps;
    player->use(usedItem);
    usedItem = nullptr;
    longArr.clear();
    usableItems.clear();
    choosingFromLongList = false;
    usingItemList = false;
    usingBattleItem = false;
    busedItem = false;
    rightClicked = false;
    clickedChoice = -1;
    psi = 0;
    closeMenus();
    print("You gained " + std::to_string(steps - 1) + " repel steps!");
}

void OverworldGui::catchMon(double bm) {
    if (!wildMon) {
        print("This can't be used now.");
        notifyUseItem(false);
        return;
    }
    int thp = 3 * wildMon->mhp;
    int sb = 0;
    if (wildMon->status == "ASLEEP" || wildMon->status == "FROZEN") {
        sb = 10;
    } else if (wildMon->status == "POISONED" || wildMon->status == "BURNED" || wildMon->status == "PARALYZED") {
        sb = 5;
    }
    print("You threw the " + usedItem->name + "!");
    int rate = CATCH_RATES[wildMon->dexNum];
    int chance = std::max(((thp - 2 * wildMon->hp) * rate * bm) / thp, 1.0) + sb;
    if (utils::randInt(0, 256) <= chance) {
        battling = false;
        print("Oh baby! You caught it!");
        wildMon->nickname = wildMon->name;
        player->give(wildMon);
        wildMon = nullptr;
    } else {
        print("But it broke out!");
    }
    notifyUseItem(true);
}

void OverworldGui::notifyUseItem(bool b) {
    if (b) {
        player->use(usedItem);
    }
    usedItem = nullptr;
    chosenMon = nullptr;
    tm = nullptr;
    choosingFromLongList = false;
    usingItemList = false;
    inMenu = false;
    busedItem = b;
    usingBattleItem = false;
}

void OverworldGui::blackout() {
    Trainer::addEliteFour();
    loadMap(lastHeal);
    playerX = lastHeal->healX;
    playerY = lastHeal->healY;
    int n = player->money / 2;
    player->money -= n;
    player->healTeam();
    wildMon = nullptr;
    print("You dropped $" + std::to_string(n) + "!");
}
