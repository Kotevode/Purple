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
#include <vector>
#include <purple/JobInfo.h>
#include <iomanip>

#define FILE_PATH   "results.csv"
#define M_MIN       50
#define M_MAX       1000
#define M_STEP      M_MAX / 100
#define N_MIN       2
#define N_MAX       50
#define N_STEP      1
#define AVG_WEIGHT  100
#define MAX_SD      AVG_WEIGHT / 3
#define MIN_SD      0
#define SD_STEP     1

using namespace Purple;
using namespace std;

random_device rd;

vector<JobInfo> generate_jobs(int n, int SD) {
    GeneratingJob j(AVG_WEIGHT, SD, rd);
    vector<JobInfo> result(n);
    generate(result.begin(), result.end(), [&]() -> JobInfo {
        return JobInfo(j, 0);
    });
    return result;
}

double v_value(vector<JobInfo> &jobs, int node_number) {
    long long node_load[node_number];
    memset(node_load, 0, sizeof(long long) * node_number);
    for_each(jobs.begin(), jobs.end(), [&](auto &t) {
        node_load[t.get_node_number()] += t.get_weight();
    });

    double avg = 0;
    for_each(node_load, node_load + node_number, [&](int i) {
        avg += i;
    });
    avg /= node_number;

    double sd = 0;
    for_each(node_load, node_load + node_number, [&](int nl) {
        sd += pow(nl - avg, 2);
    });
    sd = sqrt(sd / node_number);

    return sd / avg;
}

void make_test(void (*d_func)(vector<JobInfo>::iterator, vector<JobInfo>::iterator, size_t),
               vector<JobInfo> &jobs, int N, ofstream &f) {
    clock_t begin = clock();
    d_func(jobs.begin(), jobs.end(), N);
    clock_t end = clock();
    auto v = v_value(jobs, N);
    f << v << ";";
    cout << v << "\t";
    auto time = ((double) end - begin) / CLOCKS_PER_SEC;
    f << time << ";";
    cout << time << "\t";
}

int main(int argc, const char *argv[]) {

    ofstream f;
    f.open(FILE_PATH, ios::out | ios::trunc);
    f << "N;M;SD;h.dist;h.dist.time;ef.dist;ef.dist.time;" << endl;
    for (int SD = MIN_SD; SD < MAX_SD; SD += SD_STEP) {
        for (int N = N_MIN; N < N_MAX; N += N_STEP) {
            for (int M = M_MIN; M <= M_MAX; M += M_STEP) {
                cout << "M = " << setw(4) << M
                     << " N = "<< setw(4) << N
                     << " SD = "<< setw(4) << SD
                     << "\t;\t" << fixed;
                auto jobs = generate_jobs(M, SD);
                f << N << ";";
                f << M << ";";
                f << SD << ";";

                make_test(h_distribution, jobs, N, f);
                make_test(ef_distribution, jobs, N, f);

                f << endl;
                cout << endl;
            }
        }
    }
    f.close();
    return 0;
}
