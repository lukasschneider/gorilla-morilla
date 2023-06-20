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

    FloorManager fm;
    this->floor = fm.createFloor(render, &camera);
    this->room = floor.getStartRoom();

    userinterface = new ui(render, player, &camera);
    enemy = new Enemy(800, 800, 100, &room->activePickups);


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
        std::array<Room *, 4> neighbors{};
        neighbors = floor.getNeighbors(room);
        player->dRect.y = player->dRect.y + ((float) room->getMapPixelHeight() - 128) - 100;
        this->room = neighbors[0];
    }
    if (player->handleTeleport(*room) == TELEPORT_RIGHT) {
        std::array<Room *, 4> neighbors{};
        neighbors = floor.getNeighbors(room);
        player->dRect.x = player->dRect.x - ((float) room->getMapPixelWidth() - 128) + 200;
        this->room = neighbors[1];
    }
    if (player->handleTeleport(*room) == TELEPORT_BOTTOM) {
        std::array<Room *, 4> neighbors{};
        neighbors = floor.getNeighbors(room);
        player->dRect.y = player->dRect.y - ((float) room->getMapPixelHeight() - 128) + 100;
        this->room = neighbors[2];

    }
    if (player->handleTeleport(*room) == TELEPORT_LEFT) {
        std::array<Room *, 4> neighbors{};
        neighbors = floor.getNeighbors(room);
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
    enemy->update(deltaT,*room);
    userinterface->update();
    auto r = transformMatrix(room->map_layer[1]);
    enemy->path = aStarSearch(r, &enemy->body, &player->dRect, false);


}

void MainState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {

    // Backboard includes tree area around room and green background
    room->render_backboard(render);
    room->render_backboard_styling(render);

    room->renderPickups(camera);
    // Collision includes every tile the player can collide with
    room->render_mapborder_open(render);

    player->renderPlayer(render);
    player->gun->render(render);
    player->gun->renderBullets(render, &camera);
    SDL_RenderCopy(render, crosshair, NULL, &crossDrect);
    enemy->render(render, camera);
    // Forground renders every styling aspekt

    //room->render_markup(render);
    room->render_mapborder_styling(render);

    userinterface->drawUi();
    drawPath(enemy->path,camera,64);
}
