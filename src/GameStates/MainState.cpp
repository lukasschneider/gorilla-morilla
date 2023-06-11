#include "../gorillagame.h"
#include "../lib/rh.h"

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
    RS::getInstance().init(render);
    SDL_ShowCursor(SDL_DISABLE);
    auto gun = std::make_unique<Gun>(render);
    player = new Player(render, std::move(gun));
    surface = IMG_Load(BasePath"asset/graphic/ui/crosshair.png");
    crosshair = SDL_CreateTextureFromSurface(render, surface);
    SDL_FreeSurface(surface);

    RoomManager rm;
    room = rm.create_room(0, render, RoomManager::MapType::TP_TOP_LEFT_BOTTOM_RIGHT, &camera);
    Room *room1 = rm.create_room(1, render, RoomManager::MapType::TP_TOP_LEFT, &camera);

    floor = Floor();
    floor.addEdge(room, room1);

    userinterface = new ui(render,player,&camera);
    enemy = new Enemy(500,500,100,&room->activePickups);

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
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    // Check if the left mouse button is being held down
    Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        player->gun->fire(render, &camera);
    }


    player->handleMovement(keyboardState,deltaT,*room);

    if(keyboardState[SDL_SCANCODE_R]) {
        player->gun->reload();
    }
    if(player->handleTeleport(*room) == TELEPORT_RIGHT) {
        cout << "TELEPORT ACTION" << endl;
        std::vector<Room *> neighbors;
        neighbors = floor.getNeighbors(room);
        cout << neighbors[0]->id << endl;
        // TODO: Update Player Postion
        player->dRect.x = 500;
        player->dRect.y = 500;
        // TODO: Get information on which teleport u used for correkt new map
        this->room = neighbors[0];
    }
}




void MainState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    adjustViewportToPlayer(camera,player->dRect,1280,720);
    player->gun->update(mouseX, mouseY, player->dRect, camera,deltaT);
    crossDrect = {mouseX-50,mouseY-50,100,100};
    player->gun->updateBullets(deltaT);
    enemy->coll(player->gun->bullets);
    enemy->update(deltaT);
    userinterface->update();

}

void MainState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {

    // Backboard includes tree area around room and green background
    room->renderBackboard(render);
    room->renderPickups(camera);
    // Collision includes every tile the player can colide with
    room->renderCollision(render);
    player->renderPlayer(render);
    player->gun->render(render);
    room->renderMap(render);
    player->gun->renderBullets(render,&camera);
    SDL_RenderCopy(render,crosshair, NULL,&crossDrect);
    enemy->render(render,camera);
    userinterface->drawUi();

    player->gun->renderBullets(render, &camera);
    SDL_RenderCopy(render, crosshair, NULL, &crossDrect);
    enemy->render(render, camera);
    // Forground renders every styling aspekt
    room->renderForeground(render);
}