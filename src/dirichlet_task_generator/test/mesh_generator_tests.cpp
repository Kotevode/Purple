//
// Created by Mark on 23.04.17.
//

#include <gtest/gtest.h>
#include "../src/MeshGenerator.h"
#include "../src/config.h"

using namespace std;

TEST(MeshGeneratorTests, can_generate_u) {
    int size = 100;
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
        else
            return 0;
    });
    cout << endl;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++)
            cout << u[i * size + j] << "\t";
        cout << endl;
    }
    free(u);
}

TEST(MeshGeneratorTests, can_generate_f) {
    int size = 100;
    auto generator = new MeshGenerator(
            X0, Y0, XN, YN, size
    );
    double *f = generator->generate_f([](double x, double y) -> double {
        return F(x,y);
    });
    cout << endl;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++)
            cout << f[i * size + j] << "\t";
        cout << endl;
    }
    free(f);
}