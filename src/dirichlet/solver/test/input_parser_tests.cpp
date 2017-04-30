//
// Created by Mark on 23.04.17.
//

#include <gtest/gtest.h>
#include "../src/StreamParser.h"
#include <fstream>

using namespace Dirichlet;

TEST(InputParserTests, can_parse_from_file) {
    ifstream file("input_file.in");
    ASSERT_TRUE(file.is_open());
    ASSERT_FALSE(file.eof());
    ASSERT_TRUE(file.good());
    auto input = (new StreamParser(file))->parse_input();
    ASSERT_GT(input.width, 0);
    ASSERT_GT(input.height, 0);
    ASSERT_GT(input.jobs.size(), 0);
    for (int i = 0; i < input.height * input.width; i++){
        ASSERT_NO_THROW(input.u[i]);
        ASSERT_NO_THROW(input.f[i]);
    }
    for_each(input.jobs.begin(), input.jobs.end(), [](auto &j){
        cout << j << endl;
    });
}
