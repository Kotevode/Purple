//
// Created by Mark on 23.05.17.
//

#include "monitoring/web/Logger.h"
#include "Formatter.h"

using namespace std;
using namespace nlohmann;
using namespace Purple::Monitoring::Messages;

Monitoring::Web::Logger::Logger(const std::string &fifo) : running(true) {
    size = communicator.size();
    rank = communicator.rank();
    out = new ofstream(fifo);
    if (rank == 0) {
        communication_thread = std::thread([this]() {
            this->receive();
        });
    }
    communication_tag = rand();
    boost::mpi::broadcast(communicator, communication_tag, 0);

}

void Monitoring::Web::Logger::send(const Purple::Monitoring::Messages::ClusterCreated &message) {
    if (rank == 0)
        write(Formatter().format(message));
}

void Monitoring::Web::Logger::send(const Purple::Monitoring::Messages::ClusterFinalized &message) {
    if (rank == 0)
        write(Formatter().format(message));
}

void Monitoring::Web::Logger::send(const Purple::Monitoring::Messages::JobStatusChanged &message) {
    write(Formatter().format(message));
}

void Monitoring::Web::Logger::send(const Purple::Monitoring::Messages::ProcessingDone &message) {
    if (rank == 0)
        write(Formatter().format(message));
}

void Monitoring::Web::Logger::send(const Purple::Monitoring::Messages::ProcessingStarted &message) {
    if (rank == 0)
        write(Formatter().format(message));
}

void Monitoring::Web::Logger::send(const Purple::Monitoring::Messages::LogMessage &message) {
    write(Formatter().format(message));
}

void Monitoring::Web::Logger::write(const nlohmann::json &json) {
    auto s = json.dump();
    if (rank == 0)
        write(s);
    else
        communicator.isend(0, communication_tag, s);
}

void Monitoring::Web::Logger::receive() {
    while (running) {
        string s;
        communicator.recv(MPI_ANY_SOURCE, communication_tag, s);
        write(s);
    }
}

void Monitoring::Web::Logger::write(const std::string &dump) {
    if (out) {
        if (!dump.empty()) {
            uint64_t size = dump.size();
            stream_mutex.lock();
            out->write((const char *)&size, 8);
            out->write(dump.c_str(), size);
            out->flush();
            stream_mutex.unlock();
        }
    }
}

Monitoring::Web::Logger::~Logger() {
    running = false;
    if (rank == 0) {
        delete out;
        communicator.send(0, communication_tag, std::string());
        communication_thread.join();
    }
}
