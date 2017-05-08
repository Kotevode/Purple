//
// Created by Mark on 05.05.17.
//

#include <purple/efdist.h>
#include <purple/hdist.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <fstream>
#include <ctime>
#include "GeneratingJob.h"
#include <purple/JobInfo.h>

#define FILE_PATH   "results.csv"
#define M_MIN       100
#define M_MAX       2500
#define N_MIN       2
#define N_MAX       100
#define AVG_WEIGHT  1000
#define MAX_SD      300
#define MIN_SD      0

using namespace Purple;
using namespace std;

JobInfo * generate_jobs(int n, int SD){
    GeneratingJob j(AVG_WEIGHT, SD);
    auto *result = new JobInfo[n];
    generate(result, result + n, [&]() -> JobInfo {
        return JobInfo(j, 0);
    });
    return result;
}

double v_value(JobInfo * begin, JobInfo * end, int node_number){
    long long node_load[node_number];
    memset(node_load, 0, sizeof(long long)*node_number);
    for_each(begin, end, [&](auto &t){
        node_load[t.get_node_number()] += t.get_weight();
    });

    double avg = 0;
    for_each(node_load, node_load + node_number, [&](int i){
        avg += i;
    });
    avg /= node_number;

    double sd = 0;
    for_each(node_load, node_load + node_number, [&](int nl){
        sd += pow(nl - avg, 2);
    });
    sd = sqrt(sd / node_number);

    return sd/avg;
}

void make_test(void (* d_func)(JobInfo *,JobInfo *, size_t), JobInfo *jobs, int N, int M, ofstream &f){
    clock_t begin = clock();
    d_func(jobs, jobs + M, N);
    clock_t end = clock();
    f << v_value(jobs, jobs + M, N) << ";";
    f << ((double)end - begin)/CLOCKS_PER_SEC << ";";
}

int main(int argc, const char * argv[]) {

    ofstream f;
    f.open(FILE_PATH, ios::out | ios::trunc);
    f << "N;M;SD;h.dist;h.dist.time;ef.dist;ef.dist.time;" << endl;
    for(int SD = MIN_SD; SD < MAX_SD; SD += 30){
        for(int N = N_MIN; N < N_MAX; N+=4){
            for(int M = M_MIN; M <= M_MAX; M+=10){
                cout << "######### ";
                cout << "M = " << M;
                cout << " N = " << N;
                cout << " SD = " << SD;
                cout << "########" << endl;
                auto jobs = generate_jobs(M, SD);
                f << N << ";";
                f << M << ";";
                f << SD << ";";

                make_test(h_distribution, jobs, N, M, f);
                make_test(ef_distribution, jobs, N, M, f);

                f << endl;

                delete jobs;
            }
        }
    }
    f.close();
    return 0;
}
