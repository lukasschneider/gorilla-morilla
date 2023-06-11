#ifndef GORILLAGAME_ROOMMANAGER_H
#define GORILLAGAME_ROOMMANAGER_H

#include "../lib/global.h"
#include "../Map/Room.h"
#include <string>

class RoomManager {
public:
    enum class MapType {
        TP_RIGHT = 1,
        TP_TOP = 2,
        TP_BOTTOM_LEFT = 3,
        TP_BOTTOM_RIGHT = 4,
        TP_TOP_BOTTOM = 5,
        TP_TOP_LEFT = 6,
        TP_BOTTOM_LEFT_RIGHT = 7,
        TP_TOP_LEFT_BOTTOM_RIGHT = 8
    };

    Room *create_room(int id, SDL_Renderer *render, MapType map_type, SDL_FRect *viewport);
private:

    static std::vector<std::vector<int>> readCSVToArray(const std::string &filename);

    std::vector<std::vector<std::vector<int>>> create_map_vector(MapType map_type);

};


#endif //GORILLAGAME_ROOMMANAGER_H
