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
    Room *room2 = rm.create_room(2, render, RoomManager::MapType::TP_BOTTOM_LEFT, &camera);
    Room *room3 = rm.create_room(3, render, RoomManager::MapType::TP_BOTTOM_RIGHT, &camera);
    Room *room4 = rm.create_room(4, render, RoomManager::MapType::TP_TOP_BOTTOM, &camera);
    Room *room5 = rm.create_room(5, render, RoomManager::MapType::TP_RIGHT, &camera);
    Room *room6 = rm.create_room(6, render, RoomManager::MapType::TP_TOP, &camera);

    floor = Floor();
    floor.addEdge(room, room3);
    floor.addEdge(room, room1);
    floor.addEdge(room, room5);
    floor.addEdge(room, room4);
    floor.addEdge(room1, room2);
    floor.addEdge(room2, room3);
    floor.addEdge(room4, room6);

    userinterface = new ui(render, player, &camera);
    enemy = new Enemy(500, 500, 100, &room->activePickups);

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

    player->handleMovement(keyboardState, deltaT, *room);

    if (player->handleTeleport(*room) == TELEPORT_BOTTOM) {
        cout << "AKTUELLER RAUM: " << room->id << endl;
        if (keyboardState[SDL_SCANCODE_R]) {
            player->gun->reload();
        }
        if (player->handleTeleport(*room) == TELEPORT_RIGHT) {
            cout << "TELEPORT ACTION" << endl;
            std::vector<Room *> neighbors;
            neighbors = floor.getNeighbors(room);
            printf("TELEPORT ACTION BOTTOM: \n");
            for (Room *r: neighbors) {
                cout << r->id << endl;
            }
            player->dRect.y = player->dRect.y - ((float) room->getMapPixelHeight() - 128) + 100;
            this->room = neighbors[2];
        }
        if (player->handleTeleport(*room) == TELEPORT_LEFT) {
            cout << "AKTUELLER RAUM: " << room->id << endl;
            std::vector<Room *> neighbors;
            neighbors = floor.getNeighbors(room);
            printf("TELEPORT ACTION LEFT: \n");
            for (Room *r: neighbors) {
                cout << r->id << endl;
            }
            player->dRect.x = player->dRect.x + ((float) room->getMapPixelWidth() - 128) - 100;
            this->room = neighbors[3];
        }
        if (player->handleTeleport(*room) == TELEPORT_TOP) {
            cout << "AKTUELLER RAUM: " << room->id << endl;
            std::vector<Room *> neighbors;
            neighbors = floor.getNeighbors(room);
            printf("TELEPORT ACTION TOP: \n");
            for (Room *r: neighbors) {
                cout << r->id << endl;
            }
            player->dRect.y = player->dRect.y + ((float) room->getMapPixelHeight() - 128) - 100;
            this->room = neighbors[0];
        }

        if (player->handleTeleport(*room) == TELEPORT_RIGHT) {
            cout << "AKTUELLER RAUM: " << room->id << endl;
            printf("TELEPORT ACTION RIGHT: \n");
            std::vector<Room *> neighbors;
            neighbors = floor.getNeighbors(room);
            for (Room *r: neighbors) {
                cout << r->id << endl;
            }
            player->dRect.x = player->dRect.x - ((float) room->getMapPixelWidth() - 128) + 200;
            // TODO: Get information on which teleport u used for correkt new mapdsd
            this->room = neighbors[1];
        }
    }
}

void MainState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    adjustViewportToPlayer(camera, player->dRect, 1280, 720);
    player->gun->update(mouseX, mouseY, player->dRect, camera, deltaT);
    crossDrect = {mouseX - 50, mouseY - 50, 100, 100};
    player->gun->updateBullets(deltaT);
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
    player->gun->renderBullets(render, &camera);
    SDL_RenderCopy(render, crosshair, NULL, &crossDrect);
    enemy->render(render, camera);

    player->gun->renderBullets(render, &camera);
    SDL_RenderCopy(render, crosshair, NULL, &crossDrect);
    enemy->render(render, camera);
    // Forground renders every styling aspekt
    room->renderForeground(render);
    userinterface->drawUi();
}
