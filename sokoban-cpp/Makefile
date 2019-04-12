ARCH=linux_x86_64
NAME=SokobanSolver
OBJ=obj

CXXFLAGS=--std=c++14 -g -static -pthread
TFLITELIB=-L./lib/$(ARCH)
TFLITEINC=-I/home/atopion/atopi/Codes/docker/tensorflow \
-I/home/atopion/atopi/Codes/docker/tensorflow/tensorflow/lite/tools/make/downloads/flatbuffers/include
LDFLAGS=-lpthread -ltensorflow-lite -latomic

CXX=g++

ifeq ($(ARCH), armv5)
CXX=arm-linux-gnueabi-g++
endif

all: $(OBJ)/main.o $(OBJ)/Map.o $(OBJ)/TranspositionTable.o $(OBJ)/DeadlockDetection.o $(OBJ)/Metrics.o $(OBJ)/AssignmentAlgorithms.o $(OBJ)/Execution.o $(OBJ)/Node.o $(OBJ)/hungarian/Hungarian.o $(OBJ)/AStar.o $(OBJ)/BoundAlgorithm.o $(OBJ)/perceptron/Perceptron.o
	mkdir -p ./out/$(ARCH)
	$(CXX) $(CXXFLAGS) -o ./out/$(ARCH)/$(NAME) $(OBJ)/main.o $(OBJ)/Map.o $(OBJ)/TranspositionTable.o $(OBJ)/DeadlockDetection.o $(OBJ)/Metrics.o $(OBJ)/AssignmentAlgorithms.o $(OBJ)/Execution.o $(OBJ)/Node.o $(OBJ)/hungarian/Hungarian.o $(OBJ)/AStar.o $(OBJ)/BoundAlgorithm.o $(OBJ)/perceptron/Perceptron.o $(TFLITEINC) $(TFLITELIB) $(LDFLAGS)
	ln -sf ./out/$(ARCH)/$(NAME) $(NAME)

$(OBJ)/main.o : main.cpp Map.h Execution.h TranspositionTable.h Node.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -c main.cpp -o $(OBJ)/main.o $(TFLITEINC)

$(OBJ)/Map.o : Map.cpp Map.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -c Map.cpp -o $(OBJ)/Map.o

$(OBJ)/TranspositionTable.o : TranspositionTable.cpp TranspositionTable.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -c TranspositionTable.cpp -o $(OBJ)/TranspositionTable.o

$(OBJ)/DeadlockDetection.o : DeadlockDetection.cpp DeadlockDetection.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -c DeadlockDetection.cpp -o $(OBJ)/DeadlockDetection.o

$(OBJ)/Metrics.o : Metrics.cpp Metrics.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -c Metrics.cpp -o $(OBJ)/Metrics.o

$(OBJ)/AssignmentAlgorithms.o : AssignmentAlgorithms.cpp AssignmentAlgorithms.h hungarian/Hungarian.h Metrics.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -c AssignmentAlgorithms.cpp -o $(OBJ)/AssignmentAlgorithms.o

$(OBJ)/Execution.o : Execution.cpp Execution.h Node.h TranspositionTable.h Map.h AssignmentAlgorithms.h DeadlockDetection.h BoundAlgorithm.h perceptron/Perceptron.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -c Execution.cpp -o $(OBJ)/Execution.o $(TFLITEINC)

$(OBJ)/Node.o : Node.cpp Node.h Move.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -c Node.cpp -o $(OBJ)/Node.o

$(OBJ)/hungarian/Hungarian.o : hungarian/Hungarian.cpp hungarian/Hungarian.h
	mkdir -p $(OBJ)/hungarian
	$(CXX) $(CXXFLAGS) -c hungarian/Hungarian.cpp -o $(OBJ)/hungarian/Hungarian.o

$(OBJ)/AStar.o : AStar.cpp AStar.h Metrics.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -c AStar.cpp -o $(OBJ)/AStar.o

$(OBJ)/BoundAlgorithm.o : BoundAlgorithm.cpp BoundAlgorithm.h Metrics.h Map.h
	mkdir -p $(OBJ)/
	$(CXX) $(CXXFLAGS) -c BoundAlgorithm.cpp -o $(OBJ)/BoundAlgorithm.o

$(OBJ)/perceptron/Perceptron.o: perceptron/Perceptron.cpp perceptron/Perceptron.h
	mkdir -p $(OBJ)/perceptron
	$(CXX) $(CXXFLAGS) -c perceptron/Perceptron.cpp -o $(OBJ)/perceptron/Perceptron.o $(TFLITEINC) $(TFLITELIB)

clean:
	rm -f main.o Map.o TranspositionTable.o DeadlockDetection.o Metrics.o AssignmentAlgorithms.o Execution.o Node.o hungarian/Hungarian.o AStar.o BoundAlgorithm.o perceptron/Perceptron.o
	rm -rf $(OBJ)/
