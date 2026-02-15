#include "Trainer.h"
#include <iostream>
#include "PokeMap.h"
#include "NpcStrings.h"
#include "GameConfig.h"
#include "Player.h"
#include "Utils.h"

static std::vector<std::string> splitLinesTrainer(const std::string &s) {
    std::vector<std::string> lines;
    std::stringstream ss(s);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::string> Trainer::TEAM_STRINGS = splitLinesTrainer(R"(Route3;COOLTRAINER_F,16,9,LEFT,Lass,135,9 Pidgey,9 Pidgey;YOUNGSTER,10,6,RIGHT,Bug Catcher,100,10 Caterpie,10 Weedle,10 Caterpie;YOUNGSTER,14,4,DOWN,Youngster,165,11 Rattata,11 Ekans;YOUNGSTER,19,5,DOWN,Bug Catcher,90,9 Weedle,9 Kakuna,9 Caterpie,9 Metapod;COOLTRAINER_F,23,4,LEFT,Lass,150,10 Rattata,10 Nidoran♂;YOUNGSTER,24,6,RIGHT,Youngster,210,14 Spearow;YOUNGSTER,22,9,LEFT,Bug Catcher,110,11 Caterpie,11 Metapod;COOLTRAINER_F,33,10,UP,Lass,210,14 Jigglypuff
Route6;YOUNGSTER,0,15,RIGHT,Bug Catcher,160,16 Weedle,16 Caterpie,16 Weedle;COOLTRAINER_M,10,21,RIGHT,Jr. Trainer♂,400,20 Squirtle;COOLTRAINER_F,11,21,LEFT,Jr. Trainer♀,400,16 Rattata,16 Pikachu;YOUNGSTER,19,26,LEFT,Bug Catcher,200,20 Butterfree;COOLTRAINER_F,11,30,LEFT,Jr. Trainer♀,320,16 Pidgey,16 Pidgey,16 Pidgey;COOLTRAINER_M,11,31,LEFT,Jr. Trainer♂,380,16 Spearow,16 Raticate
Route8;COOLTRAINER_F,51,12,LEFT,Lass,330,22 Clefairy,22 Clefairy;GAMBLER,46,13,DOWN,Gambler,1680,24 Growlithe,24 Vulpix;SUPER_NERD,42,6,UP,Super Nerd,550,22 Grimer,22 Muk,22 Grimer;COOLTRAINER_F,26,3,LEFT,Lass,345,23 Nidoran♀,23 Nidorina;SUPER_NERD,26,4,RIGHT,Super Nerd,650,26 Koffing;COOLTRAINER_F,26,5,LEFT,Lass,360,24 Meowth,24 Meowth,24 Meowth;COOLTRAINER_F,26,6,RIGHT,Lass,285,19 Pidgey,19 Rattata,19 Nidoran♂,19 Meowth,19 Pikachu;GAMBLER,13,9,UP,Gambler,1540,22 Poliwag,22 Poliwag,22 Poliwhirl;SUPER_NERD,8,5,RIGHT,Super Nerd,550,20 Voltorb,20 Koffing,20 Voltorb,20 Magnemite
Route9;COOLTRAINER_F,13,10,LEFT,Jr. Trainer♀,360,18 Oddish,18 Bellsprout,18 Oddish,18 Bellsprout;HIKER,16,15,LEFT,Hiker,700,20 Machop,20 Onix;COOLTRAINER_M,24,7,LEFT,Jr. Trainer♂,420,21 Growlithe,21 Charmander;YOUNGSTER,22,2,DOWN,Bug Catcher,190,19 Beedrill,19 Beedrill;YOUNGSTER,40,8,RIGHT,Bug Catcher,200,20 Caterpie,20 Weedle,20 Venonat;COOLTRAINER_M,31,7,RIGHT,Jr. Trainer♂,380,19 Rattata,19 Diglett,19 Ekans,19 Sandshrew;HIKER,45,15,RIGHT,Hiker,735,21 Geodude,21 Onix;HIKER,43,3,LEFT,Hiker,700,20 Geodude,20 Machop,20 Geodude;COOLTRAINER_F,48,8,RIGHT,Jr. Trainer♀,460,23 Meowth
Route10;COOLTRAINER_F,7,25,LEFT,Jr. Trainer♀,360,20 Pikachu,20 Clefairy;SUPER_NERD,10,44,LEFT,Jr. Trainer♀,420,21 Pidgey,21 Pidgeotto;HIKER,3,57,UP,Hiker,735,21 Geodude,21 Onix;HIKER,3,61,DOWN,Hiker,665,19 Onix,19 Graveler;SUPER_NERD,14,64,LEFT,Pokemaniac,1000,20 Cubone,20 Slowpoke
)" R"(
Route11;GAMBLER,10,14,DOWN,Gambler,1260,18 Poliwag,18 Horsea;YOUNGSTER,13,5,LEFT,Youngster,315,21 Ekans;YOUNGSTER,22,4,UP,Youngster,285,19 Sandshrew,19 Zubat;YOUNGSTER,22,12,UP,Youngster,270,18 Nidoran♂,18 Nidorino;GAMBLER,26,9,DOWN,Gambler,1260,18 Bellsprout,18 Oddish;GAMBLER,33,3,UP,Gambler,1260,18 Growlithe,18 Vulpix;SUPER_NERD,36,11,DOWN,Engineer,1050,21 Magnemite;YOUNGSTER,43,5,RIGHT,Youngster,225,17 Rattata,17 Rattata,17 Raticate;GAMBLER,45,7,DOWN,Gambler,1260,18 Voltorb,18 Magnemite;SUPER_NERD,45,16,LEFT,Engineer,900,18 Magnemite,18 Magnemite,18 Magnemite
Route12;FISHER,14,31,LEFT,Fisherman,770,22 Goldeen,22 Poliwag,22 Goldeen;FISHER,5,39,UP,Fisherman,840,24 Tentacool,24 Goldeen;FISHER,12,40,LEFT,Fisherman,945,27 Goldeen;FISHER,9,52,RIGHT,Fisherman,735,21 Poliwag,21 Shellder,21 Goldeen,21 Horsea;SUPER_NERD,14,76,UP,Rocker,725,29 Voltorb,29 Electrode;FISHER,6,87,DOWN,Fisherman,840,24 Magikarp,25 Magikarp;COOLTRAINER_M,11,92,LEFT,Jr. Trainer,840,29 Nidoran♂,29 Nidorino
Route13;COOLTRAINER_F,50,5,DOWN,Jr. Trainer♀,560,28 Goldeen,28 Poliwag,28 Horsea;COOLTRAINER_M,49,10,RIGHT,Birdkeeper,725,29 Pidgey,29 Pidgeotto;COOLTRAINER_F,48,10,DOWN,Jr. Trainer♀,3500,24 Pidgey,24 Meowth,24 Rattata,24 Pikachu,32 Persian;BEAUTY,33,6,DOWN,Beauty,1890,27 Rattata,27 Pikachu,27 Rattata;BEAUTY,32,6,DOWN,Beauty,2030,29 Clefairy,29 Meowth;COOLTRAINER_F,27,9,DOWN,Jr. Trainer♀,600,30 Poliwag,30 Poliwag;COOLTRAINER_F,23,10,LEFT,Jr. Trainer♀,540,27 Pidgey,27 Meowth,27 Pidgey,27 Pidgeotto;COOLTRAINER_M,7,13,UP,Birdkeeper,625,25 Spearow,25 Pidgey,25 Pidgey,25 Spearow,25 Spearow;BIKER,10,7,UP,Biker,700,28 Koffing,28 Koffing,28 Koffing;COOLTRAINER_M,12,4,RIGHT,Birdkeeper,4595,45 Pidgey,43 Pidgey,27 Meowth,14 Growlithe,7 Oddish,65 Magikarp
Route14;BIKER,15,30,LEFT,Biker,580,29 Koffing,29 Muk;COOLTRAINER_M,15,31,LEFT,Birdkeeper,825,33 Farfetch'd;BIKER,5,39,DOWN,Biker,560,28 Grimer,28 Grimer,28 Koffing;BIKER,4,31,RIGHT,Biker,580,29 Koffing,29 Grimer;BIKER,4,30,RIGHT,Biker,520,26 Koffing,26 Koffing,26 Grimer,26 Koffing;COOLTRAINER_M,6,49,UP,Birdkeeper,725,29 Spearow,29 Fearow;COOLTRAINER_M,14,15,UP,Birdkeeper,650,26 Pidgey,26 Spearow,26 Pidgey,26 Fearow;COOLTRAINER_M,12,11,DOWN,Birdkeeper,650,26 Pidgey,26 Spearow,26 Pidgey,26 Fearow;COOLTRAINER_M,15,6,DOWN,Birdkeeper,700,28 Pidgey,28 Pidgeotto,28 Doduo;COOLTRAINER_M,4,4,DOWN,Birdkeeper,2725,36 Pidgeot,38 Fearow
Route15;COOLTRAINER_F,53,10,LEFT,Jr. Trainer♀,580,29 Pikachu,29 Raichu;BEAUTY,53,11,DOWN,Beauty,2030,29 Pidgeotto,29 Wigglytuff;BIKER,48,10,DOWN,Biker,500,25 Koffing,25 Koffing,25 Weezing,25 Koffing,25 Grimer;BIKER,46,10,DOWN,Biker,560,28 Koffing,28 Grimer,28 Weezing;BEAUTY,41,10,RIGHT,Beauty,2030,29 Bulbasaur,29 Ivysaur;COOLTRAINER_F,41,11,DOWN,Jr. Trainer♀,560,28 Gloom,28 Oddish,28 Oddish;COOLTRAINER_F,37,5,RIGHT,Jr. Trainer♀,660,33 Clefairy;COOLTRAINER_M,35,13,UP,Birdkeeper,725,28 Doduo,28 Doduo,28 Doduo;COOLTRAINER_M,31,13,UP,Birdkeeper,650,26 Pidgeotto,26 Farfetch'd,26 Doduo,26 Pidgey;COOLTRAINER_F,18,13,UP,Jr. Trainer♀,580,29 Bellsprout,29 Oddish,29 Tangela
Route16;BIKER,17,12,LEFT,Biker,580,29 Grimer,29 Koffing;BIKER,11,12,UP,Cue Ball,700,28 Machop,28 Mankey,28 Machop;BIKER,9,11,LEFT,Cue Ball,725,29 Mankey,28 Machop;BIKER,6,10,RIGHT,Biker,660,33 Weezing;BIKER,3,12,RIGHT,Cue Ball,825,33 Machop;BIKER,14,13,RIGHT,Biker,520,26 Grimer,26 Grimer,26 Grimer,26 Grimer
Route17;BIKER,11,16,RIGHT,Biker,560,28 Weezing,28 Koffing,28 Weezing;BIKER,4,18,UP,Cue Ball,725,29 Machop,29 Machoke;BIKER,7,32,LEFT,Cue Ball,725,29 Mankey,29 Primeape;BIKER,14,34,RIGHT,Biker,660,33 Muk;BIKER,2,68,RIGHT,Biker,580,29 Voltorb,29 Voltorb;BIKER,14,98,RIGHT,Cue Ball,825,33 Machoke;BIKER,10,118,DOWN,Cue Ball,650,29 Mankey,29 Mankey,29 Machoke,29 Machop;BIKER,5,98,LEFT,Cue Ball,725,29 Primeape,29 Machoke;BIKER,17,58,LEFT,Biker,580,29 Weezing,29 Muk;BIKER,12,19,LEFT,Biker,500,29 Koffing,29 Weezing,29 Koffing,29 Koffing,29 Weezing
Route18;COOLTRAINER_M,42,13,LEFT,Birdkeeper,725,29 Spearow,29 Fearow;COOLTRAINER_M,40,15,LEFT,Birdkeeper,650,26 Spearow,26 Spearow,26 Fearow,26 Spearow;COOLTRAINER_M,36,11,RIGHT,Birdkeeper,850,34 Dodrio
)" R"(
Route19;SWIMMER,9,11,DOWN,Swimmer,145,29 Goldeen,29 Horsea,29 Staryu;SWIMMER,13,25,LEFT,Swimmer,150,30 Tentacool,30 Shellder;SWIMMER,4,27,RIGHT,Swimmer,150,30 Horsea,30 Horsea;SWIMMER,16,31,UP,Swimmer,150,30 Poliwag,30 Poliwhirl;SWIMMER,9,42,UP,Swimmer,135,27 Horsea,27 Tentacool,27 Tentacool,27 Goldeen;SWIMMER,8,43,LEFT,Swimmer,145,29 Goldeen,29 Shellder,29 Seaking;SWIMMER,11,43,RIGHT,Beauty,2100,30 Goldeen,30 Seaking;SWIMMER,10,44,DOWN,Beauty,2030,29 Staryu,29 Staryu,29 Staryu;COOLTRAINER_M,8,7,LEFT,Swimmer,135,27 Tentacool,27 Tentacool,27 Staryu,27 Horsea,27 Tentacruel;COOLTRAINER_M,13,7,LEFT,Beauty,1890,27 Poliwag,27 Goldeen,27 Seaking,27 Goldeen,27 Poliwag
Route20;SWIMMER,15,8,UP,Swimmer,140,28 Horsea,28 Horsea,28 Seadra,28 Horsea;SWIMMER,24,12,DOWN,Swimmer,155,31 Shellder,31 Cloyster;SWIMMER,25,7,UP,Beauty,2100,30 Seadra,30 Horsea,30 Seadra;SWIMMER,38,13,DOWN,Beauty,2100,35 Seaking;SWIMMER,55,14,RIGHT,Jr. Trainer♀,4000,40 Tentacruel,41 Arcanine,46 Alakazam,43 Jolteon,45 Chansey,70 Magikarp;SWIMMER,45,10,DOWN,Swimmer,175,35 Staryu;COOLTRAINER_M,34,9,UP,Birdkeeper,750,30 Fearow,30 Fearow,30 Pidgeotto;SWIMMER,87,8,UP,Jr. Trainer♀,620,31 Goldeen,31 Seaking;SWIMMER,87,13,UP,Beauty,2170,31 Poliwag,31 Seaking
Route21;FISHER,6,25,DOWN,Fisherman,980,28 Seaking,28 Goldeen,28 Seaking,28 Seaking;FISHER,14,56,LEFT,Fisherman,945,27 Magikarp,27 Magikarp,27 Magikarp,27 Magikarp,27 Magikarp,27 Magikarp;FISHER,17,57,RIGHT,Cue Ball,945,31 Tentacool,31 Tentacool,31 Tentacool;SWIMMER,10,31,UP,Swimmer,165,33 Seadra,33 Tentacruel;FISHER,4,24,LEFT,Fisherman,1085,31 Shellder,31 Cloyster;SWIMMER,12,30,RIGHT,Fisherman,1155,33 Seaking,33 Goldeen;SWIMMER,5,71,RIGHT,Swimmer,185,37 Starmie;SWIMMER,15,71,LEFT,Swimmer,160,32 Poliwhirl,32 Tentacool,32 Seadra;SWIMMER,16,63,DOWN,Swimmer,165,33 Staryu,33 Wartortle
Route24;YOUNGSTER,11,31,LEFT,Bug Catcher,140,14 Caterpie,14 Weedle;COOLTRAINER_F,10,28,RIGHT,Lass,210,14 Pidgey,14 Nidoran♀;YOUNGSTER,11,25,LEFT,Youngster,210,14 Rattata,14 Ekans,14 Zubat;COOLTRAINER_F,10,22,RIGHT,Lass,240,16 Pidgey,16 Nidoran♀;COOLTRAINER_M,11,19,LEFT,Jr. Trainer♂,350,18 Mankey;COOLTRAINER_M,11,15,LEFT,Rocket,450,15 Ekans,15 Zubat;COOLTRAINER_M,5,20,UP,Jr. Trainer♂,350,14 Rattata,14 Ekans
Route25;HIKER,8,4,RIGHT,Hiker,525,15 Machop,15 Geodude;HIKER,13,7,RIGHT,Hiker,595,17 Onix;YOUNGSTER,14,2,DOWN,Youngster,225,15 Rattata,15 Spearow;YOUNGSTER,18,5,UP,Youngster,255,17 Slowpoke;COOLTRAINER_F,18,8,RIGHT,Lass,255,15 Nidoran♂,15 Nidoran♀;HIKER,23,9,UP,Hiker,545,13 Geodude,13 Geodude,13 Machop,13 Geodude;COOLTRAINER_M,24,4,DOWN,Jr. Trainer♂,280,14 Rattata,14 Ekans;YOUNGSTER,32,3,LEFT,Youngster,210,14 Ekans,14 Sandshrew;COOLTRAINER_F,37,4,DOWN,Lass,195,13 Oddish,13 Pidgey,13 Oddish
CeladonGym;COOLTRAINER_F,2,11,RIGHT,Lass,345,23 Bellsprout,23 Weepinbell;BEAUTY,7,10,LEFT,Beauty,1680,24 Bellsprout,24 Bellsprout;BEAUTY,1,5,DOWN,Beauty,1470,21 Oddish,21 Bellsprout,21 Oddish,21 Bellsprout;COOLTRAINER_F,9,5,DOWN,Jr. Trainer,480,24 Bulbasaur,24 Ivysaur;BEAUTY,3,3,DOWN,Beauty,1820,26 Exeggcute;COOLTRAINER_F,5,3,DOWN,Cooltrainer♀,840,24 Weepinbell,24 Gloom,24 Ivysaur;COOLTRAINER_F,6,3,DOWN,Lass,345,23 Oddish,23 Gloom
CeruleanCity;ROCKET,33,12,DOWN,Rocket,525,17 Machop,17 Drowzee
CeruleanGym;SWIMMER,8,7,LEFT,Swimmer,320,16 Horsea,16 Shellder;COOLTRAINER_F,2,3,RIGHT,Jr. Trainer,380,19 Goldeen
FuchsiaGym;ROCKER,1,12,DOWN,Juggler,1190,34 Drowzee,34 Kadabra;ROCKER,8,13,DOWN,Juggler,1330,38 Hypno;ROCKER,7,8,RIGHT,Juggler,1085,31 Drowzee,31 Drowzee,31 Kadabra,31 Drowzee;ROCKER,8,2,DOWN,Tamer,1320,33 Arbok,33 Sandslash,33 Arbok;ROCKER,3,5,UP,Tamer,1630,34 Sandslash,34 Arbok;ROCKER,2,7,LEFT,Juggler,1190,34 Drowzee,34 Hypno
MtMoon1F;YOUNGSTER,7,22,DOWN,Bug Catcher,110,11 Weedle,11 Kakuna;COOLTRAINER_F,30,4,DOWN,Lass,210,14 Clefairy;SUPER_NERD,24,31,UP,Super Nerd,275,11 Magnemite,14 Voltorb;YOUNGSTER,30,27,RIGHT,Bug Catcher,100,10 Caterpie,10 Metapod,10 Caterpie;COOLTRAINER_F,16,23,DOWN,Lass,165,11 Oddish,11 Bellsprout;YOUNGSTER,12,16,RIGHT,Youngster,150,10 Rattata,10 Rattata,10 Zubat;HIKER,5,6,DOWN,Hiker,350,10 Geodude,10 Geodude,10 Onix
MtMoonB2F;ROCKET,29,11,UP,Rocket,330,11 Sandshrew,11 Rattata,11 Zubat;ROCKET,29,17,LEFT,Rocket,360,12 Zubat,12 Ekans;ROCKET,15,22,DOWN,Rocket,390,16 Raticate;ROCKET,11,16,DOWN,Rocket,390,13 Rattata,13 Zubat;SUPER_NERD,12,8,RIGHT,Super Nerd,300,12 Grimer,12 Voltorb,12 Koffing
PewterGym;COOLTRAINER_M,3,6,RIGHT,Jr. Trainer♂,220,11 Diglett,11 Sandshrew
RockTunnelB1F;SUPER_NERD,26,30,DOWN,Pokemaniac,1520,25 Slowpoke;COOLTRAINER_F,14,28,RIGHT,Jr. Trainer♀,440,22 Oddish,22 Bulbasaur;SUPER_NERD,20,21,RIGHT,Pokemaniac,1100,22 Charmander,22 Cubone;HIKER,30,10,DOWN,Hiker,875,20 Machop,20 Onix;HIKER,33,5,RIGHT,Hiker,700,25 Geodude;COOLTRAINER_F,11,13,DOWN,Jr. Trainer♀,420,21 Jigglypuff,21 Pidgey,21 Meowth;HIKER,6,10,DOWN,Hiker,735,21 Geodude,21 Geodude,21 Graveler;SUPER_NERD,3,5,DOWN,Pokemaniac,1000,20 Slowpoke,20 Slowpoke,20 Slowpoke
PokemonTower3F;CHANNELER,12,3,LEFT,Channeler,660,23 Gastly;CHANNELER,9,8,DOWN,Channeler,660,22 Gastly;CHANNELER,10,13,DOWN,Channeler,720,24 Gastly
PokemonTower4F;CHANNELER,15,7,DOWN,Channeler,690,23 Gastly,23 Gastly;CHANNELER,14,12,LEFT,Channeler,660,22 Gastly;CHANNELER,5,10,RIGHT,Channeler,720,24 Gastly
PokemonTower5F;CHANNELER,12,8,DOWN,Channeler,660,22 Gastly;CHANNELER,6,10,RIGHT,Channeler,720,24 Gastly;CHANNELER,17,7,LEFT,Channeler,660,22 Haunter;CHANNELER,9,16,RIGHT,Channeler,690,23 Haunter
PokemonTower6F;CHANNELER,12,10,RIGHT,Channeler,660,22 Gastly,22 Gastly,22 Gastly;CHANNELER,16,5,LEFT,Channeler,720,24 Gastly;CHANNELER,9,5,DOWN,Channeler,720,24 Gastly
PokemonTower7F;ROCKET,9,11,RIGHT,Rocket,750,25 Zubat,25 Zubat,25 Golbat;ROCKET,12,9,LEFT,Rocket,780,26 Koffing,26 Drowzee;ROCKET,9,7,RIGHT,Rocket,690,23 Zubat,23 Rattata,23 Raticate,23 Zubat
RockTunnel1F;SUPER_NERD,23,8,LEFT,Pokemaniac,1150,23 Cubone,23 Slowpoke;HIKER,7,5,DOWN,Hiker,665,19 Geodude,19 Machop,19 Geodude,19 Geodude;HIKER,5,16,DOWN,Hiker,700,20 Onix,20 Onix,20 Geodude;HIKER,17,15,LEFT,Hiker,735,21 Geodude,21 Graveler;COOLTRAINER_F,37,21,LEFT,Jr. Trainer♀,440,22 Bellsprout,22 Clefairy;COOLTRAINER_F,32,24,RIGHT,Jr. Trainer♀,380,19 Pidgey,19 Rattata,19 Rattata,19 Bellsprout;COOLTRAINER_F,22,24,DOWN,Jr. Trainer♀,1700,26 Kadabra,20 Oddish,20 Pidgey
FightingDojo;HIKER,5,7,LEFT,Blackbelt,775,31 Machop,31 Mankey,31 Primeape;HIKER,3,6,RIGHT,Blackbelt,800,32 Machop,32 Machoke;HIKER,5,5,LEFT,Blackbelt,900,36 Primeape;HIKER,3,4,RIGHT,Blackbelt,775,31 Mankey,31 Mankey,31 Primeape;HIKER,5,3,LEFT,Blackbelt,925,50 Hitmonlee,50 Hitmonchan
SaffronGym;YOUNGSTER,17,13,DOWN,Psychic,330,33 Slowpoke,33 Slowpoke,33 Slowbro;YOUNGSTER,17,7,DOWN,Psychic,340,34 Mr. Mime,34 Kadabra;CHANNELER,10,1,DOWN,Channeler,1140,38 Gastly;YOUNGSTER,3,1,DOWN,Psychic,380,38 Slowbro;CHANNELER,3,7,DOWN,Channeler,1020,34 Gastly,34 Haunter;CHANNELER,3,13,DOWN,Channeler,990,33 Gastly,33 Gastly,33 Haunter;YOUNGSTER,17,1,DOWN,Psychic,310,31 Kadabra,31 Slowpoke,31 Mr. Mime,31 Kadabra
SilphCo2F;ROCKET,24,7,UP,Rocket,750,25 Golbat,25 Zubat,25 Zubat,25 Raticate,25 Zubat;SCIENTIST,5,12,DOWN,Scientist,1400,28 Magnemite,28 Voltorb,28 Magneton;ROCKET,16,11,UP,Rocket,870,29 Cubone,29 Zubat;SCIENTIST,24,13,LEFT,Scientist,1300,26 Grimer,26 Weezing,26 Koffing,26 Weezing
SilphCo3F;ROCKET,20,7,LEFT,Rocket,840,28 Raticate,28 Hypno,28 Raticate;SCIENTIST,7,9,DOWN,Scientist,1450,29 Electrode,29 Weezing
SilphCo4F;ROCKET,9,14,RIGHT,Rocket,840,28 Ekans,28 Zubat,28 Cubone;ROCKET,26,10,UP,Rocket,870,29 Machop,29 Drowzee;SCIENTIST,14,6,LEFT,Scientist,1650,33 Electrode
SilphCo5F;ROCKET,8,16,RIGHT,Rocket,990,33 Hypno;ROCKER,18,10,UP,Juggler,1015,29 Kadabra,29 Mr. Mime;SCIENTIST,8,3,RIGHT,Scientist,1450,26 Magneton,26 Koffing,26 Weezing,26 Magnemite;ROCKET,28,4,UP,Rocket,990,33 Arbok
SilphCo6F;ROCKET,17,3,RIGHT,Rocket,870,29 Machop,29 Machoke;SCIENTIST,7,8,DOWN,Scientist,1250,25 Voltorb,25 Koffing,25 Magneton,25 Magnemite,25 Koffing;ROCKET,14,15,LEFT,Rocket,840,28 Zubat,28 Zubat,28 Golbat
SilphCo7F;ROCKET,20,2,LEFT,Rocket,870,29 Cubone,29 Cubone;ROCKET,19,14,RIGHT,Rocket,780,26 Raticate,26 Arbok,26 Koffing,26 Golbat;ROCKET,13,1,DOWN,Rocket,840,29 Sandshrew,29 Sandslash;SCIENTIST,2,13,DOWN,Scientist,1450,29 Electrode,29 Muk
)" R"(
SilphCo8F;ROCKET,19,2,LEFT,Rocket,780,26 Raticate,26 Zubat,26 Golbat,26 Rattata;ROCKET,12,15,RIGHT,Rocket,840,28 Weezing,28 Golbat,28 Koffing;SCIENTIST,10,2,DOWN,Scientist,1450,29 Grimer,29 Electrode
SilphCo9F;SCIENTIST,21,13,DOWN,Scientist,1400,28 Voltorb,28 Koffing,28 Magneton;ROCKET,13,16,UP,Rocket,840,28 Golbat,28 Drowzee,28 Hypno;ROCKET,2,4,UP,Rocket,840,28 Drowzee,28 Grimer,28 Machop
SilphCo10F;SCIENTIST,10,2,LEFT,Scientist,1450,29 Magnemite,29 Koffing;ROCKET,1,9,RIGHT,Rocket,840,33 Machoke
SilphCo11F;ROCKET,3,16,UP,Rocket,750,25 Rattata,25 Rattata,25 Zubat,25 Rattata,25 Ekans;ROCKET,15,9,UP,Rocket,960,32 Cubone,32 Drowzee,32 Marowak
SSAnne1FRooms;GENTLEMAN,1,3,DOWN,Gentleman,1260,18 Growlithe,18 Growlithe;GENTLEMAN,21,4,UP,Gentleman,1330,19 Nidoran♂,19 Nidoran♀;COOLTRAINER_F,8,1,LEFT,Lass,270,18 Pidgey,18 Nidoran♀;YOUNGSTER,6,4,UP,Youngster,315,21 Nidoran♂
SSAnneB1FRooms;SAILOR,7,3,LEFT,Sailor,595,21 Shellder;SAILOR,12,2,DOWN,Sailor,510,17 Horsea,17 Shellder,17 Tentacool;SAILOR,17,1,DOWN,Sailor,510,17 Horsea,17 Horsea,17 Horsea;SAILOR,15,3,DOWN,Sailor,540,18 Tentacool,18 Staryu;SAILOR,0,2,RIGHT,Sailor,595,17 Tentacool,17 Staryu,17 Shellder;SUPER_NERD,20,3,RIGHT,Sailor,600,20 Machop
SSAnne2FRooms;FISHER,8,4,LEFT,Fisherman,350,17 Goldeen,17 Tentacool,17 Goldeen;GENTLEMAN,11,2,DOWN,Gentleman,1640,23 Pikachu;GENTLEMAN,15,4,RIGHT,Gentleman,1190,17 Growlithe,17 Ponyta;COOLTRAINER_F,17,1,DOWN,Lass,315,18 Rattata,18 Pikachu
SSAnneBow;SAILOR,4,4,DOWN,Sailor,510,17 Machop,17 Tentacool;SAILOR,4,9,DOWN,Sailor,540,18 Machop,18 Shellder
GameCorner;ROCKET,17,5,UP,Rocket,630,20 Raticate,20 Zubat
RocketHideoutB1F;ROCKET,12,6,RIGHT,Rocket,630,21 Drowzee,21 Machop;ROCKET,26,8,LEFT,Rocket,630,21 Raticate,21 Raticate;ROCKET,28,18,LEFT,Rocket,600,20 Grimer,20 Koffing,20 Koffing;ROCKET,18,17,DOWN,Rocket,570,19 Rattata,19 Raticate,19 Raticate,19 Rattata;ROCKET,15,25,RIGHT,Rocket,660,22 Grimer,22 Koffing
RocketHideoutB2F;ROCKET,20,12,DOWN,Rocket,510,17 Zubat,17 Koffing,17 Grimer,17 Zubat,17 Raticate
RocketHideoutB3F;ROCKET,26,12,UP,Rocket,630,21 Machop,21 Machop;ROCKET,10,22,RIGHT,Rocket,600,20 Rattata,20 Raticate,20 Drowzee
RocketHideoutB4F;ROCKET,11,2,DOWN,Rocket,630,21 Koffing,21 Zubat;ROCKET,24,11,DOWN,Rocket,690,23 Sandshrew,23 Ekans,23 Sandslash;ROCKET,24,12,DOWN,Rocket,690,23 Ekans,23 Sandshrew,23 Arbok
VermilionGym;SUPER_NERD,3,8,LEFT,Rocker,500,20 Voltorb,20 Magnemite,20 Pikachu;SAILOR,0,10,RIGHT,Sailor,630,21 Pikachu,21 Pikachu;GENTLEMAN,9,6,LEFT,Gentleman,1610,23 Pikachu
ViridianGym;HIKER,3,7,LEFT,Blackbelt,1000,40 Machop,40 Machoke;COOLTRAINER_M,6,5,DOWN,Cooltrainer♂,1365,39 Sandslash,39 Dugtrio;ROCKER,10,7,DOWN,Tamer,1720,43 Rhyhorn;COOLTRAINER_M,12,7,DOWN,Cooltrainer♂,1505,43 Rhyhorn;HIKER,11,11,UP,Blackbelt,950,38 Machoke,38 Machop,38 Machoke;ROCKER,2,16,RIGHT,Tamer,1365,39 Arbok,39 Tauros;HIKER,10,1,DOWN,Blackbelt,1075,43 Machoke;COOLTRAINER_M,13,5,RIGHT,Cooltrainer♂,1365,39 Nidorino,39 Nidoking
ViridianForest;YOUNGSTER,30,33,LEFT,Bug Catcher,60,6 Weedle,6 Caterpie;YOUNGSTER,30,19,LEFT,Bug Catcher,70,7 Weedle,7 Kakuna,7 Weedle;YOUNGSTER,2,18,LEFT,Bug Catcher,60,9 Weedle
VictoryRoad1F;COOLTRAINER_F,7,5,RIGHT,Cooltrainer♀,2000,44 Persian,44 Ninetales;COOLTRAINER_M,3,2,DOWN,Cooltrainer♂,4950,42 Ivysaur,42 Wartortle,42 Charmeleon,46 Charizard,46 Venusaur,46 Blastoise
VictoryRoad2F;HIKER,12,9,LEFT,Blackbelt,3350,43 Machoke,43 Machop,47 Machamp;SUPER_NERD,21,13,LEFT,Juggler,6625,41 Drowzee,41 Hypno,41 Kadabra,49 Alakazam,53 Slowpoke,89 Magikarp;COOLTRAINER_M,19,8,DOWN,Tamer,2950,44 Persian,44 Golduck;SUPER_NERD,4,2,DOWN,Juggler,1370,48 Mr. Mime;SUPER_NERD,26,3,LEFT,Pokemaniac,3150,40 Charmeleon,40 Lapras,40 Lickitung
VictoryRoad3F;COOLTRAINER_M,28,5,LEFT,Cooltrainer♂,3333,43 Exeggutor,43 Cloyster,43 Arcanine;COOLTRAINER_F,7,13,RIGHT,Cave Boss,8888,63 Parasect,53 Dewgong,43 Chansey,33 Growlithe,98 Magikarp,57 Gyarados;COOLTRAINER_M,6,14,LEFT,Cooltrainer♂,3876,43 Kingler,43 Tentacruel,43 Blastoise;COOLTRAINER_F,13,3,RIGHT,Cooltrainer♀,2890,43 Bellsprout,43 Weepinbell,43 Victreebel
CinnabarGym;SUPER_NERD,17,2,DOWN,Burglar,2700,36 Growlithe,36 Vulpix,36 Ninetales;SUPER_NERD,17,8,DOWN,Super Nerd,2700,36 Vulpix,36 Vulpix,36 Ninetales;SUPER_NERD,11,4,DOWN,Super Nerd,2850,34 Ponyta,34 Charmander,34 Vulpix,34 Growlithe;SUPER_NERD,11,8,DOWN,Burglar,1700,41 Ponyta;SUPER_NERD,11,14,DOWN,Super Nerd,1900,41 Rapidash;SUPER_NERD,3,14,DOWN,PokeBurglar,9595,49 Vulpix,59 Growlithe,49 Wartortle,59 Dragonite,92 Magikarp,81 Kakuna;SUPER_NERD,3,8,DOWN,Super Nerd,2987,37 Growlithe,37 Vulpix)");

std::vector<std::string> Trainer::LEADER_TEAM_STRINGS = splitLinesTrainer(R"(PewterGym;SUPER_NERD,4,1,DOWN,Leader Brock,1386,12 Geodude,14 Onix;Bide
CeruleanGym;BRUNETTE_GIRL,4,2,DOWN,Leader Misty,2079,18 Staryu,21 Starmie;Bubble Beam
VermilionGym;ROCKER,5,1,DOWN,Leader Lt. Surge,2376,21 Voltorb,18 Pikachu,24 Raichu;Thunderbolt
CeladonGym;SILPH_WORKER_F,4,3,DOWN,Leader Erika,2871,29 Victreebel,24 Tangela,29 Vileplume;Mega Drain
)" R"(
FuchsiaGym;KOGA,4,10,DOWN,Leader Koga,4257,37 Koffing,39 Muk,37 Koffing,43 Weezing;Toxic
SaffronGym;GIRL,9,8,DOWN,Leader Sabrina,4257,38 Kadabra,37 Mr. Mime,38 Venomoth,43 Alakazam;Psywave
CinnabarGym;MIDDLE_AGED_MAN,3,3,DOWN,Leader Blaine,4653,42 Growlithe,40 Ponyta,42 Rapidash,47 Arcanine;Fire Blast
ViridianGym;GIOVANNI,2,1,DOWN,Leader Giovanni,4950,45 Rhyhorn,42 Dugtrio,44 Nidoqueen,45 Nidoking,50 Rhydon;Fissure
FuchsiaCity;MONSTER,13,5,DOWN,RatMan Johnson,100000,120 Raticate,120 Raticate,120 Raticate,120 Raticate,120 Raticate,120 Raticate;Rat Gambit)");

std::vector<std::string> Trainer::GIO_RIVAL_TEAM_STRINGS = splitLinesTrainer(R"(RocketHideoutB4F;GIOVANNI,25,3,DOWN,Giovanni,2871,25 Onix,24 Rhyhorn,29 Kangaskhan
SilphCo11F;GIOVANNI,6,5,DOWN,Giovanni,4059,37 Nidorino,35 Kangaskhan,37 Rhyhorn,41 Nidoqueen
)" R"(
PalletTown;BIKER,12,12,DOWN,Noah Buddy,25,5 Rattata
PewterCity;BIKER,16,18,DOWN,Noah Buddy,280,9 Pidgey,8 Rattata
CeruleanCity;BIKER,30,20,DOWN,Noah Buddy,595,18 Pidgeotto,15 Abra,15 Rattata,21 Butterfree
VermilionCity;BIKER,12,20,DOWN,Noah Buddy,1330,19 Pidgeotto,20 Raticate,18 Kadabra,25 Butterfree
LavenderTown;BIKER,14,6,DOWN,Noah Buddy,1625,25 Pidgeotto,23 Exeggcute,22 Gyarados,20 Kadabra,26 Raticate
SaffronCity;BIKER,18,22,LEFT,Noah Buddy,2600,37 Pidgeot,38 Exeggcute,35 Gyarados,35 Alakazam,42 Raticate
VictoryRoad1F;BIKER,9,12,DOWN,Noah Buddy,6000,51 Pidgeot,52 Rhyhorn,53 Exeggcute,54 Gyarados,55 Alakazam,60 Raticate)");

std::vector<std::string> Trainer::E4_TEAM_STRINGS = splitLinesTrainer(R"(LoreleisRoom;LORELEI,5,1,DOWN,Elite Four Lorelei,7000,60 Dewgong,61 Cloyster,62 Slowbro,63 Jynx,67 Lapras
BrunosRoom;BRUNO,5,1,DOWN,Elite Four Bruno,8000,72 Onix,73 Hitmonchan,73 Hitmonlee,76 Onix,79 Machamp
)" R"(
AgathasRoom;AGATHA,5,1,DOWN,Elite Four Agatha,9000,84 Gengar,86 Golbat,94 Haunter,88 Arbok,89 Gengar
LancesRoom;LANCE,6,1,DOWN,Elite Four Lance,10000,95 Gyarados,96 Dragonair,99 Dragonair,93 Aerodactyl,100 Dragonite
ChampionsRoom;BIKER,4,1,DOWN,Champion Noah,20000,101 Pidgeot,102 Alakazam,103 Rhydon,104 Exeggutor,108 Gyarados,111 Raticate
RocketHideoutB1F;BLUE,3,3,DOWN,Programmer Jay,999999,190 Abra,130 Magikarp,135 Caterpie,140 Weedle,145 Ditto,165 Voidwalker)");

std::unordered_map<std::string, SDL_Texture*> Trainer::TRAINER_SPRITES;
int Trainer::tid = 0;

Trainer::Trainer(const std::string &s) {
    std::vector<std::string> a = utils::split(s, ',');
    if (a.size() < 6) {
        throw std::runtime_error("Trainer string malformed: " + s);
    }
    if (a[3] == "DOWN") {
        dir = 1;
    } else if (a[3] == "UP") {
        dir = 4;
    } else if (a[3] == "LEFT") {
        dir = 6;
    } else if (a[3] == "RIGHT") {
        dir = 8;
    } else {
        throw std::runtime_error(s);
    }
    std::string key = a[0] + std::to_string(dir);
    auto it = TRAINER_SPRITES.find(key);
    if (it == TRAINER_SPRITES.end()) {
        std::string path = "assets/sprites/" + a[0] + "/" + std::to_string(dir) + ".png";
        SDL_Texture *tex = SpriteManager::loadScaledTexture(path, GameConfig::TILE_SIZE, GameConfig::TILE_SIZE);
        TRAINER_SPRITES[key] = tex;
        bi = tex;
    } else {
        bi = it->second;
    }
    x = std::stoi(a[1]);
    y = std::stoi(a[2]);
    type = a[4];
    reward = std::stoi(a[5]);
    for (size_t i = 6; i < a.size(); i++) {
        size_t j = a[i].find(' ');
        if (j == std::string::npos) {
            throw std::runtime_error("Trainer mon malformed: " + a[i] + " in " + s);
        }
        int level = std::stoi(a[i].substr(0, j));
        std::string monName = a[i].substr(j + 1);
        if (!Monster::MONSTER_MAP.count(monName)) {
            throw std::runtime_error("Trainer mon missing: " + monName + " in " + s);
        }
        team.push_back(new Battler(level, Monster::MONSTER_MAP[monName]));
    }
}

void Trainer::beat(Player *p) {
    beaten = true;
    p->trainersBeaten[id] = true;
}

std::string Trainer::toString() const {
    return std::to_string(x) + " " + std::to_string(y);
}

Trainer::Leader::Leader(int id, const std::vector<std::string> &a) : Trainer(a[1]) {
    this->id = id;
    move = Move::MOVE_MAP[a[2]];
}

void Trainer::Leader::beat(Player *p) {
    beaten = true;
    p->leadersBeaten[id] = true;
    p->give(move);
}

Trainer::GioRival::GioRival(int id, const std::string &s) : Trainer(s) {
    this->id = id;
}

void Trainer::GioRival::beat(Player *p) {
    beaten = true;
    p->gioRivalsBeaten[id] = true;
}

Trainer::E4Trainer::E4Trainer(int id, const std::string &s) : Trainer(s) {
    this->id = id;
}

void Trainer::E4Trainer::beat(Player *p) {
    beaten = true;
    if (id == 8) {
        p->leadersBeaten[id] = true;
    }
}

void Trainer::addEliteFour() {
    for (size_t i = 0; i < E4_TEAM_STRINGS.size(); i++) {
        std::vector<std::string> a = utils::split(E4_TEAM_STRINGS[i], ';');
        if (a.size() < 2) {
            throw std::runtime_error("E4 string malformed");
        }
        if (!PokeMap::POKEMAPS.count(a[0])) {
            throw std::runtime_error("E4 map missing: " + a[0]);
        }
        PokeMap::POKEMAPS[a[0]]->addE4Trainer(static_cast<int>(i) * 2, a[1], NpcStrings::E4_SPEECH_STRINGS[i]);
    }
}

void Trainer::buildTrainers() {
    tid = 0;
    for (const std::string &s : TEAM_STRINGS) {
        std::vector<std::string> a = utils::split(s, ';');
        if (a.empty()) {
            continue;
        }
        if (!PokeMap::POKEMAPS.count(a[0])) {
            throw std::runtime_error("Trainer map missing: " + a[0]);
        }
        PokeMap::POKEMAPS[a[0]]->addTrainers(a);
    }
    for (size_t i = 0; i < LEADER_TEAM_STRINGS.size(); i++) {
        std::vector<std::string> a = utils::split(LEADER_TEAM_STRINGS[i], ';');
        if (a.empty()) {
            continue;
        }
        if (!PokeMap::POKEMAPS.count(a[0])) {
            throw std::runtime_error("Leader map missing: " + a[0]);
        }
        PokeMap::POKEMAPS[a[0]]->addLeader(static_cast<int>(i), a);
    }
    for (size_t i = 0; i < GIO_RIVAL_TEAM_STRINGS.size(); i++) {
        std::vector<std::string> a = utils::split(GIO_RIVAL_TEAM_STRINGS[i], ';');
        if (a.size() < 2) {
            throw std::runtime_error("GioRival string malformed");
        }
        if (!PokeMap::POKEMAPS.count(a[0])) {
            throw std::runtime_error("GioRival map missing: " + a[0]);
        }
        PokeMap::POKEMAPS[a[0]]->addGioRival(static_cast<int>(i), a[1]);
    }
    addEliteFour();
}

void Trainer::buildTrainers(Player *p, const std::string &trainS, const std::string &leadS, const std::string &gioRS) {
    tid = 0;
    p->trainersBeaten.assign(trainS.size(), false);
    for (const std::string &s : TEAM_STRINGS) {
        std::vector<std::string> a = utils::split(s, ';');
        PokeMap *pm = PokeMap::POKEMAPS[a[0]];
        for (size_t i = 1; i < a.size(); i++) {
            if (tid < 0 || static_cast<size_t>(tid) >= trainS.size()) {
                throw std::runtime_error("Trainer save flags out of range: tid=" + std::to_string(tid) +
                                         " size=" + std::to_string(trainS.size()));
            }
            if (trainS[tid] == '1') {
                p->trainersBeaten[tid++] = true;
            } else {
                pm->addTrainer(a[i], static_cast<int>(i));
            }
        }
    }
    p->leadersBeaten.assign(leadS.size(), false);
    for (size_t i = 0; i < LEADER_TEAM_STRINGS.size(); i++) {
        if (i >= leadS.size()) {
            throw std::runtime_error("Leader save flags out of range: i=" + std::to_string(i) +
                                     " size=" + std::to_string(leadS.size()));
        }
        if (leadS[i] == '1') {
            p->leadersBeaten[i] = true;
        } else {
            std::vector<std::string> a = utils::split(LEADER_TEAM_STRINGS[i], ';');
            PokeMap::POKEMAPS[a[0]]->addLeader(static_cast<int>(i), a);
        }
    }
    p->gioRivalsBeaten.assign(gioRS.size(), false);
    for (size_t i = 0; i < GIO_RIVAL_TEAM_STRINGS.size(); i++) {
        if (i >= gioRS.size()) {
            throw std::runtime_error("GioRival save flags out of range: i=" + std::to_string(i) +
                                     " size=" + std::to_string(gioRS.size()));
        }
        if (gioRS[i] == '1') {
            p->gioRivalsBeaten[i] = true;
        } else {
            std::vector<std::string> a = utils::split(GIO_RIVAL_TEAM_STRINGS[i], ';');
            PokeMap::POKEMAPS[a[0]]->addGioRival(static_cast<int>(i), a[1]);
        }
    }
    for (size_t i = 0; i < E4_TEAM_STRINGS.size(); i++) {
        std::vector<std::string> a = utils::split(E4_TEAM_STRINGS[i], ';');
        PokeMap::POKEMAPS[a[0]]->addE4Trainer(static_cast<int>(i) * 2, a[1], NpcStrings::E4_SPEECH_STRINGS[i]);
    }
}
