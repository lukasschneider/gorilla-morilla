#ifndef GORILLAGAME_ASTAR_H
#define GORILLAGAME_ASTAR_H

#include "global.h"
#include <fstream>
#include "rh.h"

using namespace std;

typedef pair<int, int> Pair;

typedef pair<double, pair<int, int> > pPair;


Pair convertToYXandGridPos(int x, int y, int cellSize) {
    int gridX = x / cellSize;
    int gridY = y / cellSize;

    return make_pair(gridY, gridX);
}

void unfuckPath(std::vector<std::pair<int, int>> &path) {
    for (auto &pair: path) {
        std::swap(pair.first, pair.second);
    }
}


void drawPath(std::optional<std::vector<std::pair<int, int>>> &path, const SDL_FRect &vp, float size) {
    if (path == nullopt)
        return;
    Renderer *render = RS::getInstance().get();
    for (auto &pair: *path) {
        SDL_FRect tmp = {(float) pair.first * size - vp.x, (float) pair.second * size - vp.y, size, size};
        SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
        SDL_RenderFillRectF(render, &tmp);
    }
}

struct cell {
    int parent_i, parent_j;
    double f, g, h;
};

void printMatrix(const std::vector<std::vector<int>>& matrix) {
    cout << "##########################################" << endl;
    for (const auto &row : matrix) {
        for (const auto &elem : row) {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }
    cout << "##########################################" << endl;
}


std::vector<std::vector<int>> transformMatrix(const std::vector<std::vector<int>> &matrix) {
    std::vector<std::vector<int>> transformedMatrix = matrix;

    for (auto &row: transformedMatrix) {
        for (auto &elem: row) {
            elem = (elem == -1) ? 1 : 0;
        }
    }
    //printMatrix(transformedMatrix);
    return transformedMatrix;
}

std::vector<std::vector<int>> csvToMatrix(const std::string &filepath) {
    std::ifstream file(filepath);
    std::vector<std::vector<int>> matrix;
    std::string line, value;

    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);

        while (std::getline(ss, value, ',')) {
            row.push_back(std::stoi(value));
        }

        matrix.push_back(row);
    }

    return matrix;
}

bool isValid(vector<vector<int>>& grid,int col, int row)
{
    // Returns true if col number and rowumn number
    // is in range
    return (col >= 0) && (col < grid.size()) && (row >= 0)
           && (row < grid[0].size());
}

bool isUnBlocked(vector<vector<int>>& grid, int col, int row)
{
    // Returns true if the cell is not blocked else false
    if (grid[col][row] == 1)
        return (true);
    else
        return (false);
}


bool isDestination(int col, int row, Pair dest)
{
    if (col == dest.first && row == dest.second)
        return (true);
    else
        return (false);
}

/*
double calculateHValue(int row, int col, Pair player) {
    return abs(row - player.first) + abs(col - player.second);
}

*/
double calculateHValue(int col, int row, Pair dest)
{
    // Return using the distance formula
    return ((double)sqrt(
            (col - dest.first) * (col - dest.first)
            + (row - dest.second) * (row - dest.second)));
}


std::vector<std::pair<int, int>>  tracePath(vector<vector<cell>>& cellDetails, Pair dest)
{
    std::vector<std::pair<int, int>> final;
    printf("\nThe Path is ");
    int col = dest.first;
    int row = dest.second;

    stack<Pair> Path;

    while (!(cellDetails[col][row].parent_i == col
             && cellDetails[col][row].parent_j == row)) {
        Path.push(make_pair(col, row));
        int temp_col = cellDetails[col][row].parent_i;
        int temp_row = cellDetails[col][row].parent_j;
        col = temp_col;
        row = temp_row;
    }

    Path.push(make_pair(col, row));
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        final.push_back(p);
        printf("-> (%d,%d) ", p.first, p.second);
    }

    return final;
}


