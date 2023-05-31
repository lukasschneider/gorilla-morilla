#include "../gorillagame.h"

SDL_FRect camera = {0,0,1280,720};
int mouseX,mouseY;
SDL_Rect crossDrect;

void adjustViewportToPlayer(SDL_FRect& viewport, const SDL_FRect& playerRect, int screenWidth, int screenHeight) {
    viewport.x = playerRect.x - screenWidth / 2 + playerRect.w / 2;
    viewport.y = playerRect.y - screenHeight / 2 + playerRect.h / 2;
}

SDL_Surface * surface;
SDL_Texture * crosshair;


void MainState::Init() {
    enemy = new Enemy(500,500,100);
    SDL_ShowCursor(SDL_DISABLE);
    auto gun = std::make_unique<Gun>(render);
    player = new Player(render,std::move(gun));
    surface = IMG_Load(BasePath"asset/graphic/ui/crosshair.png");
    crosshair = SDL_CreateTextureFromSurface(render,surface);
    SDL_FreeSurface(surface);

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
    room = new Room(1, render, map,&camera);}

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
    player->handleMovement(keyboardState,deltaT,*room);
}


void MainState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    adjustViewportToPlayer(camera,player->dRect,1280,720);
    player->gun->updateAngle(mouseX,mouseY,player->dRect,camera);
    crossDrect = {mouseX-50,mouseY-50,100,100};
    player->gun->updateBullets(deltaT);
    enemy->coll(player->gun->bullets);
    enemy->update();

}

void MainState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    room->renderBackboard(render);
    player->renderPlayer(render);
    player->gun->render(render);
    room->renderMap(render);
    player->gun->renderBullets(render,&camera);
    SDL_RenderCopy(render,crosshair, NULL,&crossDrect);
    enemy->render(render,camera);
}