//
// Created by atopi on 13.12.2018.
//

#include <iostream>
#include "PlayerPath.h"
#include "AStar.h"

PlayerPath::PlayerPath(int width, int box_count, int size, int* cleared_map) :
alg(width, box_count, size, cleared_map)
{}

int PlayerPath::calcPlayerPath(Node *solution, int initialPlayerPos)
{
    Node *h1, *h2 = nullptr;
    Node *top, *head;

    // Set player positions & produce double-linked list
    h1 = solution;
    while(solution->farther != nullptr)
    {
        int tmp_f = solution->move->from;
        int tmp_t = solution->move->to;
        solution->move->from = tmp_f - (tmp_t - tmp_f);
        solution->move->to   = tmp_f;

        solution->next = h2;
        h2 = solution;

        solution = solution->farther;
    }
    top = h2;
    if(top == nullptr) return -1;
    //solution = h1;
    head = h2->farther;

    /*while(top != nullptr)
    {
        std::cout << "Move: " << Move::str(*top->move) << std::endl;
        top = top->next;
    }
    top = h2;*/

    // Produce path in gaps
    h2 = top->next;
    h1 = top;
    while(h2 != nullptr)
    {
        if(h1->move->to != h2->move->from)
        {
            INode *in = alg.astar(h1->move->to, h2->move->from, h1->box_array);
            INode *tmp = in;
            in = in->parent;
            while(in != nullptr)
            {
                Move *m = new Move();
                m->from = in->point; m->to = tmp->point;

                Node *n = new Node(h1->box_array, h1->player_pos, h1, m, 0, {});
                n->next = h1->next;
                h1->next = n;

                if(n->next != nullptr)
                    n->next->farther = n;

                tmp = in;
                in = in->parent;
            }
        }
        h1 = h2;
        h2 = h2->next;
    }


    // Produce initial path
    h1 = top;

    INode *in = alg.astar(initialPlayerPos, top->move->from, head->box_array);
    INode *tmp = in;
    in = in->parent;
    while(in != nullptr)
    {
        Move *m = new Move();
        m->from = in->point; m->to = tmp->point;

        Node *n = new Node(nullptr, top->player_pos, nullptr, m, 0, {});
        n->next = h1;

        if(h1 != nullptr)
            h1->farther = n;

        h1 = n;

        tmp = in;
        in = in->parent;
    }

    return 0;
}