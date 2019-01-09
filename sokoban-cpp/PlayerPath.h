//
// Created by atopi on 13.12.2018.
//

#ifndef SOKOBANSOLVER_PLAYERPATH2_H
#define SOKOBANSOLVER_PLAYERPATH2_H


#include "Node.h"
#include "AStar.h"

class PlayerPath
{
private:
    AStar alg;
public:
    PlayerPath(int width, int box_count, int size, int* cleard_map);
    int calcPlayerPath(Node *solution, int initialPlayerPos);


};


#endif //SOKOBANSOLVER_PLAYERPATH2_H
