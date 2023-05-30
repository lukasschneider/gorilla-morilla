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
    SDL_ShowCursor(SDL_DISABLE);
    gun = std::make_unique<Gun>(render);
    player = new Player(render,std::move(gun));

    // Crosshair Init
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

void MainState::UnInit() {}

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
    player->handleMovement(keyboardState,deltaT,*room);
}

void MainState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    adjustViewportToPlayer(camera,player->dRect,1280,720);
    player->gun->updateAngle(mouseX,mouseY,player->dRect,camera);
    crossDrect = {mouseX-50,mouseY-50,100,100};
}

void MainState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    room->renderBackboard(render);
    player->renderPlayer(render);
    player->gun->render(render);
    room->renderMap(render);

    SDL_RenderCopy(render,crosshair, NULL,&crossDrect);
}