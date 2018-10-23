#include <iostream>
#include <liblsb.h>
#include <mpi.h>


#include <graph.hpp>
#include "sollin.hpp"
#include "common.hpp"
#include "algorithms.hpp"

#define N 100
#define RUNS 10

using namespace std;

typedef list<edge_EL*> v_edge_EL_t;


void test_sollin(){

	#ifdef DEBUG
	cout << "Defining graph" << endl;
	#endif

	// Test with a simple graph
	Graph_EL g(4); 
	// Add a few edges
	g.add_edge(0,1,1);
	g.add_edge(1,2,2);
	g.add_edge(2,3,1);
	g.add_edge(1,3,3);

	#ifdef DEBUG 
	cout << "Graph is constructed" << endl;
	#endif

	v_edge_EL_t mst = sollin(g);
	print_edge_EL_list(mst);
}

void test_kruskal(){
    #ifdef DEBUG
	cout << "Defining graph" << endl;
	#endif
	
	Graph g(10,0.5,0,10);
	
	#ifdef DEBUG 
	cout << "Graph is constructed : " << endl;
	g.printGraph();
	#endif
	
	Kruskal k(g);			
	vector<edge_C> MST = k.compute();
}

int main(int argc, char *argv[]){
	#ifdef DEBUG 
	cout << "Hello world!" << endl;
	#endif
	
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

	return 0;
		
}
