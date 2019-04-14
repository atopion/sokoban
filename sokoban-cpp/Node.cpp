//
// Created by atopi on 04.12.2018.
//

#include "Node.h"
#include <list>
#include <iostream>

Node::~Node()
{
    /*std::list<Node *>::iterator it;
    for(it = sons.begin(); it != sons.end(); )
    {
        //delete *it;
        it = sons.erase(it);
    }*/
    for(auto && son : sons)
    {
        if(son != nullptr)
        {
            delete son;
            son = nullptr;
        }
    }
    sons.clear();
    if(!this->root && this->box_array != nullptr)
    {
        delete[] this->box_array;
        this->box_array = nullptr;
    }
    if(this->directions != nullptr)
    {
        delete[] this->directions;
        this->directions = nullptr;
    }
}

Node::Node()
{
    Node::box_array = nullptr;
    Node::player_pos = 0;
    Node::farther = nullptr;
    Node::move = {-1, -1};
    Node::lower_bound = 1000000;
    Node::sons = std::list<Node *>();
    Node::depth = 0;
    Node::directions = nullptr;
    Node::boxMove = false,
    Node::root = false;
    Node::next = nullptr;
}

Node::Node(int *box_array, int player_pos, Node *farther, Move move, int lower_bound, const std::list<Node *> &sons, int depth)
{
    Node::box_array = box_array;
    Node::player_pos = player_pos;
    Node::farther = farther;
    Node::move = move;
    Node::lower_bound = lower_bound;
    Node::sons = sons;
    Node::depth = depth;
    Node::directions = nullptr;
    Node::boxMove = false;
    Node::root = false;

    Node::next = nullptr;
}

Node::Node(int *box_array, int player_pos, int lower_bound, bool root)
{
    Node::box_array = box_array;
    Node::player_pos = player_pos;
    Node::farther = nullptr;
    Node::move.to = -1;
    Node::move.from = -1;
    Node::lower_bound = lower_bound;
    Node::sons = std::list<Node *>();
    Node::depth = 0;
    Node::directions = nullptr;
    Node::boxMove = false;
    Node::root = root;

    Node::next = nullptr;
}