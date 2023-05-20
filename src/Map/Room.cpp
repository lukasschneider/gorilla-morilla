#include "Room.h"

Room::Room(int id, SDL_Renderer *render, Vector<Vector<int>> map)
        : id(id), map(std::move(map)) {
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0000.png",render,GROUND,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0001.png",render,GROUND_GRASS,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0006.png",render,FOREST_LEFT_TOP,true);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0007.png",render,FOREST_MIDDLE_TOP,true);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0008.png",render,FOREST_RIGHT_TOP,true);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0018.png",render,FOREST_LEFT_MID,true);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0019.png",render,FOREST_MIDDLE_MID,true);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0020.png",render,FOREST_RIGHT_MID,true);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0030.png",render,FOREST_LEFT_DOWN,true);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0031.png",render,FOREST_MIDDLE_DOWN,true);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0032.png",render,FOREST_RIGHT_DOWN,true);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0012.png",render,DIRT_LEFT_TOP,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0013.png",render,DIRT_MIDDLE_TOP,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0014.png",render,DIRT_RIGHT_TOP,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0024.png",render,DIRT_LEFT_MID,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0025.png",render,DIRT_MIDDLE_MID,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0026.png",render,DIRT_RIGHT_MID,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0036.png",render,DIRT_LEFT_DOWN,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0037.png",render,DIRT_MIDDLE_DOWN,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0038.png",render,DIRT_RIGHT_DOWN,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0039.png",render,DIRT_CONNECT_LEFT_TOP,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0040.png",render,DIRT_CONNECT_RIGHT_TOP,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0042.png",render,DIRT_CONNECT_LEFT_DOWN,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0041.png",render,DIRT_CONNECT_RIGHT_DOWN,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0004.png",render,TREE_TOP,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0016.png",render,TREE_DOWN,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0028.png",render,SMALL_TREE,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0005.png",render,BUSH,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0017.png",render,GRASS,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0043.png",render,GRAVEL,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0029.png",render,SHROOM,false);
}



void Room::renderMap(SDL_Renderer *render) {
    const int TILE_SIZE = 64; // the size of each tile in pixels
    const int MAP_WIDTH =  map[0].size(); // the width of the map in tiles
    const int MAP_HEIGHT = map.size(); // the height of the map in tiles

    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(render, &windowWidth, &windowHeight); // get the window size

    // Calculate the total size of the map in pixels
    const int MAP_PIXEL_WIDTH = MAP_WIDTH * TILE_SIZE;
    const int MAP_PIXEL_HEIGHT = MAP_HEIGHT * TILE_SIZE;

    // Calculate the starting x and y coordinates to center the map
    const int START_X = (windowWidth - MAP_PIXEL_WIDTH) / 2;
    const int START_Y = (windowHeight - MAP_PIXEL_HEIGHT) / 2;

    // Calculate how many tiles we need to add around the map
    const int BORDER_WIDTH = 1 + (windowWidth - MAP_PIXEL_WIDTH) / (2 * TILE_SIZE);
    const int BORDER_HEIGHT = 1 + (windowHeight - MAP_PIXEL_HEIGHT) / (2 * TILE_SIZE);

    // Render the border
    for (int y = -BORDER_HEIGHT; y < MAP_HEIGHT + BORDER_HEIGHT; ++y) {
        for (int x = -BORDER_WIDTH; x < MAP_WIDTH + BORDER_WIDTH; ++x) {
            // Only render the tile if it's outside of the map
            SDL_Rect dstRect = {
                    START_X + x * TILE_SIZE,
                    START_Y + y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
            };
            renderTile(render, tiles[0], dstRect); // Render Backboard
            if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT) {
                renderTile(render, tiles[6], dstRect); // Render Backboard
            }
        }
    }

    // Render the map
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int tileType = map[y][x];

            SDL_Rect dstRect = {
                    START_X + x * TILE_SIZE,
                    START_Y + y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
            };
            if(tileType != -1){
                const Tile &tile = this->tiles[tileType];
                renderTile(render, tile, dstRect);
            }
        }
    }
}



void Room::renderTile(SDL_Renderer *render, Tile tile, Rect &dstRect) {
    SDL_RenderCopy(render, tile.texture, &tile.srcRect, &dstRect);
}


