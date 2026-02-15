#include "LevelUpMove.h"
#include "Utils.h"

static std::vector<std::string> splitLinesLevelUpMove(const std::string &s) {
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

std::vector<std::string> LevelUpMove::LEVEL_UP_STRINGS = splitLinesLevelUpMove(R"(1 Psychic,2 Double-Edge,3 Hypersonic Blast,4 Burning Rage
- Tackle,- Growl,1 Desperate Kick,7 Leech Seed,13 Vine Whip,20 Poison Powder,27 Razor Leaf,34 Growth,41 Sleep Powder,48 Solar Beam,49 Sludge Bomb
7 Leech Seed,13 Vine Whip,22 Poison Powder,30 Razor Leaf,38 Growth,46 Sleep Powder,54 Solar Beam,55 Sludge Bomb
- Vine Whip,7 Leech Seed,13 Vine Whip,22 Poison Powder,30 Razor Leaf,43 Growth,55 Sleep Powder,65 Solar Beam,100 Sludge Bomb
- Scratch,- Growl,1 Desperate Kick,9 Ember,15 Leer,22 Rage,30 Slash,38 Flamethrower,46 Fire Spin
9 Ember,15 Leer,24 Rage,33 Slash,42 Flamethrower,56 Fire Spin
9 Ember,15 Leer,24 Rage,36 Slash,46 Flamethrower,55 Fire Spin
- Tackle,- Tail Whip,1 Desperate Kick,8 Bubble,15 Water Gun,22 Bite,28 Withdraw,35 Skull Bash,42 Hydro Pump
8 Bubble,15 Water Gun,24 Bite,31 Withdraw,39 Skull Bash,47 Hydro Pump
8 Bubble,15 Water Gun,24 Bite,31 Withdraw,42 Skull Bash,52 Hydro Pump
- Tackle,- String Shot,135 Hypersonic Blast,150 Burning Rage
- Harden,7 Harden
- Confusion,10 Confusion,13 Poison Powder,14 Stun Spore,15 Sleep Powder,18 Supersonic,23 Whirlwind,28 Gust,34 Psybeam
- Poison Sting,- String Shot,135 Hypersonic Blast,150 Burning Rage
- Harden
- Fury Attack,12 Fury Attack,16 Focus Energy,20 Twineedle,25 Rage,30 Pin Missile,35 Agility
- Tackle,- Agility,5 Sand Attack,6 Gust,12 Quick Attack,19 Whirlwind,28 Wing Attack,36 Agility,44 Mirror Move
- Gust,5 Sand Attack,12 Quick Attack,21 Whirlwind,31 Wing Attack,40 Agility,49 Mirror Move
- Gust,5 Sand Attack,12 Quick Attack,21 Whirlwind,31 Wing Attack,44 Agility,54 Mirror Move
- Tackle,- Tail Whip,7 Quick Attack,14 Hyper Fang,23 Focus Energy,34 Super Fang,85 Body Slam,111 Rat Gambit
- Tackle,- Tail Whip,7 Quick Attack,14 Hyper Fang,27 Focus Energy,41 Super Fang,85 Body Slam,111 Rat Gambit
- Peck,- Growl,9 Leer,15 Fury Attack,22 Mirror Move,29 Drill Peck,36 Agility
- Peck,- Growl,9 Leer,15 Fury Attack,25 Mirror Move,34 Drill Peck,43 Agility
- Wrap,- Leer,10 Poison Sting,17 Bite,24 Glare,31 Screech,38 Acid
- Wrap,- Leer,10 Poison Sting,17 Bite,27 Glare,36 Screech,47 Acid
- Thunder Shock,- Growl,6 Tail Whip,8 Thunder Wave,11 Quick Attack,15 Double Team,20 Slam,26 Thunderbolt,33 Agility,41 Thunder,50 Light Screen
- Thunder Shock,- Growl,- Thunder Wave
- Scratch,10 Sand Attack,17 Slash,24 Poison Sting,31 Swift,38 Fury Swipes
- Scratch,10 Sand Attack,17 Slash,27 Poison Sting,36 Swift,47 Fury Swipes
- Growl,- Tackle,8 Scratch,12 Double Kick,17 Poison Sting,23 Tail Whip,30 Bite,38 Fury Swipes
- Growl,- Tackle,8 Scratch,12 Double Kick,19 Poison Sting,27 Tail Whip,36 Bite,46 Fury Swipes
- Tackle,- Scratch,- Tail Whip,- Body Slam,8 Scratch,12 Double Kick,23 Body Slam
- Leer,- Tackle,8 Horn Attack,12 Double Kick,17 Poison Sting,23 Focus Energy,30 Fury Attack,38 Horn Drill
- Leer,- Tackle,8 Horn Attack,12 Double Kick,19 Poison Sting,27 Focus Energy,36 Fury Attack,46 Horn Drill
- Tackle,- Poison Sting,- Thrash,8 Horn Attack,12 Double Kick,23 Thrash
- Pound,- Growl,13 Sing,18 Double Slap,24 Minimize,31 Metronome,39 Defense Curl,48 Light Screen
- Sing,- Double Slap,- Minimize,- Metronome
- Ember,- Tail Whip,16 Quick Attack,21 Roar,28 Confuse Ray,35 Flamethrower,42 Fire Spin
- Ember,- Tail Whip,- Quick Attack,- Roar
- Sing,9 Pound,14 Disable,19 Defense Curl,24 Double Slap,29 Rest,34 Body Slam,39 Double-Edge
- Sing,- Disable,- Defense Curl,- Double Slap
- Leech Life,10 Supersonic,15 Bite,21 Confuse Ray,28 Wing Attack,36 Haze
- Leech Life,- Screech,10 Supersonic,15 Bite,21 Confuse Ray,32 Wing Attack,43 Haze
- Absorb,15 Poison Powder,17 Stun Spore,19 Sleep Powder,24 Acid,33 Petal Dance,46 Solar Beam
- Absorb,15 Poison Powder,17 Stun Spore,19 Sleep Powder,28 Acid,38 Petal Dance,52 Solar Beam
- Acid,- Petal Dance,15 Poison Powder,17 Stun Spore,19 Sleep Powder
- Scratch,13 Stun Spore,20 Leech Life,27 Spore,34 Slash,41 Growth
- Scratch,13 Stun Spore,20 Leech Life,30 Spore,39 Slash,48 Growth
- Tackle,- Disable,11 Supersonic,19 Confusion,24 Poison Powder,27 Leech Life,30 Stun Spore,35 Psybeam,38 Sleep Powder,43 Psychic
- Tackle,- Disable,- Supersonic,- Confusion,22 Poison Powder,27 Leech Life,30 Stun Spore,38 Psybeam,43 Sleep Powder,50 Psychic
- Scratch,15 Growl,19 Dig,24 Sand Attack,31 Slash,40 Earthquake
- Scratch,15 Growl,19 Dig,24 Sand Attack,35 Slash,47 Earthquake
- Scratch,- Growl,12 Bite,17 Pay Day,24 Screech,33 Fury Swipes,44 Slash
- Scratch,- Growl,12 Bite,17 Pay Day,24 Screech,37 Fury Swipes,51 Slash
- Scratch,28 Tail Whip,31 Disable,36 Confusion,43 Fury Swipes,52 Hydro Pump
- Scratch,- Tail Whip,28 Tail Whip,31 Disable,39 Confusion,48 Fury Swipes,59 Hydro Pump
- Scratch,- Leer,9 Low Kick,15 Karate Chop,21 Fury Swipes,27 Focus Energy,33 Seismic Toss,39 Thrash,45 Screech
- Scratch,- Leer,9 Low Kick,15 Karate Chop,21 Fury Swipes,27 Focus Energy,28 Rage,37 Seismic Toss,45 Screech,46 Thrash
- Bite,- Roar,18 Ember,23 Leer,30 Take Down,39 Agility,50 Flamethrower
- Roar,- Ember,- Leer,- Take Down
- Bubble,16 Hypnosis,19 Water Gun,25 Double Slap,31 Body Slam,38 Amnesia,45 Hydro Pump
- Bubble,16 Hypnosis,19 Water Gun,26 Double Slap,33 Body Slam,41 Amnesia,49 Hydro Pump
- Double Slap,- Body Slam,16 Hypnosis,19 Water Gun
- Teleport,75 Psychic,101 Hypersonic Blast,150 Burning Rage
- Teleport,- Kinesis,16 Confusion,20 Disable,27 Psybeam,31 Recover,38 Psychic,42 Reflect
- Teleport,- Kinesis,16 Confusion,20 Disable,27 Psybeam,31 Recover,38 Psychic,42 Reflect,75 Calm Mind,118 Hypersonic Blast,150 Burning Rage
- Karate Chop,20 Low Kick,25 Leer,32 Focus Energy,39 Seismic Toss,46 Submission
- Karate Chop,20 Low Kick,25 Leer,36 Focus Energy,44 Seismic Toss,52 Submission
- Karate Chop,20 Low Kick,25 Leer,36 Focus Energy,44 Seismic Toss,52 Submission
- Vine Whip,- Growth,13 Wrap,15 Poison Powder,18 Sleep Powder,21 Stun Spore,26 Acid,33 Razor Leaf,42 Slam
- Vine Whip,- Growth,13 Wrap,15 Poison Powder,18 Sleep Powder,23 Stun Spore,29 Acid,38 Razor Leaf,49 Slam
- Stun Spore,- Acid,- Razor Leaf,13 Wrap,15 Poison Powder,18 Sleep Powder
- Acid,7 Supersonic,13 Wrap,18 Poison Sting,22 Water Gun,27 Constrict,33 Barrier,40 Screech,48 Hydro Pump
- Acid,- Supersonic,7 Supersonic,13 Wrap,18 Poison Sting,22 Water Gun,27 Constrict,35 Barrier,43 Screech,50 Hydro Pump
- Tackle,11 Defense Curl,16 Rock Throw,21 Self-Destruct,26 Harden,31 Earthquake,36 Explosion
- Tackle,11 Defense Curl,16 Rock Throw,21 Self-Destruct,29 Harden,36 Earthquake,43 Explosion
- Tackle,11 Defense Curl,16 Rock Throw,21 Self-Destruct,29 Harden,36 Earthquake,43 Explosion
- Ember,30 Tail Whip,32 Stomp,35 Growl,39 Fire Spin,43 Take Down,48 Agility
- Ember,30 Tail Whip,32 Stomp,35 Growl,39 Fire Spin,47 Take Down,55 Agility
- Confusion,18 Disable,22 Headbutt,27 Growl,33 Water Gun,40 Amnesia,48 Psychic
- Confusion,18 Disable,22 Headbutt,27 Growl,33 Water Gun,37 Withdraw,44 Amnesia,55 Psychic
- Tackle,21 Sonic Boom,25 Thunder Shock,29 Supersonic,35 Thunder Wave,41 Swift,47 Screech
- Tackle,21 Sonic Boom,25 Thunder Shock,29 Supersonic,38 Thunder Wave,46 Swift,54 Screech
- Peck,- Sand Attack,7 Leer,15 Fury Attack,23 Swords Dance,31 Agility,39 Slash
- Peck,20 Growl,24 Fury Attack,30 Drill Peck,36 Rage,40 Tri Attack,44 Agility
- Peck,20 Growl,24 Fury Attack,30 Drill Peck,39 Rage,45 Tri Attack,51 Agility
- Headbutt,30 Growl,35 Aurora Beam,40 Rest,45 Take Down,50 Ice Beam
- Headbutt,30 Growl,35 Aurora Beam,44 Rest,50 Take Down,56 Ice Beam
- Pound,- Disable,30 Poison Gas,33 Minimize,37 Sludge,42 Harden,48 Screech,55 Acid Armor
- Pound,- Disable,30 Poison Gas,33 Minimize,37 Sludge,45 Harden,53 Screech,60 Acid Armor
- Tackle,- Withdraw,18 Supersonic,23 Clamp,30 Aurora Beam,39 Leer,50 Ice Beam
- Withdraw,- Supersonic,- Clamp,- Aurora Beam,50 Spike Cannon
- Lick,- Confuse Ray,- Night Shade,27 Hypnosis,35 Dream Eater
- Lick,- Confuse Ray,- Night Shade,29 Hypnosis,38 Dream Eater
- Lick,- Confuse Ray,- Night Shade,29 Hypnosis,38 Dream Eater
- Tackle,- Screech,15 Bind,19 Rock Throw,25 Rage,33 Slam,43 Harden
- Pound,- Hypnosis,12 Disable,17 Confusion,24 Headbutt,29 Poison Gas,32 Psychic,37 Meditate
- Pound,- Hypnosis,12 Disable,17 Confusion,24 Headbutt,33 Poison Gas,37 Psychic,43 Meditate
- Bubble,- Leer,20 Vise Grip,25 Guillotine,30 Stomp,35 Crabhammer,40 Harden
- Bubble,- Leer,20 Vise Grip,25 Guillotine,34 Stomp,42 Crabhammer,49 Harden
- Tackle,- Screech,17 Sonic Boom,22 Self-Destruct,29 Light Screen,36 Swift,43 Explosion
- Tackle,- Screech,17 Sonic Boom,22 Self-Destruct,29 Light Screen,40 Swift,50 Explosion
- Barrage,- Hypnosis,25 Reflect,28 Leech Seed,32 Stun Spore,37 Poison Powder,42 Solar Beam,48 Sleep Powder
- Barrage,- Hypnosis,28 Stomp
- Growl,10 Bone Club,13 Tail Whip,18 Headbutt,25 Leer,31 Focus Energy,38 Thrash,43 Bonemerang,46 Rage
10 Bone Club,13 Tail Whip,18 Headbutt,25 Leer,33 Focus Energy,41 Thrash,48 Bonemerang,55 Rage
- Double Kick,- Meditate,33 Rolling Kick,38 Jump Kick,43 Focus Energy,48 High Jump Kick,53 Mega Kick
- Comet Punch,- Agility,33 Fire Punch,38 Ice Punch,43 Thunder Punch,48 Mega Punch,53 Counter
- Wrap,- Supersonic,7 Stomp,15 Disable,23 Defense Curl,31 Slam,39 Screech
- Tackle,- Smog,32 Sludge,37 Smokescreen,40 Self-Destruct,45 Haze,48 Explosion
- Tackle,- Smog,32 Sludge,39 Smokescreen,43 Self-Destruct,49 Haze,53 Explosion
- Horn Attack,30 Stomp,35 Tail Whip,40 Fury Attack,45 Horn Drill,50 Leer,55 Take Down
- Horn Attack,30 Stomp,35 Tail Whip,40 Fury Attack,48 Horn Drill,55 Leer,64 Take Down
- Pound,- Tail Whip,12 Double Slap,24 Sing,30 Growl,38 Minimize,44 Defense Curl,48 Light Screen,54 Double-Edge
- Constrict,24 Bind,27 Absorb,29 Vine Whip,32 Poison Powder,36 Stun Spore,39 Sleep Powder,45 Slam,48 Growth
- Comet Punch,- Rage,26 Bite,31 Tail Whip,36 Mega Punch,41 Leer,46 Dizzy Punch
- Bubble,19 Smokescreen,24 Leer,30 Water Gun,37 Agility,45 Hydro Pump
- Bubble,19 Smokescreen,24 Leer,30 Water Gun,41 Agility,52 Hydro Pump
- Peck,- Tail Whip,19 Supersonic,24 Horn Attack,30 Fury Attack,37 Waterfall,45 Horn Drill,54 Agility
- Peck,- Tail Whip,19 Supersonic,24 Horn Attack,30 Fury Attack,39 Waterfall,48 Horn Drill,54 Agility
- Tackle,17 Water Gun,22 Harden,27 Recover,32 Swift,37 Minimize,42 Light Screen,47 Hydro Pump
- Tackle,- Water Gun,- Harden
- Confusion,- Barrier,23 Light Screen,31 Double Slap,39 Meditate,47 Substitute
- Quick Attack,17 Leer,20 Focus Energy,24 Double Team,29 Slash,35 Swords Dance,42 Agility,50 Wing Attack
- Pound,- Lovely Kiss,18 Lick,23 Double Slap,31 Ice Punch,39 Body Slam,47 Thrash,58 Blizzard
- Quick Attack,- Leer,34 Thunder Shock,37 Screech,42 Thunder Punch,49 Light Screen,54 Thunder
- Ember,36 Leer,39 Confuse Ray,43 Fire Punch,48 Smokescreen,52 Smog,55 Flamethrower
- Vise Grip,21 Bind,25 Seismic Toss,30 Guillotine,36 Focus Energy,43 Harden,49 Slash,54 Swords Dance
- Tackle,21 Stomp,28 Tail Whip,35 Leer,44 Rage,51 Take Down
- Splash,15 Tackle,101 Hypersonic Blast,150 Burning Rage
- Tackle,20 Bite,25 Dragon Rage,32 Leer,41 Hydro Pump,52 Hyper Beam
- Water Gun,- Growl,16 Sing,20 Mist,25 Body Slam,31 Confuse Ray,38 Ice Beam,46 Hydro Pump
- Transform,65 Hypersonic Blast,100 Hyper Beam,150 Burning Rage
- Tackle,- Tail Whip,8 Sand Attack,16 Growl,23 Quick Attack,30 Bite,36 Focus Energy,42 Take Down
- Tackle,- Tail Whip,8 Sand Attack,16 Water Gun,23 Quick Attack,30 Bite,36 Aurora Beam,42 Haze,47 Acid Armor,52 Hydro Pump
- Tackle,- Tail Whip,8 Sand Attack,16 Thunder Shock,23 Quick Attack,30 Double Kick,36 Pin Missile,42 Thunder Wave,47 Agility,52 Thunder
- Tackle,- Tail Whip,- Ember,8 Sand Attack,16 Ember,23 Quick Attack,30 Bite,36 Fire Spin,42 Smog,47 Leer,52 Flamethrower
- Tackle,- Sharpen,- Conversion,23 Psybeam,28 Recover,35 Agility,42 Tri Attack
- Water Gun,- Withdraw,34 Horn Attack,39 Leer,46 Spike Cannon,53 Hydro Pump
- Water Gun,- Withdraw,34 Horn Attack,39 Leer,44 Spike Cannon,49 Hydro Pump
- Scratch,- Harden,34 Absorb,39 Slash,44 Leer,49 Hydro Pump
- Scratch,- Harden,34 Absorb,39 Slash,46 Leer,53 Hydro Pump
- Wing Attack,- Agility,33 Supersonic,38 Bite,45 Take Down,54 Hyper Beam
- Headbutt,- Amnesia,- Rest,35 Body Slam,41 Harden,48 Double-Edge,56 Hyper Beam,80 Calm Mind,90 Agility,100 Swords Dance
- Peck,- Ice Beam,- Drill Peck,51 Blizzard,55 Agility,60 Mist,80 Calm Mind,105 Hypersonic Blast,150 Burning Rage
- Thunder Shock,- Drill Peck,51 Thunder,55 Agility,60 Light Screen,80 Calm Mind,105 Hypersonic Blast,150 Burning Rage
- Peck,- Fire Spin,- Drill Peck,51 Leer,55 Agility,60 Sky Attack,80 Calm Mind,105 Hypersonic Blast,150 Burning Rage
- Wrap,- Leer,10 Thunder Wave,20 Agility,30 Slam,40 Dragon Rage,50 Hyper Beam
- Wrap,- Leer,10 Thunder Wave,20 Agility,35 Slam,45 Dragon Rage,55 Hyper Beam
- Wrap,- Leer,10 Thunder Wave,20 Agility,35 Slam,45 Dragon Rage,60 Hyper Beam,80 Calm Mind,120 Hypersonic Blast,150 Burning Rage
- Confusion,- Disable,- Swift,63 Barrier,66 Psychic,70 Recover,75 Mist,81 Amnesia,99 Calm Mind,110 Hypersonic Blast,150 Burning Rage
- Pound,10 Transform,20 Mega Punch,30 Metronome,40 Psychic,69 Hydro Pump,85 Calm Mind,115 Hypersonic Blast,150 Burning Rage
1 Psychic,2 Solar Beam,3 Hypersonic Blast,4 Burning Rage)");

std::vector<std::vector<LevelUpMove>> LevelUpMove::LEVEL_UP_MOVES;

void LevelUpMove::buildLevelUpMoves() {
    LEVEL_UP_MOVES.clear();
    LEVEL_UP_MOVES.resize(LEVEL_UP_STRINGS.size());
    for (size_t i = 0; i < LEVEL_UP_STRINGS.size(); i++) {
        std::vector<std::string> a = utils::split(LEVEL_UP_STRINGS[i], ',');
        LEVEL_UP_MOVES[i].clear();
        LEVEL_UP_MOVES[i].reserve(a.size());
        for (const std::string &s : a) {
            LEVEL_UP_MOVES[i].emplace_back(utils::trim(s));
        }
    }
}

LevelUpMove::LevelUpMove(const std::string &s) {
    size_t i = s.find(' ');
    std::string l = s.substr(0, i);
    level = (l == "-" ? 1 : std::stoi(l));
    move = Move::MOVE_MAP[s.substr(i + 1)];
}

std::string LevelUpMove::toString() const {
    return std::to_string(level) + "=" + (move ? move->name : "");
}
