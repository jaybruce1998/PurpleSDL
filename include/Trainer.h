#pragma once

#include "Battler.h"
#include "SpriteManager.h"

class Player;

class Trainer {
public:
    static std::vector<std::string> TEAM_STRINGS;
    static std::vector<std::string> LEADER_TEAM_STRINGS;
    static std::vector<std::string> GIO_RIVAL_TEAM_STRINGS;
    static std::vector<std::string> E4_TEAM_STRINGS;
    static std::unordered_map<std::string, SDL_Texture*> TRAINER_SPRITES;
    static int tid;

    static void addEliteFour();
    static void buildTrainers();
    static void buildTrainers(Player *p, const std::string &trainS, const std::string &leadS, const std::string &gioRS);

    SDL_Texture *bi = nullptr;
    std::string type;
    int dir = 0;
    int x = 0;
    int y = 0;
    int reward = 0;
    int id = 0;
    std::vector<Battler*> team;
    bool beaten = false;
    std::vector<std::string> phrases;

    Trainer() = default;
    explicit Trainer(const std::string &s);
    virtual ~Trainer() = default;

    virtual void beat(Player *p);
    std::string toString() const;

    class Leader;
    class GioRival;
    class E4Trainer;
};

class Trainer::Leader : public Trainer {
public:
    Move *move = nullptr;
    Leader(int id, const std::vector<std::string> &a);
    void beat(Player *p) override;
};

class Trainer::GioRival : public Trainer {
public:
    GioRival(int id, const std::string &s);
    void beat(Player *p) override;
};

class Trainer::E4Trainer : public Trainer {
public:
    E4Trainer(int id, const std::string &s);
    void beat(Player *p) override;
};
