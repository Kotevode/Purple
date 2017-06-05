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
    while (begin != end - 1)
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
            ("f,file", "File name", cxxopts::value<string>())
            ("o,output", "Output file name", cxxopts::value<string>());
    options.parse(argc, argv);
    boost::mpi::environment env(argc, argv);
    auto input = get_input(options);
    std::shared_ptr<Purple::Cluster> cluster(new Purple::Cluster());
    Solver s(cluster);
    auto start = clock();
    auto result = s.process(input, 0.001);
    cluster->as_master([&] {
        cout << "Elapsed : " << (double)(clock() - start)/CLOCKS_PER_SEC << endl;
        ofstream out(options["output"].as<string>());
        for (int i = 0; i < input.height; i++) {
            out
                    << join(result.get() + i * input.width, result.get() + (i + 1) * input.width)
                    << endl;
        }
        out.close();
    });
    return 0;
}