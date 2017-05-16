//
// Created by Mark on 28.04.17.
//

#ifndef PURPLE_DISTRIBUTION_SOLVER_H
#define PURPLE_DISTRIBUTION_SOLVER_H

#include "Input.h"
#include "evaluation/Processor.h"
#include "evaluation/Section.h"
#include <purple/Cluster.h>
#include <memory>
#include <boost/shared_array.hpp>

namespace Dirichlet {

    class Solver {

    public:
        Solver(std::shared_ptr<Purple::Cluster> cluster) : cluster(cluster) {}

        boost::shared_array<double> process(Input &input, double error = 0.001);

        void swap_intersections(vector<Evaluation::Job> &jobs, double *mesh);

        template<typename __job_it>
        list<Evaluation::Section> swap_intersections(__job_it begin, __job_it end, double * mesh) {
            auto comm = cluster->get_communicator();
            list<int> receive_sources;
            while(++begin != end) {
                auto a = (begin - 1);
                auto b = begin;
                auto sender = a->get_node_number();
                auto receiver = b->get_node_number();
                if (sender == receiver)
                    continue;
                if (comm.rank() == sender) {
                    if (a->offset > b->offset)
                        comm.isend(receiver, 0, Evaluation::Section(mesh, *b, *a));
                    else
                        comm.isend(receiver, 0, Evaluation::Section(mesh, *a, *b));
                } else if (comm.rank() == receiver)
                    receive_sources.push_back(sender);
            }
            list<Evaluation::Section> result;
            for_each(receive_sources.begin(), receive_sources.end(), [&comm, &result] (auto source) {
                Evaluation::Section s;
                comm.recv(source, 0, s);
                result.push_back(s);
            });
            return result;
        }

    private:

        boost::shared_array<double> combine(vector<Evaluation::Job> &jobs, const double * mesh);

        std::shared_ptr<Purple::Cluster> cluster;

    };

}

#endif //PURPLE_DISTRIBUTION_SOLVER_H
