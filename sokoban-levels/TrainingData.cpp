//
// Created by atopi on 27.01.2019.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <pthread.h>
#include "TrainingData.h"

#define NUM_THREADS 8

TrainingData::TrainingData(std::string levelFolder)
{
    TrainingData::levelFolder = levelFolder;
    if(this->levelFolder.back() != '/')
        this->levelFolder += "/";


}

inline void TrainingData::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

void TrainingData::split(int tests)
{
    std::ifstream ifile;
    std::ofstream ofile_test, ofile_train;
    std::string str, line;

    int count = 0;

    ifile.open(this->levelFolder + "run-medium.txt", std::istream::in);
    if(!ifile.is_open()) return;

    ofile_test.open(this->levelFolder + "run-medium-test.txt", std::ostream::out);
    ofile_train.open(this->levelFolder + "run-medium-train.txt", std::ostream::out);

    while( getline(ifile, line) )
    {
        if(line[0] == '-')
        {
            count++;
        }

        if(count < tests)
        {
            ofile_test << line << "\n";
        }
        else
        {
            ofile_train << line << "\n";
        }
    }

    ifile.close();
    ofile_test.close();
    ofile_train.close();

    count = 0;

    ifile.open(this->levelFolder + "run-medium-centered.txt", std::istream::in);
    if(!ifile.is_open()) return;

    ofile_test.open(this->levelFolder + "run-medium-centered-test.txt", std::ostream::out);
    ofile_train.open(this->levelFolder + "run-medium-centered-train.txt", std::ostream::out);

    while( getline(ifile, line) )
    {
        if(line[0] == '-')
        {
            count++;
        }

        if(count < tests)
        {
            ofile_test << line << "\n";
        }
        else
        {
            ofile_train << line << "\n";
        }
    }

    ifile.close();
    ofile_test.close();
    ofile_train.close();
}

void TrainingData::solve()
{
    for(int i = 1; i < 6001; i++)
    {
        solveCore(i, this->levelFolder);
    }
    std::cout << "Finished" << std::endl;
}

std::vector<int> inits;
std::string levelF;
pthread_mutex_t lock_x;

void TrainingData::solveThread()
{
    inits = std::vector<int>(0);
    for(int i = 3600; i < 6001; i++)
        inits.emplace_back(i);
    levelF = this->levelFolder;

    pthread_t thr[NUM_THREADS];
    pthread_mutex_init(&lock_x, NULL);

    int rc;
    for(int i = 0; i < NUM_THREADS; i++)
    {
        if (rc = pthread_create(&thr[i], NULL, _solveThread, NULL)) {
            std::cerr << "Error: pthread_create, rc: " << rc << std::endl;;
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(thr[i], NULL);
    }

    std::cout << "\rFinished." << std::endl;
}

void *TrainingData::_solveThread(void *arg)
{
    while(!inits.empty())
    {
        pthread_mutex_lock(&lock_x);
        int index = inits.front();
        inits.erase(inits.begin());  
        pthread_mutex_unlock(&lock_x);

        solveCore(index, levelF);
    }
}

void TrainingData::solveCore(int i, std::string folder)
{
    std::string pad = (i < 10 ? "000" : (i < 100 ? "00" : (i < 1000 ? "0" : "")));

    std::ifstream ifile;
    std::ofstream ofile;
    std::string str, solv, line;
    std::stringstream stream;

    ifile.open(folder + "level" + pad + std::to_string(i) + ".txt");
    if(!ifile.is_open()) return;
    if(ifile.peek() == std::ifstream::traits_type::eof()) return;

    while ( getline (ifile, line) )
    {
        if(line[0] == 'u' || line[0] == 'd' || line[0] == 'l' || line[0] == 'r')
        {
            ifile.close();
            std::cout << "Skipping file " << folder + "level" + pad + std::to_string(i) + ".txt" << std::endl;
            return;
        }

        if(!line.empty())
            stream << line << '\n';
    }        

    ifile.close();
    str = stream.str();

    solv = generate(str);

    if(solv.find("No") != std::string::npos)
    {
        solv = "";
        ofile.open(folder + "level" + pad + std::to_string(i) + "-empty.txt", std::ofstream::out | std::ofstream::trunc);
        if(!ofile.is_open()) return;
        ofile << str;
        ofile.close();
        

        std::cout << "Finished file " << folder + "level" + pad + std::to_string(i) + ".txt  -  No solution." << std::endl;
    }
    else
    {
        ofile.open(folder + "level" + pad + std::to_string(i) + ".txt", std::ofstream::out | std::ofstream::trunc);
        if(!ofile.is_open()) return;
        ofile << str;
        ofile << solv << std::endl;
        ofile.close();


        std::cout << "Finished file " << folder + "level" + pad + std::to_string(i) + ".txt" << std::endl;
    }
}

void TrainingData::clear()
{
    for(int i = 1; i < 277; i++)
    {
        std::string pad = (i < 10 ? "000" : (i < 100 ? "00" : (i < 1000 ? "0" : "")));
        std::ifstream ifile;
        std::ofstream ofile;
        std::string str, line;
        std::stringstream stream;

        ifile.open(this->levelFolder + "level" + pad + std::to_string(i) + ".txt");
        if(!ifile.is_open()) continue;
        while ( getline (ifile, line) )
        {
            char c = line[0];
            if(!(c == 'l' || c == 'r' || c == 'u' || c == 'd'))
                stream << line << '\n';
        }
        ifile.close();
        str = stream.str();

        ofile.open(this->levelFolder + "level" + pad + std::to_string(i) + ".txt", std::ofstream::out | std::ofstream::trunc);
        if(!ofile.is_open()) continue;
        ofile << str;
        ofile.close();
    }
}

std::string TrainingData::generate(std::string level)
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
        result += "No solution found";
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
    return result;
}

