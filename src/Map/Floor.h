#ifndef GORILLAGAME_FLOOR_H
#define GORILLAGAME_FLOOR_H

#include "Room.h"
#include <unordered_map>

class Floor {
private:
    std::unordered_map<Room *, std::vector<Room *>> adjacency_list;
public:
    void addEdge(Room *src, Room *dst);

    bool hasEdge(Room *src, Room *dst);

    const std::vector<Room *> &getNeighbors(Room *room);

    unsigned long getNumRooms();

    ~Floor();
};


#endif //GORILLAGAME_FLOOR_H
