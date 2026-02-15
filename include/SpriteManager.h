#pragma once

#include "Utils.h"

struct SDL_Renderer;
struct SDL_Texture;

class SpriteManager {
public:
    static void setRenderer(SDL_Renderer *renderer);
    static SDL_Texture* loadScaledTexture(const std::string &path, int w, int h);
    static SDL_Texture* loadTexture(const std::string &path);
    static void clear();

private:
    static SDL_Renderer *s_renderer;
    static std::unordered_map<std::string, SDL_Texture*> s_cache;
};
