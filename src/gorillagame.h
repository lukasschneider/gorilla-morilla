#pragma once

#include "lib/global.h"

#include "gamebase.h"
#include "lib/recthelper.h"
#include "GameObjects/Player.h"
#include "Map/Room.h"
#include "lib/Camera.h"

class ExampleGame;

class ExampleGame final : public Game {
public:
    ExampleGame();

    bool HandleEvent(const Event event) override;
};

class IntroState : public GameState {
protected:
    Font *font = nullptr;
    Texture *image = nullptr;
    Music *music = nullptr;
    Texture *blendedText = nullptr;
    Point blendedTextSize = {0, 0};

    static constexpr const Color white{255, 255, 255, 255};
    static constexpr const Color black{0, 0, 0, 255};

    static constexpr const Array<Point, 8> shadowOffsets = {
            /*
            Point { -1, -1 },
            Point { +1, -1 },
            Point { -1, +1 },
            Point { +1, +1 },
            Point { +0, +2 },
            Point { +2, +0 },
            Point { +0, -2 },
            Point { -2, +0 },
            */
            1_up + 1_left,
            1_up + 1_right,
            1_down + 1_left,
            1_down + 1_right,
            2_right,
            2_left,
            2_up,
            2_down,
    };

public:
    // ctor
    using GameState::GameState;

    void Init() override;

    void UnInit() override;

    void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;
};

class MainState : public GameState {
protected:
    Player *player;
    Texture *image = nullptr;
    Room *room;
    Camera * camera;

public:
    // ctor
    using GameState::GameState;

    void Init() override;

    void UnInit() override;

    void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;
};