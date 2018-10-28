#include <iostream>
#include <fstream>
#include <liblsb.h>
#include <mpi.h>
#include <omp.h>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

#include "graph.hpp"
#include "parallel_sollin.hpp"
#include "sollin.hpp"
#include "common.hpp"
#include "kruskal.hpp"
// #include "algorithms.hpp"

#define N 100
#define RUNS 10

using namespace std;

typedef boost::graph_traits<Boost_Graph>::edge_descriptor Boost_Edge;

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

void test_sollin(){

	Graph g = graph1();

	#ifdef DEBUG 
	cout << "Graph is constructed" << endl;
	#endif

	l_edge_t mst = sollin(g);
    l_edge_t mst_kruskal = seq_kruskal(g);
	print_edge_list(mst);
	print_edge_list(mst_kruskal);

	g = graph2();

	#ifdef DEBUG 
	cout << "Graph is constructed" << endl;
	#endif

	mst = sollin(g);
	mst_kruskal = seq_kruskal(g);
	print_edge_list(mst);
	print_edge_list(mst_kruskal);
}

void test_parallel_sollin(int nTrials){

	Graph g = graph1();

	l_edge_t mst = parallel_sollin(g);
	print_edge_list(mst);

	g = graph2();

	mst = parallel_sollin(g);
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
		mst = parallel_sollin(g);
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
		int weight_boost = 0;
		for(vector<Boost_Edge>::iterator it = v.begin(); it != v.end(); it++){
			weight_boost += w[*it];
		}
		cout << "Weight Boost: " << weight_boost << endl;
	}
	

}

void time_sollin(int nTrials){

	ofstream myfile;
	myfile.open("plots/sollin.txt");

	// Test a lot of random graphs and compare the weight with the boost implementation
	vector<int> size = {10,30,100,300,1000,3000,10000};

	myfile << size.size() << endl;
	myfile << nTrials << endl;
	// Test a lot of random graphs and compare the weight with the boost implementation
	for(int j = 0; j != size.size(); j++){

		myfile << size[j] << endl;
		for(int i = 0; i != nTrials; i++){

			double time1,time2;

			// Generate a random graph and the corresponding boost graph
			Graph g(size[j],100./size[j],0,100);		
			// Apply sequential sollin
			time1 = omp_get_wtime();
			l_edge_t mst = sollin(g);
			time2 = omp_get_wtime();
			myfile << time2 - time1 << " ";

			int weight_seq(0);
			for(l_edge_it it = mst.begin(); it != mst.end(); it++){
				weight_seq += (*it)->weight;
			}
			cout << "Weight sequential: " << weight_seq << endl;
			// Apply parallel sollin
			time1 = omp_get_wtime();
			mst = parallel_sollin(g);
			time2 = omp_get_wtime();
			myfile << time2 - time1 << " ";

			int weight_par(0);
			for(l_edge_it it = mst.begin(); it != mst.end(); it++){
				weight_par += (*it)->weight;
			}
			cout << "Weight parallel: " << weight_par << endl;
			
			// Apply boost algorithm
			vector<Boost_Edge> v;
			time1 = omp_get_wtime();
			kruskal_minimum_spanning_tree(g.boost_rep,back_inserter(v));
			time2 = omp_get_wtime();
			myfile << time2 - time1 << " ";

			// Print all weights
			boost::property_map<Boost_Graph,boost::edge_weight_t>::type w = get(boost::edge_weight,g.boost_rep);
			int weight_boost = 0;
			for(vector<Boost_Edge>::iterator it = v.begin(); it != v.end(); it++){
				weight_boost += w[*it];
			}
			cout << "Weight Boost: " << weight_boost << endl;

			myfile << endl;
		}
	}
	myfile.close();
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


void test_kruskal(){

	Graph g = graph1();

	#ifdef DEBUG 
	cout << "Graph is constructed" << endl;
	#endif

	l_edge_t mst = seq_kruskal(g);
	print_edge_list(mst);

	g = graph2();

	#ifdef DEBUG 
	cout << "Graph is constructed" << endl;
	#endif

	mst = seq_kruskal(g);
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

}

int main(int argc, char *argv[]){
	#ifdef DEBUG 
	cout << "Hello world!" << endl;
	#endif
	
	int i;
	cin >> i;
	if(i == 0){
		test_sollin();
	}else if(i == 1){
		int nTrials;
		cin >> nTrials;
		test_parallel_sollin(nTrials);
	}else if(i == 2){
		int nTrials;
		cin >> nTrials;
		time_sollin(nTrials);
	}
    else if (i == 3) {
        test_kruskal();
    }

	return 0;
		
}
