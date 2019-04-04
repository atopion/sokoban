cmake_minimum_required(VERSION 3.12)
project(SokobanSolver)

SET(GCC_COVERAGE_COMPILE_FLAGS "-static-libgcc -static-libstdc++")
SET(CMAKE_EXE_LINKER_FLAGS  ${GCC_COVERAGE_COMPILE_FLAGS})
set(CMAKE_CXX_STANDARD 14)

add_executable(SokobanSolver main.cpp Map.cpp Map.h TranspositionTable.cpp TranspositionTable.h DeadlockDetection.cpp DeadlockDetection.h Metrics.cpp Metrics.h AssignmentAlgorithms.cpp AssignmentAlgorithms.h Execution.cpp Execution.h Node.cpp Node.h hungarian/Hungarian.cpp hungarian/Hungarian.h AStar.cpp AStar.h BoundAlgorithm.cpp BoundAlgorithm.h perceptron/Perceptron.cpp perceptron/Perceptron.h)
#add_executable(SokobanSolver main.cpp Map.cpp TranspositionTable.cpp DeadlockDetection.cpp Metrics.cpp AssignmentAlgorithms.cpp Execution.cpp Node.cpp)