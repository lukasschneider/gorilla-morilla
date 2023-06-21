#include "RoomManager.h"
#include <fstream>
#include <sstream>


Vector<Vector<int>> RoomManager::readCSVToArray(const std::string &filename) {

    Vector<Vector<int>> map_layer;
    std::ifstream file(filename);
    if (!file.is_open()) {
        perror("Error can't load File - RoomManager::readCSVToArray");
        return map_layer;
    }

    std::string line;
    while (std::getline(file, line)) {
        Vector<int> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            int value = std::stoi(cell);
            row.push_back(value);
        }
        map_layer.push_back(row);
    }

    file.close();

    return map_layer;
}

Vector<Vector<Vector<int>>> RoomManager::create_map_vector(MapType map_type) {

    Vector<Vector<Vector<int>>> map_layer;
    switch (map_type) {
        case MapType::TP_RIGHT:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_right/backboard.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_right/backboard_styling.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_right/mapborder_closed.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_right/mapborder_open.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_right/mapborder_styling.csv"));
            break;
        case MapType::TP_TOP:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top/backboard.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top/backboard_styling.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top/mapborder_closed.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top/mapborder_open.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top/mapborder_styling.csv"));
            break;
        case MapType::TP_BOTTOM_LEFT:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_left/backboard.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_left/backboard_styling.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_left/mapborder_closed.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_left/mapborder_open.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_left/mapborder_styling.csv"));
            break;
        case MapType::TP_BOTTOM_RIGHT:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_right/backboard.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_right/backboard_styling.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_right/mapborder_closed.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_right/mapborder_open.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_right/mapborder_styling.csv"));
            break;
        case MapType::TP_TOP_BOTTOM:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_bottom/backboard.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_bottom/backboard_styling.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_bottom/mapborder_closed.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_bottom/mapborder_open.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_bottom/mapborder_styling.csv"));
            break;
        case MapType::TP_TOP_LEFT:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left/backboard.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left/backboard_styling.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left/mapborder_closed.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left/mapborder_open.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left/mapborder_styling.csv"));
            break;
        case MapType::TP_TOP_LEFT_BOTTOM_RIGHT:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left_bottom_right/backboard.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left_bottom_right/backboard_styling.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left_bottom_right/mapborder_closed.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left_bottom_right/mapborder_open.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left_bottom_right/mapborder_styling.csv"));
            break;
        default:
            perror("No Valid Enum");
    }
    return map_layer;
}

Room *RoomManager::create_room(int id, Renderer *render, MapType map_type, SDL_FRect *viewport) {

    std::vector<std::vector<std::vector<int>>> map_vector = this->create_map_vector(map_type);

    Room *room = new Room(id, render, map_vector, viewport, (int) map_type);
    return room;
}
