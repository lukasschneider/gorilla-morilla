#include "FloorManager.h"

Floor FloorManager::createFloor(SDL_Renderer *render, SDL_FRect *camera) {
    Floor floor;

    RoomManager rm;
    Room *room = rm.create_room(0, render, RoomManager::MapType::TP_RIGHT, camera);
    Room *room1 = rm.create_room(1, render, RoomManager::MapType::TP_TOP_LEFT_BOTTOM_RIGHT, camera);
    Room *room2 = rm.create_room(2, render, RoomManager::MapType::TP_BOTTOM_RIGHT, camera);
    Room *room3 = rm.create_room(3, render, RoomManager::MapType::TP_BOTTOM_LEFT, camera);
    Room *room4 = rm.create_room(4, render, RoomManager::MapType::TP_TOP_LEFT, camera);
    Room *room5 = rm.create_room(5, render, RoomManager::MapType::TP_TOP_BOTTOM, camera);
    Room *room6 = rm.create_room(6, render, RoomManager::MapType::TP_TOP, camera);

    /* Room 0 */
    floor.addEdge(room, room, 0);
    floor.addEdge(room, room1, 1);
    floor.addEdge(room, room, 2);
    floor.addEdge(room, room, 3);

    /* Room 1 */
    floor.addEdge(room1, room2, 0);
    floor.addEdge(room1, room4, 1);
    floor.addEdge(room1, room5, 2);
    floor.addEdge(room1, room, 3);

    /* Room 2 */
    floor.addEdge(room2, room2, 0);
    floor.addEdge(room2, room3, 1);
    floor.addEdge(room2, room1, 2);
    floor.addEdge(room2, room2, 3);

    /* Room 3 */
    floor.addEdge(room3, room3, 0);
    floor.addEdge(room3, room3, 1);
    floor.addEdge(room3, room4, 2);
    floor.addEdge(room3, room2, 3);

    /* Room 4 */
    floor.addEdge(room4, room3, 0);
    floor.addEdge(room4, room4, 1);
    floor.addEdge(room4, room4, 2);
    floor.addEdge(room4, room1, 3);

    /* Room 5 */
    floor.addEdge(room5, room1, 0);
    floor.addEdge(room5, room5, 1);
    floor.addEdge(room5, room6, 2);
    floor.addEdge(room5, room5, 3);

    /* Room 6 */
    floor.addEdge(room6, room5, 0);
    floor.addEdge(room6, room6, 1);
    floor.addEdge(room6, room6, 2);
    floor.addEdge(room6, room6, 3);

    return floor;
}

