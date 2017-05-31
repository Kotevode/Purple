//
// Created by Mark on 25.05.17.
//

#include <purple/Cluster.h>
#include "wait.h"
#include <monitoring/web/Logger.h>

using namespace Monitoring::Web;
using namespace Purple;
using namespace boost::mpi;

int main(int argc, char *argv[]) {
    environment env(argc, argv);
    
    assert(argc > 1);
    const char *pipe = argv[1];

    Purple::Cluster c(new Logger(pipe));
    vector<WaitJob> jobs;
    for(int i = 0; i < 15; i++)
        jobs.push_back(WaitJob(i));
    WaitProcessor p;
    c.process(jobs, p);
}
