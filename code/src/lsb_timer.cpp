#include <iostream>
#include <fstream>
#include <string>

#include <liblsb.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>

#include <numeric>
#include "tbb/task_scheduler_init.h"

#include "graph.hpp"
#include "lsb_timer.hpp"
#include "verifier.hpp"

using namespace std;

LsbTimer::LsbTimer(list<mst_algorithm*> l, string fname, unsigned int m_threads, unsigned int n_runs) : algorithms(l){
    filename = fname;
    max_threads = m_threads;
    runs = n_runs;
}

void LsbTimer::clock(list<Graph*> g_list)
{
    unsigned int counter = 0; // Records counter
    double t1, t2;

    omp_set_dynamic(0);

    // srand(time(NULL));
    srand(42);
	LSB_Init(filename.c_str(), time(NULL));

	for(mst_algorithm* mst_alg : algorithms){
        mst_algorithm &mst_algo = *mst_alg;
        LSB_Set_Rparam_string("Algorithm", mst_algo.name.c_str());

		for (Graph* g : g_list) {
            Graph &graph = *g;
            LSB_Set_Rparam_int("Graph_nodes", graph.n);
            LSB_Set_Rparam_string("Graph_name", graph.name.c_str());

            for (unsigned int thread_nb=1; thread_nb <= max_threads; thread_nb *= 2){
                omp_set_num_threads(thread_nb);
                tbb::task_scheduler_init tbb_scheduler(thread_nb);
                LSB_Set_Rparam_int("Max_threads", thread_nb);

                vector<double> times;

                for (unsigned int run = 0; run < runs; run++) {
                    LSB_Set_Rparam_int("run", run);
                    cout << "Algorithm: " << mst_algo.name.c_str();
                    cout << "\t; Nodes number:" << graph.n;
                    cout << "\t; Max threads: " << thread_nb;
                    cout << "\t; Run " << run << endl;

                    double t1 = omp_get_wtime();

                    /* Reset the counters */
                    LSB_Res();

                    /* Perform the operation */
                    l_edge_t mst = mst_algo.algorithm(graph, thread_nb);


                    /* Register the run-th measurement of id counter*/
		    LSB_Set_Rparam_string("Internal Measurements Names", mst_algo.get_measurements_names().c_str());
		    LSB_Set_Rparam_string("Internal Measurements Values", mst_algo.get_measurements_values().c_str());
                    LSB_Rec(counter);
		    mst_algo.internal_timings.clear();



                    double t2 = omp_get_wtime();
                    times.push_back(t2 - t1);

		    // Increment counter
		    counter++;
                }
                print_times(times);

            }
		}
	}
	LSB_Finalize();
}

void LsbTimer::print_times(vector<double> times){
    double tm;

    cout << endl;
    cout << "Time (approximation over " << runs << " runs) |";
    cout << "\tmean= "<< ( tm = mean(times, runs) ) <<"s |";
    cout << "\tmed=" << median(times) << "s |";
    cout << "\tstd=" << std(times, tm) <<"s"<< endl;
    cout << endl;
}

double LsbTimer::mean(vector<double> times, int runs){
    return accumulate(times.begin(), times.end(), 0.0) / (double) runs;
}

double LsbTimer::std(vector<double> times, double m){
    double acc = 0.0;

    for_each (
            times.begin(), times.end(),
            [&](const double t) { acc += (t - m) * (t - m); }
    );

    return sqrt(acc / (times.size()-1));
}

double LsbTimer::median(vector<double> times){
    size_t size = times.size();
    sort(times.begin(), times.end());
    if (size % 2 == 0)
        return (times[size / 2 - 1] + times[size / 2]) / 2;
    else
        return times[size / 2];
}
