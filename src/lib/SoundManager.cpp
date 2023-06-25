//
// Created by Jonas Luven on 23.06.23.
//

#include "SoundManager.h"
SoundManager::SoundManager() {
    // Initialize SDL_mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }
    Mix_AllocateChannels(16);
}

SoundManager::~SoundManager() {
    for(auto& pair : soundMap) {
        Mix_FreeChunk(pair.second);
    }

    Mix_Quit();
}

void SoundManager::loadSounds() {
    std::vector<SoundPair> sounds = {
            { SoundId::MONKEY1, BasePath"asset/sounds/monk1.wav" },
            { SoundId::MONKEY2, BasePath"asset/sounds/monk2.wav" },
            { SoundId::MONKEY3, BasePath"asset/sounds/monk3.wav" },
            { SoundId::SHOOTING, BasePath"asset/sounds/gunshot2.wav" },
            { SoundId::MUSIC, BasePath"asset/sounds/junglebeats.wav" },
            { SoundId::AMBIENT, BasePath"asset/sounds/ambient.mp3" },
            { SoundId::COIN, BasePath"asset/sounds/coin.wav" },
            { SoundId::POWERUP, BasePath"asset/sounds/powerup.wav" }
    };

    for (const auto& pair : sounds) {
        Mix_Chunk* sound = Mix_LoadWAV(pair.filePath.c_str());
        if(sound == nullptr) {
            printf("Failed to load sound! SDL_mixer Error: %s\n", Mix_GetError());
        }
        soundMap[pair.id] = sound;
    }
}

void SoundManager::playSound(SoundId sound, int loop, int channel) {
    Mix_PlayChannel(channel, soundMap[sound], loop);
}

SoundId SoundManager::randomMonkeySound() {
    int randomNumber = rand() % 3;
    switch (randomNumber) {
        case 0:
            return SoundId::MONKEY1;
        case 1:
            return SoundId::MONKEY2;
        case 2:
            return SoundId::MONKEY3;
        default:
            return SoundId::MONKEY1;
    }
}

void SoundManager::playMonkeySound() {
    SoundId currentSound = randomMonkeySound();
    Mix_FadeOutChannel(1, 200);

    Mix_FadeInChannel(1, soundMap[currentSound], 0, 200);

    lastMonkeySound = currentSound;
}

