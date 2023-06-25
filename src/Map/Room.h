#ifndef SDL_BASEGAME_ROOM_H
#define SDL_BASEGAME_ROOM_H

#include "../lib/global.h"
#include "../GameObjects/Pickups.h"
#include "../lib/rh.h"
#include "../GameObjects/Enemy.h"

/**
 * Room Layout is always the same
 * [0] Backboard layer
 * [1] Collision layer
 * [2] Foreground Styling
 * [3] Foreground Styling
 * ...
 */

class Pickup;
class Enemy;

const int TILE_SIZE = 64;

enum TileType {
    FOREST_MIDDLE_MID = 19,
    TELEPORT_LEFT = 26,
    TELEPORT_TOP = 37,
    TELEPORT_BOTTOM = 13,
    TELEPORT_RIGHT = 24

};

struct Tile {
    std::string path;
    SDL_Texture *texture;
    SDL_Rect srcRect = {0, 0, 16, 16};
    [[maybe_unused]] TileType tileType;
    bool isSolid;

    void destroy() const {
        SDL_DestroyTexture(texture);
    }

    Tile(const std::string& path, SDL_Renderer *render, TileType tileType, bool isSolid)
            : path(path), tileType(tileType), isSolid(isSolid) {
        texture = SDL_CreateTextureFromSurface(render, IMG_Load(path.c_str()));
    }

};

class Room {
private:
    int
            MAP_WIDTH,
            MAP_HEIGHT,
            MAP_PIXEL_WIDTH,
            MAP_PIXEL_HEIGHT,
            BACK_PIXEL_WIDTH,
            BACK_PIXEL_HEIGHT;
    SDL_FRect *vp;

public:
    std::string currentPickupDesc;
    TTF_Font* font;
    int id;
    [[maybe_unused]] bool isShop = false;
    [[maybe_unused]] bool isStart = false;
    std::vector<Tile> tiles;
    std::vector<Pickup*> activePickups;

    std::vector<std::vector<std::vector<int>>> map_layer;

    std::vector<Enemy*>enemies;

    bool cleared = false;

    Room(int id, SDL_Renderer *render, std::vector<std::vector<std::vector<int>>> map, SDL_FRect *viewport, int map_type);

    [[nodiscard]] bool checkCollision(const SDL_Rect &rect) const;

    [[nodiscard]] int getMapPixelWidth() const;

    [[nodiscard]] int getMapPixelHeight() const;

    static void renderTile(SDL_Renderer *render, const Tile &tile, SDL_Rect &dstRect, SDL_FRect &viewport);

    void render_backboard(SDL_Renderer *render);

    void render_backboard_styling(SDL_Renderer *render);

    void render_mapborder_closed(SDL_Renderer *render);

    void render_mapborder_open(SDL_Renderer *render);

    void render_mapborder_styling(SDL_Renderer *render);

    [[nodiscard]] int checkTeleport(const SDL_Rect &rect) const;

    ~Room();

    void updatePickups();

    void renderPickups();
};


#endif //SDL_BASEGAME_ROOM_H
