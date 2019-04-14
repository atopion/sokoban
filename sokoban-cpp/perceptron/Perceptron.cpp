//
// Created by atopi on 22.03.2019.
//

#include <iostream>
#include "Perceptron.h"

Perceptron::Perceptron(std::string modelPath)
{
    this->model = tflite::FlatBufferModel::BuildFromFile(modelPath.c_str());
    if(!this->model)
    {
        std::cout << "Model could not be loaded" << std::endl;
        exit(-1);
    }

    tflite::ops::builtin::BuiltinOpResolver resolver;

    tflite::InterpreterBuilder(*model, resolver)(&this->interpreter);
    if (!this->interpreter) {
        std::cout << "Failed to construct interpreter\n";
        exit(-1);
    }

    this->b = true;

    int input1 = interpreter->inputs()[0];

    this->interpreter->AllocateTensors();

    TfLiteIntArray* dims = interpreter->tensor(input1)->dims;
    int wanted_height = dims->data[1];
    int wanted_width = dims->data[2];

    this->inputSize = wanted_height * wanted_width;
    this->inputWidth = wanted_width;
    this->inputHeight = wanted_height;

    this->input = interpreter->typed_input_tensor<float>(0);
}

void Perceptron::invoke(int *directions, int *map, int height, int width, int player)
{
    int player_x = player % width;
    int player_y = player / width;
    std::vector<std::vector<int>> mat;
    for(int i = 0; i < 10; i++)
    {
        std::vector<int> tmp;
        for(int a = 0; a < 10; a++)
        {
            tmp.emplace_back(map[i * 10 + a]);
        }
        mat.emplace_back(tmp);
    }

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

    int ic = 0; 
    for(int i = 0; i < 10; i++)
    {
        for(int a = 0; a < 10; a++)
        {
            this->input[ic] = mat[i][a];
            ic++;
        }
    }

    /*int ic = 0;
    for(int i = 0; i < 100; i++)
    {
        this->input[ic] = map[i];
        ic++;
    }*/
    delete[] map;

    interpreter->Invoke();

    auto output = interpreter->typed_output_tensor<float>(0);

    float tmp = 0.0;
    int _tmp = 0;
    for(int i = 4; i > 1; i--)
    {
        for(int j = 0; j < i-1; j++)
        {
            if(output[j] < output[j+1])
            {
                tmp = output[j];
                output[j] = output[j+1];
                output[j+1] = tmp;

                _tmp = directions[j];
                directions[j] = directions[j+1];
                directions[j+1] = _tmp;
            }
        }
    }
}
