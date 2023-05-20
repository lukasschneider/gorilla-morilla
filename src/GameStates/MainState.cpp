#include "../gorillagame.h"

void MainState::Init() {
    player = new Player;
    Vector<Vector<int>> map = {
            {1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
            {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
            {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
            {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
            {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}
    };
    room = new Room(1, render, &map);

    if (!image) {
        image = IMG_LoadTexture(render, BasePath "asset/graphic/background.png");
        if (!image)
            cerr << "IMG_LoadTexture failed: " << IMG_GetError() << endl;
    }
}

void MainState::UnInit() {}

void MainState::Events(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_PumpEvents();

    Event event;
    while (SDL_PollEvent(&event)) {
        if (game.HandleEvent(event))
            continue;

        int speed = 5;

        const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);

        if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP]) {
            player->tile.y -= speed;
        }
        if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN]) {
            player->tile.y += speed;
        }
        if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT]) {
            player->tile.x -= speed;
        }
        if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT]) {
            player->tile.x += speed;
        }
    }
}

void MainState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    /**
     *  1. Spieler gegner Kollision
     *  if (spiler->bulletile is collison enenmy
     *  2. Spieler Room Kollision
     *  3. Movement Spieler/Gegner
     *  4. Room update (Currency Fällt)
     */
}

void MainState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {

    room->renderMap(render);

//    const Point &winSize = game.GetWindowSize();
//    {
//        const Rect dst_rect{0, 0, winSize.x, winSize.y};
//        SDL_RenderCopy(render, image, EntireRect, &dst_rect);
//    }
//
//    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
//    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
//    SDL_RenderFillRect(render, &player->tile);

}