#include "Floor.h"

void Floor::addEdge(Room* src, Room* dst, int index) {
    if (adjacency_list.empty()) {
        Edge edge{};
        edge.start = src;
        edge.neighbors[index] = dst;
        adjacency_list.push_back(edge);
        return;
    }

    for (auto & i : adjacency_list) {
        if (src->id == i.start->id) {
            i.neighbors[index] = dst;
            return;
        }
    }

    Edge edge{};
    edge.start = src;
    edge.neighbors[index] = dst;
    adjacency_list.push_back(edge);
}


std::array<Room *, 4> Floor::getNeighbors(Room *room) {
    for (Edge e: adjacency_list) {
        if (e.start == room) {
            return e.neighbors;
        }
    }
}

Room *Floor::getStartRoom() {
    if (adjacency_list.empty()) {
        return nullptr;
    }
    return adjacency_list[6].start;
}



