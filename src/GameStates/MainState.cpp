#include "../gorillagame.h"

void MainState::Init() {
    player = new Player(render);
    Vector<Vector<int>> map = {
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 10,9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8, 6},
            {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6},
            {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6},
            {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6},
            {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6},
            {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6},
            {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6},
            {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6},
            {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6},
            {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6},
            {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6},
            {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6},
            {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6},
            {6, 4,3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6}
    };
    room = new Room(1, render, map);
    camera = new Camera(player);
}

void MainState::UnInit() {}

void MainState::Events(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_PumpEvents();

    Event event;
    while (SDL_PollEvent(&event)) {
        if (game.HandleEvent(event))
            continue;
    }

    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    player->handleMovement(keyboardState,deltaT,*room);
}

void MainState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    camera->updateCamera();

    /**
     *  1. Spieler gegner Kollision
     *  if (spiler->bulletile is collison enenmy
     *  2. Spieler Room Kollision
     *  3. Movement Spieler/Gegner
     *  4. Room update (Currency Fällt)
     */
}

void MainState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    room->renderBackboard(render);
    player->renderPlayer(render);
    room->renderMap(render);
    camera->renderCam(render);

}