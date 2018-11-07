#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>

#include "tbb/parallel_sort.h"
#include "omp.h"

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


l_edge_t parallel_sollin_EL(Graph& g){


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
		//cout << "Number of Trees: " << u->numTrees << endl;
		// Sort the edge list and supervertex
		tbb::parallel_sort(vectorEdges.begin(),vectorEdges.end(),c);
		//cout << "Sorted " << endl;

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
		vectorEdges.clear();
		for(l_edge_it it = aux.list.begin(); it != aux.list.end();it++){
			vectorEdges.push_back(*it);
		}

		// Find minimum ingoing edge
		edge* einit = new edge();
		einit->source = -1;

		// Do loop in parallel

		nEdges = vectorEdges.size();
		v_edge_t cheapest(n,einit);
		#pragma omp parallel for num_threads(1) reduction(findMin:cheapest)
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

struct result_AL{
	result_AL() : firstSource(-1),lastSource(-1) {}
	list<vertex_adjacency_list*> liste;
	int firstSource,lastSource;
};

void merge_AL(result_AL& v1, result_AL& v2){
	if(v1.lastSource == v2.firstSource){
		v1.liste.back()->adjacent_vertices.insert(v1.liste.back()->adjacent_vertices.begin(),
				v2.liste.front()->adjacent_vertices.begin(),v2.liste.back()->adjacent_vertices.end());
		v2.liste.pop_front();
	}
	v1.lastSource = v2.lastSource;
	v1.liste.splice(v1.liste.end(),v2.liste);
}

#pragma omp declare reduction \
	(compactVertexAL:result_AL:merge_AL(omp_out,omp_in))

l_edge_t parallel_sollin_AL(Graph& g){

	// Copy adjacency list
	vector<vertex_adjacency_list*> edges(g.n);
	for(int i = 0; i != g.adjacency_list.size(); i++){
		vertex_adjacency_list* v = new vertex_adjacency_list();
		v->index = g.adjacency_list[i]->index;
		v->adjacent_vertices = g.adjacency_list[i]->adjacent_vertices;
		edges[i] = v;	
	}
	// Create MST
	l_edge_t mst;

	// Create union-find structure
	union_find* u = new union_find(g.n);
	compVertex cV(u);
	compTargetVertex cTV(u);

	// While not connected
	while(u->numTrees > 1){	
		#ifdef DEBUG
		cout << "Number of Trees: " << u->numTrees << endl;
		#endif
		// Sort by parent vertex
		sort(edges.begin(),edges.end(),cV);


		//cout << "Sorted vertices"  << endl;

		// Merge same parent vertex
		int nVertex = edges.size();
		result_AL aux;
		int k;
		#pragma omp parallel for num_threads(1) reduction(compactVertexAL:aux)
		for(k = 0; k < nVertex; k++){
			//cout << "k: " << k << endl;
			vertex_adjacency_list* adj = edges[k];
			int p1;
			#pragma omp critical
			{
				p1 = u->find(adj->index);
			}
			if(aux.firstSource == -1){
				#ifdef DEBUG
				int ID = omp_get_thread_num();
				cout << ID << endl;
				cout << k << endl;
				#endif
				aux.firstSource = p1;
			}
			if(p1 != aux.lastSource){
				aux.liste.push_back(adj);
				aux.lastSource = p1;
			}else{
	aux.liste.back()->adjacent_vertices.insert(aux.liste.back()->adjacent_vertices.end(),
		adj->adjacent_vertices.begin(),
		adj->adjacent_vertices.end());
	
			}
		}

		// Copy list in vector
		edges.clear();
		for(l_val_it it = aux.liste.begin(); it != aux.liste.end();it++){
			edges.push_back(*it);
		}

		// Sort each list by target parent vertex
		nVertex = edges.size();
		#pragma omp parallel for num_threads(4)
		for(int i = 0; i < nVertex; i++){
			edges[i]->adjacent_vertices.sort(cTV);
		}

		// For each list merge target vertex
		//cout << "Copied back in vector" << nVertex << endl;

		#pragma omp parallel for num_threads(4)
		for(int i = 0; i < nVertex; i++){
			int target = -1;
			int p1 = u->find(edges[i]->index);
			int nit = 0;
			for(l_edge_it it = edges[i]->adjacent_vertices.begin(); it != edges[i]->adjacent_vertices.end();){
				nit++;
				//cout << "nit: " << nit << endl;
				edge* e = *it;
				int p2 = u->find(e->target);
				if(p1 == p2){
					edges[i]->adjacent_vertices.erase(it++);
				}
				else if(p2 == target){
					edges[i]->adjacent_vertices.erase(it++);
				}
				else{
					it++;
				}
				target = p2;
			}
		}
		
		#ifdef DEBUG
		cout << endl << "Removed self-loops " << endl;
		#endif


		// For all vertice find minimum outgoing edge
		int nComps = edges.size();
		edge* einit = new edge();
		einit->target = -1;
		v_edge_t cheapest(nComps,einit);

		// Do loop in parallel

		#pragma omp parallel for num_threads(4)
		for(k = 0; k < nComps; k++){
			vertex_adjacency_list* val = edges[k];

			for(l_edge_it it = val->adjacent_vertices.begin(); it != val->adjacent_vertices.end(); it++){
				#ifdef DEBUG
				cout << "Got edge " << endl;
				#endif

				int target;
				#pragma omp critical
				{
					target = u->find((*it)->target);
				}
				int weight = (*it)->weight;
				edge* e = *it;

				#ifdef DEBUG
				cout << "Checking conditions " << endl;
				cout << "Source: " << e->source << endl;
				cout << "Target: " << e->target << endl;
				cout << "Weight: " << weight << endl;
				
				#endif

				
				if(cheapest[k]->target == -1 || weight < cheapest[k]->weight){
					cheapest[k] = *it;
				}
			}
		}

		#ifdef DEBUG
		cout << "Found minimum edges " << endl;
		#endif
	
		// For all vertices, connect components
		vector<edge*> add_to_mst;
		// Connect the components via pointer-jump
		#pragma omp parallel for num_threads(4) reduction(addEdges:add_to_mst)
		for(k = 0; k < nComps; k++){
			edge* e = cheapest[k];
			bool b;
			#pragma omp critical
			{
				b = u->unite(e->source,e->target);
			}
			if (b) add_to_mst.push_back(e);
		}
		mst.insert(mst.end(),add_to_mst.begin(),add_to_mst.end());

					
	}
	return mst;
}


