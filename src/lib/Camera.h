#ifndef SDL_BASEGAME_CAMERA_H
#define SDL_BASEGAME_CAMERA_H

#include "global.h"
#include "../GameObjects/Player.h"

class Camera {
public:
    SDL_Rect viewport;
    Player * player;

    Camera(Player * player);

    void updateCamera();
    void renderCam(Renderer * render);
};


#endif //SDL_BASEGAME_CAMERA_H
