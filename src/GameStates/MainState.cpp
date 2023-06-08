#include "../gorillagame.h"

SDL_FRect camera = {0, 0, 1280, 720};
int mouseX, mouseY;
SDL_Rect crossDrect;

void adjustViewportToPlayer(SDL_FRect &viewport, const SDL_FRect &playerRect, int screenWidth, int screenHeight) {
    viewport.x = playerRect.x - screenWidth / 2 + playerRect.w / 2;
    viewport.y = playerRect.y - screenHeight / 2 + playerRect.h / 2;
}

SDL_Surface *surface;
SDL_Texture *crosshair;


void MainState::Init() {
    enemy = new Enemy(500, 500, 100);
    SDL_ShowCursor(SDL_DISABLE);
    auto gun = std::make_unique<Gun>(render);
    player = new Player(render, std::move(gun));
    surface = IMG_Load(BasePath"asset/graphic/ui/crosshair.png");
    crosshair = SDL_CreateTextureFromSurface(render, surface);
    SDL_FreeSurface(surface);

    RoomManager rm;
    room = rm.create_room(0, render, RoomManager::MapType::TP_TOP_LEFT_BOTTOM_RIGHT, &camera);
    Room *room1 = rm.create_room(1, render, RoomManager::MapType::TP_TOP_LEFT, &camera);

    Floor floor1;
    floor1.addEdge(room, room1);


}

void MainState::UnInit() {
    delete enemy;
    delete player;
    delete room;
}

void MainState::Events(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_PumpEvents();

    Event event;
    while (SDL_PollEvent(&event)) {
        if (game.HandleEvent(event))
            continue;
        else if (event.type == SDL_MOUSEMOTION) {
            mouseX = event.motion.x;
            mouseY = event.motion.y;
        }
    }

    // Check if the left mouse button is being held down
    Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        player->gun->fire(render, &camera);
    }

    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    player->handleMovement(keyboardState, deltaT, *room);
    

}


void MainState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {

    adjustViewportToPlayer(camera, player->dRect, 1280, 720);
    player->gun->updateAngle(mouseX, mouseY, player->dRect, camera);
    crossDrect = {mouseX - 50, mouseY - 50, 100, 100};
    player->gun->updateBullets(deltaT);
    std::vector<Bullet*> bulletPtrs;
    for (auto& bullet : player->gun->bullets) {
        bulletPtrs.push_back(&bullet);
    }
    enemy->coll(bulletPtrs);

    enemy->update(deltaT);

}

void MainState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    // Backboard includes tree area around room and green background
    room->renderBackboard(render);
    // Collision includes every tile the player can colide with
    room->renderCollision(render);
    player->renderPlayer(render);
    player->gun->render(render);
    player->gun->renderBullets(render, &camera);
    SDL_RenderCopy(render, crosshair, NULL, &crossDrect);
    enemy->render(render, camera);
    // Forground renders every styling aspekt
    room->renderForeground(render);
}