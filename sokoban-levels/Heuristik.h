#include "../sokoban-cpp/Map.h"
#include "../sokoban-cpp/Node.h"
#include "../sokoban-cpp/Execution.h"

class GraphPoint
{
public:
    int length, time;
};

int heuristik(std::string levelFolder, std::string outfile);
void *_solveThread(void *arg);
void solveCore(int i, std::string folder);
int runner(std::string level);
