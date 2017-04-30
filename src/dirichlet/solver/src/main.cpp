//
// Created by Mark on 22.04.17.
//

#include "Input.h"
#include <cxxopts.hpp>
#include <fstream>
#include "StreamParser.h"
#include "evaluation/Processor.h"
#include <purple/Cluster.h>

using namespace std;
using namespace Dirichlet;

Input get_input(const cxxopts::Options &options){
    if (options.count("file") > 0)
        return (new StreamParser(ifstream(options["file"].as<string>())))->parse_input();
    else
        return (new StreamParser(cin))->parse_input();
}

void calculate(Input &input) {
    Evaluation::Processor p(input.u, input.f, input.width);
    Purple::Cluster c(0, NULL);
    auto results = c.process(input.jobs, p);
}

int main(int argc, char *argv[]) {
    cxxopts::Options options(
            "Dirichlet Solver",
            "Test program that performs solving of Laplass equation via Purple Library"
    );
    options.add_options()
            ("f,file", "File name", cxxopts::value<string>());
    options.parse(argc, argv);

    auto input = get_input(options);
    calculate(input);

    return 0;
}