#include "TrainingData.h"

int main(int argc, char **argv)
{
    TrainingData data("/home/atopion/atopi/Codes/sokoban/sokoban-levels/solved-levels/");
    data.solveThread();
    data.gen();
    data.centerPlayer();
    data.split(500);
}