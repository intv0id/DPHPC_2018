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

	// Test with a second graph
	g = Graph_EL(9); 
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


	#ifdef DEBUG 
	cout << "Graph is constructed" << endl;
	#endif

	mst = sollin(g);
	print_edge_EL_list(mst);

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

}

int main(int argc, char *argv[]){
	#ifdef DEBUG 
	cout << "Hello world!" << endl;
	#endif

	test_sollin();

	return 0;
		
}
