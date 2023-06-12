#ifndef GORILLAGAME_PICKUPS_H
#define GORILLAGAME_PICKUPS_H

#include "../lib/global.h"
#include "Player.h"

class Player;


class Pickup {
protected:
    SDL_FRect pos;
    SDL_Texture *texture{};
public:
    explicit Pickup(SDL_FRect pos);

    virtual void apply(Player * player) = 0;

    bool checkCollision(const SDL_FRect& playerRect);


    virtual ~Pickup();

    void render(SDL_Renderer *renderer, const SDL_FRect &vp);;


};

class Banana : public Pickup {
public:
    int value;
    std::string path = BasePath "asset/graphic/pickups/banana.png";

    Banana(SDL_FRect pos, SDL_Renderer *renderer);

    void apply(Player * player) override;

};


#endif //GORILLAGAME_PICKUPS_H
