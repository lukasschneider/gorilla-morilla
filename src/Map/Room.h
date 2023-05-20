#ifndef SDL_BASEGAME_ROOM_H
#define SDL_BASEGAME_ROOM_H

#include "../lib/global.h"

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
    Texture *texture;
    Rect srcRect = {0, 0, 16, 16};
    TileType tileType;
    bool isSolid;

    Tile( const std::string path ,SDL_Renderer* render ,TileType tileType, bool isSolid)
            : path(path), tileType(tileType), isSolid(isSolid) {
        texture = SDL_CreateTextureFromSurface(render, IMG_Load(path.c_str()));
    }

};



class Room {
public:
    int id;
    Vector<Tile> tiles;
    std::vector<Vector<int>> map;

    Room(int id, SDL_Renderer *render, Vector<Vector<int>> map);

    void renderMap(SDL_Renderer *render);

    static void renderTile(SDL_Renderer *render, Tile tile, Rect &dstRect);

};



#endif //SDL_BASEGAME_ROOM_H
