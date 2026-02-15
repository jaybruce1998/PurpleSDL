#include "Encounter.h"
#include <iostream>
#include "PokeMap.h"
#include "Utils.h"

static std::vector<std::string> splitLinesEncounter(const std::string &s) {
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

std::vector<std::string> Encounter::ENCOUNTER_STRINGS = splitLinesEncounter(R"(PalletTown;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,50 Tentacool 15 Tentacool 15,50 Poliwag 15 Poliwag 15
Route1;Tall Grass,80 Pidgey 1 Rattata 1,19 Pidgey 2 Rattata 2,1 Meowth 5 Doduo 5
ViridianCity;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,50 Tentacool 15 Tentacool 15,50 Poliwag 15 Poliwag 15
Route22;Tall Grass,20 Rattata 3 Rattata 3,20 Nidoran♂ 3 Nidoran♀ 3,15 Rattata 4 Rattata 4,10 Nidoran♂ 4 Nidoran♀ 4,10 Rattata 2 Rattata 2,10 Nidoran♂ 2 Nidoran♀ 2,5 Spearow 3 Spearow 3,5 Spearow 5 Spearow 5,4 Nidoran♀ 3 Nidoran♂ 3,1 Nidoran♀ 4 Nidoran♂ 4;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,50 Goldeen 15 Goldeen 15,50 Poliwag 15 Poliwag 15
Route2;Tall Grass,20 Rattata 3 Rattata 3,20 Pidgey 3 Pidgey 3,15 Pidgey 4 Pidgey 4,10 Rattata 4 Rattata 4,10 Pidgey 5 Pidgey 5,10 Weedle 3 Caterpie 3,5 Rattata 2 Rattata 2,5 Rattata 5 Rattata 5,4 Weedle 4 Caterpie 4,1 Weedle 5 Caterpie 5
ViridianForest;Tall Grass,20 Weedle 4 Caterpie 4,20 Kakuna 5 Metapod 5,15 Weedle 3 Caterpie 3,10 Weedle 5 Caterpie 5,10 Kakuna 4 Metapod 4,10 Kakuna 6 Metapod 6,5 Metapod 4 Kakuna 4,5 Caterpie 3 Weedle 3,4 Pikachu 3 Pikachu 3,1 Pikachu 5 Pikachu 5
Route3;Tall Grass,20 Pidgey 6 Pidgey 6,20 Spearow 5 Spearow 5,15 Pidgey 7 Pidgey 7,10 Spearow 6 Spearow 6,10 Spearow 7 Spearow 7,10 Pidgey 8 Pidgey 8,5 Spearow 8 Spearow 8,5 Jigglypuff 3 Jigglypuff 3,4 Jigglypuff 5 Jigglypuff 5,1 Jigglypuff 7 Jigglypuff 7
MtMoon1F;Cave,20 Zubat 8 Zubat 8,20 Zubat 7 Zubat 7,15 Zubat 9 Zubat 9,10 Geodude 8 Geodude 8,10 Zubat 6 Zubat 6,10 Zubat 10 Zubat 10,5 Geodude 10 Geodude 10,5 Paras 8 Paras 8,4 Zubat 11 Zubat 11,1 Clefairy 8 Clefairy 8
MtMoonB1F;Cave,20 Zubat 8 Zubat 8,20 Zubat 7 Zubat 7,15 Geodude 7 Geodude 7,10 Geodude 8 Geodude 8,10 Zubat 9 Zubat 9,10 Paras 10 Paras 10,5 Zubat 10 Zubat 10,5 Zubat 11 Zubat 11,4 Clefairy 9 Clefairy 9,1 Geodude 9 Geodude 9
MtMoonB2F;Cave,20 Zubat 9 Zubat 9,20 Geodude 9 Geodude 9,15 Zubat 10 Zubat 10,10 Geodude 10 Geodude 10,10 Zubat 11 Zubat 11,10 Paras 10 Paras 10,5 Paras 12 Paras 12,5 Clefairy 10 Clefairy 10,4 Zubat 12 Zubat 12,1 Clefairy 12 Clefairy 12
Route4;Tall Grass,20 Rattata 10 Rattata 10,20 Spearow 10 Spearow 10,15 Rattata 8 Rattata 8,10 Ekans 6 Sandshrew 6,10 Spearow 8 Spearow 8,10 Ekans 10 Sandshrew 10,5 Rattata 12 Rattata 12,5 Spearow 12 Spearow 12,4 Ekans 8 Sandshrew 8,1 Ekans 12 Sandshrew 12;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,33 Psyduck 15 Psyduck 15,33 Goldeen 15 Goldeen 15,33 Krabby 15 Krabby 15
CeruleanCity;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,33 Psyduck 15 Psyduck 15,33 Goldeen 15 Goldeen 15,33 Krabby 15 Krabby 15
Route24;Tall Grass,20 Weedle 7 Caterpie 7,20 Kakuna 8 Metapod 8,15 Pidgey 12 Pidgey 12,10 Oddish 12 Bellsprout 12,10 Oddish 13 Bellsprout 13,10 Abra 10 Abra 10,5 Oddish 14 Bellsprout 14,5 Pidgey 13 Pidgey 13,4 Abra 8 Abra 8,1 Abra 12 Abra 12;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,33 Psyduck 15 Psyduck 15,33 Goldeen 15 Goldeen 15,33 Krabby 15 Krabby 15
Route25;Tall Grass,20 Weedle 8 Caterpie 8,20 Kakuna 9 Metapod 9,15 Pidgey 13 Pidgey 13,10 Oddish 12 Bellsprout 12,10 Oddish 13 Bellsprout 13,10 Abra 12 Abra 12,5 Oddish 14 Bellsprout 14,5 Abra 10 Abra 10,4 Metapod 7 Kakuna 7,1 Caterpie 8 Weedle 8;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,33 Psyduck 15 Psyduck 15,33 Goldeen 15 Goldeen 15,33 Krabby 15 Krabby 15
Route5;Tall Grass,20 Oddish 13 Bellsprout 13,20 Pidgey 13 Pidgey 13,15 Pidgey 15 Pidgey 15,10 Mankey 10 Meowth 10,10 Mankey 12 Meowth 12,10 Oddish 15 Bellsprout 15,5 Oddish 16 Bellsprout 16,5 Pidgey 16 Pidgey 16,4 Mankey 14 Meowth 14,1 Mankey 16 Meowth 16
CeruleanGym;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,33 Psyduck 15 Psyduck 15,33 Goldeen 15 Goldeen 15,33 Krabby 15 Krabby 15
Route6;Tall Grass,20 Oddish 13 Bellsprout 13,20 Pidgey 13 Pidgey 13,15 Pidgey 15 Pidgey 15,10 Mankey 10 Meowth 10,10 Mankey 12 Meowth 12,10 Oddish 15 Bellsprout 15,5 Oddish 16 Bellsprout 16,5 Pidgey 16 Pidgey 16,4 Mankey 14 Meowth 14,1 Mankey 16 Meowth 16;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,50 Krabby 15 Krabby 15,50 Shellder 15 Shellder 15
VermilionCity;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,50 Krabby 15 Krabby 15,50 Shellder 15 Shellder 15
VermilionDock;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,50 Krabby 15 Krabby 15,50 Shellder 15 Shellder 15
DiglettsCave;Cave,20 Diglett 18 Diglett 18,20 Diglett 19 Diglett 19,15 Diglett 17 Diglett 17,10 Diglett 20 Diglett 20,10 Diglett 16 Diglett 16,10 Diglett 15 Diglett 15,5 Diglett 21 Diglett 21,5 Diglett 22 Diglett 22,4 Dugtrio 29 Dugtrio 29,1 Dugtrio 31 Dugtrio 31
Route11;Tall Grass,20 Ekans 14 Sandshrew 14,20 Spearow 15 Spearow 15,15 Ekans 12 Sandshrew 12,10 Drowzee 9 Drowzee 9,10 Spearow 13 Spearow 13,10 Drowzee 13 Drowzee 13,5 Ekans 15 Sandshrew 15,5 Spearow 17 Spearow 17,4 Drowzee 11 Drowzee 11,1 Drowzee 15 Drowzee 15
Route9;Tall Grass,20 Rattata 16 Rattata 16,20 Spearow 16 Spearow 16,15 Rattata 14 Rattata 14,10 Ekans 11 Sandshrew 11,10 Spearow 13 Spearow 13,10 Ekans 15 Sandshrew 15,5 Rattata 17 Rattata 17,5 Spearow 17 Spearow 17,4 Ekans 13 Sandshrew 13,1 Ekans 17 Sandshrew 17
Route10;Tall Grass,20 Voltorb 16 Voltorb 16,20 Spearow 16 Spearow 16,15 Voltorb 14 Voltorb 14,10 Ekans 11 Sandshrew 11,10 Spearow 13 Spearow 13,10 Ekans 15 Sandshrew 15,5 Voltorb 17 Voltorb 17,5 Spearow 17 Spearow 17,4 Ekans 13 Sandshrew 13,1 Ekans 17 Sandshrew 17;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,50 Poliwhirl 23 Poliwhirl 23,50 Slowpoke 15 Slowpoke 15
)" R"(
RockTunnel1F;Cave,20 Zubat 16 Zubat 16,20 Zubat 17 Zubat 17,15 Geodude 17 Geodude 17,10 Machop 15 Machop 15,10 Geodude 16 Geodude 16,10 Zubat 18 Zubat 18,5 Zubat 15 Zubat 15,5 Machop 17 Machop 17,4 Onix 13 Onix 13,1 Onix 15 Onix 15
RockTunnelB1F;Cave,20 Zubat 16 Zubat 16,20 Zubat 17 Zubat 17,15 Geodude 17 Geodude 17,10 Machop 15 Machop 15,10 Geodude 16 Geodude 16,10 Zubat 18 Zubat 18,5 Machop 17 Machop 17,5 Onix 17 Onix 17,4 Onix 13 Onix 13,1 Geodude 18 Geodude 18
Route8;Tall Grass,20 Pidgey 18 Pidgey 18,20 Mankey 18 Meowth 18,15 Ekans 17 Sandshrew 17,10 Growlithe 16 Vulpix 16,10 Pidgey 20 Pidgey 20,10 Mankey 20 Meowth 20,5 Ekans 19 Sandshrew 19,5 Growlithe 17 Vulpix 17,4 Growlithe 15 Vulpix 15,1 Growlithe 18 Vulpix 18
Route7;Tall Grass,20 Pidgey 19 Pidgey 19,20 Oddish 19 Bellsprout 19,15 Mankey 17 Meowth 17,10 Oddish 22 Bellsprout 22,10 Pidgey 22 Pidgey 22,10 Mankey 18 Meowth 18,5 Growlithe 18 Vulpix 18,5 Growlithe 20 Vulpix 20,4 Mankey 19 Meowth 19,1 Mankey 20 Meowth 20
CeladonCity;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,50 Poliwhirl 23 Poliwhirl 23,50 Slowpoke 15 Slowpoke 15
PokemonTower3F;Cave,20 Gastly 20 Gastly 20,20 Gastly 21 Gastly 21,15 Gastly 22 Gastly 22,10 Gastly 23 Gastly 23,10 Gastly 19 Gastly 19,10 Gastly 18 Gastly 18,5 Gastly 24 Gastly 24,5 Cubone 20 Cubone 20,4 Cubone 22 Cubone 22,1 Haunter 25 Haunter 25
PokemonTower4F;Cave,20 Gastly 20 Gastly 20,20 Gastly 21 Gastly 21,15 Gastly 22 Gastly 22,10 Gastly 23 Gastly 23,10 Gastly 19 Gastly 19,10 Gastly 18 Gastly 18,5 Haunter 25 Haunter 25,5 Cubone 20 Cubone 20,4 Cubone 22 Cubone 22,1 Gastly 24 Gastly 24
PokemonTower5F;Cave,20 Gastly 20 Gastly 20,20 Gastly 21 Gastly 21,15 Gastly 22 Gastly 22,10 Gastly 23 Gastly 23,10 Gastly 19 Gastly 19,10 Gastly 18 Gastly 18,5 Haunter 25 Haunter 25,5 Cubone 20 Cubone 20,4 Cubone 22 Cubone 22,1 Gastly 24 Gastly 24
PokemonTower6F;Cave,20 Gastly 21 Gastly 21,20 Gastly 22 Gastly 22,15 Gastly 23 Gastly 23,10 Gastly 24 Gastly 24,10 Gastly 20 Gastly 20,10 Gastly 19 Gastly 19,5 Haunter 26 Haunter 26,5 Cubone 22 Cubone 22,4 Cubone 24 Cubone 24,1 Haunter 28 Haunter 28
PokemonTower7F;Cave,20 Gastly 21 Gastly 21,20 Gastly 22 Gastly 22,15 Gastly 23 Gastly 23,10 Gastly 24 Gastly 24,10 Gastly 20 Gastly 20,10 Haunter 28 Haunter 28,5 Cubone 22 Cubone 22,5 Cubone 24 Cubone 24,4 Haunter 28 Haunter 28,1 Haunter 30 Haunter 30
Route12;Tall Grass,20 Oddish 24 Bellsprout 24,20 Pidgey 25 Pidgey 25,15 Pidgey 23 Pidgey 23,10 Venonat 24 Venonat 24,10 Oddish 22 Bellsprout 22,10 Venonat 26 Venonat 26,5 Oddish 26 Bellsprout 26,5 Pidgey 27 Pidgey 27,4 Gloom 28 Weepinbell 28,1 Gloom 30 Weepinbell 30;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Tentacool 5 Tentacool 5,25 Krabby 15 Krabby 15,25 Goldeen 15 Goldeen 15,25 Magikarp 15 Magikarp 15
Route13;Tall Grass,20 Oddish 24 Bellsprout 24,20 Pidgey 25 Pidgey 25,15 Pidgey 27 Pidgey 27,10 Venonat 24 Venonat 24,10 Oddish 22 Bellsprout 22,10 Venonat 26 Venonat 26,5 Oddish 26 Bellsprout 26,5 Ditto 25 Ditto 25,4 Gloom 28 Weepinbell 28,1 Gloom 30 Weepinbell 30;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Tentacool 5 Tentacool 5,25 Krabby 15 Krabby 15,25 Goldeen 15 Goldeen 15,25 Magikarp 15 Magikarp 15
Route14;Tall Grass,20 Oddish 24 Bellsprout 24,20 Pidgey 26 Pidgey 26,15 Ditto 23 Ditto 23,10 Venonat 24 Venonat 24,10 Oddish 22 Bellsprout 22,10 Venonat 26 Venonat 26,5 Oddish 26 Bellsprout 26,5 Gloom 30 Weepinbell 30,4 Pidgeotto 28 Pidgeotto 28,1 Pidgeotto 30 Pidgeotto 30
Route15;Tall Grass,20 Oddish 24 Bellsprout 24,20 Ditto 26 Ditto 26,15 Pidgey 23 Pidgey 23,10 Venonat 26 Venonat 26,10 Oddish 22 Bellsprout 22,10 Venonat 28 Venonat 28,5 Oddish 26 Bellsprout 26,5 Gloom 30 Weepinbell 30,4 Pidgeotto 28 Pidgeotto 28,1 Pidgeotto 30 Pidgeotto 30
FuchsiaCity;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Seaking 23 Seaking 23,25 Krabby 15 Krabby 15,25 Goldeen 15 Goldeen 15,25 Magikarp 15 Magikarp 15
Route16;Tall Grass,20 Spearow 20 Spearow 20,20 Spearow 22 Spearow 22,15 Rattata 18 Rattata 18,10 Doduo 20 Doduo 20,10 Rattata 20 Rattata 20,10 Doduo 18 Doduo 18,5 Doduo 22 Doduo 22,5 Rattata 22 Rattata 22,4 Raticate 23 Raticate 23,1 Raticate 25 Raticate 25
)" R"(
Route17;Tall Grass,20 Spearow 20 Spearow 20,20 Spearow 22 Spearow 22,15 Raticate 25 Raticate 25,10 Doduo 24 Doduo 24,10 Raticate 27 Raticate 27,10 Doduo 26 Doduo 26,5 Doduo 28 Doduo 28,5 Raticate 29 Raticate 29,4 Fearow 25 Fearow 25,1 Fearow 27 Fearow 27;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Tentacool 5 Tentacool 5,25 Krabby 15 Krabby 15,25 Goldeen 15 Goldeen 15,25 Magikarp 15 Magikarp 15
Route18;Tall Grass,20 Spearow 20 Spearow 20,20 Spearow 22 Spearow 22,15 Raticate 25 Raticate 25,10 Doduo 24 Doduo 24,10 Fearow 25 Fearow 25,10 Doduo 26 Doduo 26,5 Doduo 28 Doduo 28,5 Raticate 29 Raticate 29,4 Fearow 27 Fearow 27,1 Fearow 29 Fearow 29;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Tentacool 5 Tentacool 5,25 Krabby 15 Krabby 15,25 Goldeen 15 Goldeen 15,25 Magikarp 15 Magikarp 15
SafariZoneCenter;Tall Grass,20 Nidoran♂ 22 Nidoran♀ 22,20 Rhyhorn 25 Rhyhorn 25,15 Venonat 22 Venonat 22,10 Exeggcute 24 Exeggcute 24,10 Nidorino 31 Nidorina 31,10 Exeggcute 25 Exeggcute 25,5 Nidorina 31 Nidorino 31,5 Parasect 30 Parasect 30,4 Scyther 23 Pinsir 23,1 Chansey 23 Chansey 23;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Dratini 15 Dratini 15,25 Krabby 15 Krabby 15,25 Psyduck 15 Psyduck 15,25 Slowpoke 15 Slowpoke 15
SafariZoneEast;Tall Grass,20 Nidoran♂ 24 Nidoran♀ 24,20 Doduo 26 Doduo 26,15 Paras 22 Paras 22,10 Exeggcute 25 Exeggcute 25,10 Nidorino 33 Nidorina 33,10 Exeggcute 23 Exeggcute 23,5 Nidoran♀ 24 Nidoran♂ 24,5 Parasect 25 Parasect 25,4 Kangaskhan 25 Kangaskhan 25,1 Scyther 28 Pinsir 28;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Dratini 15 Dratini 15,25 Krabby 15 Krabby 15,25 Psyduck 15 Psyduck 15,25 Slowpoke 15 Slowpoke 15
SafariZoneNorth;Tall Grass,20 Nidoran♂ 22 Nidoran♀ 22,20 Rhyhorn 26 Rhyhorn 26,15 Paras 23 Paras 23,10 Exeggcute 25 Exeggcute 25,10 Nidorino 30 Nidorina 30,10 Exeggcute 27 Exeggcute 27,5 Nidorina 30 Nidorino 30,5 Venomoth 32 Venomoth 32,4 Chansey 26 Chansey 26,1 Tauros 28 Tauros 28;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Dratini 15 Dratini 15,25 Krabby 15 Krabby 15,25 Psyduck 15 Psyduck 15,25 Slowpoke 15 Slowpoke 15
SafariZoneWest;Tall Grass,20 Nidoran♂ 25 Nidoran♀ 25,20 Doduo 26 Doduo 26,15 Venonat 23 Venonat 23,10 Exeggcute 24 Exeggcute 24,10 Nidorino 33 Nidorina 33,10 Exeggcute 26 Exeggcute 26,5 Nidoran♀ 25 Nidoran♂ 25,5 Venomoth 31 Venomoth 31,4 Tauros 26 Tauros 26,1 Kangaskhan 28 Kangaskhan 28;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Dratini 15 Dratini 15,25 Krabby 15 Krabby 15,25 Psyduck 15 Psyduck 15,25 Slowpoke 15 Slowpoke 15
)" R"(
PowerPlant;Cave,20 Voltorb 21 Voltorb 21,20 Magnemite 21 Magnemite 21,15 Pikachu 20 Pikachu 20,10 Pikachu 24 Pikachu 24,10 Magnemite 23 Magnemite 23,10 Voltorb 23 Voltorb 23,5 Magneton 32 Magneton 32,5 Magneton 35 Magneton 35,4 Electabuzz 33 Raichu 33,1 Electabuzz 36 Raichu 36
Route19;Surfing,20 Tentacool 5 Tentacool 5,20 Tentacool 10 Tentacool 10,15 Tentacool 15 Tentacool 15,10 Tentacool 5 Tentacool 5,10 Tentacool 10 Tentacool 10,10 Tentacool 15 Tentacool 15,5 Tentacool 20 Tentacool 20,5 Tentacool 30 Tentacool 30,4 Tentacool 35 Tentacool 35,1 Tentacool 40 Tentacool 40;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Staryu 15 Staryu 15,25 Horsea 15 Horsea 15,25 Shellder 15 Shellder 15,25 Goldeen 15 Goldeen 15
Route20;Surfing,20 Tentacool 5 Tentacool 5,20 Tentacool 10 Tentacool 10,15 Tentacool 15 Tentacool 15,10 Tentacool 5 Tentacool 5,10 Tentacool 10 Tentacool 10,10 Tentacool 15 Tentacool 15,5 Tentacool 20 Tentacool 20,5 Tentacool 30 Tentacool 30,4 Tentacool 35 Tentacool 35,1 Tentacool 40 Tentacool 40;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Staryu 15 Staryu 15,25 Horsea 15 Horsea 15,25 Shellder 15 Shellder 15,25 Goldeen 15 Goldeen 15
SeafoamIslands1F;Cave,20 Seel 30 Seel 30,20 Slowpoke 30 Psyduck 30,15 Shellder 30 Staryu 30,10 Horsea 30 Krabby 30,10 Horsea 28 Krabby 28,10 Zubat 21 Zubat 21,5 Golbat 29 Golbat 29,5 Psyduck 28 Slowpoke 28,4 Shellder 28 Staryu 28,1 Golduck 38 Slowbro 38
SeafoamIslandsB1F;Cave,20 Staryu 30 Shellder 30,20 Horsea 30 Krabby 30,15 Shellder 32 Staryu 32,10 Horsea 32 Krabby 32,10 Slowpoke 28 Psyduck 28,10 Seel 30 Seel 30,5 Slowpoke 30 Psyduck 30,5 Seel 28 Seel 28,4 Dewgong 38 Dewgong 38,1 Seadra 37 Kingler 37
SeafoamIslandsB2F;Cave,20 Seel 30 Seel 30,20 Slowpoke 30 Psyduck 30,15 Seel 32 Seel 32,10 Slowpoke 32 Psyduck 32,10 Horsea 28 Krabby 28,10 Staryu 30 Shellder 30,5 Horsea 30 Krabby 30,5 Shellder 28 Staryu 28,4 Golbat 30 Golbat 30,1 Slowbro 37 Golduck 37
SeafoamIslandsB3F;Cave,20 Slowpoke 31 Psyduck 31,20 Seel 31 Seel 31,15 Slowpoke 33 Psyduck 33,10 Seel 33 Seel 33,10 Horsea 29 Krabby 29,10 Shellder 31 Staryu 31,5 Horsea 31 Krabby 31,5 Shellder 29 Staryu 29,4 Seadra 39 Kingler 39,1 Dewgong 37 Dewgong 37;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Staryu 15 Staryu 15,25 Horsea 15 Horsea 15,25 Shellder 15 Shellder 15,25 Goldeen 15 Goldeen 15
SeafoamIslandsB4F;Cave,20 Horsea 31 Krabby 31,20 Shellder 31 Staryu 31,15 Horsea 33 Krabby 33,10 Shellder 33 Staryu 33,10 Slowpoke 29 Psyduck 29,10 Seel 31 Seel 31,5 Slowpoke 31 Psyduck 31,5 Seel 29 Seel 29,4 Slowbro 39 Golduck 39,1 Golbat 32 Golbat 32;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Staryu 15 Staryu 15,25 Horsea 15 Horsea 15,25 Shellder 15 Shellder 15,25 Goldeen 15 Goldeen 15
Route21;Tall Grass,20 Rattata 21 Rattata 21,20 Pidgey 23 Pidgey 23,15 Raticate 30 Raticate 30,10 Rattata 23 Rattata 23,10 Pidgey 21 Pidgey 21,10 Pidgeotto 30 Pidgeotto 30,5 Pidgeotto 32 Pidgeotto 32,5 Tangela 28 Tangela 28,4 Tangela 30 Tangela 30,1 Tangela 32 Tangela 32;Surfing,20 Tentacool 5 Tentacool 5,20 Tentacool 10 Tentacool 10,15 Tentacool 15 Tentacool 15,10 Tentacool 5 Tentacool 5,10 Tentacool 10 Tentacool 10,10 Tentacool 15 Tentacool 15,5 Tentacool 20 Tentacool 20,5 Tentacool 30 Tentacool 30,4 Tentacool 35 Tentacool 35,1 Tentacool 40 Tentacool 40;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Staryu 15 Staryu 15,25 Horsea 15 Horsea 15,25 Shellder 15 Shellder 15,25 Goldeen 15 Goldeen 15
CinnabarIsland;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Staryu 15 Staryu 15,25 Horsea 15 Horsea 15,25 Shellder 15 Shellder 15,25 Goldeen 15 Goldeen 15
PokemonMansion1F;Cave,20 Koffing 32 Grimer 32,20 Koffing 30 Grimer 30,15 Ponyta 34 Ponyta 34,10 Ponyta 30 Ponyta 30,10 Growlithe 34 Vulpix 34,10 Ponyta 32 Ponyta 32,5 Grimer 30 Koffing 30,5 Ponyta 28 Ponyta 28,4 Weezing 37 Muk 37,1 Muk 39 Weezing 39
PokemonMansion2F;Cave,20 Growlithe 32 Vulpix 32,20 Koffing 34 Grimer 34,15 Koffing 34 Grimer 34,10 Ponyta 30 Ponyta 30,10 Koffing 30 Grimer 30,10 Ponyta 32 Ponyta 32,5 Grimer 30 Koffing 30,5 Ponyta 28 Ponyta 28,4 Weezing 39 Muk 39,1 Muk 37 Weezing 37
PokemonMansion3F;Cave,20 Koffing 31 Grimer 31,20 Growlithe 33 Vulpix 33,15 Koffing 35 Grimer 35,10 Ponyta 32 Ponyta 32,10 Ponyta 34 Magmar 34,10 Weezing 40 Muk 40,5 Grimer 34 Koffing 34,5 Weezing 38 Muk 38,4 Ponyta 36 Ponyta 36,1 Muk 42 Weezing 42
PokemonMansionB1F;Cave,20 Koffing 33 Grimer 33,20 Koffing 31 Grimer 31,15 Growlithe 35 Vulpix 35,10 Ponyta 32 Ponyta 32,10 Koffing 31 Grimer 31,10 Weezing 40 Muk 40,5 Ponyta 34 Ponyta 34,5 Grimer 35 Koffing 35,4 Weezing 42 Magmar 38,1 Muk 42 Weezing 42
Route23;Tall Grass,20 Ekans 26 Sandshrew 26,20 Ditto 33 Ditto 33,15 Spearow 26 Spearow 26,10 Fearow 38 Fearow 38,10 Ditto 38 Ditto 38,10 Fearow 38 Fearow 38,5 Arbok 41 Sandslash 41,5 Ditto 43 Ditto 43,4 Fearow 41 Fearow 41,1 Fearow 43 Fearow 43;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Slowbro 23 Slowbro 23,25 Seaking 23 Seaking 23,25 Kingler 23 Kingler 23,25 Seadra 23 Seadra 23
VictoryRoad1F;Cave,20 Machop 24 Machop 24,20 Geodude 26 Geodude 26,15 Zubat 22 Zubat 22,10 Onix 36 Onix 36,10 Onix 39 Onix 39,10 Onix 42 Onix 42,5 Graveler 41 Graveler 41,5 Golbat 41 Golbat 41,4 Machoke 42 Machoke 42,1 Marowak 43 Marowak 43
VictoryRoad2F;Cave,20 Machop 22 Machop 22,20 Geodude 24 Geodude 24,15 Zubat 26 Zubat 26,10 Onix 36 Onix 36,10 Onix 39 Onix 39,10 Onix 42 Onix 42,5 Machoke 41 Machoke 41,5 Golbat 40 Golbat 40,4 Marowak 40 Marowak 40,1 Graveler 43 Graveler 43
VictoryRoad3F;Cave,20 Machop 24 Machop 24,20 Geodude 26 Geodude 26,15 Zubat 22 Zubat 22,10 Onix 42 Onix 42,10 Venomoth 40 Venomoth 40,10 Onix 45 Onix 45,5 Graveler 43 Graveler 43,5 Golbat 41 Golbat 41,4 Machoke 42 Machoke 42,1 Machoke 45 Machoke 45
LoreleisRoom;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10
CeruleanCave1F;Cave,20 Golbat 46 Golbat 46,20 Hypno 46 Hypno 46,15 Magneton 46 Magneton 46,10 Dodrio 49 Dodrio 49,10 Venomoth 49 Venomoth 49,10 Arbok 52 Sandslash 52,5 Kadabra 49 Kadabra 49,5 Parasect 52 Parasect 52,4 Raichu 53 Raichu 53,1 Ditto 53 Ditto 53;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Slowbro 23 Slowbro 23,25 Seaking 23 Seaking 23,25 Kingler 23 Kingler 23,25 Seadra 23 Seadra 23
CeruleanCave2F;Cave,20 Dodrio 51 Dodrio 51,20 Venomoth 51 Venomoth 51,15 Kadabra 51 Kadabra 51,10 Rhydon 52 Rhydon 52,10 Marowak 52 Marowak 52,10 Electrode 52 Electrode 52,5 Chansey 56 Chansey 56,5 Wigglytuff 54 Wigglytuff 54,4 Ditto 55 Ditto 55,1 Ditto 60 Ditto 60
CeruleanCaveB1F;Cave,20 Rhydon 55 Rhydon 55,20 Marowak 55 Marowak 55,15 Electrode 55 Electrode 55,10 Chansey 64 Chansey 64,10 Parasect 64 Parasect 64,10 Raichu 64 Raichu 64,5 Arbok 57 Sandslash 57,5 Ditto 65 Ditto 65,4 Ditto 63 Ditto 63,1 Ditto 67 Ditto 67;Old Rod,100 Magikarp 5 Magikarp 5;Good Rod,50 Goldeen 10 Goldeen 10,50 Poliwag 10 Poliwag 10;Super Rod,25 Slowbro 23 Slowbro 23,25 Seaking 23 Seaking 23,25 Kingler 23 Kingler 23,25 Seadra 23 Seadra 23
RocketHideoutB1F;Tall Grass,95 Weedle 1 Zubat 1,4 Arbok 65 Muk 65,1 Meowth 1 Mew 80)");

void Encounter::buildEncounterRates() {
    for (const std::string &s : ENCOUNTER_STRINGS) {
        std::vector<std::string> a = utils::split(s, ';');
        if (a.empty()) {
            continue;
        }
        if (!PokeMap::POKEMAPS.count(a[0])) {
            throw std::runtime_error("Encounter map missing: " + a[0]);
        }
        PokeMap::POKEMAPS[a[0]]->addEncounters(a);
    }
}

Encounter::Encounter(const std::string &s) {
    std::vector<std::string> a = utils::split(s, ' ');
    chance = std::stoi(a[0]);
    rMonster = Monster::MONSTER_MAP[a[1]];
    rLevel = std::stoi(a[2]);
    bMonster = Monster::MONSTER_MAP[a[3]];
    bLevel = std::stoi(a[4]);
}

std::string Encounter::toString() const {
    return std::to_string(chance) + " " + (rMonster ? rMonster->name : "") + " " + std::to_string(rLevel) + " " +
           (bMonster ? bMonster->name : "") + " " + std::to_string(bLevel);
}
