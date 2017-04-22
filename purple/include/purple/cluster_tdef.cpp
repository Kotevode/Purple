//
// Created by Mark on 14.03.17.
//

#include "JobInfo.h"
#include "ResultInfo.h"
#include <list>
#include <numeric>

namespace Purple {

    namespace mpi = boost::mpi;
    using namespace std;


    template<class job_type, class result_type>
    vector<result_type> Cluster::process(vector<job_type> &jobs, Processor<job_type, result_type> &processor) const {
        vector<JobInfo> info(jobs.size());
        int i = 0;
        for_each(jobs.begin(), jobs.end(), [&](job_type &j) {
            info[i] = JobInfo(j, i);
            i++;
        });
        distribute(info);

        int rank = communicator.rank();
        int partial_count = accumulate(info.begin(), info.end(), 0, [rank](int sum, auto &j) -> int {
            return sum + (j.get_node_number() == rank ? 1 : 0);
        });
        vector<ResultInfo<result_type> > partial_results(partial_count);

        int j = 0;
        for_each(info.begin(), info.end(), [&](auto &info) {
            if (info.get_node_number() != rank)
                return;
            partial_results[j] =
                    ResultInfo<result_type>(processor.process(jobs[info.get_index()]), info.get_index());
            j++;
        });
        MPI_Barrier(communicator);
        vector<vector<ResultInfo<result_type> >> gathered(communicator.size());
        mpi::gather(communicator, partial_results, gathered, 0);

        vector<result_type> results(jobs.size());
        for_each(gathered.begin(), gathered.end(), [&](auto &partial) {
            for_each(partial.begin(), partial.end(), [&](auto &result_info) {
                results[result_info.get_index()] = std::move(result_info.get_result());
            });
        });

        return results;
    }

}

