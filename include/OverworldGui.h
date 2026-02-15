#pragma once

#include <SDL.h>
#include <string>
#include <vector>

class BattleState;
class Item;
class Player;
class PokeMap;
class Battler;
class Trainer;
class Npc;
class WorldObject;
class MartItem;
class Move;

class OverworldGui {
public:
    static bool showingText;
    static bool battling;
    static bool canMap;
    static bool choosingFromLongList;
    static bool usingBattleItem;
    static bool busedItem;
    static bool rightClicked;
    static bool inMenu;
    static bool buySell;
    static bool buying;
    static bool selling;
    static bool checkingPokes;
    static bool checkingMoves;
    static bool checkingTms;
    static bool teachingMove;
    static bool replacingMove;
    static bool depWith;
    static bool depositing;
    static bool withdraw;
    static bool flying;
    static bool inside;
    static bool showingDex;
    static bool rareCandy;
    static bool pickingStarter;
    static bool surfing;
    static bool spacebar;
    static bool autoBattle;

    static int clickedChoice;
    static std::string currentText;
    static std::string currentLoc;
    static std::string longListHeaderOverride;
    static std::vector<std::string> strArr;
    static std::vector<std::string> longArr;
    static std::vector<Item*> usableItems;
    static bool usingItemList;
    static BattleState *playerState;
    static BattleState *enemyState;

    enum class Direction {
        SOUTH = 0,
        NORTH = 1,
        WEST = 2,
        EAST = 3
    };

    enum class StepPhase { NONE, MOVING, LANDING };

    static void print(const std::string &s);
    static std::string promptText(const std::string &prompt);
    static int waitForChoice(int maxChoice);
    static void setBattleStates(BattleState *a, BattleState *b);
    static void autoAdvanceBattleText();
    static void runGui();

    explicit OverworldGui(Player *player);
    OverworldGui(Player *player, SDL_Renderer *renderer);
    OverworldGui(Player *player, SDL_Renderer *renderer, const std::string &saveInfo);

    void setWildItems();
    void setTrainerItems();
    void pumpEvents();
    void handleEvent(const SDL_Event &event);
    void update();
    void render();

    Player *player;
    SDL_Renderer *renderer = nullptr;

private:
    static void advanceText();
    void stopMovement();
    void applySaveInfo(const std::string &saveInfo);
    void setup();
    void loadMap(PokeMap *map);
    void loadTileImages();
    void loadPlayerSprites();
    void closeMenus();
    void clickMouse(int mouseX, int mouseY, bool rightClick);
    void save();
    void startFlying();
    std::string printable(const std::string &s);
    bool canMove(Direction d);
    int elevate(const std::string &s, bool B, int m);
    char randomSpooky();
    void printSpooky();
    void advanceStep();
    void blackout();
    void useRevive(Battler *b, int n);
    void healHp(Battler *b, int n);
    void useElixir(Battler *b, int n);
    void healStatus(Battler *b, const std::string &s);
    void useRepel(int steps);
    void catchMon(double bm);
    void notifyUseItem(bool b);
    void drawTextBox(const std::string &text);
    void drawWrappedText(const std::string &text, int x, int y, int maxWidth);
    void drawConnection(int dirIndex, PokeMap *conn, int offset, int camX, int camY);
    int getCurrentFrame() const;

    PokeMap *pm = nullptr;
    PokeMap *lastHeal = nullptr;
    std::string mapName = "RedsHouse2F";
    std::vector<std::vector<int>> currentMap;
    std::vector<std::vector<int>> tileTypes;
    PokeMap *connections[4]{nullptr, nullptr, nullptr, nullptr};
    int connOffsets[4]{0, 0, 0, 0};

    int playerX = 0;
    int playerY = 6;
    int repelSteps = 0;
    int boxNum = 0;
    int psi = 0;
    int frames = 0;
    int phaseFrame = 0;
    int currentStepFrames = 8;
    int timesMoved = 0;
    int bet = 1;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    Direction facing = Direction::SOUTH;
    Direction heldDirection = Direction::SOUTH;
    bool hasHeldDirection = false;
    StepPhase stepPhase = StepPhase::NONE;
    bool switchingMaps = false;
    bool spinning = false;
    bool jumping = false;

    bool blackjackActive = false;
    int blackjackBet = 0;
    std::vector<int> blackjackDeck;
    std::vector<int> blackjackPlayer;
    std::vector<int> blackjackDealer;
    bool blackjackHideFirst = true;
    bool blackjackFinished = false;
    bool blackjackPlayerWon = false;
    int blackjackTieFrames = 0;
    bool blackjackShowingResults = false;

    std::vector<Direction> pressedKeys;
    Battler *chosenMon = nullptr;
    Battler *wildMon = nullptr;
    Item *usedItem = nullptr;
    std::vector<MartItem> martItems;
    std::vector<Move*> tms;
    Move *tm = nullptr;

    std::vector<SDL_Texture*> tileImages;
    std::vector<SDL_Texture*> playerFrames;
    std::vector<SDL_Texture*> seelFrames;
    std::vector<SDL_Texture*> pBattlers;
    std::vector<SDL_Texture*> eBattlers;
    std::vector<SDL_Texture*> danceFrames;
    std::vector<SDL_Texture*> oakFrames;
    std::vector<SDL_Texture*> monsterFrames;
    Npc *mattNpc = nullptr;
    Npc *oakNpc = nullptr;
    Trainer *monsterTrainer = nullptr;

private:
    static OverworldGui *instance;
};
