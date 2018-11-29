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


typedef boost::graph_traits<Boost_Graph>::edge_descriptor Boost_Edge;
using namespace std;

LsbTimer::LsbTimer(list<mst_algorithm*> l, string fname, unsigned int m_threads, unsigned int n_runs) : algorithms(l){
    filename = fname;
    max_threads = m_threads;
    runs = n_runs;
}

void LsbTimer::clock(list<Graph*> g_list)
{
    unsigned int counter = 0; // Records counter

    omp_set_dynamic(0);

    srand(time(NULL));
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
                LSB_Set_Rparam_int("Max_threads", thread_nb);

                for (unsigned int run = 0; run < runs; run++) {
                    LSB_Set_Rparam_int("run", run);
                    cout << "Algorithm: " << mst_algo.name.c_str();
                    cout << "\t; Nodes number:" << graph.n;
                    cout << "\t; Max threads: " << thread_nb ;
                    cout <<"\t; Run " << run << endl;

                    /* Reset the counters */
                    LSB_Res();

                    /* Perform the operation */
                    l_edge_t mst = mst_algo.algorithm(graph, thread_nb);

                    /* Register the run-th measurement of id counter*/
                    LSB_Rec(counter++);
                }
            }
		}
	}
	LSB_Finalize();
}
