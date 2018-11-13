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
void test_sollin(){

	Graph g = graph1();

	#ifdef DEBUG 
	cout << "Graph is constructed" << endl;
	#endif

	l_edge_t mst = sollin(g);
    l_edge_t mst_kruskal = seq_kruskal(g);
    l_edge_t mst_prim = seq_prim(g);

	print_edge_list(mst);
	print_edge_list(mst_kruskal);
	print_edge_list(mst_prim);

	g = graph2();

	#ifdef DEBUG 
	cout << "Graph is constructed" << endl;
	#endif

	mst = sollin(g);
	mst_kruskal = seq_kruskal(g);
    mst_prim = seq_prim(g);

	print_edge_list(mst);
	print_edge_list(mst_kruskal);
	print_edge_list(mst_prim);
}

void test_parallel_sollin(int nTrials){

	Graph g = graph1();

	l_edge_t mst = parallel_sollin_AL(g);
	print_edge_list(mst);

	Graph g2 = graph2();

	mst = parallel_sollin_AL(g2);
	print_edge_list(mst);

	for(int i = 0; i != nTrials; i++){
		// Generate a random graph and the corresponding boost graph
		Graph g(100,0.1,0,100);		
		// Apply sequential sollin
		l_edge_t mst = sollin(g);
		int weight_seq(0);
		for(l_edge_it it = mst.begin(); it != mst.end(); it++){
			weight_seq += (*it)->weight;
		}
		cout << "Weight sequential: " << weight_seq << endl;
		// Apply parallel sollin
		mst = parallel_sollin_EL(g);
		int weight_par(0);
		for(l_edge_it it = mst.begin(); it != mst.end(); it++){
			weight_par += (*it)->weight;
		}
		cout << "Weight parallel: " << weight_par << endl;
		
		// Apply boost algorithm
		vector<Boost_Edge> v;
		kruskal_minimum_spanning_tree(g.boost_rep,back_inserter(v));
		// Print all weights
		boost::property_map<Boost_Graph,boost::edge_weight_t>::type w = get(boost::edge_weight,g.boost_rep);
		int weight_kruskal_boost = 0;
		for(vector<Boost_Edge>::iterator it = v.begin(); it != v.end(); it++){
			weight_kruskal_boost += w[*it];
		}
		cout << "Weight Kruskal Boost: " << weight_kruskal_boost << endl;
	}
	

}
*/
/* Sharp connectivity threshold:
 * 10^4 -> 9.2
 * 10^5 -> 11.5
 * 10^6 -> 13.8
 * */

void time(){

	int edgePerVertex = 20;
	int nTrials = 3;
	vector<int> size = {3000,10000,30000,100000};
	int nSizes = size.size();
	int minWeight = 0;
	int maxWeight = 100;

	// Declare name
	string name = "plots/log_varSize_edgePerVertex="+to_string(edgePerVertex)+".txt";

	// Declare algorithms
	list<mst_algorithm*> l;
	l.push_back(new filter_kruskal);
	l.push_back(new kruskal);
	l.push_back(new sollin);
	l.push_back(new parallel_sollin_EL);
	// Problème !l.push_back(new seq_prim);

	// Create timer
	Timer t(name,l);
	t.printF("number of sizes",nSizes);
	t.printF("number of trials",nTrials);

	// Test a lot of random graphs and compare the weight with the boost implementation
	for(int j = 0; j != nSizes; j++){
		t.printF("size",size[j]);
		for(int i = 0; i != nTrials; i++){
			t.printF("trial",i);
			Graph g(size[j],(float)edgePerVertex/size[j],minWeight,maxWeight);		
            cout << "Graph " << g.unique_edges.size() << endl;
			t.time(g);

		}
	}
}

/*
void test_old_kruskal(){
    #ifdef DEBUG
	cout << "Defining graph" << endl;
	#endif
	
	Graph g(10,0.5,0,10);
	
	#ifdef DEBUG 
	cout << "Graph is constructed : " << endl;
	g.printGraph();
	#endif
	
	Old_Kruskal k(g);			
	vector<edge_C> MST = k.compute();
}
*/

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
        time();
    }
    return 0;
		
}
