#include "Npc.h"
#include "PokeMap.h"
#include "NpcStrings.h"
#include "OverworldGui.h"
#include "Player.h"
#include "Trainer.h"
#include "Utils.h"
#include "GameConfig.h"

static std::vector<std::string> splitLinesNpcDef(const std::string &s) {
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

std::vector<std::string> Npc::NPC_STRINGS = splitLinesNpcDef(R"(BluesHouse;DAISY 6 4 UP
BikeShop;BIKE_SHOP_CLERK 6 2 DOWN,MIDDLE_AGED_WOMAN 5 6 UP
CeladonChiefHouse;GRAMPS 4 2 DOWN,ROCKET 1 4 DOWN,SAILOR 5 6 LEFT
CeladonDiner;COOK 8 5 RIGHT,MIDDLE_AGED_WOMAN 7 2 DOWN,FISHER 5 3 RIGHT,GYM_GUIDE 0 1 DOWN
CeladonCity;LITTLE_GIRL 8 17 DOWN,GRAMPS 11 28 UP,GIRL 14 19 UP,GRAMPS 25 22 DOWN,FISHER 32 12 LEFT,ROCKET 32 29 RIGHT,ROCKET 42 14 RIGHT
CeladonHotel;GRANNY 3 1 DOWN,BEAUTY 2 4 DOWN,SUPER_NERD 8 4 RIGHT
CeladonMansion1F;GRANNY 1 5 DOWN
CeladonMansion3F;BIKE_SHOP_CLERK 0 4 UP,CLERK 3 4 UP,SUPER_NERD 0 7 UP,SILPH_WORKER_M 2 3 DOWN
CeladonMart1F;LINK_RECEPTIONIST 8 3 DOWN
CeladonMansionRoofHouse;HIKER 2 2 DOWN
CeladonMart2F;CLERK 5 3 DOWN,CLERK 6 3 DOWN,MIDDLE_AGED_MAN 19 5 DOWN,GIRL 14 4 UP
CeladonMart4F;CLERK 5 7 DOWN,SUPER_NERD 15 5 RIGHT,YOUNGSTER 5 2 RIGHT
CeladonMart3F;CLERK 16 5 DOWN,GAMEBOY_KID 11 6 RIGHT,GAMEBOY_KID 7 2 DOWN,GAMEBOY_KID 8 2 DOWN,LITTLE_BOY 2 5 UP
CeladonMart5F;GENTLEMAN 14 5 UP,SAILOR 2 6 DOWN,CLERK 5 3 DOWN,CLERK 6 3 DOWN
CeladonPokecenter;NURSE 3 1 DOWN,GENTLEMAN 7 3 RIGHT,BEAUTY 10 5 DOWN,LINK_RECEPTIONIST 11 2 DOWN
CeladonMartRoof;SUPER_NERD 10 4 LEFT,LITTLE_GIRL 5 5 DOWN
CeruleanBadgeHouse;MIDDLE_AGED_MAN 5 3 RIGHT
CeruleanCity;COOLTRAINER_M 31 20 DOWN,SUPER_NERD 15 18 UP,SUPER_NERD 9 21 RIGHT,COOLTRAINER_F 29 26 LEFT,COOLTRAINER_F 9 27 RIGHT
CeruleanGym;GYM_GUIDE 7 10 DOWN
CeruleanMart;CLERK 0 5 RIGHT,COOLTRAINER_M 3 4 UP,COOLTRAINER_F 6 2 RIGHT
CeruleanPokecenter;NURSE 3 1 DOWN,SUPER_NERD 10 5 DOWN,GENTLEMAN 4 3 DOWN,LINK_RECEPTIONIST 11 2 DOWN
CeruleanTrashedHouse;FISHING_GURU 2 1 DOWN
CinnabarGym;GYM_GUIDE 16 13 DOWN
CinnabarIsland;GIRL 12 5 RIGHT,GAMBLER 14 6 DOWN
CinnabarLabFossilRoom;SCIENTIST 5 2 RIGHT,SCIENTIST 7 6 UP
CinnabarLab;FISHING_GURU 1 3 DOWN
CinnabarLabMetronomeRoom;SCIENTIST 7 2 DOWN,SCIENTIST 2 3 RIGHT
CinnabarMart;CLERK 0 5 RIGHT,SILPH_WORKER_F 6 2 DOWN,SCIENTIST 3 4 DOWN
CinnabarPokecenter;NURSE 3 1 DOWN,COOLTRAINER_F 9 4 DOWN,GENTLEMAN 2 6 DOWN,LINK_RECEPTIONIST 11 2 DOWN
CopycatsHouse1F;MIDDLE_AGED_WOMAN 2 2 DOWN,MIDDLE_AGED_MAN 5 4 LEFT
Colosseum;RED 2 2 DOWN
DiglettsCaveRoute11;GAMBLER 2 3 DOWN
DiglettsCaveRoute2;FISHING_GURU 3 3 DOWN
FuchsiaBillsGrandpasHouse;MIDDLE_AGED_WOMAN 2 3 RIGHT,GAMBLER 7 2 UP,YOUNGSTER 5 5 DOWN
FuchsiaCity;YOUNGSTER 10 12 RIGHT,GAMBLER 28 17 RIGHT,FISHER 30 14 DOWN,YOUNGSTER 24 8 UP,POKE_BALL 25 6 DOWN,SEEL 8 17 DOWN,FOSSIL 6 5 DOWN
FuchsiaMart;CLERK 0 5 RIGHT,MIDDLE_AGED_MAN 4 2 DOWN,COOLTRAINER_F 6 5 UP
FuchsiaPokecenter;NURSE 3 1 DOWN,ROCKER 2 3 DOWN,COOLTRAINER_F 6 5 RIGHT,LINK_RECEPTIONIST 11 2 DOWN
FuchsiaGym;GYM_GUIDE 7 15 DOWN
FuchsiaMeetingRoom;SAFARI_ZONE_WORKER 4 1 DOWN,SAFARI_ZONE_WORKER 0 2 UP,SAFARI_ZONE_WORKER 10 1 DOWN
GameCorner;BEAUTY 2 6 DOWN,CLERK 5 6 DOWN,MIDDLE_AGED_MAN 2 10 LEFT,BEAUTY 2 13 LEFT,FISHING_GURU 5 11 RIGHT,MIDDLE_AGED_WOMAN 8 11 LEFT,GYM_GUIDE 8 14 LEFT,GAMBLER 11 15 RIGHT,CLERK 14 11 LEFT,GENTLEMAN 17 13 RIGHT
GameCornerPrizeRoom;BALDING_GUY 1 4 DOWN,GAMBLER 7 3 RIGHT
IndigoPlateauLobby;NURSE 7 5 DOWN,GYM_GUIDE 4 9 RIGHT,COOLTRAINER_F 5 1 DOWN,CLERK 0 5 RIGHT,LINK_RECEPTIONIST 13 6 DOWN
LavenderCuboneHouse;BRUNETTE_GIRL 2 4 RIGHT
LavenderMart;CLERK 0 5 RIGHT,BALDING_GUY 3 4 DOWN,COOLTRAINER_M 7 2 DOWN
LavenderPokecenter;NURSE 3 1 DOWN,GENTLEMAN 5 3 DOWN,LITTLE_GIRL 2 6 UP,LINK_RECEPTIONIST 11 2 DOWN
LavenderTown;LITTLE_GIRL 15 9 DOWN,COOLTRAINER_M 9 10 DOWN,SUPER_NERD 8 7 RIGHT
MrFujisHouse;SUPER_NERD 3 5 DOWN,LITTLE_GIRL 6 3 DOWN
MtMoonPokecenter;NURSE 3 1 DOWN,YOUNGSTER 4 3 UP,GENTLEMAN 7 3 UP,MIDDLE_AGED_MAN 10 6 RIGHT,LINK_RECEPTIONIST 11 2 DOWN
Museum1F;SCIENTIST 12 4 LEFT,GAMBLER 1 4 DOWN,SCIENTIST 15 2 DOWN,SCIENTIST 17 4 DOWN
Museum2F;YOUNGSTER 1 7 RIGHT,GRAMPS 0 5 DOWN,SCIENTIST 7 5 DOWN,BRUNETTE_GIRL 11 5 DOWN,HIKER 12 5 DOWN
OaksLab;GIRL 1 9 UP,SCIENTIST 2 10 DOWN,SCIENTIST 8 10 DOWN
NameRatersHouse;SILPH_PRESIDENT 5 3 LEFT
PalletTown;GIRL 3 8 DOWN,FISHER 11 14 DOWN
PewterGym;GYM_GUIDE 7 10 DOWN
PewterMart;CLERK 0 5 RIGHT,YOUNGSTER 3 3 UP,SUPER_NERD 5 5 DOWN
PewterCity;COOLTRAINER_F 8 15 DOWN,COOLTRAINER_M 17 25 DOWN,SUPER_NERD 27 17 DOWN,SUPER_NERD 26 25 RIGHT,YOUNGSTER 35 16 DOWN
PewterNidoranHouse;LITTLE_BOY 3 5 RIGHT,MIDDLE_AGED_MAN 1 2 DOWN
PewterPokecenter;NURSE 3 1 DOWN,GENTLEMAN 11 7 LEFT,LINK_RECEPTIONIST 11 2 DOWN
PewterSpeechHouse;GAMBLER 2 3 RIGHT,YOUNGSTER 4 5 DOWN
PokemonMansion2F;SUPER_NERD 3 17 RIGHT
PokemonMansion1F;SCIENTIST 17 17 LEFT
PokemonMansion3F;SUPER_NERD 5 11 RIGHT,SCIENTIST 20 11 LEFT
PokemonFanClub;FISHER 6 3 LEFT,GIRL 1 3 RIGHT,SEEL 1 4 RIGHT,LINK_RECEPTIONIST 5 1 DOWN
PokemonTower1F;LINK_RECEPTIONIST 15 13 UP,MIDDLE_AGED_WOMAN 6 8 DOWN,BALDING_GUY 8 12 DOWN,GIRL 13 7 DOWN,CHANNELER 17 7 LEFT
PokemonMansionB1F;SUPER_NERD 16 23 DOWN,SCIENTIST 27 11 DOWN
PokemonTower5F;CHANNELER 14 3 LEFT
RedsHouse1F;MOM 5 4 LEFT
RockTunnelPokecenter;NURSE 3 1 DOWN,GENTLEMAN 7 3 RIGHT,FISHER 2 5 DOWN,LINK_RECEPTIONIST 11 2 DOWN
Route1;YOUNGSTER 5 24 UP,YOUNGSTER 15 13 RIGHT
Route10;COOLTRAINER_F 7 54 DOWN
Route11Gate1F;GUARD 4 1 DOWN
Route11Gate2F;YOUNGSTER 4 2 RIGHT
Route12Gate1F;GUARD 1 3 DOWN
Route15Gate1F;GUARD 4 1 DOWN
Route16Gate1F;GUARD 4 8 DOWN,GAMBLER 4 3 DOWN
Route16Gate2F;LITTLE_BOY 4 2 DOWN,LITTLE_GIRL 2 5 RIGHT
Route18Gate1F;GUARD 4 1 DOWN
Route18Gate2F;YOUNGSTER 4 2 RIGHT
Route2Gate;YOUNGSTER 5 4 RIGHT
Route4;COOLTRAINER_F 9 8 DOWN
SSAnne2F;WAITER 3 7 UP
SSAnne1F;WAITER 12 6 RIGHT,SAILOR 27 5 DOWN
SSAnne1FRooms;GENTLEMAN 28 4 LEFT,GIRL 17 4 UP,MIDDLE_AGED_MAN 10 3 DOWN,LITTLE_GIRL 12 5 DOWN,GIRL 5 4 RIGHT
SSAnneB1FRooms;FISHER 0 4 RIGHT
SSAnneBow;SUPER_NERD 5 2 UP,COOLTRAINER_M 7 11 DOWN,SAILOR 10 8 UP
SSAnneKitchen;COOK 1 8 UP,COOK 5 8 UP,COOK 9 7 UP,COOK 13 6 DOWN,COOK 13 8 DOWN,COOK 13 10 DOWN,COOK 11 13 UP
SafariZoneCenterRestHouse;GIRL 3 2 DOWN,SCIENTIST 1 4 UP
SafariZoneEastRestHouse;SCIENTIST 1 3 UP,ROCKER 4 2 DOWN,SILPH_WORKER_M 5 2 DOWN
SafariZoneNorthRestHouse;SCIENTIST 6 3 RIGHT,SAFARI_ZONE_WORKER 3 4 DOWN,GENTLEMAN 1 5 UP
SafariZoneWestRestHouse;SCIENTIST 4 4 DOWN,COOLTRAINER_M 0 2 RIGHT,SILPH_WORKER_F 6 2 DOWN
SaffronGym;GYM_GUIDE 10 15 DOWN
SaffronCity;ROCKET 20 8 RIGHT,ROCKET 11 25 RIGHT,ROCKET 32 13 RIGHT,ROCKET 18 30 RIGHT,SCIENTIST 8 14 DOWN,SILPH_WORKER_M 23 23 DOWN,SILPH_WORKER_F 17 30 RIGHT,GENTLEMAN 30 12 DOWN,ROCKER 18 8 UP,ROCKET 22 22 LEFT
SaffronMart;CLERK 0 5 RIGHT,COOLTRAINER_F 6 5 DOWN
SaffronPidgeyHouse;BRUNETTE_GIRL 2 3 RIGHT,YOUNGSTER 4 1 DOWN,PAPER 3 3 DOWN
SaffronPokecenter;NURSE 3 1 DOWN,BEAUTY 5 5 DOWN,GENTLEMAN 8 3 DOWN,LINK_RECEPTIONIST 11 2 DOWN
SilphCo1F;LINK_RECEPTIONIST 4 2 DOWN
SilphCo11F;BEAUTY 10 5 DOWN
SilphCo3F;SILPH_WORKER_M 24 8 DOWN
SilphCo4F;SILPH_WORKER_M 6 2 DOWN
SilphCo6F;SILPH_WORKER_M 10 6 DOWN,SILPH_WORKER_M 20 6 DOWN,SILPH_WORKER_F 21 6 DOWN,SILPH_WORKER_F 11 10 RIGHT,SILPH_WORKER_M 18 13 UP
SilphCo7F;SILPH_WORKER_M 1 5 DOWN,SILPH_WORKER_M 13 13 UP,SILPH_WORKER_M 7 10 DOWN,SILPH_WORKER_F 10 8 DOWN
SilphCo8F;SILPH_WORKER_M 4 2 DOWN
SilphCo9F;NURSE 3 14 DOWN
TradeCenter;RED 2 2 RIGHT
UndergroundPathRoute7;MIDDLE_AGED_MAN 2 4 DOWN
UndergroundPathRoute5;LITTLE_GIRL 2 3 DOWN
UndergroundPathRoute6;GIRL 2 3 DOWN
UndergroundPathRoute8;GIRL 3 4 DOWN
VermilionCity;BEAUTY 19 7 RIGHT,GAMBLER 14 6 DOWN,GAMBLER 30 7 DOWN,SAILOR 25 27 RIGHT
VermilionGym;GYM_GUIDE 4 14 DOWN
VermilionMart;CLERK 0 5 RIGHT,COOLTRAINER_M 5 6 DOWN,COOLTRAINER_F 3 3 RIGHT
VermilionPokecenter;NURSE 3 1 DOWN,FISHING_GURU 10 5 DOWN,SAILOR 5 4 DOWN
VermilionPidgeyHouse;PAPER 4 3 DOWN
ViridianCity;YOUNGSTER 13 20 DOWN,YOUNGSTER 30 25 DOWN
ViridianForestNorthGate;SUPER_NERD 3 2 DOWN,GRAMPS 2 5 DOWN
ViridianForestSouthGate;GIRL 8 4 LEFT,LITTLE_GIRL 2 4 UP
ViridianForest;YOUNGSTER 16 43 DOWN,YOUNGSTER 27 40 DOWN
ViridianNicknameHouse;BALDING_GUY 5 3 DOWN,LITTLE_GIRL 1 4 UP
ViridianMart;CLERK 0 5 RIGHT,YOUNGSTER 5 5 UP,COOLTRAINER_M 3 3 DOWN
ViridianGym;GYM_GUIDE 16 15 DOWN
ViridianPokecenter;NURSE 3 1 DOWN,GENTLEMAN 10 5 UP,COOLTRAINER_M 4 3 DOWN,LINK_RECEPTIONIST 11 2 DOWN
ViridianBunglerHouse;BIKE_SHOP_CLERK 3 2 UP
Daycare;BLUE 5 4 LEFT
SafariZoneCenter;GYM_GUIDE 13 23 RIGHT
Route24;COOLTRAINER_F 19 0 RIGHT)");

static Move *FLASH = nullptr;

void Npc::buildNpcs() {
    if (!FLASH) {
        FLASH = Move::MOVE_MAP["Flash"];
    }
    int i = 0;
    for (const std::string &s : NPC_STRINGS) {
        std::vector<std::string> a = utils::split(s, ';');
        PokeMap *pm = PokeMap::POKEMAPS[a[0]];
        std::vector<std::string> entries = utils::split(a[1], ',');
        for (const std::string &n : entries) {
            pm->addNpc(n, NpcStrings::NPC_QUOTE_STRINGS[i++]);
        }
    }
}

Npc::Npc(const std::string &s, const std::string &q) {
    std::vector<std::string> f = utils::split(s, ' ');
    int dir = 0;
    std::string spriteProbe = "assets/sprites/" + f[0] + "/9.png";
    bool hasNine = (std::ifstream(spriteProbe).good());
    if (!hasNine) {
        if (f[3] == "DOWN") {
            dir = 0;
        } else if (f[3] == "UP") {
            dir = 1;
        } else if (f[3] == "LEFT") {
            dir = 2;
        } else if (f[3] == "RIGHT") {
            dir = 3;
        } else {
            throw std::runtime_error(s);
        }
    } else {
        if (f[3] == "DOWN") {
            dir = 1;
        } else if (f[3] == "UP") {
            dir = 4;
        } else if (f[3] == "LEFT") {
            dir = 6;
        } else if (f[3] == "RIGHT") {
            dir = 8;
        } else {
            throw std::runtime_error(s);
        }
    }
    std::string key = f[0] + std::to_string(dir);
    auto it = Trainer::TRAINER_SPRITES.find(key);
    if (it == Trainer::TRAINER_SPRITES.end()) {
        std::string path = "assets/sprites/" + f[0] + "/" + std::to_string(dir) + ".png";
        SDL_Texture *tex = SpriteManager::loadScaledTexture(path, GameConfig::TILE_SIZE, GameConfig::TILE_SIZE);
        Trainer::TRAINER_SPRITES[key] = tex;
        bi = tex;
    } else {
        bi = it->second;
    }
    x = std::stoi(f[1]);
    y = std::stoi(f[2]);
    quotes = utils::split(q, ';');
}

void Npc::interact(Player *p) {
    if (p->hasMove(FLASH) && utils::rand01() < 0.01) {
        OverworldGui::spacebar = false;
        OverworldGui::print(quotes[0]);
        OverworldGui::print(p->name + " used flash!");
        if (utils::rand01() < 0.5) {
            OverworldGui::print("NOO PLEASE DON'T!!!");
            OverworldGui::print("(They faded into the void...)");
            dead = true;
        } else {
            OverworldGui::print("Well in that case...");
            OverworldGui::print("They flashed you back!");
        }
    } else {
        OverworldGui::print(quotes[0]);
    }
}
