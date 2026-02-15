#include "Evolution.h"
#include "Utils.h"

static std::vector<std::string> splitLinesEvolution(const std::string &s) {
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

std::vector<std::string> Evolution::EVOLUTION_STRINGS = splitLinesEvolution(R"(1,(Level 16),2
2,(Level 32),3
4,(Level 16),5
5,(Level 36),6
7,(Level 16),8
8,(Level 36),9
10,(Level 7),11
11,(Level 10),12
13,(Level 7),14
14,(Level 10),15
16,(Level 18),17
17,(Level 36),18
19,(Level 20),20
21,(Level 20),22
23,(Level 22),24
25,Thunder Stone,26
27,(Level 22),28
29,(Level 16),30
30,Moon Stone,31
32,(Level 16),33
33,Moon Stone,34
35,Moon Stone,36
37,Fire Stone,38
39,Moon Stone,40
41,(Level 22),42
43,(Level 21),44
44,Leaf Stone,45
46,(Level 24),47
48,(Level 31),49
50,(Level 26),51
52,(Level 28),53
54,(Level 33),55
56,(Level 28),57
58,Fire Stone,59
60,(Level 25),61
61,Water Stone,62
63,(Level 16),64
64,Trade,65
66,(Level 28),67
67,Trade,68
69,(Level 21),70
70,Leaf Stone,71
72,(Level 30),73
74,(Level 25),75
75,Trade,76
77,(Level 40),78
79,(Level 37),80
81,(Level 30),82
84,(Level 31),85
86,(Level 34),87
88,(Level 38),89
90,Water Stone,91
92,(Level 25),93
93,Trade,94
96,(Level 26),97
98,(Level 28),99
100,(Level 30),101
102,Leaf Stone,103
104,(Level 28),105
109,(Level 35),110
111,(Level 42),112
116,(Level 32),117
118,(Level 33),119
120,Water Stone,121
129,(Level 20),130
138,(Level 40),139
140,(Level 40),141
147,(Level 30),148
148,(Level 55),149)");

std::vector<Evolution*> Evolution::EVOLUTIONS;

Evolution::Evolution(const std::string &stone, int evo) {
    this->stone = stone;
    this->evo = evo;
}

Evolution::Evolution(int level, int evo) {
    this->level = level;
    this->evo = evo;
}

void Evolution::buildEvolutions() {
    EVOLUTIONS.assign(Monster::MONSTERS.size(), nullptr);
    for (const std::string &s : EVOLUTION_STRINGS) {
        std::vector<std::string> a = utils::split(s, ',');
        int index = std::stoi(a[0]);
        int evo = std::stoi(a[2]);
        if (!a[1].empty() && a[1][0] == '(') {
            std::string levelPart = a[1];
            size_t close = levelPart.find(')');
            std::string num = levelPart.substr(levelPart.find(' ') + 1, close - levelPart.find(' ') - 1);
            EVOLUTIONS[index] = new Evolution(std::stoi(num), evo);
        } else {
            EVOLUTIONS[index] = new Evolution(a[1], evo);
        }
    }
}

std::string Evolution::toString() const {
    if (stone.empty()) {
        return "Level " + std::to_string(level) + "->" + std::to_string(evo);
    }
    return stone + "->" + std::to_string(evo);
}
