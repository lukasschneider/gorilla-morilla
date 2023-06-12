
#ifndef GORILLAGAME_FLOORMANAGER_H
#define GORILLAGAME_FLOORMANAGER_H

#include "Floor.h"

class FloorManager {
public:
    Floor createFloor(SDL_Renderer *render, SDL_FRect *camera);
};


#endif //GORILLAGAME_FLOORMANAGER_H
