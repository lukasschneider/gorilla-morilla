#ifndef GORILLAGAME_ASTAR_H
#define GORILLAGAME_ASTAR_H

#include "global.h"
#include <fstream>

using namespace std;

typedef pair<int, int> Pair;

typedef pair<double, pair<int, int> > pPair;

struct cell {
    int parent_i, parent_j;
    double f, g, h;
};

bool isValid(int row, int col, const vector<vector<int>> &grid) {
    return (row >= 0) && (row < (int)grid.size()) && (col >= 0) && (col < (int)grid[0].size());
}

std::vector<std::vector<int>> transformMatrix(const std::vector<std::vector<int>> &matrix) {
    std::vector<std::vector<int>> transformedMatrix = matrix;

    for (auto &row: transformedMatrix) {
        for (auto &elem: row) {
            elem = (elem == -1) ? 1 : 0;
        }
    }

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


bool isUnBlocked(vector<vector<int>> &grid, int row, int col) {
    if (grid[row][col] == 1) return (true);
    else return (false);
}


bool isDestination(int row, int col, Pair dest) {
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}


double calculateHValue(int row, int col, Pair dest) {

    return ((double) sqrt(
            (row - dest.first) * (row - dest.first)
            + (col - dest.second) * (col - dest.second)));
}


std::vector<std::pair<int, int>> tracePath(vector<vector<cell>> &cellDetails, Pair dest) {
    std::vector<std::pair<int, int>> path;

    cout << "\nThe Path is ";
    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;

    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col)) {
        Path.emplace(row, col);
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.emplace(row, col);
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        path.push_back(p);
        printf("-> (x: %d, y: %d) ", p.second, p.first);
    }

    return path;
}


