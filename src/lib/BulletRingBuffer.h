#ifndef GORILLAGAME_BULLETRINGBUFFER_H
#define GORILLAGAME_BULLETRINGBUFFER_H

#include "../GameObjects/Weapons/Bullet.h"


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
        if (currentSize == maxSize) {
            SDL_DestroyTexture(buffer[start]->texture);
            delete buffer[start];
            start = (start + 1) % maxSize;
        }

        buffer[end] = bullet;
        end = (end + 1) % maxSize;

        if (currentSize < maxSize) {
            currentSize++;
        }
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


#endif //GORILLAGAME_BULLETRINGBUFFER_H
