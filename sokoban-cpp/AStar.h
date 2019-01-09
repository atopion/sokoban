//
// Created by atopi on 10.12.2018.
//

#ifndef SOKOBANSOLVER_ASTAR_H
#define SOKOBANSOLVER_ASTAR_H

#include "Metrics.h"

class INode {
public:
    int point;
    int G;
    int H;
    INode *parent;

    INode();
    INode(int point, INode *parent);
};

class AStar {
private:
    int width;
    int box_count;
    int size;
    int *cleared_map;

    int *children(int point, int *box_array);

public:
    AStar(int width, int box_count, int size, int* cleared_map);
    INode *astar(int start, int goal, int *box_array);
};


#endif //SOKOBANSOLVER_ASTAR_H
