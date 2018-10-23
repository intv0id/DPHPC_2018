#include <iostream>
#include <sollin.hpp>
#include <graph.hpp>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>

using namespace std;

typedef vector<node*> v_node_t; 
typedef vector<edge*> v_edge_t;
typedef list<edge_EL*> v_edge_EL_t;

typedef vector<node*>::iterator v_node_it;
typedef vector<edge*>::iterator v_edge_it;
typedef list<edge_EL*>::iterator v_edge_EL_it;


class union_find{
	public:
	vector<int> parents;
	int size;
	int numTrees;
	union_find(int n) : parents(n), size(n), numTrees(n) {
		for(int i = 0; i != n; i++){
			parents[i] = i;
		}
	}
	int find(int x){
		if(parents[x] != x){
			parents[x] = find(parents[x]);
			return parents[x];
		}
		return x;
	}
	bool unite(int x, int y){
		int px = find(x);
		int py = find(y);
		if(px != py){
			parents[py] = px;
			numTrees--;
			return true;
		}
		return false;
	}
};

class comp{
	union_find u;

	public:
	comp(union_find u_): u(u_){}
	bool operator()(edge_EL*e1, edge_EL* e2){
		int p1 = u.find(e1->source);
		int p2 = u.find(e2->source);
		if(p1 < p2) return true;
		if(p1 > p2) return false;
		int p3 = u.find(e1->target);
		int p4 = u.find(e2->target);
		if(p3 < p4) return true;
		if(p3 < p4) return false;
		int w1 = e1->weight;
		int w2 = e2->weight;
		return (w1 <= w2);
	}
	
};

v_edge_EL_t sollin(Graph_EL g){

	// Get graph data
	int n = g.n;
	v_edge_EL_t aux = g.edges;
	
	// Create MST
	v_edge_EL_t mst;

	// Create union-find structure
	union_find u(n);
	comp c(u);

	// Create number of components variable

	// While not connected
	while(u.numTrees > 1){	
		#ifdef DEBUG
		cout << "Number of Trees: " << u.numTrees << endl;
		#endif
		// Sort the edge list according to supervertex
		aux.sort(c);	

		// Remove self-loops and multiple edges (compact graph)
		int source = -1;
		int target = -1;
		for(v_edge_EL_it it = aux.begin(); it != aux.end();){
			edge_EL* e = *it;
			int p1 = u.find(e->source);
			int p2 = u.find(e->target);
			if(p1 == p2) aux.erase(it++);
			else if(p1 == source && p2 == target){
				aux.erase(it++);
			}
			else{
				it++;
			}
			source = p1;
			target = p2;
		}

		#ifdef DEBUG
		cout << "Removed self-loops " << endl;
		#endif

		// Find minimum ingoing edge
		edge_EL* einit = new edge_EL();
		einit->source = -1;

		vector<edge_EL*> cheapest(u.numTrees,einit);
		for(v_edge_EL_it it = aux.begin(); it != aux.end(); it++){
			edge_EL* e = *it;

			#ifdef DEBUG
			cout << "Got edge " << endl;
			#endif
			
			int source = u.find(e->source);
			int weight = e->weight;
			#ifdef DEBUG
			cout << "Checking conditions " << endl;
			cout << "Source: " << source << endl;
			#endif

			if(cheapest[source]->source == -1 ||
			weight < cheapest[source]->weight){
				cheapest[source] = e;
			}
		}

		#ifdef DEBUG
		cout << "Found minimum edge " << endl;
		#endif
		
		// Connect the components via pointer-jump
		for(int i = 0; i != cheapest.size(); i++){
			#ifdef DEBUG
			cout << "i: " << i << endl;
			#endif
			edge_EL* e = cheapest[i];
			// Merge the two components
			bool b = u.unite(e->source,e->target);
			if (b) mst.push_back(e);
		}
				
	}
	return mst;
}




