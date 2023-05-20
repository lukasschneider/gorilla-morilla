#include "Room.h"

Room::Room(int id, SDL_Renderer *render, Vector<Vector<int>> *map) {
    this->id = id;
    this->tiles.emplace_back(BasePath "asset/graphic/tile_0000.png",render,GROUND,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tile_0001.png",render,TREE,false);
    this->map = map;
}



void Room::renderMap(SDL_Renderer *render) {
    const int TILE_SIZE = 16; // the size of each tile in pixels
    const int MAP_WIDTH =  (*map)[0].size(); // the width of the map in tiles
    const int MAP_HEIGHT = map->size(); // the height of the map in tiles
    cout << MAP_HEIGHT << endl;
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int tileType = (*map)[y][x]; // FICK C++
            const Tile &tile = this->tiles[tileType];

            SDL_Rect dstRect = {
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
            };
            renderTile(render, tile, dstRect);
        }
    }
}

void Room::renderTile(SDL_Renderer *render, Tile tile, Rect &dstRect) {
    SDL_RenderCopy(render, tile.texture, &tile.srcRect, &dstRect);
}


