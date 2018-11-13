#include <iostream>
#include <fstream>
#include <string>

#include <omp.h>

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

void LsbTimer::time(list<Graph&> g_list){

	srand(time(NULL));
	LSB_Init("Algo time", 0);

	LSB_Set_Rparam_int("runs", RUNS);

	for(mst_algorithm* mst_alg : algorithms){
		LSB_Set_Rparam_int("algo name", mst_algo.name);

		for (Graph& g : g_list) {
			for (run=0; run<RUNS; run++) {
				mst_algorithm &mst_algo = *mst_alg;

				/* Reset the counters */
				LSB_Res();

				/* Perform the operation */
				l_edge_t mst = mst_algo.algorithm(g);

				/* Register the run-th measurement with the number of node of the current graph */
				LSB_Rec(g.n);
			}
		}
	}

	LSB_Finalize();
}
