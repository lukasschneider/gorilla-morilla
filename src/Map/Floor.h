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
    //std::unordered_map<Room *, std::vector<Room *>> adjacency_list;
public:;
    void addEdge(Room *src, Room *dst, int index);

    std::array<Room *, 4> getNeighbors(Room *room);

    Room* getStartRoom();
};


#endif //GORILLAGAME_FLOOR_H
