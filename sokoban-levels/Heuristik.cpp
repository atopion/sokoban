#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "Heuristik.h"

#define NUM_THREADS 8

std::string folder;

std::vector<int> indices;
pthread_mutex_t lock_indices;
std::vector<GraphPoint> results;
pthread_mutex_t lock_results;

int heuristik(std::string levelFolder, std::string outfile)
{
    indices = std::vector<int>(0);
    for(int i = 2001; i < 2401; i++)
        indices.emplace_back(i);
    folder = levelFolder;

    pthread_t thr[NUM_THREADS];
    pthread_mutex_init(&lock_indices, NULL);
    pthread_mutex_init(&lock_results, NULL);

    int rc;
    for(int i = 0; i < NUM_THREADS; i++)
    {
        if (rc = pthread_create(&thr[i], NULL, _solveThread, NULL)) {
            std::cerr << "Error: pthread_create, rc: " << rc << std::endl;;
        }
    }

    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        std::cout << "Failed to get time" << std::endl;
        for (int i = 0; i < NUM_THREADS; ++i) {
            pthread_join(thr[i], NULL);
        }
    }
    pthread_join(thr[0], NULL);
    ts.tv_sec += 600;

    for (int i = 1; i < NUM_THREADS; ++i) {
        pthread_timedjoin_np(thr[i], NULL, &ts);
    }

    std::cout << "Processing finished -> Writing" << std::endl;

    std::ofstream ofile;
    ofile.open(outfile, std::ofstream::out | std::ofstream::trunc);
    ofile << "Results with using perceptron" << std::endl;
    for(auto it = results.begin(); it != results.end(); it++)
    {
        ofile << it->length << ";" << it->time << std::endl;
    }
    ofile.close();

    std::cout << "Finished." << std::endl;

    return 0;
}

void *_solveThread(void *arg)
{
    while(!indices.empty())
    {
        pthread_mutex_lock(&lock_indices);
        int index = indices.front();
        indices.erase(indices.begin());  
        pthread_mutex_unlock(&lock_indices);

        solveCore(index, folder);
    }
}

void solveCore(int i, std::string folder)
{
    std::string pad = (i < 10 ? "000" : (i < 100 ? "00" : (i < 1000 ? "0" : "")));

    std::ifstream ifile;
    std::string str, line;
    std::stringstream stream;

    ifile.open(folder + "level" + pad + std::to_string(i) + ".txt");
    if(!ifile.is_open()) return;
    if(ifile.peek() == std::ifstream::traits_type::eof()) return;

    while ( getline (ifile, line) )
    {
        if(line[0] == 'u' || line[0] == 'd' || line[0] == 'l' || line[0] == 'r')
        {
            continue;
        }

        if(!line.empty())
            stream << line << '\n';
    }        

    ifile.close();
    str = stream.str();

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    int length = runner(str);
    std::chrono::high_resolution_clock::time_point end   = std::chrono::high_resolution_clock::now();
    int time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();

    GraphPoint p;
    p.length = length;
    p.time = time;

    pthread_mutex_lock(&lock_results);
    results.push_back(p);
    pthread_mutex_unlock(&lock_results);

    if(length < 0)
    {  
        std::cout << "Finished file " << folder + "level" + pad + std::to_string(i) + ".txt  -  No solution." << std::endl;
    }
    else
    {
        std::cout << "Finished file " << folder + "level" + pad + std::to_string(i) + ".txt" << std::endl;
    }
}

int runner(std::string level)
{
    std::string result;
    Map game_map = Map(level);
    game_map.mapProduction();

    auto x0 = std::list<Node *>();
    Node *root = new Node(game_map.getBoxArray(), game_map.getPlayer(), 10000);

    Execution exec = Execution(&game_map);
    Node *tmp;

    Node *solution = exec.execute(root);

    if(solution == nullptr)
        return -1;
    else {
        int i = 0;
        while (solution->farther != nullptr) {
            i++;
            int x = solution->move.to - solution->move.from;

            if (x == 1) result.insert(0, "r");
            else if (x == -1) result.insert(0, "l");
            else if (x > 0) result.insert(0, "d");
            else if (x < 0) result.insert(0, "u");

            tmp = solution->farther;
            solution = tmp;
        }
        delete root;
    }
    return result.size();
}