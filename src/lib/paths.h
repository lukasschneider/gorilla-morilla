#ifndef GORILLAGAME_PATHS_H
#define GORILLAGAME_PATHS_H

#include "global.h"

struct Node {
    int x, y;
    float gCost, hCost;
    Node* parent;

    Node(int x, int y) : x(x), y(y), gCost(INFINITY), hCost(INFINITY), parent(nullptr) {}

    [[nodiscard]] float fCost() const {
        return gCost + hCost;
    }
};

struct CompareFcost {
    bool operator()(const Node* lhs, const Node* rhs) const {
        return lhs->fCost() > rhs->fCost();
    }
};

float distance(Node* a, Node* b) {
    int dx = abs(a->x - b->x);
    int dy = abs(a->y - b->y);
    return std::max(dx, dy) + (std::sqrt(2) - 1) * std::min(dx, dy);
}


std::vector<Node*> getNeighbors(Node* node, std::vector<std::vector<Node>>& grid) {
    std::vector<Node*> neighbors;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            // skip if no movement
            if (dx == 0 && dy == 0) continue;

            int newX = node->x + dx;
            int newY = node->y + dy;

            // check if new position is within the grid
            if (newX >= 0 && newX < grid.size() && newY >= 0 && newY < grid[0].size()) {
                neighbors.push_back(&grid[newX][newY]);
            }
        }
    }

    return neighbors;
}

#endif //GORILLAGAME_PATHS_H
