#ifndef SDL_BASEGAME_ROOM_H
#define SDL_BASEGAME_ROOM_H

#include "../lib/global.h"

enum TileType {
    GROUND = 0,
    TREE = 1,
    FOREST = 2,
    TELEPORT = 3
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
    Vector<Vector<int>> *map;

    Room(int id, SDL_Renderer *render, Vector<Vector<int>> *map);

    void renderMap(SDL_Renderer *render);

    static void renderTile(SDL_Renderer *render, Tile tile, Rect &dstRect);

};


#endif //SDL_BASEGAME_ROOM_H