std::string TrainingData::closingBorder(std::string level)
{
    unsigned int width = 0;
    unsigned int count = 0;
    std::string line;
    for(auto it = level.begin(); it != level.end(); it++)
    {
        if(*it == '\n') {
            if (count > width)
                width = count;
            count = 0;
        }
        else
            count++;
    }

    bool start = true;
    count = 0;
    for(auto it = level.begin(); it != level.end(); it++)
    {
        if(*it == ' ' && start)
        {
            *it = '#';
            count++;
        }
        else if(*it == '\n')
        {
            std::cout << "C: " << count << " W: " << width << std::endl;
            if(count < width)
                for(int a = 0; a < width - count; a++)
                    line += "#";
            count = 0;
            start = true;
        }
        else{
            count++;
            start = false;
        }
        line += *it;
    }

    return line;
}

void TrainingData::gen()
{
    std::ifstream ifile;
    std::ofstream ofile;
    std::string line, str, solution;
    std::stringstream stream;
    int level_count = 0;
    ofile.open(this->levelFolder + "run-medium.txt", std::ofstream::out | std::ofstream::app);

    for(int i = 2001; i < 6001; i++)
    {
        std::string pad = (i < 10 ? "000" : (i < 100 ? "00" : (i < 1000 ? "0" : "")));

        solution = "";
        stream.str("");
        ifile.open(this->levelFolder + "level" + pad + std::to_string(i) + ".txt");
        if(ifile.peek() == std::ifstream::traits_type::eof()) continue;
        int count = 0;
        int width = 0;
        while ( getline (ifile, line) )
        {
            if(line[0] == 'l' || line[0] == 'r' || line[0] == 'u' || line[0] == 'd')
                solution = line;
            else
            {
                if(line.length() < 10)
                    line.append(10 - line.length(), '#');
                if(line.length() > width)
                    width = line.size();
                count++;
                stream << line << '\n';
            }
        }
        ifile.close();
        if(solution.empty())
        {
            std::cout << "\tNo solution for " << i << std::endl;
            continue;
        }
        if(count < 10)
        {
            for(; count < 10; count++)
                stream << "##########\n";
        }
        if(count > 10 || width > 10)
        {
            std::cout << "\tFile " << i << " is to big. Skipping." << std::endl;
            continue;
        }
        str = stream.str();
        level_count++;


        for(unsigned int a = 0; a < solution.size(); a++)
        {
            ofile << std::setw(3) << std::setfill('0') << a;
            ofile << solution[a];
            ofile << '|';
            ofile << generateTrainingData(str, solution, a-1);
            ofile << std::endl;
        }
        ofile << "-------------------------------------------------------" << std::endl;
        std::cout << "Finished file: " << i << std::endl;
    }
    ofile.close();
    std::cout << std::endl << std::endl << "Processed " << level_count << " levels" << std::endl;
}

