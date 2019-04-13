//
// Created by atopi on 14.01.2019.
//

#ifndef SOKOBANSOLVER_BOUNDALGORITHM_H
#define SOKOBANSOLVER_BOUNDALGORITHM_H


#include <list>
#include <algorithm>
#include "Metrics.h"
#include "Map.h"

typedef struct {int p; int m;} Point;

class BoundAlgorithm
{
private:
    int *targets;
    int *map;
    int  targetCount;
    int  boxCount;

    int width;
    Metrics metrics;

public:
    explicit BoundAlgorithm(Map *map);
    int greedyBound(int* boxArray, int pos);
    Point minimum(int *array, int arraySize, std::list<int> exclude, int target);
    int boxPath(int box, int target);
    int targetPath(int target, int box);
};


#endif //SOKOBANSOLVER_BOUNDALGORITHM_H
