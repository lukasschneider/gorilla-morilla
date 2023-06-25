#include "../gorillagame.h"
#include "../lib/rh.h"
#include "../lib/ph.h"
#include "../lib/sh.h"
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
    soundManager = new SoundManager;
    soundManager->loadSounds();
    RS::getInstance().init(render);

    SDL_ShowCursor(SDL_DISABLE);
    //SDL_SetRelativeMouseMode(SDL_TRUE);

    auto gun = std::make_unique<Gun>(render);
    player = new Player(render, std::move(gun));
    surface = IMG_Load(BasePath"asset/graphic/ui/crosshair.png");
    crosshair = SDL_CreateTextureFromSurface(render, surface);
    SDL_FreeSurface(surface);

    FloorManager fm;
    this->floor = fm.createFloor(render, &camera);
    this->room = floor.getStartRoom();

    userinterface = new ui(render, player, &camera);
    //enemy = new Enemy(800, 800, 100, &room->activePickups);

    soundManager->playSound(SoundId::MUSIC, -1,2);
    soundManager->playSound(SoundId::AMBIENT,-1, 3);

    PS::getInstance().init(player);
    SMS::getInstance().init(soundManager);
}

void MainState::UnInit() {
    delete player;
    delete room;
    delete userinterface;
    delete soundManager;
    SDL_DestroyTexture(userinterface->deadScreen);
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
        else if (event.button.button == SDL_BUTTON_RIGHT) {
            player->gun->reload();
        }
    }
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    // Check if the left mouse button is being held down
    Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        player->gun->fire(render, &camera , soundManager);

    }

    if (keyboardState[SDL_SCANCODE_R]) {
        player->gun->reload();
    }

    if (player->health == 0 || userinterface->won) {
        if(keyboardState[SDL_SCANCODE_SPACE]) {
            MainState::UnInit();
            MainState::Init();
        }
    }

    if(player->health != 0) {
        player->handleMovement(keyboardState, deltaT, *room);
    }


    if (player->handleTeleport(*room) == TELEPORT_TOP) {
        std::array<Room *, 4> neighbors{};
        neighbors = floor.getNeighbors(room);
        player->dRect.y = player->dRect.y + ((float) room->getMapPixelHeight() - 128) - 125;
        this->room = neighbors[0];
    }
    if (player->handleTeleport(*room) == TELEPORT_RIGHT) {
        std::array<Room *, 4> neighbors{};
        neighbors = floor.getNeighbors(room);
        player->dRect.x = player->dRect.x - ((float) room->getMapPixelWidth() - 128) + 225;
        this->room = neighbors[1];
    }
    if (player->handleTeleport(*room) == TELEPORT_BOTTOM) {
        std::array<Room *, 4> neighbors{};
        neighbors = floor.getNeighbors(room);
        player->dRect.y = player->dRect.y - ((float) room->getMapPixelHeight() - 128) + 125;
        this->room = neighbors[2];

    }
    if (player->handleTeleport(*room) == TELEPORT_LEFT) {
        std::array<Room *, 4> neighbors{};
        neighbors = floor.getNeighbors(room);
        player->dRect.x = player->dRect.x + ((float) room->getMapPixelWidth() - 128) - 125;
        this->room = neighbors[3];
    }
}

void MainState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {

    if (player->health == 0) {
        player->maxSpeed = 0;
        player->gun->ammo = 0;
        player->currency = 0;
    }

    if (room->enemies.empty()) {
        room->cleared = true;
    }

    if (floor.checkCleared()) {
        userinterface->won = true;
    }

    adjustViewportToPlayer(camera, player->dRect, 1280, 720);
    if(player->health != 0) {
        player->gun->update(mouseX, mouseY, player->dRect, camera, deltaT);
    }
    crossDrect = {mouseX - 50, mouseY - 50, 100, 100};
    player->gun->updateBullets(deltaT);
    room->updatePickups();
    auto r = transformMatrix(room->map_layer[2]);

    for (auto m: room->enemies) {
        m->coll(player->gun->bullets);
        m->update(deltaT, *room);
        m->path = aStarSearch(r, &m->dRect, &player->dRect, false, room->enemies);
        m->attackUpdate();

    }

    userinterface->update();

}

void MainState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {

    // Backboard includes tree area around room and green background
    room->render_backboard(render);
    room->render_backboard_styling(render);


    if (room->enemies.empty()) {
        room->render_mapborder_open(render);
    } else {
        room->render_mapborder_closed(render);
    }


    player->renderPlayer(render);
    player->gun->render(render);
    player->gun->renderBullets(render, &camera);

    for(auto e : room->enemies){
        e->render(render, camera);
    }

    room->render_mapborder_styling(render);


    // Forground renders every styling aspekt
    userinterface->drawUi();

    room->renderPickups(camera);


    SDL_RenderCopy(render, crosshair, nullptr, &crossDrect);
    //drawPath(m->path,camera,64);
}
