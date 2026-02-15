#include "Blocker.h"
#include "OverworldGui.h"
#include "PokeMap.h"
#include "Player.h"
#include "Utils.h"
#include <sstream>

static std::vector<std::string> splitLinesBlockerDef(const std::string &s) {
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

std::vector<std::string> Blocker::BLOCKER_STRINGS = splitLinesBlockerDef(R"(ViridianCity;GIRL 17 9 RIGHT,Pokedex
ViridianCity;GAMBLER_ASLEEP 18 8 DOWN,Pokedex
ViridianCity;SEEL 19 9 DOWN,Pokedex
VermilionCity;SAILOR 18 30 UP,SS Ticket
VermilionCity;SAILOR 19 30 UP,SS Ticket
PokemonTower6F;FISHER 10 16 RIGHT,Silph Scope
Route5Gate;GUARD 3 3 UP,Fresh Water
Route5Gate;GUARD 4 3 UP,Lemonade
Route6Gate;GUARD 4 2 DOWN,Fresh Water
Route6Gate;GUARD 3 2 DOWN,Soda Pop
Route7Gate;GUARD 3 3 LEFT,Fresh Water
Route7Gate;GUARD 3 4 LEFT,Max Revive
Route8Gate;GUARD 2 3 RIGHT,Fresh Water
Route8Gate;GUARD 2 4 RIGHT,Max Elixir
SaffronCity;GUARD 17 22 RIGHT,Pokeflute
SaffronCity;GUARD 18 23 UP,Pokeflute
SaffronCity;GUARD 19 22 LEFT,Pokeflute
SaffronCity;ROCKET 34 4 DOWN,Master Ball
SaffronCity;ROCKET 13 12 DOWN,Master Ball
SaffronCity;ROCKET 7 6 DOWN,Master Ball
Route16Gate1F;GUARD 4 9 RIGHT,Bicycle
SilphCo11F;ROCKET 6 13 DOWN,Card Key
SilphCo11F;ROCKET 7 13 DOWN,Card Key
CinnabarIsland;BOULDER 18 4 DOWN,Secret Key
RocketHideoutB1F;GIRL 3 23 UP,Key to Darkness)");

std::vector<std::string> Blocker::BADGE_BLOCKER_STRINGS = splitLinesBlockerDef(R"(Route4;FISHER 18 6 DOWN,null;0
CeruleanCity;GUARD 27 12 DOWN,null;1
ViridianCity;GAMBLER 32 8 DOWN,null;6
Route22Gate;GUARD 5 1 LEFT,null;0
Route23;GUARD 4 32 DOWN,null;7
Route23;GUARD 10 56 DOWN,null;6
Route23;SWIMMER 8 85 DOWN,null;5
Route23;SWIMMER 11 96 DOWN,null;4
Route23;GUARD 12 105 DOWN,null;3
Route23;GUARD 8 119 DOWN,null;2
Route23;GUARD 9 119 DOWN,null;2
Route23;GUARD 8 136 DOWN,null;1
CinnabarIsland;GAMBLER_ASLEEP 6 4 DOWN,null;5
CeruleanCity;SUPER_NERD 4 12 DOWN,null;8
Route9;GAMBLER 4 8 LEFT,null;2
FuchsiaCity;OAK 13 6 DOWN,null;8)");

std::vector<std::string> Blocker::BOULDER_STRINGS = splitLinesBlockerDef(R"(SeafoamIslands1F;BOULDER 18 10 DOWN,Move=Strength;BOULDER 26 7 DOWN,Move=Strength
SeafoamIslandsB1F;BOULDER 17 6 DOWN,Move=Strength;BOULDER 22 6 DOWN,Move=Strength
SeafoamIslandsB2F;BOULDER 18 6 DOWN,Move=Strength;BOULDER 23 6 DOWN,Move=Strength
SeafoamIslandsB3F;BOULDER 5 14 DOWN,Move=Strength;BOULDER 3 15 DOWN,Move=Strength;BOULDER 8 14 DOWN,Move=Strength;BOULDER 9 14 DOWN,Move=Strength;BOULDER 18 6 DOWN,Move=Strength;BOULDER 19 6 DOWN,Move=Strength
SeafoamIslandsB4F;BOULDER 4 15 DOWN,Move=Strength;BOULDER 5 15 DOWN,Move=Strength
VictoryRoad1F;BOULDER 5 15 DOWN,Move=Strength;BOULDER 14 2 DOWN,Move=Strength;BOULDER 2 10 DOWN,Move=Strength
VictoryRoad2F;BOULDER 4 14 DOWN,Move=Strength;BOULDER 5 5 DOWN,Move=Strength;BOULDER 23 16 DOWN,Move=Strength
VictoryRoad3F;BOULDER 22 3 DOWN,Move=Strength;BOULDER 13 12 DOWN,Move=Strength;BOULDER 24 10 DOWN,Move=Strength;BOULDER 22 15 DOWN,Move=Strength
WardensHouse;BOULDER 8 4 DOWN,Move=Strength
Route20;BOULDER 48 6 DOWN,Move=Strength;BOULDER 58 10 DOWN,Move=Strength
RocketHideoutB4F;BOULDER 24 2 DOWN,Move=Strength;BOULDER 26 2 DOWN,Move=Strength;BOULDER 25 11 DOWN,Move=Strength
Route16Gate1F;BOULDER 4 7 DOWN,Move=Strength;BOULDER 4 10 DOWN,Move=Strength
Route22Gate;BOULDER 4 1 DOWN,Move=Strength
Route23;BOULDER 6 136 DOWN,Move=Strength;BOULDER 7 136 DOWN,Move=Strength;BOULDER 9 136 DOWN,Move=Strength)");

std::vector<std::string> Blocker::BLOCKER_QUOTE_STRINGS = splitLinesBlockerDef(R"(Oh goodness, I DO hope somebody will do something with that... secret sauce. I'm so tired of eating it myself!;Oh, you found someone who could bear to swallow it? Thank heavens, maybe now I won't have to do it!
I sure do love my brother! Zzz;Oh? You've found another sauce enjoyer? We must visit him immediately, we have so much to give him!
Buh?;Buh buh!
Well well well, another poor trainer... you could NEVER afford to ride this vessel!;Oh, good sir! Why you DO seem to have a most extraORdinary ticket! Yes, just being in your presence is a delight...
Heh, trust me kid, they'd just have you wipe the floors in there, go home!;Ohh? Well well, aren't you something! Come on through!
Oh noo, it's too scary in here, I can't move! If only I could see these ghosts...;Hey, let me see that... ACK! THESE GHOSTS! SO... scary... must leave...
Ahh jeez, I'm soo thirsty... if only somebody could help me...;Hehehe, looks like I got a drop to drink after all... go on through!
Just a drop, sir? I desire your fluid.;Lemonade, mmm mmm yummy yummy!
You know what I love? Water! But only the fresh kind... unless?;Hehehe, looks like I got a drop to drink after all... go on through!
I am just POPPING for a drink! Even just a drop.;WhOa, THAT hits the SPOT!
Juice is one thing, but water...;Hehehe, looks like I got a drop to drink after all... go on through!
...I actually sleep with my eyes closed, but I'm not asleep!;WHOA, I'm feeling PUMPED!
FRESH, that's how I need my water! Bring me just a few drops...;SLURP!
You will NEVER make me budge...;YOWZA, you're a strong trainer! Nooooo....
Ugh, this kid won't budge! If only there was a strong trainer around...;Huh? You want to get through? Alright, your funeral!
Gosh, this guy is soo annoying! I wish somebody would help us!;Huh? You want to get through? Alright, your funeral!
Buddy? What?? I'm not your buddy, get out of the way!;Huh? You want to get through? Alright, your funeral!
We've got this city locked down, get indoors now!;Huh? The boss is gone? I'd better scram!
No entry, pal.;Okay, I'll let you through, but only just this once.
You're not supposed to be here, you know.;ACK! You look powerful, spare meee!
Listen bucko, if I let you through here you'll get mowed down! Just go back home.;Oh, a fellow cyclist! Come on through.
Hehe, can't you tell? I'm a door, put the key in...;Oh yeah, that's the spot.
I am NOT a door. Do NOT ask me if I am.;NO! How did you know...
What a weird looking rock...;Oh, here's a hole!
You are nothing without the key.;!!)");

std::vector<std::string> Blocker::BADGE_QUOTE_STRINGS = splitLinesBlockerDef(R"(Huh? You haven't even beaten Brock? Listen kid, this cave is dangerous, get out of here!;Whoa, you look pretty tough... go on through!
Sighh, if only a strong trainer would deal with that Rocket grunt! Oh well...;Ooh, you've got such big muscles... and big pokemon, too! Go fight that evil man!
Oh, you're taking on the Pokemon League challenge! Welp, this town's gym is deserted, you'd best run along...;Whoa, people have been piling in here all day! I suppose you must be going in, too.
Stop right there, kid!;Move along, Champion in training!
You're not ready for this road.;The path is yours - proceed!
Turn back until you're stronger.;Your badge proves your strength!
I can't let you through yet.;All clear - enjoy your journey!
Rules are rules, no badge no pass.;Badge confirmed - you're free to go!
Come back when you've earned it.;Impressive - carry on!
This way is closed to you.;You're cleared for passage!
There's nothing I love more than a man in uniform!;Where's your uniform?
Nice try, but not today.;Respect - you've earned this path!
This is my house, do you like it?;Okay I'll admit it, I don't live here. Please help me.
I sometimes hear strange sounds from deep inside.;Huh, you think you're pretty tough, don't you? Yeah, me too...
Hey, slow down speed racer!;Cowabunga!
AHHHHHHHHHHHH!!!!;Oh, it's gone? I guess I am, too, then!)");

void Blocker::buildBlockers() {
    for (size_t i = 0; i < BLOCKER_STRINGS.size(); i++) {
        std::vector<std::string> a = utils::split(BLOCKER_STRINGS[i], ';');
        PokeMap::POKEMAPS[a[0]]->addBlocker(a[1], BLOCKER_QUOTE_STRINGS[i]);
    }
    PokeMap::POKEMAPS["Route10"]->addBlocker("FISHER 8 18 DOWN,Move=Flash",
                                             "Aw man, if only I'd get flashed today...;Whoa, look at you! I've never been flashed like that, follow me into this cave!");
    PokeMap::POKEMAPS["SafariZoneGate"]->addBlocker("SAFARI_ZONE_WORKER 3 4 RIGHT,Move=Big Money",
                                                    "Listen pal, we only want big spenders here...;Whoa, you've got some BIG money! Come on in.");
    PokeMap::POKEMAPS["SafariZoneGate"]->addBlocker("SAFARI_ZONE_WORKER 4 4 LEFT,Move=Big Money",
                                                    "Huh? Something smells... poor.;Hot dog, we've got a big-shot here! OH-KAY!");
    for (size_t i = 0; i < BADGE_BLOCKER_STRINGS.size(); i++) {
        std::vector<std::string> a = utils::split(BADGE_BLOCKER_STRINGS[i], ';');
        PokeMap::POKEMAPS[a[0]]->addBlocker(a[1], BADGE_QUOTE_STRINGS[i], std::stoi(a[2]));
    }
    for (const std::string &s : BOULDER_STRINGS) {
        std::vector<std::string> a = utils::split(s, ';');
        PokeMap *pm = PokeMap::POKEMAPS[a[0]];
        for (size_t i = 1; i < a.size(); i++) {
            pm->addBlocker(a[i], "Ugh, this is too heavy!;Heh, light as a feather!", 3);
        }
    }
    PokeMap *pm = PokeMap::POKEMAPS["Route23"];
    for (int i = 104; i < 108; i += 2) {
        for (int j = 10; j < 16; j++) {
            pm->addBlocker("BOULDER " + std::to_string(j) + " " + std::to_string(i) + " DOWN,Move=Strength",
                           "Ugh, this is too heavy!;Heh, light as a feather!", 3);
        }
    }
}

Blocker::Blocker(const std::string &s, const std::string &q) : Giver(s, q), numBadges(-1) {}

Blocker::Blocker(const std::string &s, const std::string &q, int n) : Giver(s, q), numBadges(n) {}

void Blocker::interact(Player *p) {
    bool missingItem = false;
    if (item != nullptr) {
        if (item->name == "Master Ball") {
            missingItem = p->ballin;
        } else {
            missingItem = !p->hasItem(item);
        }
    }
    if ((move != nullptr && !p->hasMove(move)) || (numBadges >= 0 && !p->leadersBeaten[numBadges]) || missingItem) {
        OverworldGui::print(quotes[0]);
        return;
    }
    dead = true;
    OverworldGui::print(quotes[1]);
}
