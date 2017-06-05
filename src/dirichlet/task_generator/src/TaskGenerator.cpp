//
// Created by Mark on 23.04.17.
//

#include "TaskGenerator.h"
#include <random>
#include <assert.h>

vector<struct task> TaskGenerator::generate(int mean, int height) {
    assert(mean > 9);
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> distribution(mean, mean / 3);
    vector<struct task> result;
    int intersection = mean / 6;
    int offset = intersection;
    while (offset < height) {
        offset -= intersection;
        int task_size = -1;
        while(task_size <= 0)
            task_size = (int)round(distribution(gen));
        task_size += 2 * intersection;
        task_size =
                offset + task_size > height ? height - offset : task_size;
        result.push_back({offset, task_size});
        offset += task_size;
    }
    return result;
}