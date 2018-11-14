#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>

#include "algorithms/sollin.hpp"
#include "common.hpp"
#include "graph.hpp"

using namespace std;

l_edge_t sollin::algorithm(Graph& g){

	// Get graph data
	int n = g.n;
	l_edge_t aux = g.edges;
	
	// Create MST
	l_edge_t mst;

	// Create union-find structure
	union_find* u = new union_find(n);
	comp c(u);

	// Create number of components variable

	// While not connected
	while(u->numTrees > 1){	
		// Sort the edge list according to supervertex
		aux.sort(c);	

		// Remove self-loops and multiple edges (compact graph)
		int source = -1;
		int target = -1;
		for(l_edge_it it = aux.begin(); it != aux.end();){
			edge* e = *it;
			int p1 = u->find(e->source);
			int p2 = u->find(e->target);
			if(p1 == p2){
				aux.erase(it++);
			}
			else if(p1 == source && p2 == target){
				aux.erase(it++);
			}
			else{
				it++;
			}
			source = p1;
			target = p2;
		}


		// Find minimum ingoing edge
		edge* einit = new edge();
		einit->source = -1;

		vector<edge*> cheapest(n,einit);
		for(l_edge_it it = aux.begin(); it != aux.end(); it++){
			edge* e = *it;

			#ifdef DEBUG
			cout << "Got edge " << endl;
			#endif
			
			int source = u->find(e->source);
			int weight = e->weight;
			#ifdef DEBUG
			cout << "Checking conditions " << endl;
			cout << "Source: " << e->source << endl;
			cout << "Target: " << e->target << endl;
			cout << "Weight: " << weight << endl;
			#endif

			if(cheapest[source]->source == -1 ||
			weight < cheapest[source]->weight){
				cheapest[source] = e;
			}
		}

		// Connect the components via pointer-jump
		for(int i = 0; i != n; i++){
			edge* e = cheapest[i];
			// Merge the two components
			if(e->source != -1){
				bool b = u->unite(e->source,e->target);
				if (b) mst.push_back(e);
			}
		}
		#ifdef DEBUG
		cout << "Found minimum edge " << endl;
		#endif
		
				
	}
	return mst;
}
