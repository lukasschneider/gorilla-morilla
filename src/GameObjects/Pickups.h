#ifndef GORILLAGAME_PICKUPS_H
#define GORILLAGAME_PICKUPS_H

#include "../lib/global.h"
#include "Player.h"

class Player;


class Pickup {
public:
    int cost = 0;
    SDL_FRect pos;
    SDL_Texture *texture{};
    std::string description;
public:
    explicit Pickup(SDL_FRect pos);

    virtual void apply(Player * player) = 0;

    virtual ~Pickup();

    void render(SDL_Renderer *renderer, const SDL_FRect &vp);;


    virtual bool checkCollision(const SDL_FRect &playerRect, float range);
};

class Banana : public Pickup {
public:
    int value;
    std::string path = BasePath "asset/graphic/pickups/banana.png";

    Banana(SDL_FRect pos, SDL_Renderer *renderer);

    void apply(Player * player) override;

    bool checkCollision(const SDL_FRect& playerRect, float range) override {
        return SDL_HasIntersectionF(&pos, &playerRect) == SDL_TRUE;
    }

};

class DMGBuff : public Pickup {
public:
    std::string path = BasePath "asset/graphic/pickups/powerup.png";
    DMGBuff(SDL_FRect pos, SDL_Renderer *renderer);
    void apply(Player * player) override;
};

class Firerate : public Pickup {
public:
    std::string path = BasePath "asset/graphic/pickups/powerup.png";
    Firerate(SDL_FRect pos, SDL_Renderer *renderer);
    void apply(Player * player) override;
};

class Magbuff : public Pickup {
public:
    std::string path = BasePath "asset/graphic/pickups/powerup.png";
    Magbuff(SDL_FRect pos, SDL_Renderer *renderer);
    void apply(Player * player) override;
};
#endif //GORILLAGAME_PICKUPS_H
