#include "Move.h"
#include "Utils.h"

static std::vector<std::string> splitLines(const std::string &s) {
    std::vector<std::string> lines;
    std::stringstream ss(s);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::string> Move::MOVE_STRINGS = splitLines(R"(Absorb	Grass	Special	20	100	25	hp UP damage 0.5
Acid	Poison	Special	40	100	30	10/100 oppSpdef DOWN 1
Acid Armor	Poison	Status	-	-	20	def UP 2
Agility	Psychic	Status	-	-	30	spd UP 2
Amnesia	Psychic	Status	-	-	20	spdef UP 2
Aurora Beam	Ice	Special	65	100	20	10/100 oppAtk DOWN 1
Barrage	Normal	Physical	15	85	20	HIT 2-5
Barrier	Psychic	Status	-	-	20	def UP 2
Bide	Normal	Physical	-	-	10	ATTACK_FIRST CHARGE ATTACK_SECOND BIDE
Bind	Normal	Physical	15	85	20	END_DAMAGE TRAP 4-5 0.0625
Bite	Normal	Physical	60	100	25	30/100 FLINCH
Blizzard	Ice	Special	110	70	5	10/100 FREEZE
Body Slam	Normal	Physical	85	100	15	30/100 PARALYZE
Bone Club	Ground	Physical	65	85	20	10/100 FLINCH
Bonemerang	Ground	Physical	50	90	10	HIT 2
Bubble	Water	Special	40	100	30	10/100 oppSpd DOWN 1
Bubble Beam	Water	Special	65	100	20	10/100 oppSpd DOWN 1
Clamp	Water	Physical	35	85	15	END_DAMAGE TRAP 4-5 0.0625
Comet Punch	Normal	Physical	18	85	15	HIT 2-5
Confuse Ray	Ghost	Status	-	100	10	CONFUSE
Confusion	Psychic	Special	50	100	25	10/100 CONFUSE
Constrict	Normal	Physical	10	100	35	10/100 oppSpd DOWN 1
Conversion	Normal	Status	-	-	30	CONVERT
Counter	Fighting	Physical	-	100	20	ATTACK_SECOND oppHp DOWN physical_damage 2
Crabhammer	Water	Physical	100	90	10	CRIT_CHANCE 3
Cut	Normal	Physical	50	95	30
Defense Curl	Normal	Status	-	-	40	def UP 1
Dig	Ground	Physical	80	100	10	IMMUNE
Disable	Normal	Status	-	100	20	DISABLE 1-8
Dizzy Punch	Normal	Physical	70	100	10	20/100 CONFUSE
Double Kick	Fighting	Physical	30	100	30	HIT 2
Double Slap	Normal	Physical	15	85	10	HIT 2-5
Double Team	Normal	Status	-	-	15	evasion UP 1
Double-Edge	Normal	Physical	120	100	15	hp DOWN damage 0.25
Dragon Rage	Dragon	Special	-	100	10	oppHp DOWN 40
Dream Eater	Psychic	Special	100	100	15	SLEEPING hp UP damage 0.5
Drill Peck	Flying	Physical	80	100	20	
Earthquake	Ground	Physical	100	100	10
Egg Bomb	Normal	Physical	100	75	10	
Ember	Fire	Special	40	100	25	10/100 BURN
Explosion	Normal	Physical	340	100	5	hp DOWN 999
Fire Blast	Fire	Special	110	85	5	10/100 BURN
Fire Punch	Fire	Physical	75	100	15	10/100 BURN
Fire Spin	Fire	Special	35	85	15	END_DAMAGE TRAP 4-5 0.0625
Fissure	Ground	Physical	-	30	5	oppHp DOWN 999
Flamethrower	Fire	Special	90	100	15	10/100 BURN
Flash	Normal	Status	-	100	20	oppAccuracy DOWN 1
Fly	Flying	Physical	90	95	15	IMMUNE
Focus Energy	Normal	Status	-	-	30	crit_chance UP 4
Fury Attack	Normal	Physical	15	85	20	HIT 2-5
Fury Swipes	Normal	Physical	18	80	15	HIT 2-5
Glare	Normal	Status	-	100	30	PARALYZE
Growl	Normal	Status	-	100	40	oppAtk DOWN 1
Growth	Normal	Status	-	-	20	atk UP 1 spatk UP 1
Guillotine	Normal	Physical	-	30	5	oppHp DOWN 999
Gust	Flying	Special	40	100	35
Harden	Normal	Status	-	-	30	def UP 1
Haze	Ice	Status	-	-	30	RESET_STATS
Headbutt	Normal	Physical	70	100	15	30/100 FLINCH
High Jump Kick	Fighting	Physical	130	90	10	MISS hp DOWN max_hp 0.5
Horn Attack	Normal	Physical	65	100	25	
Horn Drill	Normal	Physical	-	30	5	oppHp DOWN 999
Hydro Pump	Water	Special	110	80	5	
Hyper Beam	Normal	Special	150	90	5	RECHARGE
Hyper Fang	Normal	Physical	80	90	15	10/100 FLINCH
Hypnosis	Psychic	Status	-	60	20	SLEEP
Ice Beam	Ice	Special	90	100	10	10/100 FREEZE
Ice Punch	Ice	Physical	75	100	15	10/100 FREEZE
Jump Kick	Fighting	Physical	100	95	10	MISS hp DOWN max_hp 0.5
Karate Chop	Fighting	Physical	50	100	25	CRIT_CHANCE 3
Kinesis	Psychic	Status	-	80	15	oppAccuracy DOWN 1
Leech Life	Bug	Physical	80	100	10	hp UP damage 0.5
Leech Seed	Grass	Status	-	90	10	END_DAMAGE LEECH_SEED 0 0.0625
Leer	Normal	Status	-	100	30	oppDef DOWN 1
Lick	Ghost	Physical	30	100	30	30/100 PARALYZE
Light Screen	Psychic	Status	-	-	30	SPDEF 2 5
Lovely Kiss	Normal	Status	-	75	10	SLEEP
Low Kick	Fighting	Physical	50	100	20
Meditate	Psychic	Status	-	-	40	atk UP 1
Mega Drain	Grass	Special	40	100	15	hp UP damage 0.5
Mega Kick	Normal	Physical	120	75	5	
Mega Punch	Normal	Physical	80	85	20	
Metronome	Normal	Status	-	-	10	RANDOM_MOVE
Mimic	Normal	Status	-	-	10	COPY_LAST_OPPONENT_MOVE
Minimize	Normal	Status	-	-	10	evasion UP 1
Mirror Move	Flying	Status	-	-	20	LAST_OPPONENT_MOVE
Mist	Ice	Status	-	-	30	DISABLE_STAT_CHANGES
Night Shade	Ghost	Special	-	100	15	oppHp DOWN level 1
Pay Day	Normal	Physical	40	100	20	SCATTER_COINS
Peck	Flying	Physical	35	100	35	
Petal Dance	Grass	Special	120	100	10	CONTINUE 2-3 CONFUSE_ME
Pin Missile	Bug	Physical	25	95	20	HIT 2-5
Poison Gas	Poison	Status	-	90	40	POISON
Poison Powder	Poison	Status	-	75	35	POISON
Poison Sting	Poison	Physical	15	100	35	30/100 POISON
Pound	Normal	Physical	40	100	35	
Psybeam	Psychic	Special	65	100	20	10/100 CONFUSE
Psychic	Psychic	Special	90	100	10	10/100 oppSpdef DOWN 1
Psywave	Psychic	Special	-	100	15	oppHp DOWN level 0.5-1.5
Quick Attack	Normal	Physical	40	100	30	ATTACK_FIRST
Rage	Normal	Physical	20	100	20	RAGING
Razor Leaf	Grass	Physical	55	95	25	CRIT_CHANCE 3
Razor Wind	Normal	Special	80	100	10	CHARGE CRIT_CHANCE 3
Recover	Normal	Status	-	-	5	hp UP max_hp 0.5
Reflect	Psychic	Status	-	-	20	DEF 2 5
Rest	Psychic	Status	-	-	5	hp UP 999 SLEEP_FOR 2
Roar	Normal	Status	-	-	20	SWITCH_OPPONENT
Rock Slide	Rock	Physical	75	90	10	30/100 FLINCH
Rock Throw	Rock	Physical	50	90	15	
Rolling Kick	Fighting	Physical	60	85	15	30/100 FLINCH
Sand Attack	Ground	Status	-	100	15	oppAccuracy DOWN 1
Scratch	Normal	Physical	40	100	35	
Rock Rampage	Rock	Physical	100	100	10  
Screech	Normal	Status	-	85	40	oppDef DOWN 2
Seismic Toss	Fighting	Physical	-	100	20	oppHp DOWN level 1
Self-Destruct	Normal	Physical	130	100	5	hp DOWN 999
Sharpen	Normal	Status	-	-	30	atk UP 1
Sing	Normal	Status	-	55	15	SLEEP
Skull Bash	Normal	Physical	130	100	10	def UP 1 CHARGE
Sky Attack	Flying	Physical	140	90	5	CHARGE CRIT_CHANCE 3 30/100 FLINCH
Slam	Normal	Physical	80	75	20	
Slash	Normal	Physical	70	100	20	CRIT_CHANCE 3
Sleep Powder	Grass	Status	-	75	15	SLEEP
Sludge	Poison	Special	65	100	20	30/100 POISON
Smog	Poison	Special	30	70	20	40/100 POISON
Smokescreen	Normal	Status	-	100	20	oppAccuracy DOWN 1
Soft-Boiled	Normal	Status	-	-	5	hp UP max_hp 0.5
Solar Beam	Grass	Special	120	100	10	CHARGE
Sonic Boom	Normal	Special	-	90	20	oppHp DOWN 20
Spike Cannon	Normal	Physical	20	100	15	HIT 2-5
Splash	Normal	Status	-	-	40	1/100 oppHp DOWN 999
Spore	Grass	Status	-	100	15	SLEEP
Stomp	Normal	Physical	65	100	20	30/100 FLINCH
Strength	Normal	Physical	80	100	15	
String Shot	Bug	Status	-	95	40	oppSpd DOWN 2
Struggle	???	Physical	50	-	-	hp DOWN damage 0.25
Stun Spore	Grass	Status	-	75	30	PARALYZE
Submission	Fighting	Physical	80	80	20	hp DOWN damage 0.25
Substitute	Normal	Status	-	-	10	MAKE_SUBSTITUTE
Super Fang	Normal	Physical	-	90	10	oppHp DOWN oppHp 0.5
Supersonic	Normal	Status	-	55	20	CONFUSE
Surf	Water	Special	90	100	15
Swift	Normal	Special	60	-	20
Swords Dance	Normal	Status	-	-	20	atk UP 2
Tackle	Normal	Physical	40	100	35	
Tail Whip	Normal	Status	-	100	30	oppDef DOWN 1
Take Down	Normal	Physical	90	85	20	hp DOWN damage 0.25
Teleport	Psychic	Status	-	-	20	SWITCH
Thrash	Normal	Physical	120	100	10	CONTINUE 2-3 CONFUSE_ME
Thunder	Electric	Special	110	70	10	30/100 PARALYZE
Thunder Punch	Electric	Physical	75	100	15	10/100 PARALYZE
Thunder Shock	Electric	Special	40	100	30	10/100 PARALYZE
Thunder Wave	Electric	Status	-	90	20	PARALYZE
Thunderbolt	Electric	Special	90	100	15	10/100 PARALYZE
Toxic	Poison	Status	-	90	10	TOXIC
Transform	Normal	Status	-	-	10	TRANSFORM_INTO_OPPONENT
Tri Attack	Normal	Special	99	99	33	33/100 BURN/PARALYZE/FREEZE
Twineedle	Bug	Physical	25	100	20	HIT 2 20/100 POISON
Vine Whip	Grass	Physical	45	100	25	atk UP 1
Vise Grip	Normal	Physical	55	100	30	atk UP 1
Water Gun	Water	Special	50	100	25	
Waterfall	Water	Physical	80	100	15	20/100 FLINCH
Whirlwind	Normal	Status	-	-	20	SWITCH_OPPONENT
Wing Attack	Flying	Physical	60	100	35	atk UP 1
Withdraw	Water	Status	-	-	40	def UP 1
Wrap	Normal	Physical	15	90	20	END_DAMAGE TRAP 4-5 0.0625
CONFUSION	???	Physical	40	100	-
RECHARGE	???	Status	-	-	-	CHARGE
Hypersonic Blast	Dragon	Special	999	100	10	spatk DOWN 6
Burning Rage	Dragon	Special	250	100	10	spatk UP 12 CHARGE CONFUSE
Calm Mind	Psychic	Status	-	-	20	spatk UP 1 spdef UP 1
Rat Gambit	Normal	Physical	200	-	1	atk UP 6 10/100 evasion UP 6
Desperate Kick	Fighting	Physical	30	100	40	def UP 1
Big Money	Bug	Physical	100	100	5	BURN
Sludge Bomb	Poison	Special	90	100	10	30/100 POISON)");

std::vector<Move> Move::MOVES;
std::unordered_map<std::string, Move*> Move::MOVE_MAP;

void Move::buildMoves() {
    MOVES.clear();
    MOVES.reserve(MOVE_STRINGS.size());
    for (const std::string &line : MOVE_STRINGS) {
        if (utils::trim(line).empty()) {
            continue;
        }
        MOVES.emplace_back(line);
    }
}

Move::Move(const std::string &s) {
    std::vector<std::string> a = utils::split(s, '\t');
    if (a.size() < 6) {
        throw std::runtime_error("Bad move line: " + s);
    }
    name = a[0];
    MOVE_MAP[name] = this;
    type = a[1];
    atkType = a[2];
    power = (a[3] == "-" ? 0 : std::stoi(a[3]));
    acc = (a[4] == "-" ? 100000 : std::stoi(a[4]));
    pp = (a[5] == "-" ? 100000 : std::stoi(a[5]));
    if (a.size() == 6) {
        effects.clear();
    } else {
        try {
            effects = Effect::getEffects(utils::split(a[6], ' '), 0);
        } catch (const std::exception &e) {
            throw std::runtime_error("Move parse failed: " + name + " line=[" + s + "] error=" + e.what());
        }
    }
}

Move::Move(const Move &m, int i) {
    name = m.name;
    type = m.type;
    atkType = m.atkType;
    power = m.power;
    acc = m.acc;
    pp = m.pp;
    for (; i < static_cast<int>(m.effects.size()); i++) {
        effects.push_back(m.effects[i]);
    }
}

Move::Move(const Move &m) : Move(m, 0) {
    if (!effects.empty()) {
        const Effect &e = effects[0];
        effects[0] = Effect(e.effect, e.amount + utils::randInt(0, e.variation) - 1);
    }
}

int Move::priority() const {
    if (effects.empty()) {
        return 0;
    }
    const Effect &e = effects[0];
    if (e.effect == "ATTACK_FIRST") {
        return 1;
    }
    if (e.effect == "ATTACK_SECOND") {
        return -1;
    }
    return 0;
}

int Move::critChance() const {
    if (effects.empty()) {
        return 1;
    }
    const Effect &e = effects[0];
    return e.effect == "CRIT_CHANCE" ? e.amount : 1;
}

bool Move::shouldPrintDamage() const {
    if (power == 0) {
        return false;
    }
    for (const Effect &e : effects) {
        if (e.effect == "CHARGE" || e.effect == "IMMUNE") {
            return false;
        }
    }
    return true;
}

std::string Move::toString() const {
    std::ostringstream ss;
    ss << name << "(" << type << ")\n\tType=" << atkType << "\n\tPower=" << power << "\n\tAccuracy=" << acc
       << "\n\tPP=" << pp << "\n\tEffects=";
    for (const auto &e : effects) {
        ss << e.toString();
    }
    return ss.str();
}
