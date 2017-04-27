//
// Created by Mark on 23.04.17.
//

#include <cstdlib>
#include <assert.h>
#include "MeshGenerator.h"

double *MeshGenerator::generate_u(double (*g)(double, double)) {
    double *result = (double *) malloc(sizeof(double) * size * size);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (i == 0 || i == size - 1 || j == 0 || j == size - 1)
                result[i * size + j] = g(x0 + i * dx, y0 + j * dy);
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
    this->dx = (xN - x0) / (size - 1);
    this->dy = (yN - y0) / (size - 1);
}

double *MeshGenerator::generate_f(double (*f)(double, double)) {
    double *result = (double *) malloc(sizeof(double) * size * size);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            result[i * size + j] = dx * dy * f(x0 + i * dx, y0 + j * dy);
    return result;
}
