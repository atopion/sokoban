//
// Created by atopi on 14.01.2019.
//

#include "BoundAlgorithm.h"

BoundAlgorithm::BoundAlgorithm(Map *map) : metrics(map->getWidth(), map->getHeight(), map->getTargetsArray(), map->getTargetsCount(), map->getClearedMap())
{
    BoundAlgorithm::targets = map->getTargetsArray();
    BoundAlgorithm::targetCount = map->getTargetsCount();
    BoundAlgorithm::boxCount = map->getBoxCount();
    BoundAlgorithm::width = map->getWidth();
}

Point BoundAlgorithm::minimum(int *array, int arraySize, std::list<int> exclude, int target)
{
    int _min = 1000000;
    int _pos = -1;
    int _a = 0;

    for(int i = 0; i < arraySize; i++)
    {
        if(std::find(exclude.begin(), exclude.end(), array[i]) == exclude.end())
        {
            if((_a = metrics.pull_distance(target, array[i])) < _min)
            {
                _min = _a;
                _pos = array[i];
            }
        }
    }

    Point res = {_pos, _min};
    return res;
}

int BoundAlgorithm::greedyBound(int *boxArray, int pos)
{
    std::list<int> matchedBoxes   = std::list<int>();
    std::list<int> matchedTargets = std::list<int>();
    int distance_sum = 0;

    int b = 0;
    for(int i = 0; i < targetCount; i++)
    {
        for(int j = 0; j < boxCount; j++)
        {
            if(targets[i] == boxArray[j])
            {
                matchedBoxes.push_front(boxArray[j]);
                matchedTargets.push_front(targets[i]);
                b++;
                break;
            }
        }
    }

    Point x = {0, 0}, y = {pos, 0};

    for(int i = 0; i < boxCount - b; i++)
    {
        x = minimum(boxArray, boxCount, matchedBoxes, y.p);
        y = minimum(targets, targetCount, matchedTargets, x.p);
        matchedBoxes.push_front(x.p);
        matchedTargets.push_front(y.p);

        if(x.p == -1 || y.p == -1)
        {
            std::cout << "No more boxes or targets" << std::endl;
            break;
        }
        else
            distance_sum += x.m + 2*y.m;
    }

    return distance_sum;
}