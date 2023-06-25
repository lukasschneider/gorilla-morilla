#include "Gun.h"

Gun::Gun(SDL_Renderer *render) :ammo(30), magazin(30), bullets(200) {
    int width, height;
    SDL_Surface * surface = IMG_Load(path.c_str());
    srcRect = {0, 0, surface->w, surface->h};
    texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_FreeSurface(surface);
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    [[maybe_unused]] auto ratio = width/height;
    dstRect = { 0, 0, 64,static_cast<float>(64.0/ratio)};

    reloadTime = 1.0f;
    timeSinceLastReload = 0.0f;
}


void Gun::update(int mouseX, int mouseY, const SDL_FRect &playerRect, const SDL_FRect &viewport, float dt) {
    float centerX = playerRect.x + playerRect.w / 2 - viewport.x;
    float centerY = playerRect.y + playerRect.h / 2 - viewport.y;

    // Adjust the gun's position to the center of the player.
    dstRect.x = centerX - dstRect.w / 2;
    dstRect.y = centerY - dstRect.h / 2;

    float deltaX = static_cast<float>(mouseX) - centerX;
    float deltaY = static_cast<float>(mouseY) - centerY;

    // Assuming that the right direction (positive x-axis) is the reference.
    float refX = 1.0f, refY = 0.0f;

    // Dot product of reference vector and the vector to the mouse.
    float dot = refX * deltaX + refY * deltaY;
    // Cross product of reference vector and the vector to the mouse.
    float cross = refX * deltaY - refY * deltaX;

    angle = static_cast<float>(atan2(cross, dot) * (180 / M_PI));
    if (angle < 0) {
        angle += 360;
    }
    // Assuming the radius at which the gun should orbit the player.
    float orbitRadius = 30;
    // Calculate the new position of the gun's center.
    dstRect.x = static_cast<float>(centerX + orbitRadius * cos(angle * M_PI / 180.0f) - dstRect.w / 2);
    dstRect.y = static_cast<float>(centerY + orbitRadius * sin(angle * M_PI / 180.0f) - dstRect.h / 2);

    // Reloading the weapon
    if (isReloading) {
        timeSinceLastReload += dt;

        if (timeSinceLastReload >= reloadTime) {
            ammo = magazin;
            isReloading = false;
        }
    }

}

void Gun::render(SDL_Renderer *renderer) {
    SDL_Rect dstRectInt = {static_cast<int>(dstRect.x), static_cast<int>(dstRect.y), static_cast<int>(dstRect.w), static_cast<int>(dstRect.h)};
    SDL_Point center = {dstRectInt.w / 2, dstRectInt.h / 2}; // The gun's playerTexture is now centered

    if(angle >= 90 && angle <= 270){
        SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRectInt, angle, &center, SDL_FLIP_VERTICAL);
    } else{
        SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRectInt, angle, &center, SDL_FLIP_NONE);

    }
    //SDL_RenderDrawRect(renderer ,&dstRectInt);
}

SDL_FPoint Gun::getBulletExitPosition() const {
    // The position of the base of the gun
    float baseX = dstRect.x ;
    float baseY = dstRect.y + dstRect.h / 2;

    // The offset from the base to the exit point
    auto offsetX = static_cast<double >(50 * cos(angle * M_PI / 180));
    auto offsetY = static_cast<double >(50 * sin(angle * M_PI / 180));

    // The position of the exit point
    auto exitX = static_cast<float>(baseX + offsetX);
    auto exitY = static_cast<float>(baseY + offsetY);

    return {exitX, exitY};
}



void Gun::fire(Renderer * renderer, SDL_FRect *vp, SoundManager * sm) {
    if (timeSinceLastShot >= shotDelay && !isEmpty() && !isReloading) {
        sm->playSound(SoundId::SHOOTING,0,4);
        SDL_FPoint pos = getBulletExitPosition();
        float x = pos.x;
        float y = pos.y;
        auto angela = static_cast<float>(angle * M_PI / 180);
        auto * bullet = new Bullet (x, y, 1500.0f, angela, renderer, vp);
        bullets.push(bullet);
        timeSinceLastShot = 0.0f;
        ammo--;
    }
}



void Gun::updateBullets(float dt) {
    timeSinceLastShot += dt;
    if(!bullets.isEmpty()){
        for (int i = 0; i < bullets.size(); i++) {
            Bullet* bullet = bullets.get(i);
            bullet->update(dt);
        }
    }
}

void Gun::renderBullets(SDL_Renderer *renderer, SDL_FRect *vp) const {

    for (int i = 0; i < bullets.size(); i++) {
        Bullet* bullet = bullets.get(i);
        if(bullet->isActive)
        bullet->render(renderer, vp);
    }
}

bool Gun::isEmpty() const {
    return ammo == 0;
}

void Gun::reload() {
    if (!isReloading && ammo < magazin) {
        isReloading = true;
        timeSinceLastReload = 0.0f;
    }
}

Gun::~Gun() {
    while (!bullets.isEmpty()) {
        Bullet* bullet = bullets.pop();
        delete bullet;
    }
}