//
// Created by Mark on 23.04.17.
//

#include <cstdlib>
#include <assert.h>
#include "MeshGenerator.h"

double *MeshGenerator::generate(double (*g)(double, double)) {
    double *result = (double *) malloc(sizeof(double) * size * size);
    double dx = (xN - x0) / (size - 1);
    double dy = (yN - y0) / (size - 1);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (i == 0 || i == size - 1 || j == 0 || j == size - 1)
                result[i * size + j] = g(i * dx, j * dy);
            else
                result[i * size + j] = 0.0;
    return result;
}

MeshGenerator::MeshGenerator(double x0, double y0, double xN, double yN, int size) {
    assert(xN > x0);
    assert(yN > y0);
    assert(size >= 3);
    this->x0 = x0;
    this->y0 = y0;
    this->xN = xN;
    this->yN = xN;
    this->size = size;
}
