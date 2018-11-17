#include <iostream>
#include <fstream>
#include <mpi.h>
#include <omp.h>

#include "algorithms/parallel_sollin.hpp"
#include "algorithms/sollin.hpp"
#include "algorithms/kruskal.hpp"
#include "algorithms/filter_kruskal.hpp"
#include "algorithms/prim.hpp"

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


void lsb_time(string output_file, int n_threads){

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
	cout << "Creating graphs";
	list<Graph*> g_list;
	for (int i: size) {
		g_list.push_back(new Graph(i, (float) edgePerVertex / i, minWeight, maxWeight));
		cout << ".";
	}
	cout << " done." << endl;

	// Create timer
	LsbTimer t(l, output_file, n_threads);

	// Time
	t.clock(g_list);
}
    

/*
 * PARSING AND ARGUMENTS HELP
 */


void print_help(int rank){
    if (rank  == 0)
        cout << endl << "USAGE: ./bin/exec lsb_time <filename> <max threads>" << endl << endl;
}

void parse(int *argc, char **argv[], int rank){
    // Check the number of parameters
    if (*argc < 3) {
        print_help(rank);
        return;
    }

    // Use the first parameter as the main one. Others can be used as options
    string arg = (*argv)[1];
    if (arg == "lsb_time") {
        // Get args
        string fn = (*argv)[2];
        int n_threads = atoi((*argv)[3]);
        // call time function
        lsb_time(fn, n_threads);
        return;
    }

    print_help(rank);
}


/*
 * MAIN: keep it simple
 */

int main(int argc, char *argv[]){
    // Initialize MPI
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    parse(&argc, &argv, rank);

    MPI_Finalize();
    return 0;
}
