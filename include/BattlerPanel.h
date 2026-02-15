#pragma once

class Battler;
struct SDL_Renderer;

class BattlerPanel {
public:
    void update(Battler *battler);
    void render(SDL_Renderer *renderer);
};
