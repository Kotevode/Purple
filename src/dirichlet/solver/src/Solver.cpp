//
// Created by Mark on 28.04.17.
//

//
// Solver.cpp
//
#include "Solver.h"
#include <float.h>
#include "evaluation/Section.h"
#include <boost/mpi/collectives.hpp>
#include <execinfo.h>

using namespace Dirichlet;
using namespace Evaluation;
using namespace std;
using namespace boost::mpi;
using namespace boost;

shared_array<double> Dirichlet::Solver::process(Dirichlet::Input &input, double error) {

    // Создаем Processor для обработки задач
    Processor p(input.f, input.u);
    auto jobs = input.jobs;
    vector<double> results;
    bool is_done = false;
    double max_error = 0;

    do {

        // Производим итерацию параллельно с использованеим
        // класса Cluster
        results = cluster->process(jobs, p);

        // Выбираем максимальную невязку на нулевом процессе
        cluster->as_master([&is_done, error, &results, &max_error] {
            max_error = *max_element(results.begin(), results.end());
            is_done = max_error < error;
        });

        // Заканчиваем итерации, если достигнута требуемая точность
        broadcast(cluster->get_communicator(), is_done, 0);
        if (is_done) {
            break;
        }

        // Производим обмен пересечениями
        this->swap_intersections(jobs, p.u.get());
    } while (true);

    // Собираем результаты на нулевом процессе
    return combine(jobs, p.u.get(), input.height * input.width);
}

void Dirichlet::Solver::swap_intersections(vector<Evaluation::Job> &jobs, double *mesh) {
    list<Section> sections;

    // Получаем нижние границы
    sections.splice(sections.end(), swap_intersections(jobs.begin(), jobs.end(), mesh));

    // Получаем верхние границы
    sections.splice(sections.end(), swap_intersections(jobs.rbegin(), jobs.rend(), mesh));

    // Вставляем полученные значения локальную сетку
    for_each(sections.begin(), sections.end(), [mesh](auto &s) {
        mesh += s;
    });
}

shared_array<double>
Dirichlet::Solver::combine(vector<Job> &jobs, const double *mesh, size_t size) {
    communicator comm;

    // Все ненулевые процессы отправляют части сетки, захватываемые
    // задчами, выполняемыми на данном процессе
    if (comm.rank() != 0)
        for_each(jobs.begin(), jobs.end(), [mesh, &comm](auto &j) {
            if (j.get_node_number() != comm.rank())
                return;
            Section s(j.offset * j.width, j.width * j.height, mesh + j.offset * j.width);
            comm.send(0, 1, s);
        });

    shared_array<double> result(new double[size]);

    // Нулевой процесс принимает отправленные части и вставляет их
    // в свою локальную сетку
    cluster->as_master([&] {
        memcpy(result.get(), mesh, size * sizeof(double));
        for_each(jobs.begin(), jobs.end(), [&comm, &result](auto &j) {
            if (j.get_node_number() != 0) {
                Section s;
                comm.recv(j.get_node_number(), 1, s);
                result.get() += s;
            }
        });
    });
    return result;
}