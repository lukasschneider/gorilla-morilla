#include "Room.h"

Room::Room(int id, SDL_Renderer *render, Vector<Vector<int>> map,SDL_FRect *viewport)
        : id(id), map(map), vp(viewport) {
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0000.png",render,GROUND,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0001.png",render,GROUND_GRASS,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0006.png",render,FOREST_LEFT_TOP,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0007.png",render,FOREST_MIDDLE_TOP,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0008.png",render,FOREST_RIGHT_TOP,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0018.png",render,FOREST_LEFT_MID,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0019.png",render,FOREST_MIDDLE_MID,true);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0020.png",render,FOREST_RIGHT_MID,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0030.png",render,FOREST_LEFT_DOWN,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0031.png",render,FOREST_MIDDLE_DOWN,false);
    this->tiles.emplace_back(BasePath "asset/graphic/tiles/tile_0032.png",render,FOREST_RIGHT_DOWN,false);
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

    this->MAP_WIDTH = this->map[0].size();
    this->MAP_HEIGHT = this->map.size();

    this->MAP_PIXEL_WIDTH = this->MAP_WIDTH * TILE_SIZE;
    this->MAP_PIXEL_HEIGHT = this->MAP_HEIGHT * TILE_SIZE;

    this->BACK_PIXEL_WIDTH = MAP_PIXEL_WIDTH + 1280;
    this->BACK_PIXEL_HEIGHT = MAP_PIXEL_HEIGHT + 720;
}

void Room::renderTile(SDL_Renderer *render, const Tile& tile, SDL_Rect &dstRect, SDL_FRect &viewport) {
    SDL_Rect offsetRect = {
            static_cast<int>(dstRect.x - viewport.x),
            static_cast<int>(dstRect.y - viewport.y),
            dstRect.w,
            dstRect.h
    };
    SDL_RenderCopy(render, tile.texture, &tile.srcRect, &offsetRect);
}

void Room::renderMap(SDL_Renderer *render) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int tileType = map[y][x];
            SDL_Rect dstRect = {
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
            };

            if (tileType != -1) {
                const Tile &tile = this->tiles[tileType];
                renderTile(render, tile, dstRect, *vp);
            }
        }
    }
}

void Room::renderBackboard(SDL_Renderer *render) {
    const int BORDER_WIDTH = 1 + (BACK_PIXEL_WIDTH / TILE_SIZE);
    const int BORDER_HEIGHT = 1 + (BACK_PIXEL_HEIGHT / TILE_SIZE);

    // Render the border
    for (int y = -BORDER_HEIGHT/2; y < (MAP_HEIGHT + BORDER_HEIGHT/2); ++y) {
        for (int x = -BORDER_WIDTH/2; x < (MAP_WIDTH + BORDER_WIDTH/2); ++x) {
            SDL_Rect dstRect = {
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
            };

            const Tile &tile = this->tiles[GROUND];
            renderTile(render, tile, dstRect, *vp);
            if(!(dstRect.x >= 0 && dstRect.x < MAP_PIXEL_WIDTH && dstRect.y >= 0 && dstRect.y < MAP_PIXEL_HEIGHT)){
                const Tile &tile = this->tiles[FOREST_MIDDLE_MID];
                renderTile(render, tile, dstRect, *vp);
            }
        }
    }
}




bool Room::checkCollision(const Rect& rect) const {
    int startX = static_cast<int>(rect.x) / TILE_SIZE;
    int startY = static_cast<int>(rect.y) / TILE_SIZE;
    int endX = static_cast<int>(rect.x + rect.w) / TILE_SIZE;
    int endY = static_cast<int>(rect.y + rect.h) / TILE_SIZE;

    startX = std::max(0, std::min(startX, MAP_WIDTH - 1));
    startY = std::max(0, std::min(startY, MAP_HEIGHT - 1));
    endX = std::max(0, std::min(endX, MAP_WIDTH - 1));
    endY = std::max(0, std::min(endY, MAP_HEIGHT - 1));

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            if (map[y][x] != -1 && tiles[map[y][x]].isSolid) {
                return true;
            }
        }
    }
    return false;
}



