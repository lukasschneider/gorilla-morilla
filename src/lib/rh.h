#ifndef RH_H
#define RH_H
#include "SDL.h"

class RS {
private:
    SDL_Renderer* renderer = nullptr;
    static RS* instance;

    RS() {}

public:
    static RS& getInstance();

    void init(SDL_Renderer* ren);

    [[nodiscard]] SDL_Renderer* get() const;

    // Prevent copy and assignment
    RS(RS const&) = delete;
    void operator=(RS const&)  = delete;
};




#endif