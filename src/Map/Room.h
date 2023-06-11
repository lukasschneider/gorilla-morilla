#ifndef SDL_BASEGAME_ROOM_H
#define SDL_BASEGAME_ROOM_H

#include "../lib/global.h"
#include "../GameObjects/Pickups.h"
#include "../lib/rh.h"

const int TILE_SIZE = 64;

enum TileType {
    GROUND = 0,
    GROUND_GRASS = 1,
    FOREST_LEFT_TOP = 2,
    FOREST_MIDDLE_TOP = 3,
    FOREST_RIGHT_TOP = 4,
    FOREST_LEFT_MID = 5,
    FOREST_MIDDLE_MID = 6,
    FOREST_RIGHT_MID = 7,
    FOREST_LEFT_DOWN = 8,
    FOREST_MIDDLE_DOWN = 9,
    FOREST_RIGHT_DOWN = 10,
    DIRT_LEFT_TOP = 11,
    DIRT_MIDDLE_TOP = 12,
    DIRT_RIGHT_TOP = 13,
    DIRT_LEFT_MID = 14,
    DIRT_MIDDLE_MID = 15,
    DIRT_RIGHT_MID = 16,
    DIRT_LEFT_DOWN = 17,
    DIRT_MIDDLE_DOWN = 18,
    DIRT_RIGHT_DOWN = 19,
    DIRT_CONNECT_LEFT_TOP = 20,
    DIRT_CONNECT_RIGHT_TOP = 21,
    DIRT_CONNECT_LEFT_DOWN = 22,
    DIRT_CONNECT_RIGHT_DOWN = 23,
    TREE_TOP = 24,
    TREE_DOWN = 25,
    SMALL_TREE = 26,
    BUSH = 27,
    GRASS = 28,
    GRAVEL = 29,
    SHROOM = 30
};

struct Tile {
    std::string path;
    SDL_Texture *texture;
    SDL_Rect srcRect = {0, 0, 16, 16};
    TileType tileType;
    bool isSolid;

    Tile( const std::string path ,SDL_Renderer* render ,TileType tileType, bool isSolid)
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
    SDL_FRect * vp;
public:

    int id;
    std::vector<Tile> tiles;
    std::vector<std::vector<int>> map;
    std::vector<Pickup*> activePickups;


    Room(int id, SDL_Renderer *render, std::vector<std::vector<int>> map, SDL_FRect *viewport);

    [[nodiscard]] bool checkCollision(const SDL_Rect &rect) const;

    void renderTile(SDL_Renderer *render, const Tile &tile, SDL_Rect &dstRect, SDL_FRect &viewport);

    void renderMap(SDL_Renderer *render);

    void renderBackboard(SDL_Renderer *render);

    void renderPickups(const SDL_FRect &viewport);
};



#endif //SDL_BASEGAME_ROOM_H
