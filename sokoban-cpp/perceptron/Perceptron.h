//
// Created by atopi on 22.03.2019.
//

#ifndef SOKOBANSOLVER_PERCEPTRON_H
#define SOKOBANSOLVER_PERCEPTRON_H

#include <tensorflow/lite/version.h>
#include <tensorflow/lite/c/c_api_internal.h>

#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/optional_debug_tools.h>
#include <tensorflow/lite/string_util.h>
#include <tensorflow/lite/interpreter.h>
#include <string>
#include <vector>


class Pair
{
public:
    std::vector<int> data;
    int label;

    Pair()
    {
        this->data = std::vector<int>(0);
        this->label = 0;
    }

    Pair(std::vector<int>* _data, int _label)
    {
        this->data = *_data;
        this->label = _label;
    }

    explicit Pair(Pair *pair)
    {
        this->data = pair->data;
        this->label = pair->label;
    }
};

class Perceptron {
private:
    std::unique_ptr<tflite::FlatBufferModel> model;
    std::unique_ptr<tflite::Interpreter> interpreter;
    float *input;
    int inputSize;
    int inputWidth;
    int inputHeight;

    // DEBUG
    bool b;

public:
    explicit Perceptron(std::string modelPath);

    void invoke(int* directions, int *map, int height, int width, int player);
};


#endif //SOKOBANSOLVER_PERCEPTRON_H
