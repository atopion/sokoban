#include "TrainingData.h"

int main(int argc, char **argv)
{
    TrainingData data("/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run3/levels");
    data.combine("/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run1/run_train.txt", "/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run2/run_train.txt", "/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run3/run_train.txt", "/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run_train.txt");
    data.combine("/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run1/run_test.txt", "/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run2/run_test.txt", "/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run3/run_test.txt", "/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run_test.txt");
    data.combine("/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run1/run_centered_train.txt", "/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run2/run_centered_train.txt", "/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run3/run_centered_train.txt", "/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run_centered_train.txt");
    data.combine("/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run1/run_centered_test.txt", "/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run2/run_centered_test.txt", "/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run3/run_centered_test.txt", "/home/atopion/atopi/Codes/sokoban/sokoban-levels/result/run_centered_test.txt");

}