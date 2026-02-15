#pragma once

#include "Utils.h"

class Effect {
public:
    double acc = 1000.0;
    double modifier = 0.0;
    std::string effect;
    std::string stat;
    bool me = false;
    bool up = false;
    int amount = 0;
    int variation = 0;
    std::vector<std::string> effects;

    Effect() = default;
    explicit Effect(const Effect &e);
    Effect(const std::string &effect, const std::vector<Effect> &e);
    Effect(const std::vector<std::string> &a, const std::vector<Effect> &e);
    Effect(bool me, bool up, const std::string &stat, double modifier);
    Effect(bool me, bool up, const std::string &stat, const std::vector<std::string> &a);
    Effect(const std::string &effect, bool me, bool up, int amount);
    explicit Effect(const std::string &effect);
    Effect(const std::string &effect, int amount);
    Effect(const std::string &effect, const std::vector<std::string> &a);
    Effect(const std::string &effect, const std::string &stat, int amount, double modifier);
    Effect(const std::string &effect, const std::string &stat, const std::vector<std::string> &a, double modifier);
    explicit Effect(double modifier);
    Effect(const std::string &effect, int amount, int variation);
    explicit Effect(const std::vector<std::string> &effects);

    static std::vector<Effect> getEffects(const std::vector<std::string> &e, int i);
    std::string toString() const;
};
