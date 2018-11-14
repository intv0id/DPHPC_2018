#include <iostream>
#include <fstream>
#include <mpi.h>
#include <omp.h>

#include "algorithms/parallel_sollin.hpp"
#include "algorithms/sollin.hpp"
#include "algorithms/kruskal.hpp"
#include "algorithms/filter_kruskal.hpp"
#include "algorithms/prim.hpp"

#include "tbb/task_scheduler_init.h"
#include "graph.hpp"
#include "common.hpp"
#include "timer.hpp"
#include "lsb_timer.hpp"
#include "verifier.hpp"
#include "experiments.hpp"


using namespace std;

/*
 * CUSTOM FUNCTIONS SPACE
 */


void lsb_time(){

	// Declare graph params
	int edgePerVertex = 20;
	vector<int> size = {3000,10000,30000,100000};
	int minWeight = 0;
	int maxWeight = 100;

	// Declare algorithms
	list<mst_algorithm*> l;
	l.push_back(new sollin());
	l.push_back(new parallel_sollin_EL());
	l.push_back(new parallel_sollin_AL());

	// Declare graphs
	list<Graph*> g_list;
	for (int i: size) {
		g_list.push_back(new Graph(i, (float) edgePerVertex / i, minWeight, maxWeight));
	}

	// Create timer
	LsbTimer t(l);

	// Time
	t.clock(g_list);
}


/*
 * PARSING AND ARGUMENTS HELP
 */

void print_help(int rank){
    if (rank  == 0)
        cout << endl << "USAGE: ./bin/exec [ lsb_time ]" << endl << endl;
}

void parse(int *argc, char **argv[], int rank){
    // Check the number of parameters
    if (*argc < 2) {
        print_help(rank);
        return;
    }

    // Use the first parameter as the main one. Others can be used as options
    string arg = (*argv)[1];
    if (arg == "lsb_time") {
        lsb_time();
    } else {
        print_help(rank);
    }
}


/*
 * MAIN: keep it simple
 */

int main(int argc, char *argv[]){

    // Init tbb
    tbb::task_scheduler_init init(4);

    // Initialize MPI
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    parse(&argc, &argv, rank);

    MPI_Finalize();
    return 0;
		
}
