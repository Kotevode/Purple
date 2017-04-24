//
// Created by Mark on 23.04.17.
//

#include <cstdlib>
#include <iostream>
#include "MeshGenerator.h"
#include "config.h"
#include "TaskGenerator.h"

using namespace std;

int main(int argc, char *argv[]) {
    int size = 1000;
    int count = 50;
    if (argc > 1)
        size = atoi(argv[1]);
    if (argc > 2)
        count = atoi(argv[2]);

    cout << size << "\t" << size << endl;

    // Generating mesh
    auto generator = new MeshGenerator(
            X0, Y0, XN, YN, size
    );
    double *mesh = generator->generate([](double x, double y) -> double {
        if (x == X0)
            return F_X0(y);
        else if (x == XN)
            return F_XN(y);
        else if (y == Y0)
            return F_Y0(x);
        else if (y == YN)
            return F_YN(x);
        else return 0;
    });

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            cout << mesh[i * size + j] << "\t";
        cout << endl;
    }

    // Generating tasks
    auto tasks = TaskGenerator::generate(count, size);
    cout << tasks.size() << endl;
    for_each(tasks.begin(), tasks.end(), [](auto &t) {
        cout << t << endl;
    });

    free(mesh);
    return 0;
}
