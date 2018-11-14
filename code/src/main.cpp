#include <iostream>
#include <fstream>
#include <mpi.h>
#include <omp.h>

#include "tbb/task_scheduler_init.h"
#include "graph.hpp"
#include "parallel_sollin.hpp"
#include "sollin.hpp"
#include "common.hpp"
#include "kruskal.hpp"
#include "filter_kruskal.hpp"
#include "prim.hpp"
#include "timer.hpp"
#include "lsb_timer.hpp"
#include "verifier.hpp"


using namespace std;

void lsb_time(int *argc, char **argv[]){

	// Declare graph params
	int edgePerVertex = 20;
	vector<int> size = {3000,10000,30000,100000};
	int minWeight = 0;
	int maxWeight = 100;

	// Declare name
	string name = "plots/lsb_log_varSize_edgePerVertex="+to_string(edgePerVertex)+".txt";

	// Declare algorithms
	list<mst_algorithm*> l;
	l.push_back(new sollin("sollin"));
	l.push_back(new parallel_sollin_EL("parallel_sollin_EL"));
	l.push_back(new parallel_sollin_EL("parallel_sollin_AL"));

	// Declare graphs
	list<Graph*> g_list;
	for (int i: size) {
		g_list.push_back(new Graph(i, (float) edgePerVertex / i, minWeight, maxWeight));
	}

	// Create timer
	LsbTimer t(name, l);

	// Time
	t.clock(g_list, argc, argv);
}

void print_help(){
    cout << endl << "USAGE: ./bin/exec [ lsb_time ]" << endl << endl;
}

int main(int argc, char *argv[]){

    // Init tbb
    tbb::task_scheduler_init init(4);


    // Check the number of parameters
    if (argc < 2) {
        print_help();
        return 1;
    }

    string arg = argv[1];
    if (arg == "lsb_time") {
        lsb_time(&argc, &argv);
    } else {
        print_help();
    }
    return 0;
		
}