std::string TrainingData::generateTrainingData(std::string &level, const std::string solution, int step)
{
    RunningMap rmap = RunningMap(level);

    if(step > 0 && step > solution.length() -1)
        return "";

    std::string result;

    for(int i = 0; i <= step; i++)
        rmap.move(solution[i]);

    result = rmap.getCodes();

    return result;
}

void TrainingData::centerPlayer()
{
    std::ifstream ifile;
    std::ofstream ofile;
    std::string line, str, solution;
    std::stringstream stream;
    ifile.open(this->levelFolder + "run-medium.txt");
    ofile.open(this->levelFolder + "run-medium-centered.txt", std::ofstream::out | std::ofstream::app);

    if(ifile.peek() == std::ifstream::traits_type::eof())
    {
        std::cerr << "Error eof" << std::endl;
        return;
    }

    while( getline(ifile, line) )
    {
        if(line.empty())
            continue;

        if(line[0] == '-')
        {
            ofile << "-------------------------------------------------------" << std::endl;
            continue;
        }

        std::vector<std::vector<char>> mat;
        std::vector<char> tmp;
        bool init = true;
        std::string numb;
        int number = 0;
        char result = ' ';
        int a = 0, b = 0, player_x = 0, player_y = 0;


        for(char c : line)
        {
            if(init) {
                if (std::isdigit(c))
                    numb += c;
                else if(c == '|')
                    init = false;
                else
                    result = c;
            }
            else {
                if(c == '|')
                {
                    mat.emplace_back(std::vector<char>(tmp));
                    tmp.clear();
                    a = 0;
                    b++;
                }
                else if(c == '1')
                {
                    player_x = a;
                    player_y = b;
                    tmp.push_back(c);
                }
                else {
                    tmp.push_back(c);
                    a++;
                }

            }
        }

        number = atoi(numb.c_str());
        int alpha;
        if((alpha = player_y - 5) > 0)
            std::rotate(mat.begin(), mat.begin() + alpha, mat.end());
        else
            std::rotate(mat.begin(), mat.end() + alpha, mat.end());

        for(auto it = mat.begin(); it != mat.end(); it++)
        {
            if((alpha = player_x - 5) > 0)
                std::rotate(it->begin(), it->begin() + alpha, it->end());
            else
                std::rotate(it->begin(), it->end() + alpha, it->end());
        }

        ofile << std::setw(3) << std::setfill('0') << number;
        ofile << result;
        ofile << '|';

        for(std::vector<char> &v : mat)
        {
            for(char c : v)
            {
                ofile << c;
            }
            ofile << '|';
        }
        ofile << std::endl;
    }

    ifile.close();
    ofile.close();
}

void TrainingData::combine(std::string level1, std::string level2, std::string level3, std::string out)
{
    std::ifstream ifile1, ifile2, ifile3;
    std::ofstream ofile;
    std::string line, str, solution;
    std::stringstream stream;
    ifile1.open(level1);
    ifile2.open(level2);
    ifile3.open(level3);
    ofile.open(out, std::ofstream::out | std::ofstream::app);

    if(ifile1.peek() == std::ifstream::traits_type::eof())
    {
        std::cerr << "Error eof 1" << std::endl;
        return;
    }

    if(ifile2.peek() == std::ifstream::traits_type::eof())
    {
        std::cerr << "Error eof 2" << std::endl;
        return;
    }

    if(ifile3.peek() == std::ifstream::traits_type::eof())
    {
        std::cerr << "Error eof 2" << std::endl;
        return;
    }

    while( getline(ifile1, line) )
    {
        if(line.empty())
            continue;

        if(line[0] == '-')
        {
            ofile << "-------------------------------------------------------" << std::endl;
            continue;
        }

        ofile << line << std::endl;
    }
    ifile1.close();

    while( getline(ifile2, line) )
    {
        if(line.empty())
            continue;

        if(line[0] == '-')
        {
            ofile << "-------------------------------------------------------" << std::endl;
            continue;
        }

        ofile << line << std::endl;
    }
    ifile2.close();

    while( getline(ifile3, line) )
    {
        if(line.empty())
            continue;

        if(line[0] == '-')
        {
            ofile << "-------------------------------------------------------" << std::endl;
            continue;
        }

        ofile << line << std::endl;
    }
    ifile3.close();

    ofile.close();
}