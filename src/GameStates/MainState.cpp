#include "../gorillagame.h"
#include "../lib/rh.h"
#include "../lib/ph.h"
#include "../lib/astar.h"
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

    SDL_ShowCursor(SDL_DISABLE);
    auto gun = std::make_unique<Gun>(render);
    player = new Player(render, std::move(gun));
    surface = IMG_Load(BasePath"asset/graphic/ui/crosshair.png");
    crosshair = SDL_CreateTextureFromSurface(render, surface);
    SDL_FreeSurface(surface);

    //FloorManager fm;
    //this->floor = fm.createFloor(render, &camera);
    //this->room = floor.getStartRoom();
    RoomManager rm;
    this->room = rm.create_room(0,render,RoomManager::MapType::TEST,&camera);

    userinterface = new ui(render, player, &camera);
    enemy = new Enemy(600, 600, 100, &room->activePickups);


    RS::getInstance().init(render);
    PS::getInstance().init(player);
}

void MainState::UnInit() {
    delete enemy;
    delete player;
    delete room;
    delete userinterface;
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

    if(keyboardState[SDL_SCANCODE_R]) {
        player->gun->reload();
    }

    player->handleMovement(keyboardState, deltaT, *room);


    if (player->handleTeleport(*room) == TELEPORT_TOP) {
        cout << "AKTUELLER RAUM: " << room->id << endl;
        std::array<Room *, 4> neighbors{};
        neighbors = floor.getNeighbors(room);
        printf("TELEPORT ACTION TOP: \n");
        player->dRect.y = player->dRect.y + ((float) room->getMapPixelHeight() - 128) - 100;
        this->room = neighbors[0];
    }
    if (player->handleTeleport(*room) == TELEPORT_RIGHT) {
        cout << "AKTUELLER RAUM: " << room->id << endl;
        printf("TELEPORT ACTION RIGHT: \n");
        std::array<Room *, 4> neighbors{};
        neighbors = floor.getNeighbors(room);
        player->dRect.x = player->dRect.x - ((float) room->getMapPixelWidth() - 128) + 200;
        this->room = neighbors[1];
    }
    if (player->handleTeleport(*room) == TELEPORT_BOTTOM) {
        cout << "AKTUELLER RAUM: " << room->id << endl;
        std::array<Room *, 4> neighbors{};
        neighbors = floor.getNeighbors(room);
        printf("TELEPORT ACTION LEFT: \n");
        player->dRect.y = player->dRect.y - ((float) room->getMapPixelHeight() - 128) + 100;
        this->room = neighbors[2];

    }
    if (player->handleTeleport(*room) == TELEPORT_LEFT) {
        cout << "AKTUELLER RAUM: " << room->id << endl;
        std::array<Room *, 4> neighbors{};
        neighbors = floor.getNeighbors(room);
        printf("TELEPORT ACTION LEFT: \n");
        player->dRect.x = player->dRect.x + ((float) room->getMapPixelWidth() - 128) - 100;
        this->room = neighbors[3];
    }
}

void MainState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    adjustViewportToPlayer(camera,player->dRect,1280,720);
    player->gun->update(mouseX, mouseY, player->dRect, camera,deltaT);
    crossDrect = {mouseX-50,mouseY-50,100,100};
    player->gun->updateBullets(deltaT);
    room->updatePickups();
    enemy->coll(player->gun->bullets);
    enemy->update(deltaT);
    userinterface->update();


}

void MainState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {

    // Backboard includes tree area around room and green background
    room->renderBackboard(render);
    room->renderPickups(camera);
    // Collision includes every tile the player can collide with
    room->renderCollision(render);
    player->renderPlayer(render);
    player->gun->render(render);
    player->gun->renderBullets(render, &camera);
    SDL_RenderCopy(render, crosshair, NULL, &crossDrect);
    enemy->render(render, camera);
    // Forground renders every styling aspekt
    room->renderForeground(render);
    userinterface->drawUi();
    int playerX = (int)player->dRect.x, playerY = (int)player->dRect.y;
    int enemyX = (int)enemy->body.x, enemyY = (int)enemy->body.y;
    Pair playerPos = convertToYXandGridPos(playerX,playerY,TILE_SIZE);
    Pair enemyPos = convertToYXandGridPos(enemyX,enemyY,TILE_SIZE);
    Pair ePos = make_pair(1,1);

    //playerPos = make_pair(21,10);
    auto r = transformMatrix(room->map_layer[1]);
    auto path = aStarSearch(r, ePos, playerPos);
    drawPath(path,camera, TILE_SIZE);
}
