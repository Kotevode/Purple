//
// Created by Mark on 21.05.17.
//

#include "monitoring/console/Logger.h"
#include "console/Formatter.h"
#include <iomanip>

using namespace std;
using namespace Purple::Monitoring;
using namespace Purple::Monitoring::Messages;
using namespace boost::mpi;

#define OUTLINE "\n\n_____________________________________\n\n"

Monitoring::Console::Logger::Logger() : running(true) {
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

void Monitoring::Console::Logger::send(const Purple::Monitoring::Messages::ClusterCreated &message) {
    if (rank == 0)
        print(Formatter().format(message));
}

void Monitoring::Console::Logger::send(const Purple::Monitoring::Messages::ClusterFinalized &message) {
    if (rank == 0)
        print(Formatter().format(message));
}

void Monitoring::Console::Logger::send(const Purple::Monitoring::Messages::JobStatusChanged &message) {
    print(Formatter().format(message));
}

void Monitoring::Console::Logger::send(const Purple::Monitoring::Messages::ProcessingDone &message) {
    if (rank == 0)
        print(Formatter().format(message));
}

void Monitoring::Console::Logger::send(const Purple::Monitoring::Messages::ProcessingStarted &message) {
    if (rank == 0)
        print(Formatter().format(message));
}

void Monitoring::Console::Logger::send(const Purple::Monitoring::Messages::LogMessage &message) {
    if (rank == 0)
        print(Formatter().format(message));
}

void Monitoring::Console::Logger::receive() {
    while (running) {
        string s;
        communicator.recv(MPI_ANY_SOURCE, communication_tag, s);
        cout << s << endl;
    }
}

void Monitoring::Console::Logger::print(const std::string &s) {
    if (rank == 0)
        cout << s << endl;
    else
        communicator.isend(0, communication_tag, s);
}

Monitoring::Console::Logger::~Logger() {
    running = false;
    if (rank == 0) {
        communicator.send(0, communication_tag, "");
        communication_thread.join();
    }
}
