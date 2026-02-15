#pragma once

class Battler;
struct SDL_Renderer;

class BattleInfoPanel {
public:
    void update(Battler *player, Battler *enemy);
    void render(SDL_Renderer *renderer);
};
