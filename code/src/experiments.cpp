#include "graph.hpp"
#include "timer.hpp"
#include "common.hpp"
#include "filter_kruskal.hpp"
#include "kruskal.hpp"
#include "sollin.hpp"
#include "parallel_sollin.hpp"
#include "prim.hpp"

using namespace std;

void time_variable_size(){

	int edgePerVertex = 20;
	int nTrials = 3;
	//vector<int> size = {100, 200};
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

void time_variable_nthreads(){

	int edgePerVertex = 20;
	int nTrials = 3;
	vector<int> size = {100};
	//vector<int> size = {3000,10000,30000,100000};
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

void time_variable_nedges(){

	int edgePerVertex = 20;
	int nTrials = 3;
	vector<int> size = {100};
	//vector<int> size = {3000,10000,30000,100000};
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
