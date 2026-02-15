#include "Monster.h"
#include "Utils.h"

static std::vector<std::string> splitLinesMonster(const std::string &s) {
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

std::vector<std::string> Monster::MONSTER_STRINGS = splitLinesMonster(R"(Missingno.,Dragon/Psychic,300,300,300,300,300,300
Bulbasaur,Grass/Poison,45,49,49,65,65,45
Ivysaur,Grass/Poison,60,62,63,80,80,60
Venusaur,Grass/Poison,80,82,83,100,100,80
Charmander,Fire,39,52,43,60,50,65
Charmeleon,Fire,58,64,58,80,65,80
Charizard,Fire/Flying,78,84,78,109,85,100
Squirtle,Water,44,48,65,50,64,43
Wartortle,Water,59,63,80,65,80,58
Blastoise,Water,79,83,100,85,105,78
Caterpie,Bug,45,30,35,20,20,45
Metapod,Bug,50,20,55,25,25,30
Butterfree,Bug/Flying,60,45,50,90,80,70
Weedle,Bug/Poison,40,35,30,20,20,50
Kakuna,Bug/Poison,45,25,50,25,25,35
Beedrill,Bug/Poison,65,90,40,45,80,75
Pidgey,Normal/Flying,40,45,40,35,35,56
Pidgeotto,Normal/Flying,63,60,55,50,50,71
Pidgeot,Normal/Flying,83,80,75,70,70,101
Rattata,Normal,30,56,35,25,35,72
Raticate,Normal,55,81,60,50,70,97
Spearow,Normal/Flying,40,60,30,31,31,70
Fearow,Normal/Flying,65,90,65,61,61,100
Ekans,Poison,35,60,44,40,54,55
Arbok,Poison,60,95,69,65,79,80
Pikachu,Electric,35,55,40,50,50,90
Raichu,Electric,60,90,55,90,80,110
Sandshrew,Ground,50,75,85,20,30,40
Sandslash,Ground,75,100,110,45,55,65
Nidoran♀,Poison,55,47,52,40,40,41
Nidorina,Poison,70,62,67,55,55,56
Nidoqueen,Poison/Ground,90,92,87,75,85,76
Nidoran♂,Poison,46,57,40,40,40,50
Nidorino,Poison,61,72,57,55,55,65
Nidoking,Poison/Ground,81,102,77,85,75,85
Clefairy,Normal,70,45,48,60,65,35
Clefable,Normal,95,70,73,95,90,60
Vulpix,Fire,38,41,40,50,65,65
Ninetales,Fire,73,76,75,81,100,100
Jigglypuff,Normal,115,45,20,45,25,20
Wigglytuff,Normal,140,70,45,85,50,45
Zubat,Poison/Flying,40,45,35,30,40,55
Golbat,Poison/Flying,75,80,70,65,75,90
Oddish,Grass/Poison,45,50,55,75,65,30
Gloom,Grass/Poison,60,65,70,85,75,40
Vileplume,Grass/Poison,75,80,85,110,90,50
Paras,Bug/Grass,35,70,55,45,55,25
Parasect,Bug/Grass,60,95,80,60,80,30
Venonat,Bug/Poison,60,55,50,40,55,45
Venomoth,Bug/Poison,70,65,60,90,75,90
Diglett,Ground,10,55,25,35,45,95
Dugtrio,Ground,35,100,50,50,70,120
Meowth,Normal,40,45,35,40,40,90
Persian,Normal,65,70,60,65,65,115
Psyduck,Water,50,52,48,65,50,55
Golduck,Water,80,82,78,95,80,85
Mankey,Fighting,40,80,35,35,45,70
Primeape,Fighting,65,105,60,60,70,95
Growlithe,Fire,55,70,45,70,50,60
Arcanine,Fire,90,110,80,100,80,95
Poliwag,Water,40,50,40,40,40,90
Poliwhirl,Water,65,65,65,50,50,90
Poliwrath,Water/Fighting,90,95,95,70,90,70
Abra,Psychic,25,20,15,105,55,90
Kadabra,Psychic,40,35,30,120,70,105
Alakazam,Psychic,55,50,45,135,95,120
Machop,Fighting,70,80,50,35,35,35
Machoke,Fighting,80,100,70,50,60,45
Machamp,Fighting,90,130,80,65,85,55
Bellsprout,Grass/Poison,50,75,35,70,30,40
Weepinbell,Grass/Poison,65,90,50,85,45,55
Victreebel,Grass/Poison,80,105,65,100,70,70
Tentacool,Water/Poison,40,40,35,50,100,70
Tentacruel,Water/Poison,80,70,65,80,120,100
Geodude,Rock/Ground,40,80,100,30,30,20
Graveler,Rock/Ground,55,95,115,45,45,35
Golem,Rock/Ground,80,120,130,55,65,45
Ponyta,Fire,50,85,55,65,65,90
Rapidash,Fire,65,100,70,80,80,105
Slowpoke,Water/Psychic,90,65,65,40,40,15
Slowbro,Water/Psychic,95,75,110,100,80,30
Magnemite,Electric,25,35,70,95,55,45
Magneton,Electric,50,60,95,120,70,70
Farfetch'd,Normal/Flying,52,90,55,58,62,60
Doduo,Normal/Flying,35,85,45,35,35,75
Dodrio,Normal/Flying,60,110,70,60,60,110
Seel,Water,65,45,55,45,70,45
Dewgong,Water/Ice,90,70,80,70,95,70
Grimer,Poison,80,80,50,40,50,25
Muk,Poison,105,105,75,65,100,50
Shellder,Water,30,65,100,45,25,40
Cloyster,Water/Ice,50,95,180,85,45,70
Gastly,Ghost/Poison,30,35,30,100,35,80
Haunter,Ghost/Poison,45,50,45,115,55,95
Gengar,Ghost/Poison,60,65,60,130,75,110
Onix,Rock/Ground,35,45,160,30,45,70
Drowzee,Psychic,60,48,45,43,90,42
Hypno,Psychic,85,73,70,73,115,67
Krabby,Water,30,105,90,25,25,50
Kingler,Water,55,130,115,50,50,75
Voltorb,Electric,40,30,50,55,55,100
Electrode,Electric,60,50,70,80,80,150
Exeggcute,Grass/Psychic,60,40,80,60,45,40
Exeggutor,Grass/Psychic,95,95,85,125,75,55
Cubone,Ground,50,50,95,40,50,35
Marowak,Ground,60,80,110,50,80,45
Hitmonlee,Fighting,50,120,53,35,110,87
Hitmonchan,Fighting,50,105,79,35,110,76
Lickitung,Normal,90,55,75,60,75,30
Koffing,Poison,40,65,95,60,45,35
Weezing,Poison,65,90,120,85,70,60
Rhyhorn,Ground/Rock,80,85,95,30,30,25
Rhydon,Ground/Rock,105,130,120,45,45,40
Chansey,Normal,250,5,5,35,105,50
Tangela,Grass,65,55,115,100,40,60
Kangaskhan,Normal,105,95,80,40,80,90
Horsea,Water,30,40,70,70,25,60
Seadra,Water,55,65,95,95,45,85
Goldeen,Water,45,67,60,35,50,63
Seaking,Water,80,92,65,65,80,68
Staryu,Water,30,45,55,70,55,85
Starmie,Water/Psychic,60,75,85,100,85,115
Mr. Mime,Psychic,40,45,65,100,120,90
Scyther,Bug/Flying,70,110,80,55,80,105
Jynx,Ice/Psychic,65,50,35,115,95,95
Electabuzz,Electric,65,83,57,95,85,105
Magmar,Fire,65,95,57,100,85,93
Pinsir,Bug,65,125,100,55,70,85
Tauros,Normal,75,100,95,40,70,110
Magikarp,Water,20,10,55,15,20,80
Gyarados,Water/Flying,95,125,79,60,100,81
Lapras,Water/Ice,130,85,80,85,95,60
Ditto,Normal,48,48,48,48,48,48
Eevee,Normal,55,55,50,45,65,55
Vaporeon,Water,130,65,60,110,95,65
Jolteon,Electric,65,65,60,110,95,130
Flareon,Fire,65,130,60,95,110,65
Porygon,Normal,65,60,70,85,75,40
Omanyte,Rock/Water,35,40,100,90,55,35
Omastar,Rock/Water,70,60,125,115,70,55
Kabuto,Rock/Water,30,80,90,55,45,55
Kabutops,Rock/Water,60,115,105,65,70,80
Aerodactyl,Rock/Flying,80,105,65,60,75,130
Snorlax,Normal,160,110,65,65,110,30
Articuno,Ice/Flying,90,85,100,95,125,85
Zapdos,Electric/Flying,90,90,85,125,90,100
Moltres,Fire/Flying,90,100,90,125,85,90
Dratini,Dragon,41,64,45,50,50,50
Dragonair,Dragon,61,84,65,70,70,70
Dragonite,Dragon/Flying,91,134,95,100,100,80
Mewtwo,Psychic,106,110,90,154,90,130
Mew,Psychic,100,100,100,100,100,100
Voidwalker,Psychic/Normal,200,200,200,200,200,200)");

std::vector<Monster> Monster::MONSTERS;
std::unordered_map<std::string, Monster*> Monster::MONSTER_MAP;

Monster::Monster() {
    name = "";
}

Monster::Monster(const std::string &s, int dexNum) {
    std::vector<std::string> a = utils::split(s, ',');
    name = a[0];
    types = utils::split(a[1], '/');
    hp = std::stoi(a[2]);
    atk = std::stoi(a[3]);
    def = std::stoi(a[4]);
    spatk = std::stoi(a[5]);
    spdef = std::stoi(a[6]);
    spd = std::stoi(a[7]);
    this->dexNum = dexNum;
    MONSTER_MAP[name] = this;
}

void Monster::buildMonsters() {
    MONSTERS.clear();
    MONSTERS.reserve(MONSTER_STRINGS.size());
    for (int i = 0; i < static_cast<int>(MONSTER_STRINGS.size()); i++) {
        MONSTERS.emplace_back(MONSTER_STRINGS[i], i);
    }
}

void Monster::addMoves(const std::string &s) {
    for (const std::string &m : utils::split(s, ',')) {
        learnable.insert(Move::MOVE_MAP[m]);
    }
}

std::string Monster::toString() const {
    std::ostringstream ss;
    ss << dexNum << ": " << name << " (" << (types.empty() ? "" : types[0]);
    for (size_t i = 1; i < types.size(); i++) {
        ss << "/" << types[i];
    }
    ss << ")\n\tHP=" << hp << "\n\tAttack=" << atk << "\n\tDefense=" << def << "\n\tSpecial Attack=" << spatk
       << "\n\tSpecial Defense=" << spdef << "\n\tSpeed=" << spd;
    return ss.str();
}
