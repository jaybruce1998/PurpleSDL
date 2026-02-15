#include "Effect.h"

Effect::Effect(const Effect &e) {
    acc = e.acc;
    modifier = e.modifier;
    effect = e.effect;
    stat = e.stat;
    me = e.me;
    up = e.up;
    amount = e.amount;
    variation = e.variation;
}

Effect::Effect(const std::string &effect, const std::vector<Effect> &e) : Effect(e[0]) {
    this->effect = effect;
}

Effect::Effect(const std::vector<std::string> &a, const std::vector<Effect> &e) : Effect(e[0]) {
    acc = std::stod(a[0]) / std::stod(a[1]);
}

Effect::Effect(bool me, bool up, const std::string &stat, double modifier) {
    effect = "hp";
    this->me = me;
    this->up = up;
    this->stat = stat;
    this->modifier = modifier;
}

Effect::Effect(bool me, bool up, const std::string &stat, const std::vector<std::string> &a) {
    effect = "hp";
    this->me = me;
    this->up = up;
    this->stat = stat;
    modifier = std::stod(a[0]);
    acc = std::stod(a[1]);
}

Effect::Effect(const std::string &effect, bool me, bool up, int amount) {
    this->effect = effect;
    this->me = me;
    this->up = up;
    this->amount = amount;
}

Effect::Effect(const std::string &effect) {
    this->effect = effect;
}

Effect::Effect(const std::string &effect, int amount) {
    this->effect = effect;
    this->amount = amount;
}

Effect::Effect(const std::string &effect, const std::vector<std::string> &a) {
    this->effect = effect;
    amount = std::stoi(a[0]);
    variation = std::stoi(a[1]) - amount + 1;
}

Effect::Effect(const std::string &effect, const std::string &stat, int amount, double modifier) {
    this->effect = effect;
    this->stat = stat;
    this->amount = amount;
    this->modifier = modifier;
}

Effect::Effect(const std::string &effect, const std::string &stat, const std::vector<std::string> &a, double modifier) {
    this->effect = effect;
    this->stat = stat;
    amount = std::stoi(a[0]);
    variation = std::stoi(a[1]) - amount;
    this->modifier = modifier;
}

Effect::Effect(double modifier) {
    effect = "RECOIL";
    this->modifier = modifier;
}

Effect::Effect(const std::string &effect, int amount, int variation) {
    this->effect = effect;
    this->amount = amount;
    this->variation = variation;
}

Effect::Effect(const std::vector<std::string> &effects) {
    this->effects = effects;
}

static bool isNumber(const std::string &s) {
    if (s.empty()) {
        return false;
    }
    for (char c : s) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}

static bool containsDash(const std::string &s) {
    return s.find('-') != std::string::npos;
}

