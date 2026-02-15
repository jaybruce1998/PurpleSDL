#include "TextRenderer.h"
#include <SDL.h>
#include <SDL_ttf.h>

static TTF_Font *s_font = nullptr;
static int s_lineHeight = 0;

bool TextRenderer::init(const std::string &fontPath, int pointSize) {
    cleanup();
    s_font = TTF_OpenFont(fontPath.c_str(), pointSize);
    if (!s_font) {
        return false;
    }
    s_lineHeight = TTF_FontLineSkip(s_font);
    return true;
}

void TextRenderer::shutdown() {
    cleanup();
}

SDL_Texture *TextRenderer::renderText(SDL_Renderer *renderer, const std::string &text, SDL_Color color, int wrapWidth) {
    if (!s_font || !renderer) {
        return nullptr;
    }
    SDL_Surface *surface = nullptr;
    if (wrapWidth > 0) {
        surface = TTF_RenderUTF8_Blended_Wrapped(s_font, text.c_str(), color, static_cast<Uint32>(wrapWidth));
    } else {
        surface = TTF_RenderUTF8_Blended(s_font, text.c_str(), color);
    }
    if (!surface) {
        return nullptr;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return tex;
}

int TextRenderer::lineHeight() {
    return s_lineHeight;
}

void TextRenderer::cleanup() {
    if (s_font) {
        TTF_CloseFont(s_font);
        s_font = nullptr;
    }
    s_lineHeight = 0;
}
