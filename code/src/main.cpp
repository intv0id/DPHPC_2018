#include <iostream>
#include <fstream>
#include <liblsb.h>
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
#include "verifier.hpp"
#include "experiments.hpp"
// #include "algorithms.hpp"

#define N 100
#define RUNS 10

using namespace std;


Graph graph1(){
	// Test with a simple graph
	Graph g(4); 
	// Add a few edges
	g.add_edge(0,1,1);
	g.add_edge(1,2,2);
	g.add_edge(2,3,1);
	g.add_edge(1,3,3);
	return g;

}

Graph graph2(){
	// Test with a second graph
	Graph g(9); 
	// Add a few edges
	g.add_edge(0,1,10);
	g.add_edge(0,2,12);
	g.add_edge(1,2,9);
	g.add_edge(1,3,8);
	g.add_edge(2,4,4);
	g.add_edge(3,4,7);
	g.add_edge(4,5,3);
	g.add_edge(2,5,1);
	g.add_edge(3,6,8);
	g.add_edge(3,7,5);
	g.add_edge(5,7,6);
	g.add_edge(6,7,9);
	g.add_edge(6,8,2);
	g.add_edge(7,8,11);
	return g;

	/* Must return:
	 * (0,1)
	 * (1,3)
	 * (2,5)
	 * (4,5)
	 * (3,7)
	 * (3,6)
	 * (5,7)
	 * (6,8)
	 */

}
 



/*
void test_filter_kruskal(){

	Graph g = graph1();

	l_edge_t mst = seq_filter_kruskal(g);
	print_edge_list(mst);

    if (verify(g, mst)) {
        cout << "Worked" << endl;
    }
    else {
        cout << "Didn't work" << endl;
    }

	Graph g2 = graph2();

	mst = seq_filter_kruskal(g2);
	print_edge_list(mst);

    if (verify(g2, mst)) {
        cout << "Worked" << endl;
    }
    else {
        cout << "Didn't work" << endl;
    }
}

void test_kruskal(){

	Graph g = graph1();

	l_edge_t mst = seq_kruskal(g);
	print_edge_list(mst);

    if (verify(g, mst)) {
        cout << "Worked" << endl;
    }
    else {
        cout << "Didn't work" << endl;
    }

	Graph g2 = graph2();

	mst = seq_kruskal(g2);
	print_edge_list(mst);

    if (verify(g2, mst)) {
        cout << "Worked" << endl;
    }
    else {
        cout << "Didn't work" << endl;
    }
}


void test_prim(){

        Graph g = graph1();

        #ifdef DEBUG 
        cout << "Graph is constructed" << endl;
        #endif

        l_edge_t mst = seq_prim(g);
        print_edge_list(mst);

        g = graph2();

        #ifdef DEBUG 
        cout << "Graph is constructed" << endl;
        #endif

        mst = seq_prim(g);
        print_edge_list(mst);
}


void find_a_name_2(int argc, char *argv[]){
	
	// Variables
	int rank, i, records_version;
	char const * records_filename;
	
	
	// Results file
	records_filename = "tests";
	records_version = 1;
	
	// Init MPI & LSB
	MPI_Init(&argc, &argv);
	LSB_Init(records_filename, records_version);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	// Set parameters
	LSB_Set_Rparam_int("rank", rank);
 	LSB_Set_Rparam_int("runs", RUNS);
 	
 	
 	// Start measures
 	LSB_Res();
 	
 	// Measure param
 	i = 0; 	
 	
	test_kruskal();
	test_sollin();
	
	// Stop & write measures
	LSB_Rec(i); // Measure name
	
	// Finalize MPI & LSB
	LSB_Finalize();
	MPI_Finalize();

}*/

int main(int argc, char *argv[]){

    // Init tbb
    tbb::task_scheduler_init init(4);

    cout << "Hello world!" << endl;
    cout << "Enter a command" << endl;
	
    int i;
    cin >> i;
    if (i == 2){
        time_variable_size();
    }
    return 0;
		
}
