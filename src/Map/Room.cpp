#include "Room.h"
#include <utility>

/**
 * Room Layout is always the same
 * [0] Backboard layer
 * [1] Collision layer
 * [2] Foreground Styling
 * [3] Foreground Styling
 * ...
 */

Room::Room(int id, SDL_Renderer *render, Vector<Vector<Vector<int>>> map, SDL_FRect *viewport) {

    this->id = id;
    this->map_layer = std::move(map);
    this->vp = viewport;

    SDL_Surface *spritesheetSurface = IMG_Load(BasePath "asset/graphic/tilemap/tilemap.png");
    SDL_FreeSurface(spritesheetSurface);

    int spritesheet_height = 176;
    int spritesheet_width = 192;
    int tile_size = 16;

    for (int y = 0; y < spritesheet_height; y += tile_size) {
        for (int x = 0; x < spritesheet_width; x += tile_size) {

            Rect tile_rect;
            tile_rect.x = x;
            tile_rect.y = y;
            tile_rect.w = tile_size;
            tile_rect.h = tile_size;

            auto tileType = static_cast<TileType>(tiles.size());
            bool isSolid = false;

            switch (static_cast<TileType>(tiles.size())) {
                case FOREST_MIDDLE_MID:
                    tileType = FOREST_MIDDLE_MID;
                    isSolid = true;
                    break;
                case TELEPORT_BOTTOM:
                    tileType = TELEPORT_BOTTOM;
                    break;
                case TELEPORT_LEFT:
                    tileType = TELEPORT_LEFT;
                    break;
                case TELEPORT_TOP:
                    tileType = TELEPORT_TOP;
                    break;
                case TELEPORT_RIGHT:
                    tileType = TELEPORT_RIGHT;
                    break;
            }


            Tile tile(BasePath "asset/graphic/tilemap/tilemap.png", render, tileType, isSolid);
            tile.srcRect = tile_rect;

            tiles.push_back(tile);

        }

    }

    this->MAP_WIDTH = this->map_layer[0][0].size();
    this->MAP_HEIGHT = this->map_layer[0].size();

    this->MAP_PIXEL_WIDTH = this->MAP_WIDTH * TILE_SIZE;
    this->MAP_PIXEL_HEIGHT = this->MAP_HEIGHT * TILE_SIZE;

    this->BACK_PIXEL_WIDTH = MAP_PIXEL_WIDTH + 1280;
    this->BACK_PIXEL_HEIGHT = MAP_PIXEL_HEIGHT + 720;

}

int Room::getMapPixelHeight() {
    return this->MAP_PIXEL_HEIGHT;
}

int Room::getMapPixelWidth() {
    return this->MAP_PIXEL_WIDTH;
}

void Room::renderTile(SDL_Renderer *render, const Tile &tile, SDL_Rect &dstRect, SDL_FRect &viewport) {
    SDL_Rect offsetRect = {
            static_cast<int>(dstRect.x - viewport.x),
            static_cast<int>(dstRect.y - viewport.y),
            dstRect.w,
            dstRect.h
    };
    SDL_RenderCopy(render, tile.texture, &tile.srcRect, &offsetRect);
}

void Room::renderBackboard(SDL_Renderer *render) {
    const int BORDER_WIDTH = 1 + (BACK_PIXEL_WIDTH / TILE_SIZE);
    const int BORDER_HEIGHT = 1 + (BACK_PIXEL_HEIGHT / TILE_SIZE);

    // Render the border
    for (int y = -BORDER_HEIGHT / 2; y < (MAP_HEIGHT + BORDER_HEIGHT / 2); ++y) {
        for (int x = -BORDER_WIDTH / 2; x < (MAP_WIDTH + BORDER_WIDTH / 2); ++x) {
            SDL_Rect dstRect = {
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
            };

            if (!(dstRect.x >= 0 && dstRect.x < MAP_PIXEL_WIDTH && dstRect.y >= 0 &&
                  dstRect.y < MAP_PIXEL_HEIGHT)) {
                const Tile &tile = this->tiles[FOREST_MIDDLE_MID];
                renderTile(render, tile, dstRect, *vp);
            }
        }
    }

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int tileType = map_layer[0][y][x];
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

int Room::checkTeleport(const Rect &rect) const {
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
            switch (map_layer[1][y][x]) {
                case TELEPORT_TOP:
                    return TELEPORT_TOP;
                case TELEPORT_LEFT:
                    return TELEPORT_LEFT;
                case TELEPORT_BOTTOM:
                    return TELEPORT_BOTTOM;
                case TELEPORT_RIGHT:
                    return TELEPORT_RIGHT;
            }
        }
    }
    return 0;
}


bool Room::checkCollision(const Rect &rect) const {
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
            if (map_layer[1][y][x] != -1 && tiles[map_layer[1][y][x]].isSolid) {
                return true;
            }
        }
    }
    return false;
}

void Room::renderForeground(SDL_Renderer *render) {
    for (unsigned long z = 2; z < map_layer.size(); ++z) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                int tileType = map_layer[z][y][x];
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
}

void Room::renderCollision(SDL_Renderer *render) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int tileType = map_layer[1][y][x];
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

void Room::renderPickups(const SDL_FRect &vp) {
    if(!activePickups.empty()){
        for(auto pickup : activePickups){
            pickup->render(RS::getInstance().get(),vp);
        }
    }
}

Room::~Room() {

    for (auto& tile : tiles) {
        tile.destroy();
    }

    for (auto pickup : activePickups) {
        delete pickup;
    }
    activePickups.clear();
}

