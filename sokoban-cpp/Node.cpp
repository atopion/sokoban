//
// Created by atopi on 04.12.2018.
//

#include "Node.h"
#include <list>
#include <iostream>

Node::~Node()
{
    std::list<Node *>::iterator it;
    for(it = sons.begin(); it != sons.end(); )
    {
        //delete *it;
        it = sons.erase(it);
    }
    //delete move;
    delete box_array;
}

Node::Node(int *box_array, int player_pos, Node *farther, Move *move, int lower_bound, const std::list<Node *> &sons, int depth)
{
    Node::box_array = box_array;
    Node::player_pos = player_pos;
    Node::farther = farther;
    Node::move = move;
    Node::lower_bound = lower_bound;
    Node::sons = sons;
    Node::depth = depth;

    Node::next = nullptr;
}

Node::Node() = default;