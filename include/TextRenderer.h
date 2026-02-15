#pragma once

#include <string>

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Color;

class TextRenderer {
public:
    static bool init(const std::string &fontPath, int pointSize);
    static void shutdown();
    static SDL_Texture *renderText(SDL_Renderer *renderer, const std::string &text, SDL_Color color, int wrapWidth = 0);
    static int lineHeight();

private:
    static void cleanup();
};
