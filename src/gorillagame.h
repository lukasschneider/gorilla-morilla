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
#include "GameObjects/Weapons/Knife.h"
#include "lib/SoundManager.h"


class ExampleGame;

class ExampleGame final : public Game {
public:
    ExampleGame();

    bool HandleEvent(Event event) override;
};

class MainState : public GameState {
protected:
    Player *player;
    Room *room;
    ui *userinterface;
    Floor floor;
    SoundManager * soundManager;
    std::vector<MeleeEnemy*> eVec;
public:
    // ctor
    using GameState::GameState;

    void Init() override;

    void UnInit() override;

    void Events(u32 frame, u32 totalMSec, float deltaT) override;

    void Update(u32 frame, u32 totalMSec, float deltaT) override;

    void Render(u32 frame, u32 totalMSec, float deltaT) override;
};