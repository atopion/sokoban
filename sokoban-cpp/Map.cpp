//
// Created by atopi on 26.11.2018.
//

#include <sstream>
#include <iomanip>
#include <vector>
#include "Map.h"

Map::Map(std::string &str)
{
    width = 0;
    height = 0;
    size = str.size();
    for(char c : str) if(c == '\n') size--;
    this->mapstring = str;
    this->map = new int[size];
    this->clearedMap = new int[size];
    this->player = 0;
}

Map::~Map()
{
    if(this->map != nullptr)
    {
        delete[] this->map;
        this->map = nullptr;
    }
    if(this->clearedMap != nullptr)
    {
        delete[] this->clearedMap;
        this->clearedMap = nullptr;
    }
    if(this->boxArray != nullptr)
    {
        delete[] this->boxArray;
        this->boxArray = nullptr;
    }
    if(this->targetArray != nullptr)
    {
        delete[] this->targetArray;
        this->targetArray = nullptr;
    }
}

void Map::mapProduction()
{
    int tmp = 0, j = 0, max = 0;
    std::vector<int> boxes, targets;
    
    size_t ind = 0;
    while (true) {
        ind = mapstring.find("\r\n", ind);
        if (ind == std::string::npos) break;

        mapstring.replace(ind, 2, "\n");

        ind += 2;
    }


    for(char c : mapstring)
    {
        tmp++;
        switch (c)
        {
            case '@': // Player
                map[j] = 1;
                clearedMap[j] = 5;
                player = j;
                j++;
                break;
            case '$': // Box
                map[j] = 2;
                clearedMap[j] = 5;
                boxes.push_back(j);
                j++;
                break;
            case '.': // Target
                map[j] = 3;
                clearedMap[j] = 3;
                targets.push_back(j);
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
                targets.push_back(j);
                boxes.push_back(j);
                j++;
                break;
            case '+': // Agent placed on goal
                map[j] = 7;
                clearedMap[j] = 3;
                targets.push_back(j);
                player = j;
                j++;
                break;

            case '\n': // New line
                if(tmp > max)
                    max = tmp;
                tmp = 0;
                break;

            default:
                break;
        }
    }
    this->width = max -1;
    //std::cout << "Width: " << width << std::endl;
    this->size = j;
    this->height = size / this->width;

    this->boxArray = new int[boxes.size()];
    this->boxCount = boxes.size();
    for(int i = 0; i < boxes.size(); i++) this->boxArray[i] = boxes[i];
    this->targetArray = new int[targets.size()];
    this->targetCount = targets.size();
    for(int i = 0; i < targets.size(); i++) this->targetArray[i] = targets[i];
}

void Map::printMap()
{
    std::stringstream ss;
    for(int i = 0; i < size; i++)
    {
        switch (map[i])
        {
            case 1:
                std::cout << "@";
                break;
            case 2:
                std::cout << "$";
                break;
            case 3:
                std::cout << ".";
                break;
            case 4:
                std::cout << "#";
                break;
            case 5:
                std::cout << " ";
                break;
            case 6:
                std::cout << "*";
                break;
            case 7:
                std::cout << "+";
                break;

            default:
                break;
        }
        ss << ' ' << std::setfill('0') << std::setw(2) << i;

        if((i+1) % width == 0)
        {
            std::cout << "   " << ss.str() << std::endl;
            ss.str(std::string());
        }
    }
}

void Map::calculateReachableArea(int reachable[], int box_array[], int pos)
{
    if((pos < 0) || (pos > size) || reachable[pos] == 1 || map[pos] == 4)
        return;
    for(int i = 0; i < boxCount; i++)
        if(box_array[i] == pos)
            return;

    reachable[pos] = 1;
    Map::calculateReachableArea(reachable, box_array, pos + 1);
    Map::calculateReachableArea(reachable, box_array, pos - 1);
    Map::calculateReachableArea(reachable, box_array, pos + width);
    Map::calculateReachableArea(reachable, box_array, pos - width);
}

bool Map::canReach(int *visited, int box_array[], int pos, int index)
{
    for(int i = 0; i < size; i++)
        visited[i] = 0;
    Map::calculateReachableArea(visited, box_array, pos);
    return visited[index] == 1;
}

int Map::normalizedPlayerPosition(int *visited, int *box_array, int pos)
{
    for(int i = 0; i < size; i++)
        if(visited[i] == 1)
            return i;
    return 0;
}

int Map::getWidth() { return width; }
int Map::getHeight() { return height; }
int Map::getSize() { return size; }
int Map::getBoxCount() { return boxCount; }
int Map::getTargetsCount() { return targetCount; }
int Map::getPlayer() { return player; }

int* Map::getClearedMap() { return clearedMap; }
int* Map::getBoxArray() { return boxArray; }
int* Map::getTargetsArray() { return targetArray; }

int* Map::getFullMap(int *boxArray, int pos)
{
    int *new_map = new int[this->size];
    for(int i = 0; i < this->size; i++)
    {
        for(int j = 0; j < this->boxCount; j++)
        {
            if(i == boxArray[j])
            {
                new_map[i] = 2;
                goto loopEnd;
            }
        }
        new_map[i] = this->clearedMap[i];
        loopEnd:
        int a = 0;
    }

    new_map[pos] = 1;
    return new_map;
}
