#pragma once

#include "lib/global.h"

#include "gamebase.h"
#include "lib/recthelper.h"
#include "GameObjects/Player.h"
#include "Map/Room.h"
#include "GameObjects/Enemy.h"

class ExampleGame;

class ExampleGame final : public Game {
public:
    ExampleGame();

    bool HandleEvent(const Event event) override;
};

class MainState : public GameState {
protected:
    Player *player;
    Texture *image = nullptr;
    Room *room;
    Enemy *enemy;
    //std::unique_ptr<Gun> gun;
public:
    // ctor
    using GameState::GameState;

    void Init() override;

    void UnInit() override;

    void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;
};