std::optional<std::vector<std::pair<int, int>>> aStarSearch(vector<vector<int>> &grid, Pair src, Pair dest) {

    if (!isValid(src.first, src.second, grid)) {
        printf("Source is invalid\n");
        return nullopt;
    }


    if (!isValid(dest.first, dest.second, grid)) {
        printf("Destination is invalid\n");
        return nullopt;
    }


    if (!isUnBlocked(grid, src.first, src.second)
        || !isUnBlocked(grid, dest.first, dest.second)) {
        printf("Source or the destination is blocked\n");
        return nullopt;
    }


    if (isDestination(src.first, src.second, dest)) {
        printf("We are already at the destination\n");
        return nullopt;
    }


    vector<vector<bool>> closedList(grid.size(), vector<bool>(grid[0].size(), false));


    vector<vector<cell>> cellDetails(grid.size(), vector<cell>(grid[0].size()));

    int i, j;

    for (i = 0; i < (int)grid.size(); i++) {
        for (j = 0; j < (int)grid[0].size(); j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }


    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    /*
    Create an open list having information as-
    <f, <i, j>>
    where f = g + h,
    and i, j are the row and column index of that cell
    Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    This open list is implemented as a set of pair of
    pair.*/
    set<pPair> openList;


    openList.insert(make_pair(0.0, make_pair(i, j)));


    bool foundDest = false;

    while (!openList.empty()) {
        pPair p = *openList.begin();


        openList.erase(openList.begin());


        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        /*
        Generating all the 8 successor of this cell

            N.W N N.E
            \ | /
                \ | /
            W----Cell----E
                / | \
                / | \
            S.W S S.E

        Cell-->Popped Cell (i, j)
        N --> North	 (i-1, j)
        S --> South	 (i+1, j)
        E --> East	 (i, j+1)
        W --> West		 (i, j-1)
        N.E--> North-East (i-1, j+1)
        N.W--> North-West (i-1, j-1)
        S.E--> South-East (i+1, j+1)
        S.W--> South-West (i+1, j-1)*/


        double gNew, hNew, fNew;


        if (isValid(i - 1, j, grid)) {


            if (isDestination(i - 1, j, dest)) {

                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, dest);
                foundDest = true;
                return path;
            } else if (!closedList[i - 1][j]
                       && isUnBlocked(grid, i - 1, j)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, dest);
                fNew = gNew + hNew;


                if (cellDetails[i - 1][j].f == FLT_MAX
                    || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i - 1, j)));


                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }


        if (isValid(i + 1, j, grid)) {


            if (isDestination(i + 1, j, dest)) {

                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, dest);
                foundDest = true;
                return path;
            } else if (!closedList[i + 1][j]
                       && isUnBlocked(grid, i + 1, j)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, dest);
                fNew = gNew + hNew;


                if (cellDetails[i + 1][j].f == FLT_MAX
                    || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i + 1, j)));

                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }


        if (isValid(i, j + 1, grid)) {


            if (isDestination(i, j + 1, dest)) {

                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, dest);
                foundDest = true;
                return path;
            } else if (!closedList[i][j + 1]
                       && isUnBlocked(grid, i, j + 1)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i][j + 1].f == FLT_MAX
                    || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i, j + 1)));


                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }


        if (isValid(i, j - 1, grid)) {


            if (isDestination(i, j - 1, dest)) {

                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, dest);
                foundDest = true;
                return path;
            } else if (!closedList[i][j - 1]
                       && isUnBlocked(grid, i, j - 1)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i][j - 1].f == FLT_MAX
                    || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i, j - 1)));


                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }


        if (isValid(i - 1, j + 1, grid)) {


            if (isDestination(i - 1, j + 1, dest)) {

                cellDetails[i - 1][j + 1].parent_i = i;
                cellDetails[i - 1][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, dest);
                foundDest = true;
                return path;
            } else if (!closedList[i - 1][j + 1]
                       && isUnBlocked(grid, i - 1, j + 1)) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j + 1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i - 1][j + 1].f == FLT_MAX
                    || cellDetails[i - 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i - 1, j + 1)));


                    cellDetails[i - 1][j + 1].f = fNew;
                    cellDetails[i - 1][j + 1].g = gNew;
                    cellDetails[i - 1][j + 1].h = hNew;
                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                }
            }
        }


        if (isValid(i - 1, j - 1, grid)) {


            if (isDestination(i - 1, j - 1, dest)) {

                cellDetails[i - 1][j - 1].parent_i = i;
                cellDetails[i - 1][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, dest);
                foundDest = true;
                return path;
            } else if (!closedList[i - 1][j - 1]
                       && isUnBlocked(grid, i - 1, j - 1)) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j - 1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i - 1][j - 1].f == FLT_MAX
                    || cellDetails[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i - 1, j - 1)));

                    cellDetails[i - 1][j - 1].f = fNew;
                    cellDetails[i - 1][j - 1].g = gNew;
                    cellDetails[i - 1][j - 1].h = hNew;
                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                }
            }
        }


        if (isValid(i + 1, j + 1, grid)) {


            if (isDestination(i + 1, j + 1, dest)) {

                cellDetails[i + 1][j + 1].parent_i = i;
                cellDetails[i + 1][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, dest);
                foundDest = true;
                return path;
            } else if (!closedList[i + 1][j + 1]
                       && isUnBlocked(grid, i + 1, j + 1)) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j + 1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i + 1][j + 1].f == FLT_MAX
                    || cellDetails[i + 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i + 1, j + 1)));


                    cellDetails[i + 1][j + 1].f = fNew;
                    cellDetails[i + 1][j + 1].g = gNew;
                    cellDetails[i + 1][j + 1].h = hNew;
                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                }
            }
        }


        if (isValid(i + 1, j - 1, grid)) {


            if (isDestination(i + 1, j - 1, dest)) {

                cellDetails[i + 1][j - 1].parent_i = i;
                cellDetails[i + 1][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                auto path = tracePath(cellDetails, dest);
                foundDest = true;
                return path;
            } else if (!closedList[i + 1][j - 1]
                       && isUnBlocked(grid, i + 1, j - 1)) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j - 1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i + 1][j - 1].f == FLT_MAX
                    || cellDetails[i + 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i + 1, j - 1)));


                    cellDetails[i + 1][j - 1].f = fNew;
                    cellDetails[i + 1][j - 1].g = gNew;
                    cellDetails[i + 1][j - 1].h = hNew;
                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
                }
            }
        }
    }


    if (foundDest) {
        auto path = tracePath(cellDetails, dest);
        return path;
    } else {
        printf("Failed to find the Destination Cell\n");
        return std::nullopt;
    }
}
#endif
