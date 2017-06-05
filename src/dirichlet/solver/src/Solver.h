//
// Created by Mark on 28.04.17.
//

#ifndef PURPLE_DISTRIBUTION_SOLVER_H
#define PURPLE_DISTRIBUTION_SOLVER_H

//
// Solver.h
//
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
            boost::mpi::communicator comm;

            // Список номером процессов, от которых ожидается отправка секций
            list<int> receive_sources;

            // Список запросов на отправку секций
            list<boost::mpi::request> sends;
            while(++begin != end) {
                auto a = (begin - 1);
                auto b = begin;

                // Извлекаем номер принимающего и получающего процесса
                auto sender = a->get_node_number();
                auto receiver = b->get_node_number();

                // В рамках одного процесса обмен не является необходимостью
                if (sender == receiver)
                    continue;

                // Если текущий процесс является отправляющим, то отправляем пересекаемую область
                if (comm.rank() == sender) {
                    if (a->offset > b->offset)
                        sends.push_back(comm.isend(receiver, 0, Evaluation::Section(mesh, *b, *a)));
                    else
                        sends.push_back(comm.isend(receiver, 0, Evaluation::Section(mesh, *a, *b)));
                } else

                    // Если текущий процесс является принимающим, то добавляем номер отправляющего процесса
                    // в список
                if (comm.rank() == receiver)
                    receive_sources.push_back(sender);
            }
            list<Evaluation::Section> result;

            // Для каждого процесса, от которого ожидается отправка секций, принимаем секцию
            // и добавляем в результат
            for_each(receive_sources.begin(), receive_sources.end(), [&comm, &result] (auto source) {
                Evaluation::Section s;
                comm.recv(source, 0, s);
                result.push_back(s);
            });
            boost::mpi::wait_all(sends.begin(), sends.end());
            return result;
        }

    private:

        boost::shared_array<double> combine(vector<Evaluation::Job> &jobs, const double * mesh, size_t size);

        std::shared_ptr<Purple::Cluster> cluster;

    };

}

#endif //PURPLE_DISTRIBUTION_SOLVER_H
