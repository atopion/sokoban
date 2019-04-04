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
    std::string path = "C:\\Users\\atopi\\Codes\\sokoban\\sokoban-cpp\\perceptrons\\levels\\";
#endif

    std::string line, level, level_name, execute_command;
    if(argc < 2)
    {
        level_name = "level2.txt";
        file.open(path + level_name);
        std::cout << "Using " << path + level_name << std::endl;
    }
    else if(argc < 3)
    {
        level_name = std::string(argv[1]);
        file.open(level_name);
        std::cout << "Using " << level_name << std::endl;
    }
    else
    {
        level_name = std::string(argv[1]);
        file.open(level_name);
        std::cout << "Using " << level_name << std::endl;
        execute_command = std::string(argv[2]);
    }

    while(std::getline(file, line))
        stream << line << std::endl;
    file.close();
    level = stream.str();

    if(level.empty())
    {
        std::cout << "Level string empty. File not found. Exit" << std::endl;
    }

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    Map game_map = Map(level);
    game_map.mapProduction();
    game_map.printMap();

    Node *root = new Node(game_map.getBoxArray(), game_map.getPlayer(), 10000);

    Execution exec = Execution(&game_map);
    Node *tmp;
    std::chrono::high_resolution_clock::time_point end   = std::chrono::high_resolution_clock::now();
    std::cout << "Preparation time: " << std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    Node *solution = exec.execute(root);
    end   = std::chrono::high_resolution_clock::now();
    std::cout << "Execution time:   " << std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count() << " ms" << std::endl;

    if(solution == nullptr)
        std::cout << "No solution found" << std::endl;
    else
    {
        int i = 0;
        std::string result, r1;
        while(solution->farther != nullptr)
        {
            i++;
            int x = solution->move.to - solution->move.from;
            Move m = solution->move;
            result.insert(0, Move::str(m) + ", ");

            if(x == 1) r1.insert(0, "r");
            else if(x == -1) r1.insert(0, "l");
            else if(x > 0) r1.insert(0, "d");
            else if(x < 0) r1.insert(0, "u");

            tmp = solution->farther;
            solution = tmp;
        }
        delete root;
        result.erase(result.size()-2, 2);
        result = "[" + result + "]";

        std::cout << "Solution found: " << result << std::endl;
        std::cout << "Path: " << r1 << std::endl;
        std::cout << std::endl;
        std::cout << "result path length: " << i << std::endl;
        std::cout << "number of examined nodes: " << exec.getPathCount() << std::endl;
        std::cout << "number of mistakes: " << exec.getSecondTryCount() << std::endl;
        std::cout << "C: " << exec.getCCount() << std::endl;

        if(!execute_command.empty())
        {
            execute_command = execute_command + " " + r1;
            system(execute_command.c_str());
        }
    }
}
