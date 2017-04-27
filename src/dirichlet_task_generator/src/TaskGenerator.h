//
// Created by Mark on 23.04.17.
//

#ifndef PURPLE_DISTRIBUTION_TASKGENERATOR_H
#define PURPLE_DISTRIBUTION_TASKGENERATOR_H

#include <vector>
#include <ostream>

using namespace std;

struct task {
    int offset;
    int size;

    friend ostream &operator<<(ostream &os, const task &task1) {
        os << task1.offset << "\t" << task1.size;
        return os;
    }
};

class TaskGenerator {

public:
    static vector<struct task> generate(int count, int height);

};

#endif //PURPLE_DISTRIBUTION_TASKGENERATOR_H
