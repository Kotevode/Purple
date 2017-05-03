//
// Created by Mark on 23.04.17.
//

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "MeshGenerator.h"
#include "config.h"
#include "TaskGenerator.h"

using namespace std;

int main(int argc, char *argv[]) {
    int size = 10000;
    int count = 50;
    if (argc > 1)
        size = atoi(argv[1]);
    if (argc > 2)
        count = atoi(argv[2]);

    cout << size << "\t" << size << endl;

    // Generating u-u
    auto generator = new MeshGenerator(
            X0, Y0, XN, YN, size
    );
    double *u = generator->generate_u([](double x, double y) -> double {
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
            cout << u[i * size + j] << "\t";
        cout << endl;
    }
    // Generating f-u
    double *f = generator->generate_f([](double x, double y) -> double {
        return F(x, y);
    });

    for(int i = 0; i < size; i++){
        for( int j = 0; j < size; j++)
            cout << f[i * size + j] << "\t";
        cout << endl;
    }

    // Generating tasks
    auto tasks = TaskGenerator::generate(count, size);
    cout << tasks.size() << endl;
    for_each(tasks.begin(), tasks.end(), [](auto &t) {
        cout << t << endl;
    });

    free(u);
    free(f);
    return 0;
}
