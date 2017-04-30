//
// Created by Mark on 23.04.17.
//

#include "TaskGenerator.h"
#include <random>
#include <assert.h>

vector<struct task> TaskGenerator::generate(int count, int height) {
    assert(count > 0);
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> distribution(height / count, height / count / 3);
    vector<struct task> result;
    int intersection = height / count / 6;
    int offset = intersection;
    while (offset < height) {
        offset -= intersection;
        int task_size = round(distribution(gen));
        task_size =
                offset + task_size > height ? height - offset : task_size;
        result.push_back({offset, task_size});
        offset += task_size;
    }
    return result;
}