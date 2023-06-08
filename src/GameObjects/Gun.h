#ifndef GORILLAGAME_GUN_H
#define GORILLAGAME_GUN_H

#include "../lib/global.h"
#include "Bullet.h"

class BulletRingBuffer {
private:
    std::vector<Bullet*> buffer;
    int maxSize;
    int currentSize;
    int start;
    int end;

public:
    BulletRingBuffer(int size) : maxSize(size), currentSize(0), start(0), end(0) {
        buffer.resize(maxSize);
    }

    void push(Bullet* bullet) {
        buffer[end] = bullet;
        if (currentSize < maxSize) {
            currentSize++;
        } else {
            // The buffer is full, so we override the oldest element.
            start = (start + 1) % maxSize;
        }
        end = (end + 1) % maxSize;
    }

    Bullet* pop() {
        if (currentSize == 0) {
            return nullptr; // buffer is empty
        }
        Bullet* result = buffer[start];
        start = (start + 1) % maxSize;
        currentSize--;
        return result;
    }

    Bullet* get(int index) const {
        if (index < 0 || index >= currentSize) {
            return nullptr; // index out of range
        }
        return buffer[(start + index) % maxSize];
    }

    int size() const {
        return currentSize;
    }

    bool isFull() const {
        return currentSize == maxSize;
    }

    bool isEmpty() const {
        return currentSize == 0;
    }
};

class Gun {
public:
    int ammo;
    float reloadTime;
    float timeSinceLastReload;
    bool isReloading;

    std::string path = BasePath "asset/graphic/guns/ak.png";
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_FRect dstRect;

    BulletRingBuffer bullets;

    float timeSinceLastShot = 0.0f;
    const float shotDelay = 0.14f;
    float angle = 0.0f;

    Gun(SDL_Renderer * render);

    SDL_FPoint getBulletExitPosition() const;

    void render(SDL_Renderer *renderer);

    void update(int mouseX, int mouseY, const SDL_FRect &playerRect, const SDL_FRect &viewport, float dt);

    // Bullet Logic
    void fire(Renderer* renderer, SDL_FRect *vp);

    void updateBullets(float dt);

    void renderBullets(SDL_Renderer *renderer, SDL_FRect *vp);

    bool isEmpty();

    void reload();
};



#endif //GORILLAGAME_GUN_H
