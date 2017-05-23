//
// Created by Mark on 23.05.17.
//

#include "Formatter.h"
#include <sstream>
#include <iomanip>

using namespace std;
using namespace Purple::Monitoring::Messages;

#define OUTLINE "\n\n_____________________________________\n\n"

std::string Monitoring::Console::Formatter::format(const Purple::Monitoring::Messages::ClusterCreated &message) const {
    stringstream s;
    s << OUTLINE
      << "Cluster created " << endl
      << "size : " << this->communicator.size();
    return s.str();
}

std::string
Monitoring::Console::Formatter::format(const Purple::Monitoring::Messages::ProcessingStarted &message) const {
    stringstream s;
    int size = communicator.size();

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

    // Printing node load
    s << "|";
    for (int i = 0; i < size; i++)
        s << setw(8) << total_weight[i] << "|";
    return s.str();
}

std::string
Monitoring::Console::Formatter::format(const Purple::Monitoring::Messages::JobStatusChanged &message) const {
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
    return s.str();
}

std::string Monitoring::Console::Formatter::format(const Purple::Monitoring::Messages::ProcessingDone &message) const {
    stringstream s;
    s << OUTLINE
      << "Processing done";
    return s.str();
}

std::string
Monitoring::Console::Formatter::format(const Purple::Monitoring::Messages::ClusterFinalized &message) const {
    stringstream s;
    s << OUTLINE
      << "Cluster finalized";
    return s.str();
}

std::string Monitoring::Console::Formatter::format(const Purple::Monitoring::Messages::LogMessage &message) const {
    stringstream s;
    s << OUTLINE
      << message.body << endl;
    return s.str();
}
