#include <iostream>
#include <fstream>
#include <string>

#include <liblsb.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>

#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include "graph.hpp"
#include "lsb_timer.hpp"



#define RUNS 5 //TODO: to define

typedef boost::graph_traits<Boost_Graph>::edge_descriptor Boost_Edge;
using namespace std;

LsbTimer::LsbTimer(list<mst_algorithm*> l, string fname, unsigned int m_threads) : algorithms(l){
    filename = fname;
    max_threads = m_threads;
}

void LsbTimer::clock(list<Graph*> g_list)
{
	srand(time(NULL));

	LSB_Init(filename.c_str(), time(NULL));

	for(mst_algorithm* mst_alg : algorithms){
        mst_algorithm &mst_algo = *mst_alg;

		for (Graph* g : g_list) {
            Graph &graph = *g;

            for (unsigned int thread_nb=1; thread_nb <= max_threads; thread_nb *= 2){
                omp_set_num_threads(thread_nb);
                LSB_Set_Rparam_int("Max threads", thread_nb);

                for (unsigned int run = 0; run < RUNS; run++) {
                    LSB_Set_Rparam_int("run", run);
                    cout << "Algorithm: " << mst_algo.name.c_str();
                    cout << "\t; Nodes number:" << graph.n;
                    cout << "\t; Max threads: " << thread_nb ;
                    cout <<"\t; Run " << run <<endl;

                    /* Reset the counters */
                    LSB_Res();

                    /* Perform the operation */
                    l_edge_t mst = mst_algo.algorithm(graph, thread_nb);

                    /* Register the run-th measurement with the number of node of the current graph */
                    LSB_Rec(graph.n);
                }
            }
		}
	}
	LSB_Finalize();
}
