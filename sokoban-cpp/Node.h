//
// Created by atopi on 04.12.2018.
//

#ifndef SOKOBANSOLVER_NODE_H
#define SOKOBANSOLVER_NODE_H


#include "Move.h"
#include <list>

class Node
{
public:
    int *box_array;
    int player_pos;
    Node *farther;
    Node *next;
    Move *move;
    int lower_bound;
    int depth;
    std::list<Node *> sons;

    Node(int *box_array, int player_pos, Node *farther, Move *move, int lower_bound, const std::list<Node *> &sons = std::list<Node *>(), int depth = 0);
    Node();
    ~Node();
};


#endif //SOKOBANSOLVER_NODE_H
