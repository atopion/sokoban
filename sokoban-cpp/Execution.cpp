//
// Created by atopi on 03.12.2018.
//

#include "Execution.h"
#include <stdio.h>
#include <list>

#define MODELPATH "perceptron/models/model_not_center.tflite"

//#define QUICK_SOLUTION

Execution::Execution(Map *map) : transpositionTable(map->getBoxCount(), map->getSize()),
assignmentAlgorithms(map->getWidth(), map->getHeight(), map->getBoxCount(), map->getTargetsArray(), map->getTargetsCount(), map->getClearedMap()),
deadlockDetection(map->getClearedMap(), map->getSize(), map->getWidth(), map->getTargetsArray(), map->getTargetsCount()),
boundAlgorithms(map), perceptron(MODELPATH)
{
    Execution::map = map;
    Execution::width = map->getWidth();
    Execution::height = map->getHeight();
    Execution::clearedMap = map->getClearedMap();
    Execution::size = map->getSize();
    Execution::boxCount = map->getBoxCount();
    Execution::openSet = std::list<Node *>();

    Execution::transpositionTable.insert(transpositionTable.computeHash(map->getBoxArray(), map->getPlayer()));
    Execution::PathCount = 0;
    Execution::secondTryCount = 0;
    Execution::b = true;
    Execution::c = 0;
}

int Execution::cont(int *array, int size, int item)
{
    for(int i = 0; i < size; i++)
        if(array[i] == item)
            return i;
    return -1;
}

Move** Execution::possibleMoves(int pos, int *box_array)
{
    Move **result = new Move*[4];
    for(int i = 0; i < 4; i++)
        result[i] = new Move;

    int i = 0;
    int directions[] = {-width, width, -1, 1};
    this->perceptron.invoke(directions, map->getFullMap(box_array, pos), map->getHeight(), map->getWidth(), pos);
    //std::random_shuffle(&directions[0], &directions[3]);
    for(int a : directions)
    {
        if(clearedMap[pos + a] != 4 && pos >= 0 && pos < size && !(cont(box_array, boxCount, pos + a) != -1 &&
        (cont(box_array, boxCount, pos + 2*a) != -1 || clearedMap[pos + 2*a] == 4)) &&
        !(cont(box_array, boxCount, pos + a) != -1 && deadlockDetection.lookup(pos + 2*a)))
            *result[i] = {.from = pos, .to = pos + a};
        else
            *result[i] = {0, 0};
        ++i;
    }

    return result;
}

Node *Execution::analyseState(Node *node)
{
    int *box_array = node->box_array;
    int pos = node->player_pos;

    Move **m = possibleMoves(pos, box_array);
    for(int j = 0; j < 4; j++)
    {
        if(m[j]->to == 0 && m[j]->from == 0)
            continue;

        bool boxMoved = false;
        int *new_box_array = new int[boxCount];
        for(int k = 0; k < boxCount; k++)
        {
            if(box_array[k] == m[j]->to)
            {
                new_box_array[k] = 2*m[j]->to - m[j]->from;
                boxMoved = true;
            }
            else
                new_box_array[k] = box_array[k];
        }

        if(transpositionTable.lookup(transpositionTable.computeHash(new_box_array, m[j]->to)))
        {
            delete[] new_box_array;
            continue;
        }

        transpositionTable.insert(transpositionTable.computeHash(new_box_array, m[j]->to));
        PathCount++;
        if(j != 0)
            secondTryCount++;

        int bound = boundAlgorithms.greedyBound(new_box_array, m[j]->to);
        Move new_move;
        new_move.to = m[j]->to;
        new_move.from = m[j]->from;

        int *directions = new int[4];
        for(int i = 0; i < 4; i++)
            directions[i] = m[i]->to - m[i]->from;

        Node* n = new Node;
        n->box_array = new_box_array;
        n->player_pos = m[j]->to;
        n->farther = node;
        n->move = new_move;
        n->lower_bound = bound;
        n->sons = std::list<Node*>();
        n->depth = node->depth+1;
        n->root = false;
        n->directions = directions;
        n->map = map->getFullMap(node->box_array, node->player_pos);
        n->boxMove = boxMoved;
        node->sons.push_back(n);


        if(bound == 0)
        {
            for(int i = 0; i < 4; i++) delete m[i];
            delete[] m;
            return n;
        }
        
        if(bound < node->lower_bound)
        {
            //std::cout << "Analysing " << m[j]->to - m[j]->from << "\t bound = " << bound << "\t Farther: " << node->lower_bound << std::endl;
            Node *res = analyseState(n);
            //std::cout << "Result " << n << std::endl;
            if (res != nullptr)
            {
                for(int i = 0; i < 4; i++) delete m[i];
                delete[] m;
                return res;
            }
            else
            {
                //this->c++;
            }
        }
        else
        {
            //std::cout << "Direction: " << m[j]->to - m[j]->from << "\t Bound " << bound << std::endl;
            for(auto it = openSet.begin(); it != openSet.end(); it++)
            {
                if(bound + n->depth < (*it)->lower_bound + (*it)->depth)
                {
                    openSet.insert(it, n);
                    goto done;
                }
            }
            openSet.push_back(n);
            done: 
            1;
        }
    }
    for(int i = 0; i < 4; i++)
        delete m[i];
    delete[] m;
    return nullptr;
}

Node *Execution::execute(Node *current_node, int depth)
{
    openSet.push_front(current_node);
    while(!openSet.empty())
    {
        current_node = openSet.front(); openSet.pop_front();
        Node* res = analyseState(current_node);
        if(res != nullptr)
        {
            return res;
        }
        else
        {
            this->c++;
        }
    }
    return nullptr;

    return analyseState(current_node);
}

// DEBUG
void Execution::printDeadlockMap()
{
    deadlockDetection.printMap();
}
