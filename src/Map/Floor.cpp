#include "Floor.h"

void Floor::addEdge(Room *src, Room *dst) {
    //Hinzufügen einer Kante in die Adjazenzliste, sowohl Hinweg als auch Rückweg da ungerichtet
    this->adjacency_list[src].push_back(dst);
    this->adjacency_list[dst].push_back(src);
}

bool Floor::hasEdge(Room *src, Room *dst) {
    auto srcIt = adjacency_list.find(src);
    if (srcIt != adjacency_list.end()) {
        const auto &neighbors = srcIt->second;
        return std::find(neighbors.begin(), neighbors.end(), dst) != neighbors.end();
    }
}

const std::vector<Room *> &Floor::getNeighbors(Room *room) {
    auto it = adjacency_list.find(room);
    if (it != adjacency_list.end()) {
        return it->second;
    }
    static const std::vector<Room *> empty;
    return empty;
}

unsigned long Floor::getNumRooms() {
    return this->adjacency_list.size();
}

Floor::~Floor() {
    for (auto & it : adjacency_list) {
        delete it.first;
    }
    adjacency_list.clear();
}
