//
// Created by Mark on 28.04.17.
//

#include <gtest/gtest.h>
#include "../src/conjugation/Processor.h"

using namespace std;
using namespace Dirichlet;
using namespace Dirichlet::Conjugation;

TEST(ConjugationTests, can_conjugate_mesh) {
    int width = 10;
    int height = 10;

    double *a_u = (double *) malloc(sizeof(double) * width * height);
    fill_n(a_u, width * height, 0);
    Result a(width, height, a_u, 0, 0);
    double *b_u = (double *) malloc(sizeof(double) * width * height);
    fill_n(b_u, width * height, 1);
    Result b(width, height, b_u, 0, height / 2);

    Processor p(width);
    Job j(a, b);
    Result r = p.process(j);
    ASSERT_EQ(r.height, (int)(height * 1.5));
    ASSERT_EQ(r.width, height);
    for_each(r.mesh, r.mesh + width * height / 2, [](double v){
        ASSERT_EQ(v, 0);
    });
    for_each(r.mesh + width * height / 2, r.mesh + width * height, [](double v){
        ASSERT_EQ(v, 0.5);
    });
    for_each(r.mesh + width * height, r.mesh + (int)(width * height * 1.5), [](double v){
        ASSERT_EQ(v, 1);
    });
}
