//
// Created by atopi on 03.12.2018.
//

#ifndef SOKOBANSOLVER_EXECUTION_H
#define SOKOBANSOLVER_EXECUTION_H


#include "Node.h"
#include "TranspositionTable.h"
#include "Map.h"
#include "AssignmentAlgorithms.h"
#include "DeadlockDetection.h"
#include "BoundAlgorithm.h"
#include "perceptron/Perceptron.h"

class Execution
{
private:
    Map *map;
    int width;
    int height;
    int *clearedMap;
    int size;
    int boxCount;
    std::list<Node*> openSet;

    // DEBUG
    int PathCount;
    int secondTryCount;
    bool b;
    int c;

    TranspositionTable transpositionTable;
    AssignmentAlgorithms assignmentAlgorithms;
    DeadlockDetection deadlockDetection;
    BoundAlgorithm boundAlgorithms;
    Perceptron perceptron;

public:
    explicit Execution(Map *map);

    Move **possibleMoves(int pos, int *box_array);
    Node *analyseState(Node *node);
    Node *execute(Node *current_node, int depth=0);

    int cont(int *array, int size, int item);

    //DEBUG
    void printDeadlockMap();
    int getPathCount() { return PathCount; }
    int getSecondTryCount() { return secondTryCount; }
    int getCCount() { return c; }
};

#endif //SOKOBANSOLVER_EXECUTION_H
