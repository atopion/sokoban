//
// Created by atopi on 28.01.2019.
//

#include <sstream>
#include "RunningMap.h"

RunningMap::RunningMap(std::string level)
{
    width = 0;
    height = 0;
    size = level.size();
    for(char c : level) if(c == '\n') size--;
    map = new int[size]; //(int *) malloc(str.size());
    clearedMap = new int[size];
    player = 0;

    int tmp = 0, j = 0;
    for(char c : level)
    {
        tmp++;
        switch (c)
        {
            case '@': // Player
                map[j] = 1;
                player = j;
                clearedMap[j] = 5;
                j++;
                break;
            case '$': // Box
                map[j] = 2;
                clearedMap[j] = 5;
                j++;
                break;
            case '.': // Target
                map[j] = 3;
                clearedMap[j] = 3;
                j++;
                break;
            case '#': // Wall
                map[j] = 4;
                clearedMap[j] = 4;
                j++;
                break;
            case ' ': // Empty
                map[j] = 5;
                clearedMap[j] = 5;
                j++;
                break;
            case '*': // Box placed on goal
                map[j] = 6;
                clearedMap[j] = 3;
                j++;
                break;
            case '+': // Agent placed on goal
                map[j] = 7;
                player = j;
                clearedMap[j] = 3;
                j++;
                break;

            case '\n': // New line
                if(tmp > width)
                    width = tmp;
                tmp = 0;
                break;

            default:
                break;
        }
    }
    width = width -1;
    size = j;
    height = size / width;
}

std::string RunningMap::getMap()
{
    std::stringstream ss;
    for(int i = 0; i < size; i++)
    {
        switch (map[i])
        {
            case 1:
                ss << "@";
                break;
            case 2:
                ss << "$";
                break;
            case 3:
                ss << ".";
                break;
            case 4:
                ss << "#";
                break;
            case 5:
                ss << " ";
                break;
            case 6:
                ss << "*";
                break;
            case 7:
                ss << "+";
                break;

            default:
                break;
        }

        if((i+1) % width == 0)
        {
            ss << std::endl;
        }
    }

    return ss.str();
}

std::string RunningMap::getCodes()
{
    std::stringstream ss;
    for(int i = 0; i < size; i++)
    {
        ss << map[i];
        if((i+1) % width == 0) ss << "|";
    }
    return ss.str();
}

RunningMap::~RunningMap()
{
    delete[] map;
    delete[] clearedMap;
}

void RunningMap::move(char c)
{
    if(c == 'u')
    {
        if(player - width < 0) return;
        if(map[player - width] == 2)
        {
            if(player - 2*width < 0) return;
            map[player - 2*width] = 2;
        }
        map[player - width] = 1;
        map[player] = clearedMap[player];
        player -= width;
    }
    else if(c == 'd')
    {
        if(player + width > size) return;
        if(map[player + width] == 2)
        {
            if(player + 2*width > size) return;
            map[player + 2*width] = 2;
        }
        map[player + width] = 1;
        map[player] = clearedMap[player];
        player += width;
    }
    else if(c == 'l')
    {
        if(player - 1 < 0) return;
        if(map[player - 1] == 2)
        {
            if(player - 2 < 0) return;
            map[player - 2] = 2;
        }
        map[player - 1] = 1;
        map[player] = clearedMap[player];
        player -= 1;
    }
    else if(c == 'r')
    {
        if(player + 1 > size) return;
        if(map[player + 1] == 2)
        {
            if(player + 2 < 0) return;
            map[player + 2] = 2;
        }
        map[player + 1] = 1;
        map[player] = clearedMap[player];
        player += 1;
    }
}