#ifndef GORILLAGAME_PICKUPS_H
#define GORILLAGAME_PICKUPS_H

#include "../lib/global.h"

class Pickup {
protected:
    SDL_FRect pos;
    SDL_Texture *texture{};
public:
    explicit Pickup(SDL_FRect pos) : pos(pos) {
    }

    virtual void apply(float * val) = 0;

    bool checkCollision(const SDL_FRect& playerRect) {
        return SDL_HasIntersectionF(&pos, &playerRect) == SDL_TRUE;
    }




    void render(SDL_Renderer *renderer, const SDL_FRect &vp) {
        SDL_FRect tmp = {pos.x - vp.x, pos.y - vp.y, pos.w, pos.h};
        SDL_RenderCopyF(renderer, texture, nullptr, &tmp);
    };


};

class Banana : public Pickup {
public:
    int value;
    std::string path = BasePath "asset/graphic/pickups/banana.png";

    Banana(SDL_FRect pos, SDL_Renderer *renderer)
            : Pickup(pos), value(rand() % 2 + 1) {
        SDL_Surface *sheet = IMG_Load(path.c_str());
        texture = SDL_CreateTextureFromSurface(renderer, sheet);
        SDL_FreeSurface(sheet);
    }

    void apply(float * val){
        val += value;
    }

};


#endif //GORILLAGAME_PICKUPS_H
