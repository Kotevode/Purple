//
// Created by Mark on 23.04.17.
//

#include <gtest/gtest.h>
#include "../src/MeshGenerator.h"
#include "../src/config.h"

using namespace std;

TEST(MeshGeneratorTests, can_generate_mesh) {
    int size = 100;
    auto generator = new MeshGenerator(
            X0, Y0, XN, YN, size
    );
    double *mesh = generator->generate([](double x, double y) -> double {
        if (x == X0)
            return F_X0(y);
        else if(x == XN)
            return F_XN(y);
        else if(y == Y0)
            return F_Y0(x);
        else if(y == YN)
            return F_YN(x);
    });
    cout << endl;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++)
            cout << mesh[i * size + j] << "\t";
        cout << endl;
    }
    free(mesh);
}
