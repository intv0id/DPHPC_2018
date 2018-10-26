#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>

#include <omp.h>

#include <graph.hpp>

using namespace std;

typedef vector<node*> v_node_t; 
typedef vector<edge*> v_edge_t;
typedef list<edge_EL*> l_edge_EL_t;
typedef vector<edge_EL*> v_edge_EL_t;

typedef vector<node*>::iterator v_node_it;
typedef vector<edge*>::iterator v_edge_it;
typedef list<edge_EL*>::iterator l_edge_EL_it;

struct result{
	result() : firstSource(-1),firstTarget(-1),lastSource(-1),lastTarget(-1) {}
	l_edge_EL_t list;
	int firstSource, firstTarget;
	int lastSource, lastTarget;
};


result merge(result v1, result v2){
	if(v1.lastSource == v2.firstSource && v1.lastTarget == v2.firstTarget ){
		v2.list.pop_front();
	}
	result r;
	r.firstSource = v1.firstSource;
	r.lastSource = v1.lastSource;
	r.firstTarget = v1.firstTarget;
	r.lastTarget = v1.lastTarget;
	v1.list.splice(v1.list.end(),v2.list);
	r.list = v1.list;
	return r;
}


#pragma omp declare reduction \
	(listEdges:result:omp_out=merge(omp_out,omp_in))



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
	union_find* u;

	public:
	comp(union_find* u_): u(u_){}

	bool operator()(edge_EL*e1, edge_EL* e2){
		int p1 = u->find(e1->source);
		int p2 = u->find(e2->source);
		if(p1 < p2) return true;
		if(p1 > p2) return false;
		int p3 = u->find(e1->target);
		int p4 = u->find(e2->target);
		if(p3 < p4) return true;
		if(p3 > p4) return false;
		int w1 = e1->weight;
		int w2 = e2->weight;
		return (w1 <= w2);
	}
	
};

l_edge_EL_t parallel_sollin(Graph_EL g){

	// Get graph data
	int n = g.n;
	l_edge_EL_t aux = g.edges;
	
	// Create MST
	l_edge_EL_t mst;

	// Create union-find structure
	union_find* u = new union_find(n);
	comp c(u);

	// Create number of components variable

	// While not connected
	while(u->numTrees > 1){	
		#ifdef DEBUG
		cout << "Number of Trees: " << u->numTrees << endl;
		#endif
		// Sort the edge list according to supervertex
		aux.sort(c);	

		// Remove self-loops and multiple edges (compact graph)
		int source = -1;
		int target = -1;
		
		// Copy everything to a vector
		v_edge_EL_t vectorEdges;
		for(l_edge_EL_it it = aux.begin(); it != aux.end();it++){
			vectorEdges.push_back(*it);
		}

		#ifdef DEBUG
		cout << "Launching reduction" << endl;
		#endif
		
		result aux;
		#pragma omp parallel for num_threads(4) reduction(listEdges:aux)
		for(int k = 0; k != vectorEdges.size(); k++){
			cout << k << endl;
			edge_EL* e = vectorEdges[k];
			int p1 = u->find(e->source);
			int p2 = u->find(e->target);
			if(aux.firstSource == -1){
				aux.firstSource = p1;
				aux.firstTarget = p2;
			}
			if(p1 != p2  && (p1 != aux.lastSource || p2 != aux.lastTarget)){
				aux.list.push_back(e);
				aux.lastSource = p1;
				aux.lastTarget = p2;
			}
		}

		#ifdef DEBUG
		cout << endl << "Removed self-loops " << endl;
		cout << "Size: " << aux.list.size() << endl;
		#endif

		// Find minimum ingoing edge
		edge_EL* einit = new edge_EL();
		einit->source = -1;


		v_edge_EL_t cheapest(n,einit);
		for(l_edge_EL_it it = aux.list.begin(); it != aux.list.end(); it++){
			edge_EL* e = *it;

			#ifdef DEBUG
			cout << "Got edge " << endl;
			#endif
			
			int source = u->find(e->source);
			int target = u->find(e->target);
			int weight = e->weight;
			#ifdef DEBUG
			cout << "Checking conditions " << endl;
			cout << "Source: " << e->source << endl;
			cout << "Source comp: " << source << endl;
			cout << "Target: " << e->target << endl;
			cout << "Target comp: " << target << endl;
			cout << "Weight: " << weight << endl;
			#endif

		   	
			if(cheapest[source]->source == -1 || weight < cheapest[source]->weight){
				cheapest[source] = e;
			}
		}

		#ifdef DEBUG
		cout << "Found minimum edge " << endl;
		#endif
		
		// Connect the components via pointer-jump
		for(int i = 0; i != n; i++){
			edge_EL* e = cheapest[i];
			// Merge the two components
			if(e->source != -1){
				bool b = u->unite(e->source,e->target);
				if (b) mst.push_back(e);
			}
		}
		#ifdef DEBUG
		cout << "Connect " << endl;
		#endif
				
	}
	return mst;
}