std::optional<std::vector<std::pair<int, int>>> aStarSearch(vector<vector<int>> &grid, Pair enemy, Pair player)
{
    // If the source is out of range
    // y : x
    if (isValid(grid, enemy.first, enemy.second) == false) {
        printf("Source is invalid\n");
        return nullopt;
    }

    // If the destination is out of range
    if (isValid(grid, player.first, player.second) == false) {
        printf("Destination is invalid\n");
        return nullopt;
    }

    // Either the source or the destination is blocked
    if (isUnBlocked(grid, enemy.first, enemy.second) == false
        || isUnBlocked(grid, player.first, player.second)
           == false) {
        printf("Source or the destination is blocked\n");
        return nullopt;
    }

    // If the destination cell is the same as source cell
    if (isDestination(enemy.first, enemy.second, player)
        == true) {
        printf("We are already at the destination\n");
        return nullopt;
    }

    // Create a closed list and initialise it to false which
    // means that no cell has been included yet This closed
    // list is implemented as a boolean 2D array
    vector<vector<bool>> closedList(grid.size(), vector<bool>(grid[0].size(), false));


    // Declare a 2D array of structure to hold the details
    // of that cell
    vector<vector<cell>> cellDetails(grid.size(), vector<cell>(grid[0].size()));

    int y, x;

    for (y = 0; y < (int)grid.size(); y++) {
        for (x = 0; x < (int)grid[0].size(); x++) {
            cellDetails[y][x].f = FLT_MAX;
            cellDetails[y][x].g = FLT_MAX;
            cellDetails[y][x].h = FLT_MAX;
            cellDetails[y][x].parent_i = -1;
            cellDetails[y][x].parent_j = -1;
        }
    }

    // Initialising the parameters of the starting node
    y = enemy.first, x = enemy.second;
    cellDetails[y][x].f = 0.0;
    cellDetails[y][x].g = 0.0;
    cellDetails[y][x].h = 0.0;
    cellDetails[y][x].parent_i = y;
    cellDetails[y][x].parent_j = x;

    /*
    Create an open list having information as-
    <f, <y, x>>
    where f = g + h,
    and y, x are the col and rowumn index of that cell
    Note that 0 <= y <= COL-1 & 0 <= x <= ROW-1
    This open list is implemented as a set of pair of
    pair.*/
    set<pPair> openList;

    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair(0.0, make_pair(y, x)));

    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;

    while (!openList.empty()) {
        pPair p = *openList.begin();

        // Remove this vertex from the open list
        openList.erase(openList.begin());

        // Add this vertex to the closed list
        y = p.second.first;
        x = p.second.second;
        closedList[y][x] = true;

        /*
        Generating all the 8 successor of this cell

            N.W N N.E
            \ | /
                \ | /
            W----Cell----E
                / | \
                / | \
            S.W S S.E

        Cell-->Popped Cell (y, x)
        N --> North	 (y-1, x)
        S --> South	 (y+1, x)
        E --> East	 (y, x+1)
        W --> West		 (y, x-1)
        N.E--> North-East (y-1, x+1)
        N.W--> North-West (y-1, x-1)
        S.E--> South-East (y+1, x+1)
        S.W--> South-West (y+1, x-1)*/

        // To store the 'g', 'h' and 'f' of the 8 successors
        double gNew, hNew, fNew;

        //----------- 1st Successor (North) ------------

        // Only process this cell if this is a valid one
        if (isValid(grid,y - 1, x) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(y - 1, x, player) == true) {
                // Set the Parent of the destination cell
                cellDetails[y - 1][x].parent_i = y;
                cellDetails[y - 1][x].parent_j = x;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, player);
                foundDest = true;
                unfuckPath(path);
                return path;
            }
                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[y - 1][x] == false
                     && isUnBlocked(grid, y - 1, x)
                        == true) {
                gNew = cellDetails[y][x].g + 1.0;
                hNew = calculateHValue(y - 1, x, player);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[y - 1][x].f == FLT_MAX
                    || cellDetails[y - 1][x].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(y - 1, x)));

                    // Update the details of this cell
                    cellDetails[y - 1][x].f = fNew;
                    cellDetails[y - 1][x].g = gNew;
                    cellDetails[y - 1][x].h = hNew;
                    cellDetails[y - 1][x].parent_i = y;
                    cellDetails[y - 1][x].parent_j = x;
                }
            }
        }

        //----------- 2nd Successor (South) ------------

        // Only process this cell if this is a valid one
        if (isValid(grid,y + 1, x) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(y + 1, x, player) == true) {
                // Set the Parent of the destination cell
                cellDetails[y + 1][x].parent_i = y;
                cellDetails[y + 1][x].parent_j = x;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, player);
                foundDest = true;
                unfuckPath(path);
                return path;
            }
                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[y + 1][x] == false
                     && isUnBlocked(grid, y + 1, x)
                        == true) {
                gNew = cellDetails[y][x].g + 1.0;
                hNew = calculateHValue(y + 1, x, player);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[y + 1][x].f == FLT_MAX
                    || cellDetails[y + 1][x].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(y + 1, x)));
                    // Update the details of this cell
                    cellDetails[y + 1][x].f = fNew;
                    cellDetails[y + 1][x].g = gNew;
                    cellDetails[y + 1][x].h = hNew;
                    cellDetails[y + 1][x].parent_i = y;
                    cellDetails[y + 1][x].parent_j = x;
                }
            }
        }

        //----------- 3rd Successor (East) ------------

        // Only process this cell if this is a valid one
        if (isValid(grid, y, x + 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(y, x + 1, player) == true) {
                // Set the Parent of the destination cell
                cellDetails[y][x + 1].parent_i = y;
                cellDetails[y][x + 1].parent_j = x;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, player);
                foundDest = true;
                unfuckPath(path);
                return path;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[y][x + 1] == false
                     && isUnBlocked(grid, y, x + 1)
                        == true) {
                gNew = cellDetails[y][x].g + 1.0;
                hNew = calculateHValue(y, x + 1, player);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[y][x + 1].f == FLT_MAX
                    || cellDetails[y][x + 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(y, x + 1)));

                    // Update the details of this cell
                    cellDetails[y][x + 1].f = fNew;
                    cellDetails[y][x + 1].g = gNew;
                    cellDetails[y][x + 1].h = hNew;
                    cellDetails[y][x + 1].parent_i = y;
                    cellDetails[y][x + 1].parent_j = x;
                }
            }
        }

        //----------- 4th Successor (West) ------------

        // Only process this cell if this is a valid one
        if (isValid(grid, y, x - 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(y, x - 1, player) == true) {
                // Set the Parent of the destination cell
                cellDetails[y][x - 1].parent_i = y;
                cellDetails[y][x - 1].parent_j = x;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, player);
                foundDest = true;
                unfuckPath(path);
                return path;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[y][x - 1] == false
                     && isUnBlocked(grid, y, x - 1)
                        == true) {
                gNew = cellDetails[y][x].g + 1.0;
                hNew = calculateHValue(y, x - 1, player);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[y][x - 1].f == FLT_MAX
                    || cellDetails[y][x - 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(y, x - 1)));

                    // Update the details of this cell
                    cellDetails[y][x - 1].f = fNew;
                    cellDetails[y][x - 1].g = gNew;
                    cellDetails[y][x - 1].h = hNew;
                    cellDetails[y][x - 1].parent_i = y;
                    cellDetails[y][x - 1].parent_j = x;
                }
            }
        }

        //----------- 5th Successor (North-East)
        //------------

        // Only process this cell if this is a valid one
        if (isValid(grid, y - 1, x + 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(y - 1, x + 1, player) == true) {
                // Set the Parent of the destination cell
                cellDetails[y - 1][x + 1].parent_i = y;
                cellDetails[y - 1][x + 1].parent_j = x;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, player);
                foundDest = true;
                unfuckPath(path);
                return path;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[y - 1][x + 1] == false
                     && isUnBlocked(grid, y - 1, x + 1)
                        == true) {
                gNew = cellDetails[y][x].g + 1.414;
                hNew = calculateHValue(y - 1, x + 1, player);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[y - 1][x + 1].f == FLT_MAX
                    || cellDetails[y - 1][x + 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(y - 1, x + 1)));

                    // Update the details of this cell
                    cellDetails[y - 1][x + 1].f = fNew;
                    cellDetails[y - 1][x + 1].g = gNew;
                    cellDetails[y - 1][x + 1].h = hNew;
                    cellDetails[y - 1][x + 1].parent_i = y;
                    cellDetails[y - 1][x + 1].parent_j = x;
                }
            }
        }

        //----------- 6th Successor (North-West)
        //------------

        // Only process this cell if this is a valid one
        if (isValid(grid, y - 1, x - 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(y - 1, x - 1, player) == true) {
                // Set the Parent of the destination cell
                cellDetails[y - 1][x - 1].parent_i = y;
                cellDetails[y - 1][x - 1].parent_j = x;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, player);
                foundDest = true;
                unfuckPath(path);
                return path;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[y - 1][x - 1] == false
                     && isUnBlocked(grid, y - 1, x - 1)
                        == true) {
                gNew = cellDetails[y][x].g + 1.414;
                hNew = calculateHValue(y - 1, x - 1, player);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[y - 1][x - 1].f == FLT_MAX
                    || cellDetails[y - 1][x - 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(y - 1, x - 1)));
                    // Update the details of this cell
                    cellDetails[y - 1][x - 1].f = fNew;
                    cellDetails[y - 1][x - 1].g = gNew;
                    cellDetails[y - 1][x - 1].h = hNew;
                    cellDetails[y - 1][x - 1].parent_i = y;
                    cellDetails[y - 1][x - 1].parent_j = x;
                }
            }
        }

        //----------- 7th Successor (South-East)
        //------------

        // Only process this cell if this is a valid one
        if (isValid(grid, y + 1, x + 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(y + 1, x + 1, player) == true) {
                // Set the Parent of the destination cell
                cellDetails[y + 1][x + 1].parent_i = y;
                cellDetails[y + 1][x + 1].parent_j = x;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, player);
                foundDest = true;
                unfuckPath(path);
                return path;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[y + 1][x + 1] == false
                     && isUnBlocked(grid, y + 1, x + 1)
                        == true) {
                gNew = cellDetails[y][x].g + 1.414;
                hNew = calculateHValue(y + 1, x + 1, player);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[y + 1][x + 1].f == FLT_MAX
                    || cellDetails[y + 1][x + 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(y + 1, x + 1)));

                    // Update the details of this cell
                    cellDetails[y + 1][x + 1].f = fNew;
                    cellDetails[y + 1][x + 1].g = gNew;
                    cellDetails[y + 1][x + 1].h = hNew;
                    cellDetails[y + 1][x + 1].parent_i = y;
                    cellDetails[y + 1][x + 1].parent_j = x;
                }
            }
        }

        //----------- 8th Successor (South-West)
        //------------

        // Only process this cell if this is a valid one
        if (isValid(grid, y + 1, x - 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(y + 1, x - 1, player) == true) {
                // Set the Parent of the destination cell
                cellDetails[y + 1][x - 1].parent_i = y;
                cellDetails[y + 1][x - 1].parent_j = x;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, player);
                foundDest = true;
                unfuckPath(path);
                return path;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[y + 1][x - 1] == false
                     && isUnBlocked(grid, y + 1, x - 1)
                        == true) {
                gNew = cellDetails[y][x].g + 1.414;
                hNew = calculateHValue(y + 1, x - 1, player);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[y + 1][x - 1].f == FLT_MAX
                    || cellDetails[y + 1][x - 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(y + 1, x - 1)));

                    // Update the details of this cell
                    cellDetails[y + 1][x - 1].f = fNew;
                    cellDetails[y + 1][x - 1].g = gNew;
                    cellDetails[y + 1][x - 1].h = hNew;
                    cellDetails[y + 1][x - 1].parent_i = y;
                    cellDetails[y + 1][x - 1].parent_j = x;
                }
            }
        }
    }

    // When the destination cell is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destination cell. This may happen when the
    // there is no way to destination cell (due to
    // blockages)
    if (foundDest) {
        auto path = tracePath(cellDetails, player);
        return path;
    } else {
        //printf("Failed to find the Destination Cell\n");
        return std::nullopt;
    }
}

#endif
