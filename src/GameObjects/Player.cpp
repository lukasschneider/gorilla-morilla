#include "Player.h"

Player::Player(SDL_Renderer *renderer, std::unique_ptr<Gun> gun){
    this->gun = std::move(gun);
    dRect = {static_cast<float>(500),static_cast<float>(500),64,64};
    SDL_Surface * surface = IMG_Load(path.c_str());
    sRect = {0, 0, surface->w, surface->h};
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Player::renderPlayer(SDL_Renderer *renderer) {
    /*cout << "Player" << endl;
    cout << "x: " << dRect.x << endl;
    cout << "y: " << dRect.y << endl;*/
    SDL_RendererFlip flip = (dir == RIGHT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    int screenWidth = 1280;
    int screenHeight = 720;
    SDL_FRect screenRect = {
            screenWidth / 2 - dRect.w / 2,
            screenHeight / 2 - dRect.h / 2,
            dRect.w,
            dRect.h
    };
    SDL_RenderCopyExF(renderer, texture, nullptr, &screenRect, 0.0, nullptr, flip);

    {/*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // set the draw color to red

    SDL_Rect outlineRect = {
            static_cast<int>(screenWidth / 2 - dRect.w / 2),
            static_cast<int>(screenHeight / 2 - dRect.h / 2),
            static_cast<int>(dRect.w),
            static_cast<int>(dRect.h)
    };
    SDL_RenderDrawRect(renderer, &outlineRect); // draw the outline

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // reset the draw color to black*/}
}



void Player::handleMovement(const Uint8* keyboardState, float deltaTime ,const Room & room) {
    float newX = dRect.x;
    float newY = dRect.y;

    float moveSpeed = speed * deltaTime;
    float dirX = 0.0f;
    float dirY = 0.0f;

    if (keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_W]) {
        dirY -= 1.0f;
    }
    if (keyboardState[SDL_SCANCODE_DOWN] || keyboardState[SDL_SCANCODE_S]) {
        dirY += 1.0f;
    }
    if (keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_A]) {
        dirX -= 1.0f;
        dir = LEFT;
    }
    if (keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_D]) {
        dirX += 1.0f;
        dir = RIGHT;
    }

    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length > 0.0f) {
        dirX /= length;
        dirY /= length;
        newX += dirX * moveSpeed;
        newY += dirY * moveSpeed;
    }

    // Check horizontal movement
    if (!room.checkCollision({static_cast<int>(newX), static_cast<int>(dRect.y), static_cast<int>(dRect.w), static_cast<int>(dRect.h)})) {
        dRect.x = newX;
    } else {
        // If collision, move the player as close to the obstacle as possible
        while (!room.checkCollision({static_cast<int>(dRect.x + dirX), static_cast<int>(dRect.y), static_cast<int>(dRect.w), static_cast<int>(dRect.h)})) {
            dRect.x += dirX;
        }
    }

    // Check vertical movement
    if (!room.checkCollision({static_cast<int>(dRect.x), static_cast<int>(newY), static_cast<int>(dRect.w), static_cast<int>(dRect.h)})) {
        dRect.y = newY;
    } else {
        // If collision, move the player as close to the obstacle as possible
        while (!room.checkCollision({static_cast<int>(dRect.x), static_cast<int>(dRect.y + dirY), static_cast<int>(dRect.w), static_cast<int>(dRect.h)})) {
            dRect.y += dirY;
        }
    }
}


