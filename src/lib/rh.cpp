#include "rh.h"

RS &RS::getInstance() {
    if (instance == nullptr) {
        instance = new RS;
    }
    return *instance;
}

void RS::init(SDL_Renderer *ren) {
    renderer = ren;
}

SDL_Renderer *RS::get() const {
    return renderer;
}

RS* RS::instance = nullptr;

