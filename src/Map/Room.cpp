#include "Room.h"
#include "../lib/ph.h"
#include <utility>

/**
 * Room Layout is always the same
 * [0] Backboard layer
 * [1] Collision layer
 * [2] Foreground Styling
 * [3] Foreground Styling
 * ...
 */

Room::Room(int id, SDL_Renderer *render, Vector<Vector<Vector<int>>> map, SDL_FRect *viewport, int map_type) {

    this->id = id;
    this->map_layer = std::move(map);
    this->vp = viewport;

    SDL_Surface *spritesheetSurface = IMG_Load(BasePath "asset/graphic/tilemap/tilemap.png");
    SDL_FreeSurface(spritesheetSurface);
    font = TTF_OpenFont(BasePath "asset/font/MonkeyIsland-1991-refined.ttf", 24);
    if (font == nullptr) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
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
            bool isSolid;

            switch (static_cast<TileType>(tiles.size())) {
                case TELEPORT_BOTTOM:
                    tileType = TELEPORT_BOTTOM;
                    isSolid = false;
                    break;
                case TELEPORT_LEFT:
                    tileType = TELEPORT_LEFT;
                    isSolid = false;
                    break;
                case TELEPORT_TOP:
                    tileType = TELEPORT_TOP;
                    isSolid = false;
                    break;
                case TELEPORT_RIGHT:
                    tileType = TELEPORT_RIGHT;
                    isSolid = false;
                    break;
                default:
                    isSolid = true;
            }


            Tile tile(BasePath "asset/graphic/tilemap/tilemap.png", render, tileType, isSolid);
            tile.srcRect = tile_rect;

            tiles.push_back(tile);

        }

    }

    switch (map_type) {

        case 1:
            /* Start Room RIGHT */
            this->enemies.clear();
            break;

        case 2:
            /* Shop Room TOP */
            this->enemies.clear();
            this->activePickups.emplace_back(new DMGBuff({576, 896, 64,64}, render));
            this->activePickups.emplace_back(new Firerate({384, 896, 64,64}, render));
            this->activePickups.emplace_back(new Magbuff({192, 896, 64,64}, render));
            break;
        case 3:
            /* BOTTOM_LEFT */
            this->enemies.push_back(new Enemy(1280, 384, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(1280, 512, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(576, 320, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(832, 320, 100, &this->activePickups));

            break;
        case 4:
            /* BOTTOM RIGHT */
            this->enemies.push_back(new Enemy(128, 256, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(320, 256, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(256, 832, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(448, 1024, 100, &this->activePickups));
            break;
        case 5:
            /* TOP BOTTOM */
            this->enemies.push_back(new Enemy(1408, 320, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(1600, 896, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(448, 320, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(320, 960, 100, &this->activePickups));
            break;
        case 6:
            /* TOP LEFT */
            this->enemies.push_back(new Enemy(1536, 256, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(1600, 960, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(192, 1088, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(256, 128, 100, &this->activePickups));
            break;
        case 7:
            /* TOP LEFT BOTTOM RIGHT */
            this->enemies.push_back(new Enemy(256, 192, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(1536, 192, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(192, 1024, 100, &this->activePickups));
            this->enemies.push_back(new Enemy(448, 1088, 100, &this->activePickups));
            break;
            break;
        default:
            perror("No valid enum - Room Constructor");
            return;
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

void Room::render_backboard(SDL_Renderer *render) {

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

void Room::render_backboard_styling(SDL_Renderer *render) {
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

void Room::render_mapborder_closed(SDL_Renderer *render) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int tileType = map_layer[2][y][x];
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

void Room::render_mapborder_open(SDL_Renderer *render) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int tileType = map_layer[3][y][x];
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

void Room::render_mapborder_styling(SDL_Renderer *render) {

    const int BORDER_WIDTH = 1 + (BACK_PIXEL_WIDTH / TILE_SIZE);
    const int BORDER_HEIGHT = 1 + (BACK_PIXEL_HEIGHT / TILE_SIZE);


    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int tileType = map_layer[4][y][x];
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
            switch (map_layer[3][y][x]) {
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
            if(this->enemies.empty()) {
                if (map_layer[3][y][x] != -1 && tiles[map_layer[3][y][x]].isSolid) {
                    return true;
                }
            } else {
                if (map_layer[2][y][x] != -1 && tiles[map_layer[2][y][x]].isSolid) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Room::renderPickups(const SDL_FRect &vp) {
    if (!activePickups.empty()) {
        for (auto pickup: activePickups) {
            pickup->render(RS::getInstance().get(), vp);
            if (dynamic_cast<Banana*>(pickup) == nullptr && pickup->checkCollision(PS::getInstance().get()->dRect, 32)) {
                std::string currentPickupDesc = pickup->description;

                std::string textToRender = std::to_string(pickup->cost) + " :\n"+ currentPickupDesc;

                SDL_Color textColor = { 255, 255, 255 , 255};
                SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, textToRender.c_str(), textColor, 300);
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(RS::getInstance().get(), textSurface);

                int textW, textH;
                SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);

                SDL_FPoint centerPoint = {175, 300};

                SDL_FRect textRect;
                textRect.x = centerPoint.x - textW / 2;
                textRect.y = centerPoint.y - textH / 2;
                textRect.w = textW;
                textRect.h = textH;

                SDL_RenderCopyF(RS::getInstance().get(), textTexture, nullptr, &textRect);

                SDL_Surface * banana  = IMG_Load(BasePath "asset/graphic/pickups/banana.png");
                SDL_Texture* pickupTexture = SDL_CreateTextureFromSurface(RS::getInstance().get(), banana);
                SDL_FreeSurface(banana);

                SDL_FRect pickupRect;
                pickupRect.x = textRect.x + 56;
                pickupRect.y = textRect.y + 2;
                pickupRect.w = 32;
                pickupRect.h = 32;


                SDL_RenderCopyF(RS::getInstance().get(), pickupTexture, nullptr, &pickupRect);

                SDL_DestroyTexture(textTexture);
                SDL_FreeSurface(textSurface);
            }
        }
    }
}


Room::~Room() {

    for (auto &tile: tiles) {
        tile.destroy();
    }

    for (auto e: enemies) {
        delete e;
    }
    enemies.clear();

    for (auto pickup: activePickups) {
        delete pickup;
    }
    activePickups.clear();
}

void Room::updatePickups() {
    Player *player = PS::getInstance().get();
    const Uint8* state = SDL_GetKeyboardState(NULL);
    currentPickupDesc = "";
    for (unsigned long i = 0; i < activePickups.size(); ++i) {
        if (dynamic_cast<Banana*>(activePickups[i]) != nullptr) {
            if (activePickups[i]->checkCollision(player->dRect, 0)) {
                SMS::getInstance().get()->playSound(SoundId::COIN,0);
                activePickups[i]->apply(player);
                delete activePickups[i];
                activePickups.erase(activePickups.begin() + i);
                break;
            }
        } else if (activePickups[i]->checkCollision(player->dRect, 48)) {
            currentPickupDesc = activePickups[i]->description;
            if(player->currency >= activePickups[i]->cost){
                if(state[SDL_SCANCODE_E]) {
                    SMS::getInstance().get()->playSound(SoundId::POWERUP,0);
                    activePickups[i]->apply(player);
                    delete activePickups[i];
                    activePickups.erase(activePickups.begin() + i);
                    break;
                }
            }
        }
    }
}


