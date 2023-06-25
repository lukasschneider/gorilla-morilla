//
// Created by Jonas Luven on 23.06.23.
//

#ifndef GORILLAGAME_SOUNDMANAGER_H
#define GORILLAGAME_SOUNDMANAGER_H


#include <SDL_mixer.h>
#include <map>
#include <vector>
#include <string>
#include "global.h"

enum class SoundId {
    MONKEY1 = 0,
    MONKEY2 = 1,
    MONKEY3 = 2,
    SHOOTING = 3,
    MUSIC = 4,
    AMBIENT = 5,
    COIN = 6,
    POWERUP = 7
};

// Pair of SoundId and corresponding file path
struct SoundPair {
    SoundId id;
    std::string filePath;
};

class SoundManager {
public:
    [[maybe_unused]] SoundId lastMonkeySound;

    SoundManager();
    ~SoundManager();

    void loadSounds();

    static SoundId randomMonkeySound();

    void playMonkeySound();

    std::map<SoundId, Mix_Chunk*> soundMap;

    void playSound(SoundId sound, int loop, int channel = -1);
};

#endif //GORILLAGAME_SOUNDMANAGER_H
