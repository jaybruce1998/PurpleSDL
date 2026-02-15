#include "SpriteManager.h"

SDL_Renderer *SpriteManager::s_renderer = nullptr;
std::unordered_map<std::string, SDL_Texture*> SpriteManager::s_cache;

void SpriteManager::setRenderer(SDL_Renderer *renderer) {
    s_renderer = renderer;
}

SDL_Texture* SpriteManager::loadTexture(const std::string &path) {
    if (!s_renderer) {
        throw std::runtime_error("Renderer not set in SpriteManager.");
    }
    auto it = s_cache.find(path);
    if (it != s_cache.end()) {
        return it->second;
    }
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface) {
        throw std::runtime_error("Failed to load texture: " + path + " (" + IMG_GetError() + ")");
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(s_renderer, surface);
    SDL_FreeSurface(surface);
    if (!tex) {
        throw std::runtime_error("Failed to create texture: " + path + " (" + SDL_GetError() + ")");
    }
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    s_cache[path] = tex;
    return tex;
}

SDL_Texture* SpriteManager::loadScaledTexture(const std::string &path, int w, int h) {
    SDL_Texture *src = loadTexture(path);
    SDL_Texture *target = SDL_CreateTexture(s_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    if (!target) {
        throw std::runtime_error("Failed to create target texture: " + path + " (" + SDL_GetError() + ")");
    }
    SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);
    SDL_Texture *prev = SDL_GetRenderTarget(s_renderer);
    SDL_SetRenderTarget(s_renderer, target);
    SDL_SetRenderDrawBlendMode(s_renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 0);
    SDL_RenderClear(s_renderer);
    SDL_Rect dst{0, 0, w, h};
    SDL_RenderCopy(s_renderer, src, nullptr, &dst);
    SDL_SetRenderTarget(s_renderer, prev);
    return target;
}

void SpriteManager::clear() {
    for (auto &pair : s_cache) {
        SDL_DestroyTexture(pair.second);
    }
    s_cache.clear();
}
