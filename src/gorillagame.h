#pragma once

#include "lib/global.h"

#include "gamebase.h"
#include "lib/recthelper.h"
#include "GameObjects/Player.h"
#include "Map/FloorManager.h"
#include "GameObjects/Enemy.h"
#include "GameObjects/Enemy/BaseEnemy.h"
#include "lib/ui.h"
#include "GameObjects/Enemy/MeleeEnemy.h"

class ExampleGame;

class ExampleGame final : public Game {
public:
    ExampleGame();

    bool HandleEvent(const Event event) override;
};

class MainState : public GameState {
protected:
    Player *player;
    Room *room;
    Enemy *enemy;
    ui *userinterface;
    Floor floor;
    std::vector<MeleeEnemy*> eVec;
public:
    // ctor
    using GameState::GameState;

    void Init() override;

    void UnInit() override;

    void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;
};