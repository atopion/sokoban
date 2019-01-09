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
#include "PlayerPath_old.h"
#include "PlayerPath.h"

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
        std::string level_name = "level1.txt";
        file.open(path + level_name);
        std::cout << "Using lavel1" << std::endl;
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
    Node root = Node(game_map.getBoxArray(), game_map.getPlayer(), nullptr, nullptr, 10000, x0);

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
        PlayerPath ppp = PlayerPath(game_map.getWidth(), game_map.getBoxCount(), game_map.getSize(), game_map.getClearedMap());
        ppp.calcPlayerPath(solution, game_map.getPlayer());

        std::string result;
        while(solution != nullptr)
        {
            result.insert(0, Move::str(*(solution->move)) + ", ");
            tmp = solution->farther;
            //delete solution;
            solution = tmp;
        }
        result.erase(result.size()-2, 1).erase(result.size()-1, 1).erase(result.size()-2, 1);
        result = "[" + result + "]";

        std::cout << "Solution found: " << result << std::endl;

        /*int count = 0;
        Node *x1 = solution;
        while(solution->farther != nullptr)
        {
            count++;
            solution = solution->farther;
        }
        //std::cout << "COUNT: " << count << std::endl;
        solution = x1;

        int* box_arrays[count];
        Move* moves[count];
        for(int i = 0; i < count; i++)
        {
            box_arrays[i] = new int[game_map.getBoxCount()];
            moves[i] = new Move();
        }

        //std::cout << "ALPHA: " << Move::str(*solution->move) << std::endl;
        int *n_box_array = new int[game_map.getBoxCount()];
        std::list<Node> result = std::list<Node>();
        int i = 0;
        while(solution->farther != nullptr)
        {
            Move *m = new Move();
            int tmp_x1 = solution->move->from;
            m->from = tmp_x1;
            tmp_x1 = solution->move->to;
            m->to = tmp_x1;

            moves[i]->from = solution->move->from;
            moves[i]->to   = solution->move->to;

            int *boxes = new int[game_map.getBoxCount()];
            memcpy(box_arrays[i], solution->box_array, game_map.getBoxCount() * sizeof(int));
            //std::cout << " A " << Move::str(*m);

            for(const Node& p : result)
                std::cout << " A " << Move::str(*p.move) << " ";
            std::cout << std::endl;

            std::cout << "New 2" << std::endl;
            for(int a = 0; a < game_map.getBoxCount(); a++)
                std::cout << " " << a << " " << solution->box_array[a];
            std::cout << std::endl;

            std::cout << "BOXES: " << boxes << "  ";
            for(int a = 0; a < game_map.getBoxCount(); a++)
                std::cout << solution->box_array[a] << " ";
            std::cout << std::endl << "MOVES: " << moves[i] << "   " << Move::str(*moves[i]) << std::endl;

            result.push_front(Node(box_arrays[i], solution->player_pos, nullptr, moves[i], 0, std::list<Node *>()));
            tmp = solution->farther;
            delete solution;
            solution = tmp;
            if(solution->farther->farther == nullptr)
            {
                solution = solution->farther;
            }
            else {
                tmp = solution->farther;
                //delete solution;
                solution = tmp;
            }
            i++;
        }
        memcpy(n_box_array, solution->box_array, game_map.getBoxCount() * sizeof(int));

        //std::cout << std::endl << std::endl;

        for(auto it = result.begin(); it != result.end(); it++)
        {
            std::cout << "New1" << std::endl;
            std::cout << "BOXES: " << it->box_array << std::endl;
            std::cout << "MOVES: " << it->move << std::endl;
            for(int a = 0; a < game_map.getBoxCount(); a++)
                std::cout << "  " << a << " : " << it->box_array[a];
            std::cout << std::endl;
        }

        for(const Node& p : result)
            std::cout << " B " << Move::str(*p.move) << " ";
        std::cout << std::endl;
        PlayerPath_old ppath = PlayerPath_old(game_map.getPlayer(), game_map.getWidth(), game_map.getBoxCount(), game_map.getSize(), game_map.getClearedMap());

        //int c = 0;
        ppath.calculatePlayerPath(&result, 0);
        //c = 1;
        for(const Node& p : result)
            if(p.move != nullptr)
                std::cout << " C " << Move::str(*p.move) << " ";
        std::cout << std::endl;
        //c = 2;
        ppath.calculateEdgesPath(&result, 0);
        for(Node &n : result)
        {
            std::cout << "M: " << n.move << std::endl;
            if(n.move != nullptr)
                std::cout << "Node1: " << Move::str(*n.move) << std::endl;
        }
        std::cout << std::endl;

        ppath.calculateInitialPath(&result, n_box_array);
        std::cout << "[";
        for(Node &n : result)
            if(n.move != nullptr)
                std::cout << " " << Move::str(*n.move);
        std::cout << " ]" << std::endl;

        delete solution;*/
    }
}