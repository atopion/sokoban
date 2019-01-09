//
// Created by atopi on 11.12.2018.
//

#ifndef SOKOBANSOLVER_PLAYERPATH_H
#define SOKOBANSOLVER_PLAYERPATH_H

#include "Map.h"
#include "AStar.h"
#include "Node.h"

class PlayerPath_old
{
private:
    int playerPos;
    int box_count;
    AStar alg;

public:
    PlayerPath_old(int playerPos, int width, int box_count, int size, int *cleared_map);
    int calculatePlayerPath(std::list<Node> *path, int path_length);
    std::list<Node> calculateEdgesPath(std::list<Node> *path, int path_length);
    std::list<Node> calculateInitialPath(std::list<Node> *path, int *box_array);
};


#endif //SOKOBANSOLVER_PLAYERPATH_H
