//
// Created by Mark on 23.04.17.
//

#ifndef PURPLE_DISTRIBUTION_MESHGENERATOR_H
#define PURPLE_DISTRIBUTION_MESHGENERATOR_H

class MeshGenerator {

public:
    MeshGenerator(double x0, double y0, double xN, double yN, int size);

    double *generate(double (*g)(double, double));

private:
    double x0, y0, xN, yN;
    int size;
};

#endif //PURPLE_DISTRIBUTION_MESHGENERATOR_H
