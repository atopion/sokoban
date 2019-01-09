//
// Created by atopi on 11.12.2018.
//

#include "PlayerPath_old.h"

PlayerPath_old::PlayerPath_old(int playerPos, int width, int box_count, int size, int *cleared_map) :
alg(width, box_count, size, cleared_map)
{
    PlayerPath_old::playerPos = playerPos;
    PlayerPath_old::box_count = box_count;
}

int PlayerPath_old::calculatePlayerPath(std::list<Node> *path, int path_length)
{
    for(auto it = path->begin(); it != path->end(); it++)
    {
        int tmp_f = it->move->from;
        int tmp_t = it->move->to;
        //std::cout << " D [" << tmp_f << "," << tmp_t << "] ";
        it->move->from = tmp_f - (tmp_t - tmp_f);
        it->move->to   = tmp_f;
    }
    //std::cout << std::endl;

    return path_length;
}

std::list<Node> PlayerPath_old::calculateEdgesPath(std::list<Node> *path, int path_length)
{
    //std::cout << std::endl;
    std::list<Node *> x0 = std::list<Node *>();
    std::list<Node> result = std::list<Node>();

    /*for(auto it = path->begin(); it != path->end(); it++)
    {
        std::cout << "New" << std::endl;
        for(int i = 0; i < box_count; i++)
            std::cout << "Box " << i << " : " << it->box_array[i];
        std::cout << std::endl;
    }*/


    //result.push_back(path->begin());
    //auto old = path->begin();
    for(auto it = ++path->begin(); it != path->end(); it++)
    {
        std::cout << "      " << Move::str(*std::prev(it)->move) << std::endl;
        std::cout << "LOOP: " << Move::str(*it->move) << std::endl;
        if(it->move->from != std::prev(it)->move->to)
        {
            /*std::cout << "ENTER" << std::endl;
            for(int i = 0; i < box_count; i++)
                std::cout << "Box " << i << " : " << it->box_array[i] << std::endl;*/
            INode *in = alg.astar(std::prev(it)->move->to, it->move->from, std::prev(it)->box_array);
            INode *tmp = in;
            in = in->parent;
            int c = 0;
            while(in != nullptr)
            {
                Move *m = new Move();
                m->from = in->point; m->to = tmp->point;
                Node n = Node(nullptr, std::prev(it)->player_pos, nullptr, m, 0, x0);
                //std::cout << "Insert: " << Move::str(*n.move) << std::endl;
                path->insert(it, n);
                it--;
                c++;
                tmp = in;
                in = in->parent;
                delete tmp;
            }
            std::advance(it, c);
        }
        //result.push_back(it);y
    }
    return result;
    /*int size = result.size();
    Node *res = new Node[size];
    auto it = result.begin();
    for(int i = 0; i < size; i++)
    {
        res[i] = *it;
        result.erase(it);
        it++;
    }
    path = res;
    return size;*/
}

std::list<Node> PlayerPath_old::calculateInitialPath(std::list<Node> *path, int* box_array)
{
    //for(int i = 0; i < box_count; i++)
    //    std::cout << "Init Box: " << box_array[i] << std::endl;
    //std::cout << "Player Pos: " << playerPos << std::endl;
    //std::cout << "Target: " << path->begin()->move->from << std::endl;
    INode *in = alg.astar(playerPos, path->begin()->move->from, box_array);
    std::list<Node> result = std::list<Node>();
    std::list<Node *> x0 = std::list<Node *>();
    INode *tmp = in;
    in = in->parent;
    while(in != nullptr)
    {
        Move *m = new Move();
        m->from = in->point; m->to = tmp->point;
        path->push_front(Node(nullptr, path->begin()->player_pos, nullptr, m, 0, x0));
        tmp = in;
        in = in->parent;
        //delete tmp;
    }
    return result;
    /*int size = result.size();
    Node *res = new Node[size];
    auto it = result.begin();
    for(int i = 0; i < size; i++)
    {
        res[i] = *it;
        result.erase(it);
        it++;
    }
    path = res;
    return size;*/
}