std::vector<Effect> Effect::getEffects(const std::vector<std::string> &e, int i) {
    std::vector<Effect> effects;
    if (i >= static_cast<int>(e.size())) {
        return effects;
    }
    auto getToken = [&](int idx) -> const std::string & {
        if (idx >= static_cast<int>(e.size())) {
            std::ostringstream ss;
            for (size_t j = 0; j < e.size(); j++) {
                if (j > 0) {
                    ss << ' ';
                }
                ss << e[j];
            }
            throw std::runtime_error("Effect tokens out of range at index " + std::to_string(idx) + " in [" + ss.str() + "]");
        }
        return e[idx];
    };
    std::string eff = e[i];
    if (eff == "SLEEPING" || eff == "MISS") {
        std::vector<Effect> next = getEffects(e, i + 1);
        effects.emplace_back(eff, next);
        return effects;
    }
    if (eff.find('/') != std::string::npos) {
        std::vector<std::string> ea = utils::split(eff, '/');
        if (!ea.empty() && isNumber(ea[0])) {
            std::vector<Effect> next = getEffects(e, i + 1);
            effects.emplace_back(ea, next);
        } else {
            effects.emplace_back(ea);
            std::vector<Effect> rest = getEffects(e, i + 1);
            effects.insert(effects.end(), rest.begin(), rest.end());
        }
        return effects;
    }

    bool me = eff.rfind("opp", 0) != 0;
    if (!me) {
        eff = eff.substr(3);
        std::transform(eff.begin(), eff.end(), eff.begin(), ::tolower);
    }

    if (eff == "hp") {
        if (!isNumber(getToken(i + 2))) {
            if (containsDash(getToken(i + 3))) {
                effects.emplace_back(me, getToken(i + 1) == "UP", getToken(i + 2), utils::split(getToken(i + 3), '-'));
            } else {
                effects.emplace_back(me, getToken(i + 1) == "UP", getToken(i + 2), std::stod(getToken(i + 3)));
            }
            i += 4;
            std::vector<Effect> rest = getEffects(e, i);
            effects.insert(effects.end(), rest.begin(), rest.end());
            return effects;
        } else {
            // fallthrough to stat handling
        }
    }

    if (eff == "accuracy" || eff == "def" || eff == "spd" || eff == "spatk" || eff == "spdef" || eff == "evasion" ||
        eff == "atk" || eff == "crit_chance") {
        effects.emplace_back(eff, me, getToken(i + 1) == "UP", std::stoi(getToken(i + 2)));
        i += 3;
    } else if (eff == "ATTACK_FIRST" || eff == "ATTACK_SECOND" || eff == "BIDE" || eff == "BURN" ||
               eff == "CONFUSE" || eff == "FREEZE" || eff == "PARALYZE" || eff == "SLEEP" || eff == "POISON" ||
               eff == "TOXIC" || eff == "FLINCH" || eff == "CONVERT" || eff == "IMMUNE" || eff == "RESET_STATS" ||
               eff == "RECHARGE" || eff == "RANDOM_MOVE" || eff == "COPY_LAST_OPPONENT_MOVE" ||
               eff == "LAST_OPPONENT_MOVE" || eff == "DISABLE_STAT_CHANGES" || eff == "CHARGE" ||
               eff == "SWITCH_OPPONENT" || eff == "MAKE_SUBSTITUTE" || eff == "SWITCH" ||
               eff == "TRANSFORM_INTO_OPPONENT" || eff == "CONFUSE_ME" || eff == "SCATTER_COINS" || eff == "RAGING") {
        effects.emplace_back(eff);
        i += 1;
    } else if (eff == "HIT" || eff == "CRIT_CHANCE" || eff == "DISABLE" || eff == "CONTINUE" || eff == "SLEEP_FOR") {
        if (containsDash(getToken(i + 1))) {
            effects.emplace_back(eff, utils::split(getToken(i + 1), '-'));
        } else {
            effects.emplace_back(eff, std::stoi(getToken(i + 1)));
        }
        i += 2;
    } else if (eff == "END_DAMAGE") {
        if (containsDash(getToken(i + 2))) {
            effects.emplace_back(eff, getToken(i + 1), utils::split(getToken(i + 2), '-'), std::stod(getToken(i + 3)));
        } else {
            effects.emplace_back(eff, getToken(i + 1), std::stoi(getToken(i + 2)), std::stod(getToken(i + 3)));
        }
        i += 4;
    } else if (eff == "DEF" || eff == "SPDEF") {
        effects.emplace_back(eff, std::stoi(getToken(i + 1)), std::stoi(getToken(i + 2)));
        i += 3;
    } else if (eff == "hp") {
        effects.emplace_back("hp", me, getToken(i + 1) == "UP", std::stoi(getToken(i + 2)));
        i += 3;
    } else {
        throw std::runtime_error("Unknown effect: " + eff);
    }

    std::vector<Effect> rest = getEffects(e, i);
    effects.insert(effects.end(), rest.begin(), rest.end());
    return effects;
}

std::string Effect::toString() const {
    std::ostringstream ss;
    ss << "Accuracy=" << acc << ", Modifier=" << modifier << ", Name=" << effect << ", Stat=" << stat
       << ", Me=" << me << ", Up=" << up << ", Amount=" << amount << ", Variation=" << variation << ";";
    return ss.str();
}
