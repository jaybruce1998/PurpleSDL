#include "TmLearnsets.h"
#include "LevelUpMove.h"
#include "MartItem.h"
#include "Utils.h"
#include <iostream>
#include <unordered_set>

static std::vector<std::string> splitLinesTm(const std::string &s) {
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

std::vector<std::string> TmLearnsets::LEARNSET_STRINGS = splitLinesTm(R"(Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Rage,Mega Drain,Solar Beam,Mimic,Double Team,Reflect,Bide,Rest,Substitute,Cut,Dig,Rock Rampage
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Rage,Mega Drain,Solar Beam,Mimic,Double Team,Reflect,Bide,Rest,Substitute,Cut
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Rage,Mega Drain,Solar Beam,Mimic,Double Team,Reflect,Bide,Rest,Substitute,Cut
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Mega Drain,Solar Beam,Mimic,Double Team,Reflect,Bide,Rest,Substitute,Cut,Rock Rampage
Mega Punch,Swords Dance,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Submission,Counter,Seismic Toss,Rage,Dragon Rage,Dig,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Substitute,Cut,Strength
Mega Punch,Swords Dance,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Submission,Counter,Seismic Toss,Rage,Dragon Rage,Dig,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Substitute,Cut,Strength
Mega Punch,Swords Dance,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Dragon Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Substitute,Cut,Fly,Strength,Rock Rampage
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Submission,Counter,Seismic Toss,Rage,Dig,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute,Surf,Strength
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Submission,Counter,Seismic Toss,Rage,Dig,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute,Surf,Strength
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute,Surf,Strength,Rock Rampage
Big Money
Big Money
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Hyper Beam,Rage,Mega Drain,Solar Beam,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Swift,Rest,Psywave,Substitute,Flash
Big Money
Big Money
Swords Dance,Toxic,Take Down,Double-Edge,Hyper Beam,Rage,Mega Drain,Mimic,Double Team,Reflect,Bide,Swift,Skull Bash,Rest,Substitute,Cut
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Rage,Mimic,Double Team,Reflect,Bide,Swift,Sky Attack,Rest,Substitute,Fly
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Rage,Mimic,Double Team,Reflect,Bide,Swift,Sky Attack,Rest,Substitute,Fly
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Hyper Beam,Rage,Mimic,Double Team,Reflect,Bide,Swift,Sky Attack,Rest,Substitute,Fly
Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Blizzard,Rage,Thunderbolt,Thunder,Dig,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Substitute,Rat Gambit
Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Rage,Thunderbolt,Thunder,Dig,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Substitute,Rat Gambit
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Rage,Mimic,Double Team,Bide,Swift,Sky Attack,Rest,Substitute,Fly
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Hyper Beam,Rage,Mimic,Double Team,Bide,Swift,Sky Attack,Rest,Substitute,Fly
Toxic,Body Slam,Take Down,Double-Edge,Rage,Mega Drain,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Skull Bash,Rest,Rock Slide,Substitute,Strength,Rock Rampage
Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Mega Drain,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Skull Bash,Rest,Rock Slide,Substitute,Strength,Rock Rampage
)" R"(
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Pay Day,Submission,Seismic Toss,Rage,Thunderbolt,Thunder,Mimic,Double Team,Reflect,Bide,Swift,Skull Bash,Rest,Thunder Wave,Substitute,Flash
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Pay Day,Submission,Seismic Toss,Rage,Thunderbolt,Thunder,Mimic,Double Team,Reflect,Bide,Swift,Skull Bash,Rest,Thunder Wave,Substitute,Flash
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Submission,Seismic Toss,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Rock Slide,Substitute,Cut,Strength,Rock Rampage
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Submission,Seismic Toss,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Rock Slide,Substitute,Cut,Strength,Rock Rampage
Toxic,Body Slam,Take Down,Double-Edge,Blizzard,Rage,Thunderbolt,Thunder,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute
Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Thunderbolt,Thunder,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute
Mega Punch,Mega Kick,Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Pay Day,Submission,Counter,Seismic Toss,Rage,Thunderbolt,Thunder,Earthquake,Fissure,Mimic,Double Team,Reflect,Bide,Fire Blast,Skull Bash,Rest,Rock Slide,Substitute,Surf,Strength
Toxic,Body Slam,Take Down,Double-Edge,Blizzard,Rage,Thunderbolt,Thunder,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute
Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Thunderbolt,Thunder,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute
Mega Punch,Mega Kick,Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Pay Day,Submission,Counter,Seismic Toss,Rage,Thunderbolt,Thunder,Earthquake,Fissure,Mimic,Double Team,Reflect,Bide,Fire Blast,Skull Bash,Rest,Rock Slide,Substitute,Surf,Strength
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Submission,Counter,Seismic Toss,Rage,Solar Beam,Thunderbolt,Thunder,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Metronome,Fire Blast,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Strength,Flash
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Solar Beam,Thunderbolt,Thunder,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Metronome,Fire Blast,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Strength,Flash
Toxic,Body Slam,Take Down,Double-Edge,Rage,Dig,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Substitute
Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Dig,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Substitute
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Submission,Counter,Seismic Toss,Rage,Solar Beam,Thunderbolt,Thunder,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Fire Blast,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Strength,Flash
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Solar Beam,Thunderbolt,Thunder,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Fire Blast,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Strength,Flash
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Rage,Mega Drain,Mimic,Double Team,Bide,Swift,Rest,Substitute
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Hyper Beam,Rage,Mega Drain,Mimic,Double Team,Bide,Swift,Rest,Substitute
Swords Dance,Toxic,Take Down,Double-Edge,Rage,Mega Drain,Solar Beam,Mimic,Double Team,Reflect,Bide,Rest,Substitute,Cut
Swords Dance,Toxic,Take Down,Double-Edge,Rage,Mega Drain,Solar Beam,Mimic,Double Team,Reflect,Bide,Rest,Substitute,Cut
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Mega Drain,Solar Beam,Mimic,Double Team,Reflect,Bide,Rest,Substitute,Cut
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Rage,Mega Drain,Solar Beam,Dig,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute,Cut
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Mega Drain,Solar Beam,Dig,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute,Cut
Toxic,Take Down,Double-Edge,Rage,Mega Drain,Solar Beam,Psychic,Mimic,Double Team,Reflect,Bide,Rest,Psywave,Substitute,Flash
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Hyper Beam,Rage,Mega Drain,Solar Beam,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Swift,Rest,Psywave,Substitute,Flash
Toxic,Body Slam,Take Down,Double-Edge,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Rest,Rock Slide,Substitute,Cut,Rock Rampage
Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Rest,Rock Slide,Substitute,Cut,Rock Rampage
Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Pay Day,Rage,Thunderbolt,Thunder,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Substitute
Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Hyper Beam,Pay Day,Rage,Thunderbolt,Thunder,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Substitute
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Pay Day,Submission,Counter,Seismic Toss,Rage,Dig,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Substitute,Surf,Strength
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Pay Day,Submission,Counter,Seismic Toss,Rage,Dig,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Substitute,Surf,Strength
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Pay Day,Submission,Counter,Seismic Toss,Rage,Thunderbolt,Thunder,Dig,Mimic,Double Team,Bide,Metronome,Swift,Skull Bash,Rest,Rock Slide,Substitute,Strength,Rock Rampage
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Pay Day,Submission,Counter,Seismic Toss,Rage,Thunderbolt,Thunder,Dig,Mimic,Double Team,Bide,Metronome,Swift,Skull Bash,Rest,Rock Slide,Substitute,Strength,Rock Rampage
Toxic,Body Slam,Take Down,Double-Edge,Rage,Dragon Rage,Dig,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Substitute
Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Dragon Rage,Dig,Teleport,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Substitute
Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Psychic,Mimic,Double Team,Bide,Skull Bash,Rest,Psywave,Substitute,Surf
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Submission,Counter,Seismic Toss,Rage,Earthquake,Fissure,Psychic,Mimic,Double Team,Bide,Metronome,Skull Bash,Rest,Psywave,Substitute,Surf,Strength
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Earthquake,Fissure,Psychic,Mimic,Double Team,Bide,Metronome,Skull Bash,Rest,Psywave,Substitute,Surf,Strength
Mega Punch,Toxic,Body Slam,Take Down,Double-Edge,Submission,Counter,Seismic Toss,Rage,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Metronome,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Flash
Mega Punch,Toxic,Body Slam,Take Down,Double-Edge,Submission,Counter,Seismic Toss,Rage,Dig,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Metronome,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Flash
Mega Punch,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Dig,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Metronome,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Flash
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Submission,Counter,Seismic Toss,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Metronome,Fire Blast,Skull Bash,Rest,Rock Slide,Substitute,Strength,Rock Rampage
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Submission,Counter,Seismic Toss,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Metronome,Fire Blast,Skull Bash,Rest,Rock Slide,Substitute,Strength,Rock Rampage
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Metronome,Fire Blast,Skull Bash,Rest,Rock Slide,Substitute,Strength,Rock Rampage
Swords Dance,Toxic,Take Down,Double-Edge,Rage,Mega Drain,Solar Beam,Mimic,Double Team,Reflect,Bide,Rest,Substitute,Cut
Swords Dance,Toxic,Take Down,Double-Edge,Rage,Mega Drain,Solar Beam,Mimic,Double Team,Reflect,Bide,Rest,Substitute,Cut
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Mega Drain,Solar Beam,Mimic,Double Team,Reflect,Bide,Rest,Substitute,Cut
Swords Dance,Toxic,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Mega Drain,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute,Cut,Surf
Swords Dance,Toxic,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Rage,Mega Drain,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute,Cut,Surf
Mega Punch,Toxic,Body Slam,Take Down,Double-Edge,Submission,Counter,Seismic Toss,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Metronome,Self-Destruct,Fire Blast,Rest,Explosion,Rock Slide,Substitute,Strength,Rock Rampage
Mega Punch,Toxic,Body Slam,Take Down,Double-Edge,Submission,Counter,Seismic Toss,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Metronome,Self-Destruct,Fire Blast,Rest,Explosion,Rock Slide,Substitute,Strength,Rock Rampage
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Metronome,Self-Destruct,Fire Blast,Rest,Explosion,Rock Slide,Substitute,Strength,Rock Rampage
Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Rage,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Substitute
)" R"(
Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Substitute
Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Pay Day,Rage,Earthquake,Fissure,Dig,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Surf,Strength,Flash
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Pay Day,Submission,Counter,Seismic Toss,Rage,Earthquake,Fissure,Dig,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Surf,Strength,Flash
Toxic,Take Down,Double-Edge,Rage,Thunderbolt,Thunder,Teleport,Mimic,Double Team,Reflect,Bide,Swift,Rest,Thunder Wave,Substitute,Flash
Toxic,Take Down,Double-Edge,Hyper Beam,Rage,Thunderbolt,Thunder,Teleport,Mimic,Double Team,Reflect,Bide,Swift,Rest,Thunder Wave,Substitute,Flash
Razor Wind,Swords Dance,Whirlwind,Toxic,Body Slam,Take Down,Double-Edge,Rage,Mimic,Double Team,Reflect,Bide,Swift,Skull Bash,Rest,Substitute,Cut,Fly
Whirlwind,Toxic,Body Slam,Take Down,Double-Edge,Rage,Mimic,Double Team,Reflect,Bide,Skull Bash,Sky Attack,Rest,Tri Attack,Substitute,Fly
Whirlwind,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Mimic,Double Team,Reflect,Bide,Skull Bash,Sky Attack,Rest,Tri Attack,Substitute,Fly
Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Pay Day,Rage,Mimic,Double Team,Bide,Skull Bash,Rest,Substitute,Surf,Strength
Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Pay Day,Rage,Mimic,Double Team,Bide,Skull Bash,Rest,Substitute,Surf,Strength
Toxic,Body Slam,Rage,Mega Drain,Thunderbolt,Thunder,Mimic,Double Team,Bide,Self-Destruct,Fire Blast,Rest,Explosion,Substitute
Toxic,Body Slam,Hyper Beam,Rage,Mega Drain,Thunderbolt,Thunder,Mimic,Double Team,Bide,Self-Destruct,Fire Blast,Rest,Explosion,Substitute
Toxic,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Teleport,Mimic,Double Team,Reflect,Bide,Self-Destruct,Swift,Rest,Explosion,Tri Attack,Substitute,Surf
Toxic,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Rage,Teleport,Mimic,Double Team,Reflect,Bide,Self-Destruct,Swift,Rest,Explosion,Tri Attack,Substitute,Surf
Toxic,Rage,Mega Drain,Thunderbolt,Thunder,Psychic,Mimic,Double Team,Bide,Self-Destruct,Dream Eater,Rest,Psywave,Explosion,Substitute
Toxic,Rage,Mega Drain,Thunderbolt,Thunder,Psychic,Mimic,Double Team,Bide,Self-Destruct,Dream Eater,Rest,Psywave,Explosion,Substitute
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Mega Drain,Thunderbolt,Thunder,Psychic,Mimic,Double Team,Bide,Metronome,Self-Destruct,Skull Bash,Dream Eater,Rest,Psywave,Explosion,Substitute,Strength
Toxic,Body Slam,Take Down,Double-Edge,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Self-Destruct,Rest,Explosion,Rock Slide,Substitute,Strength
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Submission,Counter,Seismic Toss,Rage,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Metronome,Skull Bash,Dream Eater,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Flash
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Metronome,Skull Bash,Dream Eater,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Flash
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Mimic,Double Team,Bide,Skull Bash,Rest,Substitute,Cut,Surf,Strength
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Rage,Mimic,Double Team,Bide,Skull Bash,Rest,Substitute,Cut,Surf,Strength
Toxic,Take Down,Thunderbolt,Thunder,Teleport,Mimic,Double Team,Reflect,Bide,Self-Destruct,Swift,Rest,Thunder Wave,Explosion,Substitute,Flash
Toxic,Take Down,Hyper Beam,Thunderbolt,Thunder,Teleport,Mimic,Double Team,Reflect,Bide,Self-Destruct,Swift,Rest,Thunder Wave,Explosion,Substitute,Flash
Toxic,Take Down,Double-Edge,Rage,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Self-Destruct,Egg Bomb,Rest,Psywave,Explosion,Substitute
Toxic,Take Down,Double-Edge,Hyper Beam,Rage,Mega Drain,Solar Beam,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Self-Destruct,Egg Bomb,Rest,Psywave,Explosion,Substitute,Strength
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Submission,Counter,Seismic Toss,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Fire Blast,Skull Bash,Rest,Substitute,Strength
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Fire Blast,Skull Bash,Rest,Substitute,Strength
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Submission,Counter,Seismic Toss,Rage,Mimic,Double Team,Bide,Metronome,Swift,Skull Bash,Rest,Substitute,Strength
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Submission,Counter,Seismic Toss,Rage,Mimic,Double Team,Bide,Metronome,Swift,Skull Bash,Rest,Substitute,Strength
Mega Punch,Swords Dance,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Thunderbolt,Thunder,Earthquake,Fissure,Mimic,Double Team,Bide,Fire Blast,Skull Bash,Rest,Substitute,Cut,Surf,Strength
Toxic,Rage,Thunderbolt,Thunder,Mimic,Double Team,Bide,Self-Destruct,Fire Blast,Rest,Explosion,Substitute
Toxic,Hyper Beam,Rage,Thunderbolt,Thunder,Mimic,Double Team,Bide,Self-Destruct,Fire Blast,Rest,Explosion,Substitute
Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Rage,Thunderbolt,Thunder,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Fire Blast,Skull Bash,Rest,Rock Slide,Substitute,Strength,Rock Rampage
Mega Punch,Mega Kick,Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Pay Day,Submission,Counter,Seismic Toss,Rage,Thunderbolt,Thunder,Earthquake,Fissure,Dig,Mimic,Double Team,Bide,Fire Blast,Skull Bash,Rest,Rock Slide,Substitute,Surf,Strength,Rock Rampage
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Solar Beam,Thunderbolt,Thunder,Psychic,Mimic,Double Team,Reflect,Bide,Metronome,Egg Bomb,Fire Blast,Skull Bash,Soft-Boiled,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Strength,Flash
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Mega Drain,Solar Beam,Mimic,Double Team,Bide,Skull Bash,Rest,Substitute,Cut
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Thunderbolt,Thunder,Earthquake,Fissure,Mimic,Double Team,Bide,Fire Blast,Skull Bash,Rest,Rock Slide,Substitute,Surf,Strength,Rock Rampage
Toxic,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Substitute,Surf
Toxic,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Rage,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Substitute,Surf
Toxic,Horn Drill,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Substitute,Surf
Toxic,Horn Drill,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Rage,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Substitute,Surf
Toxic,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Thunderbolt,Thunder,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Swift,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Surf,Flash
Toxic,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Rage,Thunderbolt,Thunder,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Swift,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Surf,Flash
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Solar Beam,Thunderbolt,Thunder,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Metronome,Skull Bash,Rest,Thunder Wave,Psywave,Substitute,Flash
Swords Dance,Toxic,Take Down,Double-Edge,Hyper Beam,Rage,Mimic,Double Team,Bide,Swift,Skull Bash,Rest,Substitute,Cut
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Metronome,Skull Bash,Rest,Psywave,Substitute
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Thunderbolt,Thunder,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Metronome,Swift,Skull Bash,Rest,Thunder Wave,Psywave,Substitute,Strength,Flash
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Submission,Counter,Seismic Toss,Rage,Psychic,Teleport,Mimic,Double Team,Bide,Metronome,Fire Blast,Skull Bash,Rest,Psywave,Substitute,Strength
Swords Dance,Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Submission,Seismic Toss,Rage,Mimic,Double Team,Bide,Rest,Substitute,Cut,Strength
Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Ice Beam,Blizzard,Hyper Beam,Rage,Thunderbolt,Thunder,Earthquake,Fissure,Mimic,Double Team,Bide,Fire Blast,Skull Bash,Rest,Substitute,Strength
)" R"(
Big Money
Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Rage,Dragon Rage,Thunderbolt,Thunder,Mimic,Double Team,Reflect,Bide,Fire Blast,Skull Bash,Rest,Substitute,Surf,Strength
Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Rage,Solar Beam,Dragon Rage,Thunderbolt,Thunder,Psychic,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Psywave,Substitute,Surf,Strength
Big Money
Toxic,Body Slam,Take Down,Double-Edge,Rage,Mimic,Double Team,Reflect,Bide,Swift,Skull Bash,Rest,Substitute
Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Rage,Mimic,Double Team,Reflect,Bide,Swift,Skull Bash,Rest,Substitute,Surf
Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Thunderbolt,Thunder,Mimic,Double Team,Reflect,Bide,Swift,Skull Bash,Rest,Thunder Wave,Substitute,Flash
Toxic,Body Slam,Take Down,Double-Edge,Hyper Beam,Rage,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Substitute
Toxic,Take Down,Double-Edge,Ice Beam,Blizzard,Hyper Beam,Rage,Thunderbolt,Thunder,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Swift,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Flash
Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Mimic,Double Team,Reflect,Bide,Rest,Substitute,Surf
Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Submission,Seismic Toss,Rage,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute,Surf
Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Mimic,Double Team,Reflect,Bide,Rest,Substitute,Surf
Razor Wind,Swords Dance,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Submission,Seismic Toss,Rage,Mimic,Double Team,Reflect,Bide,Skull Bash,Rest,Substitute,Cut,Surf
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Hyper Beam,Rage,Dragon Rage,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Sky Attack,Rest,Substitute,Fly
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Pay Day,Submission,Counter,Seismic Toss,Rage,Solar Beam,Thunderbolt,Thunder,Earthquake,Fissure,Psychic,Mimic,Double Team,Reflect,Bide,Metronome,Self-Destruct,Fire Blast,Skull Bash,Rest,Psywave,Rock Slide,Substitute,Surf,Strength
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Rage,Mimic,Double Team,Reflect,Bide,Swift,Sky Attack,Rest,Substitute,Fly
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Hyper Beam,Rage,Thunderbolt,Thunder,Mimic,Double Team,Reflect,Bide,Swift,Sky Attack,Rest,Thunder Wave,Substitute,Fly,Flash
Razor Wind,Whirlwind,Toxic,Take Down,Double-Edge,Hyper Beam,Rage,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Sky Attack,Rest,Substitute,Fly
Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Dragon Rage,Thunderbolt,Thunder,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Thunder Wave,Substitute,Surf
Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Rage,Dragon Rage,Thunderbolt,Thunder,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Thunder Wave,Substitute,Surf
Razor Wind,Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Rage,Dragon Rage,Thunderbolt,Thunder,Mimic,Double Team,Reflect,Bide,Fire Blast,Swift,Skull Bash,Rest,Thunder Wave,Substitute,Surf,Strength
Mega Punch,Mega Kick,Toxic,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Pay Day,Submission,Counter,Seismic Toss,Rage,Solar Beam,Thunderbolt,Thunder,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Metronome,Self-Destruct,Fire Blast,Skull Bash,Rest,Thunder Wave,Psywave,Tri Attack,Substitute,Strength,Flash,Rock Rampage
Mega Punch,Razor Wind,Swords Dance,Whirlwind,Mega Kick,Toxic,Horn Drill,Body Slam,Take Down,Double-Edge,Bubble Beam,Water Gun,Ice Beam,Blizzard,Hyper Beam,Pay Day,Submission,Counter,Seismic Toss,Rage,Mega Drain,Solar Beam,Dragon Rage,Thunderbolt,Thunder,Earthquake,Fissure,Dig,Psychic,Teleport,Mimic,Double Team,Reflect,Bide,Metronome,Self-Destruct,Egg Bomb,Fire Blast,Swift,Skull Bash,Soft-Boiled,Dream Eater,Sky Attack,Rest,Thunder Wave,Psywave,Explosion,Rock Slide,Tri Attack,Substitute,Cut,Fly,Surf,Strength,Flash,Rat Gambit,Rock Rampage
)");

void TmLearnsets::buildTmLearnsets() {
    size_t count = std::min(LEARNSET_STRINGS.size(), Monster::MONSTERS.size());

    std::unordered_set<Move*> soldTmMoves;
    for (const std::string &s : MartItem::MART_ITEM_STRINGS) {
        std::vector<std::string> parts = utils::split(s, ',');
        if (parts.size() < 3) {
            continue;
        }
        for (size_t i = 2; i < parts.size(); i++) {
            std::string token = utils::trim(parts[i]);
            if (token.rfind("Move=", 0) != 0) {
                continue;
            }
            size_t lastSpace = token.find_last_of(' ');
            if (lastSpace == std::string::npos || lastSpace <= 5) {
                continue;
            }
            std::string moveName = token.substr(5, lastSpace - 5);
            Move *m = Move::MOVE_MAP[moveName];
            if (m) {
                soldTmMoves.insert(m);
            }
        }
    }

    for (size_t i = 0; i < count; i++) {
        Monster::MONSTERS[i].addMoves(LEARNSET_STRINGS[i]);
    }

    size_t levelupCount = std::min(count, LevelUpMove::LEVEL_UP_MOVES.size());
    for (size_t i = 0; i < levelupCount; i++) {
        for (const LevelUpMove &lu : LevelUpMove::LEVEL_UP_MOVES[i]) {
            if (lu.move && soldTmMoves.find(lu.move) != soldTmMoves.end()) {
                Monster::MONSTERS[i].learnable.insert(lu.move);
            }
        }
    }
}
