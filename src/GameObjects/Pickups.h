#ifndef GORILLAGAME_PICKUPS_H
#define GORILLAGAME_PICKUPS_H

#include "../lib/global.h"

class Pickup {
protected:
    SDL_FRect pos;
    SDL_Texture* texture;

public:
    Pickup(SDL_FRect pos, SDL_Renderer* renderer, const std::string& path) : pos(pos) {
        SDL_Surface *sheet = IMG_Load(path.c_str());
        texture = SDL_CreateTextureFromSurface(renderer, sheet);
        SDL_FreeSurface(sheet);
    }

    //virtual void apply(Player * player) = 0;

    void render(SDL_Renderer* renderer){
        SDL_RenderCopyF(renderer,texture, nullptr,&pos);
    };
};

class Banana : public Pickup {
private:
    int value;
public:
    std::string path = BasePath "asset/graphic/pickups/banane.png";
    Banana(SDL_FRect pos, SDL_Renderer* renderer) : Pickup(pos,renderer,path) {
        value = rand() % 5;
    };
};




#endif //GORILLAGAME_PICKUPS_H
