//
// Created by atopi on 10.12.2018.
//

#include "AStar.h"
#include <list>
#include <queue>
#include <iostream>

INode::INode() = default;

INode::INode(int point, INode *parent)
{
    INode::point = point;
    INode::G = 0;
    INode::H = 0;
    INode::parent = parent;
}

AStar::AStar(int width, int box_count, int size, int *cleared_map)
{
    AStar::width = width;
    AStar::size = size;
    AStar::cleared_map = cleared_map;
    AStar::box_count = box_count;
}

int* AStar::children(int point, int *box_array)
{
    int * res = new int[4];
    int i = 0;
    for(int d : {-1, 1, -width, width})
    {
        res[i] = -1;
        i++;
        if(point + d >= size || point + d < 0)
            continue;

        if(cleared_map[point + d] % 2 == 0)
            continue;

        for(int j = 0; j < box_count; j++)
            if(box_array[j] == point + d)
                goto contin;

        res[i-1] = point + d;

        contin:;
    }
    return res;
}

INode* AStar::astar(int start, int goal, int* box_array)
{
    auto *s = new INode(start, nullptr);

    bool *closedSet = new bool[AStar::size];
    for(int i = 0; i < size; i++) closedSet[i] = false;
    std::list<INode *> openSet = std::list<INode *>();

    INode *current = s;
    //openSet.push(current);
    /*for(auto it = openSet.begin(); it != openSet.end(); it++)
        if((*it)->G + (*it)->H > (current->G + current->H))
            openSet.insert(it, current);*/
    openSet.push_front(current);

    while(!openSet.empty())
    {
        current = openSet.front(); openSet.erase(openSet.begin());
        if(current->point == goal)
        {
            delete[] closedSet;
            return current;
        }

        closedSet[current->point] = true;

        int *ch = AStar::children(current->point, box_array);
        for(int i = 0; i < 4; i++)
        {
            int a = ch[i];
            if(ch[i] == -1)
                continue;

            if(closedSet[ch[i]])
                continue;

            for(auto it = openSet.begin(); it != openSet.end(); it++)
            {
                if((*it)->point == ch[i])
                {
                    int new_g = current->G + 1;
                    if((*it)->G > new_g)
                    {
                        (*it)->G = new_g;
                        (*it)->parent = current;
                    }
                    goto updated;
                }
            }

            {
                auto node = new INode(ch[i], current);
                node->G = current->G + 1;
                node->H = Metrics::manhattan_distance(ch[i], goal, width); //METRICS
                for (auto it = openSet.begin(); it != openSet.end(); it++)
                {
                    if ((*it)->G + (*it)->H > (node->G + node->H))
                    {
                        openSet.insert(it, node);
                        goto inserted;
                    }
                }
                openSet.push_back(node);
            }

            inserted:
            updated:
            continue;
        }
    }
    delete[] closedSet;
    return nullptr;
}
