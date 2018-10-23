#include <iostream>

#include <graph.hpp>
#include "sollin.hpp"
#include "common.hpp"

#include "algorithms.hpp"

using namespace std;

typedef list<edge_EL*> v_edge_EL_t;
typedef list<edge_EL*>::iterator v_edge_EL_it;

void test_sollin_graph(Graph_EL g){

}

void test_sollin(){
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

void find_a_name(){
	Graph g(10,0.5,0,10);
	g.printGraph();
	Kruskal k(g);
	vector<edge_C> MST = k.compute();

}

int main(){
	#ifdef DEBUG 
	cout << "Hello world!" << endl;
	#endif
	//find_a_name();
	
	test_sollin();
		
}
