#ifndef SDL_BASEGAME_ROOM_H
#define SDL_BASEGAME_ROOM_H

#include "../lib/global.h"
#include "../GameObjects/Pickups.h"
#include "../lib/rh.h"

/**
 * Room Layout is always the same
 * [0] Backboard layer
 * [1] Collision layer
 * [2] Foreground Styling
 * [3] Foreground Styling
 * ...
 */

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
    TileType tileType;
    bool isSolid;

    Tile(const std::string path, SDL_Renderer *render, TileType tileType, bool isSolid)
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

    int id;
    std::vector<Tile> tiles;
    std::vector<Pickup*> activePickups;
    std::vector<std::vector<std::vector<int>>> map_layer;


    Room(int id, SDL_Renderer *render, std::vector<std::vector<std::vector<int>>> map, SDL_FRect *viewport);

    [[nodiscard]] bool checkCollision(const SDL_Rect &rect) const;

    int getMapPixelWidth();

    int getMapPixelHeight();

    void renderTile(SDL_Renderer *render, const Tile &tile, SDL_Rect &dstRect, SDL_FRect &viewport);

    void renderBackboard(SDL_Renderer *render);

    void renderCollision(SDL_Renderer *render);

    void renderForeground(SDL_Renderer *render);

    [[nodiscard]] int checkTeleport(const SDL_Rect &rect) const;

    void renderPickups(const SDL_FRect &viewport);
};


#endif //SDL_BASEGAME_ROOM_H
