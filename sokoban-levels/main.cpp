#include "TrainingData.h"

int main(int argc, char **argv)
{
    TrainingData data("/home/atopion/atopi/Codes/sokoban/sokoban-levels/backup/x2");
    data.solveCore(5840, data.levelFolder);
    data.gen();
}