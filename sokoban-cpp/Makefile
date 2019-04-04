ARCH=linux_x86_64

CXXFLAGS=--std=c++14 -g -static -pthread
TFLITELIB=-L./lib/$(ARCH)
TFLITEINC=-I/home/atopion/atopi/Codes/docker/tensorflow \
-I/home/atopion/atopi/Codes/docker/tensorflow/tensorflow/lite/tools/make/downloads/flatbuffers/include
LDFLAGS=-lpthread -ltensorflow-lite -latomic

CXX=g++

ifeq ($(ARCH), armv5)
CXX=arm-linux-gnueabi-g++
endif

all: main.o Map.o TranspositionTable.o DeadlockDetection.o Metrics.o AssignmentAlgorithms.o Execution.o Node.o hungarian/Hungarian.o AStar.o BoundAlgorithm.o perceptron/Perceptron.o
	mkdir -p ./out/$(ARCH)
	$(CXX) $(CXXFLAGS) -o ./out/$(ARCH)/SokobanSolver main.o Map.o TranspositionTable.o DeadlockDetection.o Metrics.o AssignmentAlgorithms.o Execution.o Node.o hungarian/Hungarian.o AStar.o BoundAlgorithm.o perceptron/Perceptron.o $(TFLITEINC) $(TFLITELIB) $(LDFLAGS)

main.o : main.cpp Map.h Execution.h TranspositionTable.h Node.h
	$(CXX) $(CXXFLAGS) -c main.cpp $(TFLITEINC)

Map.o : Map.cpp Map.h
	$(CXX) $(CXXFLAGS) -c Map.cpp

TranspositionTable.o : TranspositionTable.cpp TranspositionTable.h
	$(CXX) $(CXXFLAGS) -c TranspositionTable.cpp

DeadlockDetection.o : DeadlockDetection.cpp DeadlockDetection.h
	$(CXX) $(CXXFLAGS) -c DeadlockDetection.cpp

Metrics.o : Metrics.cpp Metrics.h
	$(CXX) $(CXXFLAGS) -c Metrics.cpp

AssignmentAlgorithms.o : AssignmentAlgorithms.cpp AssignmentAlgorithms.h hungarian/Hungarian.h Metrics.h
	$(CXX) $(CXXFLAGS) -c AssignmentAlgorithms.cpp

Execution.o : Execution.cpp Execution.h Node.h TranspositionTable.h Map.h AssignmentAlgorithms.h DeadlockDetection.h BoundAlgorithm.h perceptron/Perceptron.h
	$(CXX) $(CXXFLAGS) -c Execution.cpp $(TFLITEINC)

Node.o : Node.cpp Node.h Move.h
	$(CXX) $(CXXFLAGS) -c Node.cpp

hungarian/Hungarian.o : hungarian/Hungarian.cpp hungarian/Hungarian.h
	$(CXX) $(CXXFLAGS) -c hungarian/Hungarian.cpp -o hungarian/Hungarian.o

AStar.o : AStar.cpp AStar.h Metrics.h
	$(CXX) $(CXXFLAGS) -c AStar.cpp

BoundAlgorithm.o : BoundAlgorithm.cpp BoundAlgorithm.h Metrics.h Map.h
	$(CXX) $(CXXFLAGS) -c BoundAlgorithm.cpp

perceptron/Perceptron.o: perceptron/Perceptron.cpp perceptron/Perceptron.h
	$(CXX) $(CXXFLAGS) -c perceptron/Perceptron.cpp -o perceptron/Perceptron.o $(TFLITEINC) $(TFLITELIB)

clean:
	rm main.o Map.o TranspositionTable.o DeadlockDetection.o Metrics.o AssignmentAlgorithms.o Execution.o Node.o hungarian/Hungarian.o AStar.o BoundAlgorithm.o perceptron/Perceptron.o
