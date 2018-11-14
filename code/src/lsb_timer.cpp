#include <iostream>
#include <fstream>
#include <string>

#include <liblsb.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>

#include "graph.hpp"
#include "lsb_timer.hpp"

#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

#define RUNS 10 //TODO: to define

typedef boost::graph_traits<Boost_Graph>::edge_descriptor Boost_Edge;
using namespace std;

LsbTimer::LsbTimer(string filename,list<mst_algorithm*> l) : algorithms(l)
{
	o.open(filename);
}
void LsbTimer::clock(list<Graph*> g_list)
{

	int run;

	srand(time(NULL));


	LSB_Init("Algo time", 0);
	LSB_Set_Rparam_int("runs", RUNS);

	for(mst_algorithm* mst_alg : algorithms){

        mst_algorithm &mst_algo = *mst_alg;
		LSB_Set_Rparam_string("algo name", mst_algo.name.c_str());
		for (Graph* g : g_list) {
            Graph &graph = *g;
			for (run=0; run<RUNS; run++) {
				/* Reset the counters */
				LSB_Res();

				/* Perform the operation */
				l_edge_t mst = mst_algo.algorithm(graph);

				/* Register the run-th measurement with the number of node of the current graph */
				LSB_Rec(graph.n);
			}
		}
	}
	LSB_Finalize();
}
