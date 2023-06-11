#ifndef GORILLAGAME_PICKUPS_H
#define GORILLAGAME_PICKUPS_H

#include "../lib/global.h"

class Pickup {
protected:
    SDL_FRect pos;
    SDL_Texture *texture{};
public:
    explicit Pickup(SDL_FRect pos);

    virtual void apply() = 0;

    bool checkCollision(const SDL_FRect& playerRect);




    void render(SDL_Renderer *renderer, const SDL_FRect &vp);;


};

class Banana : public Pickup {
public:
    int value;
    std::string path = BasePath "asset/graphic/pickups/banana.png";

    Banana(SDL_FRect pos, SDL_Renderer *renderer);

    void apply() override;

};


#endif //GORILLAGAME_PICKUPS_H
