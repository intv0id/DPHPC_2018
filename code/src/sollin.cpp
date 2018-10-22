#include <sollin.hpp>
#include <graph.hpp>
#include <vector>
#include <limits>

using namespace std;

typedef vector<node*> v_node_t; 
typedef vector<edge*> v_edge_t;

typedef vector<node*>::iterator v_node_it;
typedef vector<edge*>::iterator v_edge_it;



v_edge_t sollin(Graph g){
	v_edge_t F;
	int size = g.nodes.size();
	Graph h = g.copy();
	
	// For all edge, find incident vertice with minimum weight
	for(v_node_it it = h.nodes.begin(); it != h.nodes.end(); it++){
		edge* e;
		int w = numeric_limits<int>::max();

		// Add this edge to F
		node* n = *it;
		v_edge_t adj = n->adjacentEdges;
		for(v_edge_it ite = adj.begin(); ite != adj.end(); ite++){
			edge* aux = *ite;
			int aux1 = aux->weight;
			if(aux1 < w){
				e = aux;
				w = aux1;
			}
		}

		// Contract graph
		
		// Clean new graph
}




