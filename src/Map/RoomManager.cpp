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
    if(map_type != MapType::TEST){
        //map_layer.push_back(this->readCSVToArray(BasePath "maps/backboards/Default.csv"));
    }
    switch (map_type) {
        case MapType::TP_RIGHT:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_right/backboard.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_right/backboard_styling.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_right/mapborder_closed.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_right/mapborder_open.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_right/markup.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_right/mapborder_styling.csv"));
            break;
        case MapType::TP_TOP:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top/MapBorder.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top/MapBorderStyling.csv"));
            break;
        case MapType::TP_BOTTOM_LEFT:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_left/MapBorder.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_left/MapBorderStyling.csv"));
            break;
        case MapType::TP_BOTTOM_RIGHT:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_right/MapBorder.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_right/MapBorderStyling.csv"));
            break;
        case MapType::TP_TOP_BOTTOM:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_bottom/MapBorder.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_bottom/MapBorderStyling.csv"));
            break;
        case MapType::TP_TOP_LEFT:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left/MapBorder.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left/MapBorderStyling.csv"));
            break;
        case MapType::TP_BOTTOM_LEFT_RIGHT:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_left_right/MapBorder.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_bottom_left_right/MapBorderStyling.csv"));
            break;
        case MapType::TP_TOP_LEFT_BOTTOM_RIGHT:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left_bottom_right/MapBorder.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/tp_top_left_bottom_right/MapBorderStyling.csv"));
            break;
        case MapType::TEST:
            map_layer.push_back(this->readCSVToArray(BasePath "maps/test/test_Backboard.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/test/test_Collision.csv"));
            map_layer.push_back(this->readCSVToArray(BasePath "maps/test/test_Style.csv"));
        default:
            perror("No Valid Enum");
    }
    return map_layer;
}

Room *RoomManager::create_room(int id, Renderer *render, MapType map_type, SDL_FRect *viewport) {
    std::vector<std::vector<std::vector<int>>> map_vector = this->create_map_vector(map_type);
    Room *room = new Room(id, render, map_vector, viewport);
    return room;
}

