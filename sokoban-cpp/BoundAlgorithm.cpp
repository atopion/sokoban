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
    BoundAlgorithm::map = map->getClearedMap();
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
    int distance_sum = 0, b = 0, t = 0, p = 0;

    int k = 0;
    for(int i = 0; i < targetCount; i++)
    {
        for(int j = 0; j < boxCount; j++)
        {
            if(targets[i] == boxArray[j])
            {
                matchedBoxes.push_front(boxArray[j]);
                matchedTargets.push_front(targets[i]);
                k++;
                break;
            }
        }
    }

    Point x = {0, 0}, y = {pos, 0};

    for(int i = 0; i < boxCount - k; i++)
    {
        p = y.p;
        x = minimum(boxArray, boxCount, matchedBoxes, y.p); // Box point
        y = minimum(targets, targetCount, matchedTargets, x.p); // Target point
        matchedBoxes.push_front(x.p);
        matchedTargets.push_front(y.p);

        if(x.p == -1 || y.p == -1)
        {
            std::cout << "No more boxes or targets" << std::endl;
            break;
        }

        b = boxPath(y.p, x.p);
        t = targetPath(y.p, b);
        b = metrics.pull_distance(b, p);

        //distance_sum += 2*x.m + 4*y.m;
        //distance_sum += x.m + (int)(2.131814 * y.m);
        distance_sum += b + (int)(2.131814 * t);
    }

    return distance_sum;
}

int BoundAlgorithm::boxPath(int box, int target)
{
    int p[5], min = 100000, pos = -1;

    p[0] = metrics.pull_distance(target, box);
    if(map[box-width] != 4)
        p[1] = metrics.pull_distance(target, box-width);
    else
        p[1] = 100000;

    if(map[box+width] != 4)
        p[2] = metrics.pull_distance(target, box+width);
    else
        p[2] = 100000;
    
    if(map[box-1] != 4)
        p[3] = metrics.pull_distance(target, box-1);
    else
        p[3] = 100000;

    if(map[box+1] != 4)
        p[4] = metrics.pull_distance(target, box+1);
    else
        p[4] = 100000;

    for(int i = 0; i < 5; i++)
    {
        if(p[i] < min)
        {
            min = p[i];
            pos = i;
        }
    }
        
    if(min == 100000)
        return box;
    
    switch(pos)
    {
        case 0:
            return box;
        case 1:
            return box + width;
        case 2:
            return box - width;
        case 3:
            return box + 1;
        case 4:
            return box - 1;
        default:
            return box;
    }
}

int BoundAlgorithm::targetPath(int target, int box)
{
    int p[5], min = 100000;

    p[0] = metrics.pull_distance(target, box);
    if(map[target-width] != 4)
        p[1] = metrics.pull_distance(target-width, box);
    else
        p[1] = 100000;

    if(map[target+width] != 4)
        p[2] = metrics.pull_distance(target+width, box);
    else
        p[2] = 100000;
    
    if(map[target-1] != 4)
        p[3] = metrics.pull_distance(target-1, box);
    else
        p[3] = 100000;

    if(map[target+1] != 4)
        p[4] = metrics.pull_distance(target+1, box);
    else
        p[4] = 100000;

    for(int i = 0; i < 5; i++)
        if(p[i] < min)
            min = p[i];
        
    if(min != 100000)
        return min;
    else
        return p[0];
}