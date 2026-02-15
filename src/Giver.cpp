#include "Giver.h"
#include "Main.h"
#include "OverworldGui.h"
#include "PokeMap.h"
#include "Player.h"
#include "Utils.h"
#include <sstream>

static std::vector<std::string> splitLinesGiverDef(const std::string &s) {
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

std::vector<std::string> Giver::GIVER_STRINGS = splitLinesGiverDef(R"(Route2Gate;SCIENTIST 1 4 LEFT,Move=Flash;10
Route11;SCIENTIST 2 6 DOWN,Amulet Coin;30
Route15Gate2F;SCIENTIST 4 2 DOWN,Lucky Egg;50
HallOfFame;OAK 5 2 DOWN,Shiny Charm;152
RocketHideoutB1F;BIKE_SHOP_CLERK 3 17 UP,Key to Darkness;150
OaksLab;OAK 5 2 DOWN,Pokedex;...Secret Sauce
BikeShop;YOUNGSTER 1 3 UP,Bicycle;Bike Voucher
WardensHouse;WARDEN 2 3 DOWN,Move=Strength;Gold Teeth
VermilionOldRodHouse;FISHING_GURU 2 4 RIGHT,Old Rod
FuchsiaGoodRodHouse;FISHING_GURU 5 3 RIGHT,Good Rod
Route12SuperRodHouse;FISHING_GURU 2 4 RIGHT,Super Rod
Route12Gate2F;BRUNETTE_GIRL 3 4 UP,Move=Swift
CeruleanTrashedHouse;GIRL 5 6 RIGHT,Move=Dig
CeladonCity;GRAMPS 22 16 DOWN,Move=Soft-Boiled
MrPsychicsHouse;FISHING_GURU 5 3 LEFT,Move=Psychic
CopycatsHouse2F;BRUNETTE_GIRL 4 3 DOWN,Move=Mimic
SilphCo2F;SILPH_WORKER_F 10 1 UP,Move=Self-Destruct
SSAnneCaptainsRoom;CAPTAIN 4 2 UP,Move=Cut
Route16FlyHouse;BRUNETTE_GIRL 2 3 RIGHT,Move=Fly
SafariZoneSecretHouse;FISHING_GURU 3 3 DOWN,Move=Surf
BillsHouse;MONSTER 6 5 DOWN,SS Ticket
CeladonDiner;MIDDLE_AGED_MAN 1 4 DOWN,Fake ID
PokemonFanClub;GENTLEMAN 3 1 DOWN,Bike Voucher
ViridianCity;FISHER 6 23 DOWN,Move=Dream Eater
ViridianCity;GAMBLER_ASLEEP 18 9 DOWN,...Secret Sauce
SilphCo11F;SILPH_PRESIDENT 7 5 DOWN,Master Ball
PokemonTower7F;MR_FUJI 10 3 DOWN,Pokeflute)");

std::vector<std::string> Giver::GIVER_QUOTE_STRINGS = splitLinesGiverDef(R"(Hey there! You haven't caught many pokemon, huh?;Howdy, what do you think of this?... what, you don't like being flashed? Well now you can flash, too!;Have fun flashing all of your friends!
Oh, it's you! But... you haven't done much pokemon catching.;Hey, you're pretty poor! I bet this can help you out!;Just you wait, you'll be loaded in no time!
Hmm, I wonder if there are any PokeLovers around here...;Oh! Well you're... so weak. Take this, it should help!;Oh yeah, now you can get strong like me!
Congratulations!... although your pokedex could use some work. Go catch more pokemon!;...heh, you found the forbidden one, huh? I guess you're worthy of this, then.;Seriously though, congratulations! And thanks for saving the world, that beast should serve you well.
Oh, you seem lost... this is the source of the world's voids, I'm sure you've seen them. But you are still so weak.;Maybe you can help.;...although we're probably doomed.
Oh bother, if only I had some secret sauce...;Oh, is that secret sauce?! This will go great with my PokePasta! Here, take this.;Huh, tastes a bit... sour...
Dangit, how am I ever gonna afford a bike here?;Whoa, is that a voucher? I guess I don't need this!;...wait, he doesn't want this, either? Oh no...
Buh? Buh buh buh?;Buh? Oh hey, I can talk! Thanks, take this!;Buh buh... weird, I got so used to buh'ing they just come out sometimes!
Oh no, don't tell me... you don't have Gyarados? Well, maybe this will help you!;What? Yeah, of course you can catch a Gyarados with that! Just be patient...
Okay hear me out, I may not be the BEST fisher, but I'm pretty GOOD! HAH! Get it?;Oh yeah, you got it.
Ooh golly, I can already tell, you're a fishing enthusiast, and a super one at that! Take this.;Oh yeah buddy, now you can catch the BIG one!
Did you know? If you miss an attack, you might faint! Take this.;Okay sure, it doesn't work when you're sleeping, but neither do you!
I often imagine living in a hole for the rest of my days... perhaps you'll join me?;I've been waiting for you, my love, come to the underground with me!
Oh goodness, I seem to have soiled my pantaloons!;Heh, does somebody smell eggs?
Yesss, I can see it... you find me... incredibly attractive! No? Then I don't deserve this...;Wait, how can I hone my craft now??
Heh, bet you didn't know mirrors could talk... HUH?! You know I'm not you? Maybe you can use this better.;No? Well maybe you should give it back now... unless?
Ahh jeez, looks like I'm stuck here forever. I may as well just-;Huh? I'm still alive?
Ooh mama, I do NOT feel good. Oh? You'll rub me? Well shucks, I guess you've earned this!;Mmm yeah, just like that...
Hehe, you came all this way to see me? ...wait what, you're leaving already?;Noo please, I feel so alone...
Wow, I didn't think anybody would find me! Take this!;I learned that one at the beach, if you were wondering.
Heh, nothing like getting hugged by a cutie... oh, you want something? Uhh...;Man, I REALLY gotta stop talking to these kids maaan!
YOU LOVE GAMBLING? Oh, you don't? I can fix that!;Heh, tell me when you need some work...
I love pokemon, but I love you even more... I mean uhh, take this!;Feel free to join my harem, just have a seat.
Yawn... mmm, that was tasty!;Ahh, nothing like a tasty nap... Zzz
Mmm, that's the spot... oh, you want me to wake up? Hmm, come back later...;Zzz
Holy macaroni, you're the real deal! I bet you've got what it takes to be a pokemon master!;Of course, if you waste it, you are a pokemon LOSER!
My my, you've traveled far to save me, thank you! But I'm actually just wandering about for fun... do you like music?;Because I hate it, goodbye now!
)");

int Giver::gid = 0;

void Giver::buildGivers() {
    for (int i = 0; i < 5; i++) {
        std::vector<std::string> a = utils::split(GIVER_STRINGS[i], ';');
        PokeMap::POKEMAPS[a[0]]->addGiver(a[1], std::stoi(a[2]), GIVER_QUOTE_STRINGS[i]);
    }
    for (int i = 5; i < 8; i++) {
        std::vector<std::string> a = utils::split(GIVER_STRINGS[i], ';');
        PokeMap::POKEMAPS[a[0]]->addGiver(a[1], a[2], GIVER_QUOTE_STRINGS[i]);
    }
    for (size_t i = 8; i < GIVER_STRINGS.size(); i++) {
        std::vector<std::string> a = utils::split(GIVER_STRINGS[i], ';');
        PokeMap::POKEMAPS[a[0]]->addGiver(a[1], GIVER_QUOTE_STRINGS[i]);
    }
}

Giver::Giver(const std::string &s, const std::string &q) : Npc(utils::split(s, ',')[0], q) {
    std::vector<std::string> a = utils::split(s, ',');
    id = gid++;
    if (a.size() > 1) {
        if (a[1].rfind("Move", 0) == 0) {
            move = Move::MOVE_MAP[a[1].substr(5)];
        } else if (a[1] == "null") {
            item = nullptr;
        } else {
            item = Item::ITEM_MAP[a[1]];
        }
    }
}

void Giver::interact(Player *p) {
    if (!dead) {
        OverworldGui::print(quotes[0]);
        if (move == nullptr) {
            p->give(item);
        } else {
            p->give(move);
        }
        dead = true;
    }
    OverworldGui::print(quotes[1]);
}

Giver::Aide::Aide(const std::string &s, int dexNum, const std::string &q) : Giver(s, q), dexNum(dexNum) {}

void Giver::Aide::interact(Player *p) {
    if (p->numCaught < dexNum) {
        OverworldGui::print(quotes[0]);
        return;
    }
    if (!dead) {
        OverworldGui::print(quotes[1]);
        if (move == nullptr) {
            if (item && item->name == "Shiny Charm") {
                Main::SHINY_CHANCE = 256;
            }
            p->give(item);
        } else {
            p->give(move);
        }
        dead = true;
    }
    OverworldGui::print(quotes[2]);
}

Giver::IfGiver::IfGiver(const std::string &s, const std::string &i, const std::string &q) : Giver(s, q) {
    preReq = Item::ITEM_MAP[i];
}

void Giver::IfGiver::interact(Player *p) {
    if (!p->hasItem(preReq)) {
        OverworldGui::print(quotes[0]);
        return;
    }
    if (!dead) {
        OverworldGui::print(quotes[1]);
        if (move == nullptr) {
            p->give(item);
        } else {
            p->give(move);
        }
        dead = true;
    }
    OverworldGui::print(quotes[2]);
}
