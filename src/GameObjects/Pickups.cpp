#include "Pickups.h"


Pickup::Pickup(SDL_FRect pos) : pos(pos) {
}

bool Pickup::checkCollision(const SDL_FRect &playerRect) {
    return SDL_HasIntersectionF(&pos, &playerRect) == SDL_TRUE;
}

Pickup::~Pickup() {
    SDL_DestroyTexture(texture);
}

void Pickup::render(SDL_Renderer *renderer, const SDL_FRect &vp) {
    SDL_FRect tmp = {pos.x - vp.x, pos.y - vp.y, pos.w, pos.h};
    SDL_RenderCopyF(renderer, texture, nullptr, &tmp);
}

Banana::Banana(SDL_FRect pos, SDL_Renderer *renderer)
        : Pickup(pos), value(rand() % 2 + 1) {
    SDL_Surface *sheet = IMG_Load(path.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, sheet);
    SDL_FreeSurface(sheet);
}

void Banana::apply(Player * player) {
    player->currency += value;
}
