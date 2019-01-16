//
// Created by atopi on 02.12.2018.
//

#ifndef SOKOBANSOLVER_METRICS_H
#define SOKOBANSOLVER_METRICS_H

class Metrics
{
private:
    int width;
    int height;
    int size;
    int targets_count;

    int *target_codes;
    int **distance_to_goals;

public:
    Metrics(int width, int height, int *targets, int targets_count, int* clearedBoard);
    ~Metrics();

    static int manhattan_distance(int a, int b, int width);
    int pythagoran_distance(int a, int b);
    int pull_distance(int target, int pos);

    int lookupTarget(int target);
};

#endif //SOKOBANSOLVER_METRICS_H
