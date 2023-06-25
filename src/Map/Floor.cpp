#include "Floor.h"

void Floor::addEdge(Room *src, Room *dst, int index) {

    if (adjacency_list.empty()) {
        Edge edge{};
        edge.start = src;
        edge.neighbors[index] = dst;
        adjacency_list.push_back(edge);
        return;
    }

    for (auto &i: adjacency_list) {
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
    std::array<Room*, 4> result= {};

    for (Edge e: adjacency_list) {
        if (e.start == room) {
            return e.neighbors;
        }
    }
    return result;
}

Room *Floor::getStartRoom() {
    if (adjacency_list.empty()) {
        return nullptr;
    }
    return adjacency_list[0].start;
}

bool Floor::checkCleared() {
    int count = 0;
    for (auto n: nodes) {
        if (n->cleared) {
            count++;
        }
    }
    if(count == (int) nodes.size()) {
        return true;
    }
    return false;
}

void Floor::addNode(Room *n) {
    nodes.push_back(n);
}



