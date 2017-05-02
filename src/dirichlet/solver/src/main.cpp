//
// Created by Mark on 22.04.17.
//

#include "Input.h"
#include <cxxopts.hpp>
#include <fstream>
#include <memory>
#include "StreamParser.h"
#include "evaluation/Processor.h"
#include <purple/Cluster.h>
#include "Solver.h"

using namespace std;
using namespace Dirichlet;

Input get_input(const cxxopts::Options &options) {
    if (options.count("file") > 0)
        return (new StreamParser(ifstream(options["file"].as<string>())))->parse_input();
    else
        return (new StreamParser(cin))->parse_input();
}

template<typename __it>
string join(__it begin, __it end) {
    stringstream s;
    while (begin != end--)
        s << *begin++ << "\t";
    s << *begin;
    return s.str();
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
    std::shared_ptr<Purple::Cluster> cluster(new Purple::Cluster(argc, argv));
    Solver s(cluster);
    auto result = s.process(input);
    cluster->as_master([&] {
        for (int i = 0; i < result.height; i++) {
            cout
                    << join(result.mesh + i * result.width, result.mesh + (i + 1) * result.width)
                    << endl;
        }
    });
    return 0;
}