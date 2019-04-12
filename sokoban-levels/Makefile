CXX=g++

SOLVER=../sokoban-cpp
OBJ=obj

CXXFLAGS=--std=c++14 -g -pthread
TFLITELIB=-L./lib/
TFLITEINC=-I/home/atopion/atopi/Codes/docker/tensorflow -I/home/atopion/atopi/Codes/docker/tensorflow/tensorflow/lite/tools/make/downloads/flatbuffers/include
LDFLAGS=-lpthread -ltensorflow-lite -latomic

all: $(OBJ)/main.o $(OBJ)/TrainingData.o $(OBJ)/RunningMap.o $(OBJ)/Map.o $(OBJ)/TranspositionTable.o $(OBJ)/DeadlockDetection.o $(OBJ)/Metrics.o $(OBJ)/AssignmentAlgorithms.o $(OBJ)/Execution.o $(OBJ)/Node.o $(OBJ)/hungarian/Hungarian.o $(OBJ)/AStar.o $(OBJ)/BoundAlgorithm.o $(OBJ)/perceptron/Perceptron.o
	$(CXX) $(CXXFLAGS) -o Generator $(OBJ)/main.o $(OBJ)/TrainingData.o $(OBJ)/RunningMap.o $(OBJ)/Map.o $(OBJ)/TranspositionTable.o $(OBJ)/DeadlockDetection.o $(OBJ)/Metrics.o $(OBJ)/AssignmentAlgorithms.o $(OBJ)/Execution.o $(OBJ)/Node.o $(OBJ)/hungarian/Hungarian.o $(OBJ)/AStar.o $(OBJ)/BoundAlgorithm.o $(OBJ)/perceptron/Perceptron.o $(TFLITEINC) $(TFLITELIB) $(LDFLAGS)

$(OBJ)/main.o : main.cpp TrainingData.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/main.o -c main.cpp $(TFLITEINC)

$(OBJ)/TrainingData.o : TrainingData.cpp TrainingData.h RunningMap.h $(SOLVER)/Execution.h $(SOLVER)/Map.h $(SOLVER)/Node.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/TrainingData.o -c TrainingData.cpp $(TFLITEINC)

$(OBJ)/RunningMap.o : RunningMap.cpp RunningMap.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/RunningMap.o -c RunningMap.cpp

$(OBJ)/Map.o : $(SOLVER)/Map.cpp $(SOLVER)/Map.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/Map.o -c $(SOLVER)/Map.cpp

$(OBJ)/TranspositionTable.o : $(SOLVER)/TranspositionTable.cpp $(SOLVER)/TranspositionTable.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/TranspositionTable.o -c $(SOLVER)/TranspositionTable.cpp

$(OBJ)/DeadlockDetection.o : $(SOLVER)/DeadlockDetection.cpp $(SOLVER)/DeadlockDetection.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/DeadlockDetection.o -c $(SOLVER)/DeadlockDetection.cpp

$(OBJ)/Metrics.o : $(SOLVER)/Metrics.cpp $(SOLVER)/Metrics.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/Metrics.o -c $(SOLVER)/Metrics.cpp

$(OBJ)/AssignmentAlgorithms.o : $(SOLVER)/AssignmentAlgorithms.cpp $(SOLVER)/AssignmentAlgorithms.h $(SOLVER)/hungarian/Hungarian.h $(SOLVER)/Metrics.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/AssignmentAlgorithms.o -c $(SOLVER)/AssignmentAlgorithms.cpp

$(OBJ)/Execution.o : $(SOLVER)/Execution.cpp $(SOLVER)/Execution.h $(SOLVER)/Node.h $(SOLVER)/TranspositionTable.h $(SOLVER)/Map.h $(SOLVER)/AssignmentAlgorithms.h $(SOLVER)/DeadlockDetection.h $(SOLVER)/BoundAlgorithm.h $(SOLVER)/perceptron/Perceptron.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/Execution.o -c $(SOLVER)/Execution.cpp $(TFLITEINC)

$(OBJ)/Node.o : $(SOLVER)/Node.cpp $(SOLVER)/Node.h $(SOLVER)/Move.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/Node.o -c $(SOLVER)/Node.cpp

$(OBJ)/hungarian/Hungarian.o : $(SOLVER)/hungarian/Hungarian.cpp $(SOLVER)/hungarian/Hungarian.h
	mkdir -p $(OBJ)/hungarian/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/hungarian/Hungarian.o -c $(SOLVER)/hungarian/Hungarian.cpp

$(OBJ)/AStar.o : $(SOLVER)/AStar.cpp $(SOLVER)/AStar.h $(SOLVER)/Metrics.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/AStar.o -c $(SOLVER)/AStar.cpp

$(OBJ)/BoundAlgorithm.o : $(SOLVER)/BoundAlgorithm.cpp $(SOLVER)/BoundAlgorithm.h $(SOLVER)/Metrics.h $(SOLVER)/Map.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -o $(OBJ)/BoundAlgorithm.o -c $(SOLVER)/BoundAlgorithm.cpp

$(OBJ)/perceptron/Perceptron.o: $(SOLVER)/perceptron/Perceptron.cpp $(SOLVER)/perceptron/Perceptron.h
	mkdir -p $(OBJ)/perceptron/
	$(CXX) $(CXXFLAGS) -c $(SOLVER)/perceptron/Perceptron.cpp -o $(OBJ)/perceptron/Perceptron.o $(TFLITEINC) $(TFLITELIB)

clean:
	rm -r $(OBJ)/
