//
// Created by atopi on 27.01.2019.
//

#ifndef SOKOBANSOLVER_TRAININGDATA_H
#define SOKOBANSOLVER_TRAININGDATA_H

#include <string>

#include "../sokoban-cpp/Map.h"
#include "../sokoban-cpp/Node.h"
#include "../sokoban-cpp/Execution.h"
#include "RunningMap.h"

class TrainingData {

public:
    std::string levelFolder;

    TrainingData(std::string levelFolder);

    // Solve single thread
    void solve();

    // Solve multithread
    void solveThread();
    static void *_solveThread(void *arg);
    static void solveCore(int index, std::string folder);

    void clear();
    static std::string generate(std::string level);
    std::string closingBorder(std::string level);
    void split(int tests);
    static inline void rtrim(std::string &s);

    std::string generateTrainingData(std::string &level, std::string solution, int step);
    void gen();
    void centerPlayer();
    void combine(std::string level1, std::string level2, std::string level3, std::string out);
};


#endif //SOKOBANSOLVER_TRAININGDATA_H
