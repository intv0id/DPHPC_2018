#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>

#include <omp.h>

#include "graph.hpp"
#include "common.hpp"

using namespace std;


struct result{
	result() : firstSource(-1),firstTarget(-1),lastSource(-1),lastTarget(-1) {}
	l_edge_t list;
	int firstSource, firstTarget;
	int lastSource, lastTarget;
};


void merge(result& v1, result& v2){
	if(v1.lastSource == v2.firstSource && v1.lastTarget == v2.firstTarget ){
		v2.list.pop_front();
	}
	v1.lastSource = v2.lastSource;
	v1.lastTarget = v2.lastTarget;
	v1.list.splice(v1.list.end(),v2.list);
}

void findmin(v_edge_t& v1, v_edge_t& v2){
	for(int i = 0; i < v1.size(); i++){
		if(v1[i] -> source == -1){
			v1[i] = v2[i];
		}else if(v2[i] -> source != -1){
			int w1 = v1[i] -> weight;
			int w2 = v2[i] -> weight;
			if(w2 < w1){
				v1[i] = v2[i];
			}
		}

	}
}

#pragma omp declare reduction \
	(findMin:v_edge_t:findmin(omp_out,omp_in)) \
	initializer(omp_priv(omp_orig))

#pragma omp declare reduction \
	(listEdges:result:merge(omp_out,omp_in))


#pragma omp declare reduction \
	(addEdges:v_edge_t: omp_out.insert(omp_out.end(),omp_in.begin(),omp_in.end()))


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

	bool operator()(edge*e1, edge* e2){
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

l_edge_t parallel_sollin(Graph g){

	// Get graph data
	int n = g.n;
	v_edge_t vectorEdges;
	for(l_edge_it it = g.edges.begin(); it != g.edges.end();it++){
		vectorEdges.push_back(*it);
	}
	
	// Create MST
	l_edge_t mst;

	// Create union-find structure
	union_find* u = new union_find(n);
	comp c(u);

	// While not connected
	while(u->numTrees > 1){	
		#ifdef DEBUG
		cout << "Number of Trees: " << u->numTrees << endl;
		#endif

		// Remove self-loops and multiple edges (compact graph)
		result aux;
		int k;
		int nEdges = vectorEdges.size();
		#pragma omp parallel for num_threads(4) reduction(listEdges:aux)
		for(k = 0; k < nEdges; k++){
			edge* e = vectorEdges[k];
			int p1, p2;
			#pragma omp critical
			{
				p1 = u->find(e->source);
				p2 = u->find(e->target);
			}
			if(aux.firstSource == -1){
				#ifdef DEBUG
				int ID = omp_get_thread_num();
				cout << ID << endl;
				cout << k << endl;
				#endif
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

		// Update vectorEdges
		vectorEdges = vector<edge*>(0);
		for(l_edge_it it = aux.list.begin(); it != aux.list.end();it++){
			vectorEdges.push_back(*it);
		}

		// Find minimum ingoing edge
		edge* einit = new edge();
		einit->source = -1;

		// Do loop in parallel

		nEdges = vectorEdges.size();
		v_edge_t cheapest(n,einit);
		#pragma omp parallel for num_threads(4) reduction(findMin:cheapest)
		for(k = 0; k < nEdges; k++){
			edge* e = vectorEdges[k];

			#ifdef DEBUG
			cout << "Got edge " << endl;
			#endif

			int source, target;
			#pragma omp critical
			{
				source = u->find(e->source);
				target = u->find(e->target);
			}
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
		
		vector<edge*> add_to_mst;
		// Connect the components via pointer-jump
		#pragma omp parallel for num_threads(4) reduction(addEdges:add_to_mst)
		for(k = 0; k < n; k++){
			edge* e = cheapest[k];
			if(e->source != -1){
				bool b;
				#pragma omp critical
				{
				b = u->unite(e->source,e->target);
				}
				if (b) add_to_mst.push_back(e);
			}
		}
		mst.insert(mst.end(),add_to_mst.begin(),add_to_mst.end());

					
	}
	return mst;
}




