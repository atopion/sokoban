//
// Created by atopi on 28.01.2019.
//

#ifndef SOKOBANSOLVER_RUNNINGMAP_H
#define SOKOBANSOLVER_RUNNINGMAP_H


#include <string>

class RunningMap {
private:
    int * map;
    int * clearedMap;
    int width;
    int height;
    int player;
    int size;
public:
    explicit RunningMap(std::string level);
    ~RunningMap();

    std::string getMap();
    std::string getCodes();
    void move(char c);
};


#endif //SOKOBANSOLVER_RUNNINGMAP_H
