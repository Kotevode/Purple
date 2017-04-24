//
// Created by Mark on 23.04.17.
//

#include <gtest/gtest.h>
#include "../src/TaskGenerator.h"

TEST(TaskGeneratorTests, can_generate_tasks) {
    int count = 50;
    int size = 1000;
    auto tasks = TaskGenerator::generate(count, size);
    for_each(tasks.begin(), tasks.end(), [](auto &t) {
        cout << t << endl;
    });
}
