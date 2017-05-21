//
// Created by Mark on 21.05.17.
//

#include "monitoring/ConsoleLogger.h"
#include <iomanip>
#include <thread>
#include <boost/mpi.hpp>

using namespace std;
using namespace Purple::Monitoring;
using namespace Purple::Monitoring::Messages;
using namespace boost::mpi;

#define OUTLINE "\n\n_____________________________________\n\n"

Monitoring::ConsoleLogger::ConsoleLogger():running(true) {
    size = communicator.size();
    rank = communicator.rank();
    if (rank == 0) {
        communication_thread = std::thread([this]() {
            this->receive();
        });
    }
    communication_tag = rand();
    boost::mpi::broadcast(communicator, communication_tag, 0);
}

void Monitoring::ConsoleLogger::send(const Purple::Monitoring::Messages::ClusterCreated &message) {
    if (rank == 0) {
        stringstream s;
        s << OUTLINE
          << "Cluster created "
          << "size : " << size;
        print(s.str());
    }
}

void Monitoring::ConsoleLogger::send(const Purple::Monitoring::Messages::ClusterFinalized &message) {
    if (rank == 0) {
        stringstream s;
        s << OUTLINE
          << "Cluster finalized";
        print(s.str());
    }
}

void Monitoring::ConsoleLogger::send(const Purple::Monitoring::Messages::JobStatusChanged &message) {
    stringstream s;
    s << OUTLINE
      << "Job " << message.index << endl;
    switch (message.status) {
        case JobStatusChanged::RUNNING:
            s << "   running";
            break;
        case JobStatusChanged::WAITING:
            s << "   waiting";
            break;
        case JobStatusChanged::DONE:
            s << "   done";
            break;
        case JobStatusChanged::ERROR:
            s << "   error";
            break;
    }
    print(s.str());
}

void Monitoring::ConsoleLogger::send(const Purple::Monitoring::Messages::ProcessingDone &message) {
    if (rank == 0) {
        stringstream s;
        s << OUTLINE
          << "Processing done";
        print(s.str());
    }
}

void Monitoring::ConsoleLogger::send(const Purple::Monitoring::Messages::ProcessingStarted &message) {
    if (rank == 0) {
        stringstream s;
        s << OUTLINE
          << "Processing started"
          << endl;
        size_t total_weight[size];
        fill_n(total_weight, size, 0);
        for_each(message.info.begin(), message.info.end(), [&](auto &p) {
            total_weight[p.node] += p.weight;
        });

        // Printing header
        s << "|";
        for (int i = 0; i < size; i++)
            s << setw(8) << i << "|";
        s << endl;

        // Printing node load)
        s << "|";
        for (int i = 0; i < size; i++)
            s << setw(8) << total_weight[i] << "|";
        s << endl;
        print(s.str());
    }
}

void Monitoring::ConsoleLogger::send(const Purple::Monitoring::Messages::LogMessage &message) {
    if (rank == 0) {
        stringstream s;
        s << OUTLINE
          << message.message << endl;
    }
}

void Monitoring::ConsoleLogger::receive() {
    while (running) {
        string s;
        communicator.recv(MPI_ANY_SOURCE, communication_tag, s);
        cout << s << endl;
    }
}

void Monitoring::ConsoleLogger::print(const std::string &s) {
    if (rank == 0)
        cout << s << endl;
    else
        communicator.isend(0, communication_tag, s);
}

Monitoring::ConsoleLogger::~ConsoleLogger() {
    running = false;
    if (rank == 0) {
        communicator.send(0, communication_tag, "");
        communication_thread.join();
    }
}
