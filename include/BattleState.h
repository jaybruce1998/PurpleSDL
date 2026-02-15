#pragma once

#include "Battler.h"
#include "Item.h"
#include <unordered_set>

class OverworldGui;
class Player;

class BattleState {
public:
    static Move *STRUGGLE;
    static Move *CONFUSION;
    static Move *RECHARGE;
    static Move *ROAR;
    static Move *TELEPORT;
    static Move *WHIRLWIND;
    static std::vector<int> EXPERIENCE_TABLE;
    static std::vector<std::string> ACTIONS;
    static Item *LUCKY_EGG;

    static int guiChoice(int m);
    static int wildBattle(std::vector<Battler*> &monsters, Battler *wildMon, OverworldGui *gui);
    static int trainerBattle(std::vector<Battler*> &monsters, const std::string &tName, std::vector<Battler*> &tMonsters,
                             OverworldGui *gui);

    Battler *monster = nullptr;
    std::vector<Battler*> monsters;
    int coins = 0;
    int accStage = 0;
    int evsnStage = 0;
    int spdStage = 0;
    int atkStage = 0;
    int defStage = 0;
    int spatkStage = 0;
    int spdefStage = 0;
    int critMul = 1;
    int spd = 0;
    int atk = 0;
    int def = 0;
    int spatk = 0;
    int spdef = 0;
    int sTurns = 0;
    int tTurns = 0;
    int cTurns = 0;
    int disabled = 0;
    int dTurns = 0;
    int spdefTurns = 0;
    int defTurns = 0;
    int subHp = 0;
    int poisonDamage = 0;
    std::vector<std::string> types;
    bool crit = false;
    bool wasPhysical = false;
    bool flinched = false;
    bool immune = false;
    bool seeded = false;
    bool canLower = true;
    bool raging = false;
    Move *nextMove = nullptr;
    Move *lastMove = nullptr;
    int lastDamage[2]{0, 0};
    std::vector<Move*> moves;

    BattleState() = default;
    explicit BattleState(Battler *monster);
    BattleState(Battler *monster, BattleState *b);

    bool goBefore(Move *m, BattleState *b, Move *o);
    bool canMove();
    bool doMove(Move *m, BattleState *b);
    bool endTurn();

private:
    static int getMoveDex(BattleState *b);
    static int randomMoveDex(BattleState *b);
    static bool blackedOut(const std::vector<Battler*> &monsters);
    static int coinsPickup(BattleState *pState, BattleState *oState);
    static bool canSwitch(const std::vector<Battler*> &monsters);
    static int switchMon(std::vector<Battler*> &monsters, int i, bool mustSwitch);
    static void seed(BattleState *sucker, BattleState *sucked);
    void gainXp(Player *p, const std::unordered_set<Battler*> &participants, Battler *fainted, bool trainer, int lucky);
    static double effect(int stage);
    double speed();
    static double accuracy(int stage);
    double stab(Move *m);
    bool hasType(const std::string &type);
    int calcDamage(Move *m, BattleState *b);
    bool setLastMove(Move *m, int d);
    bool disable(Move *m, int a, int b);
    bool shouldStruggle();
    void resetStats();
    void processMiss(Move *m);
    std::string es;
};
