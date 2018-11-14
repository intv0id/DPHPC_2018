#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>

#include "tbb/parallel_sort.h"
#include "omp.h"

#include "algorithms/parallel_sollin.hpp"
#include "graph.hpp"
#include "common.hpp"

#define NUM_THREADS 4

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
	for(unsigned int i = 0; i < v1.size(); i++){
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


l_edge_t parallel_sollin_EL::algorithm(Graph& g){


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
		#pragma omp parallel for ordered num_threads(NUM_THREADS) reduction(listEdges:aux)
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
		#pragma omp parallel for num_threads(NUM_THREADS) reduction(findMin:cheapest)
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
		#pragma omp parallel for num_threads(NUM_THREADS) reduction(addEdges:add_to_mst)
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

	// Get size of v1
	int t = v1.liste.size();
	int u = v2.liste.size();
	
	/*cout << "v1:" << t << endl;
	cout << "v2:" << u << endl;
	cout << "v1.firstSource: " << v1.firstSource << endl;
	cout << "v1.lastSource: " << v1.lastSource << endl;
	cout << "v2.firstSource: " << v2.firstSource << endl;
	cout << "v2.lastSource: " << v2.lastSource << endl;
	*/
	if(v1.lastSource == v2.firstSource){
		l_edge_t& ref1 = v1.liste.back()->adjacent_vertices;
		l_edge_t& ref2 = v2.liste.front()->adjacent_vertices;
		ref1.splice(ref1.end(),ref2);
		v2.liste.pop_front();
	}
	if(t == 0){
		v1.firstSource = v2.firstSource;
	}
	if(u != 0){
		v1.lastSource = v2.lastSource;
	}
	v1.liste.splice(v1.liste.end(),v2.liste);
		
	/*cout << "merged:" << v1.liste.size() << endl;
	cout << "v1.firstSource: " << v1.firstSource << endl;
	cout << "v2.lastSource: " << v2.lastSource << endl;
	cout << endl;
	*/
}

#pragma omp declare reduction \
	(compactVertexAL:result_AL:merge_AL(omp_out,omp_in))

l_edge_t parallel_sollin_AL::algorithm(Graph& g){

	// Copy adjacency list
	vector<vertex_adjacency_list*> edges(g.n);
	for(unsigned int i = 0; i != g.adjacency_list.size(); i++){
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
		//tbb::parallel_sort(edges.begin(),edges.end(),cV);
		sort(edges.begin(),edges.end(),cV);


		//cout << "Sorted vertices"  << endl;

		// Merge same parent vertex
		int nVertex = edges.size();
		int k;

		vector<result_AL> auxs(NUM_THREADS);
		#pragma omp parallel num_threads(NUM_THREADS) 
		{
			int id = omp_get_thread_num();
			#pragma omp for
			for(k = 0; k < nVertex; k++){
				vertex_adjacency_list* adj = edges[k];
				int p1;
				#pragma omp critical
				{
					p1 = u->find(adj->index);
				}
				if(auxs[id].firstSource == -1){
					#ifdef DEBUG
					int ID = omp_get_thread_num();
					cout << ID << endl;
					#endif
					auxs[id].firstSource = p1;
				}
				if(p1 != auxs[id].lastSource){
					auxs[id].liste.push_back(adj);
					auxs[id].lastSource = p1;
				}else{
					l_edge_t& ref = auxs[id].liste.back()->adjacent_vertices;
					l_edge_t& ref0 = adj->adjacent_vertices;
					ref.insert(ref.end(),
					ref0.begin(),
					ref0.end());
		
				}
			}
		}

		// Merge partial results
		result_AL aux;
		for(int k = 0; k != NUM_THREADS; k++){
			merge_AL(aux,auxs[k]);
		}

		// Copy list in vector
		edges.clear();
		list<vertex_adjacency_list*> & aLRef = aux.liste;
		for(l_val_it it = aLRef.begin(); it != aLRef.end();it++){
			edges.push_back(*it);
		}

		// Sort each list by target parent vertex
		nVertex = edges.size();
		#pragma omp parallel for num_threads(NUM_THREADS)
		for(int i = 0; i < nVertex; i++){
			edges[i]->adjacent_vertices.sort(cTV);
		}

		// For each list merge target vertex
		//cout << "Copied back in vector" << nVertex << endl;

		#pragma omp parallel for num_threads(NUM_THREADS)
		for(int i = 0; i < nVertex; i++){
			int target = -1;
			int p1 = u->find(edges[i]->index);
			int nit = 0;
			l_edge_t& ref = edges[i]->adjacent_vertices;
			for(l_edge_it it = ref.begin(); it != ref.end();){
				nit++;
				//cout << "nit: " << nit << endl;
				edge* e = *it;
				int p2 = u->find(e->target);
				if(p1 == p2){
					ref.erase(it++);
				}
				else if(p2 == target){
					ref.erase(it++);
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

		#pragma omp parallel for num_threads(NUM_THREADS)
		for(k = 0; k < nComps; k++){
			vertex_adjacency_list* val = edges[k];
			l_edge_t& ref = val->adjacent_vertices;
			for(l_edge_it it = ref.begin(); it != ref.end(); it++){
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
		#pragma omp parallel for num_threads(NUM_THREADS) reduction(addEdges:add_to_mst)
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


