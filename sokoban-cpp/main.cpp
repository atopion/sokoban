#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include "Map.h"
#include "TranspositionTable.h"
#include "Node.h"
#include "Execution.h"

int main(int argc, char **argv) {

    std::ifstream file;
    std::stringstream stream;

#ifdef __unix__
    std::string path = "level/";
#elif defined(_WIN32)
    std::string path = "C:\\Users\\atopi\\Codes\\c++\\SokobanSolver_ver2\\level\\";
#endif

    std::string line, level;
    if(argc < 2)
    {
        std::string level_name = "level3.txt";
        file.open(path + level_name);
        std::cout << "Using level1" << std::endl;
    }
    else
    {
        std::string level_name = std::string(argv[1]);
        file.open(level_name);
        std::cout << "Using " << level_name << std::endl;
    }

    while(std::getline(file, line))
        stream << line << std::endl;
    file.close();
    level = stream.str();

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    Map game_map = Map(level);
    game_map.mapProduction();
    game_map.printMap();

    auto x0 = std::list<Node *>();
    Node root = Node(game_map.getBoxArray(), game_map.getPlayer(), nullptr, nullptr, 10000, x0, 0);

    Execution exec = Execution(game_map);
    Node *tmp;
    std::chrono::high_resolution_clock::time_point end   = std::chrono::high_resolution_clock::now();
    std::cout << "Preparation time: " << std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    Node *solution = exec.execute(&root);
    end   = std::chrono::high_resolution_clock::now();
    std::cout << "Execution time:   " << std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count() << " ms" << std::endl;

    if(solution == nullptr)
        std::cout << "No solution found" << std::endl;
    else
    {
        int i = 0;
        std::string result;
        while(solution->move != nullptr)
        {
            i++;
            result.insert(0, Move::str(*(solution->move)) + ", ");
            tmp = solution->farther;
            //delete solution;
            solution = tmp;
        }
        result.erase(result.size()-2, 1).erase(result.size()-1, 1).erase(result.size()-2, 1);
        result = "[" + result + "]";

        std::cout << "Solution found: " << result << std::endl;
    }
}