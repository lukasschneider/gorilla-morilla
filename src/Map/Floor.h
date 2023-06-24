#ifndef GORILLAGAME_FLOOR_H
#define GORILLAGAME_FLOOR_H

#include "RoomManager.h"

class Floor {
private:

    struct Edge {
        Room *start;
        std::array<Room *,4> neighbors;
    };
    std::vector<Edge> adjacency_list;
    std::vector<Room*> nodes;

public:;

    void addNode(Room *n);

    void addEdge(Room *src, Room *dst, int index);

    std::array<Room *, 4> getNeighbors(Room *room);

    bool checkCleared();

    Room* getStartRoom();
};


#endif //GORILLAGAME_FLOOR_H
