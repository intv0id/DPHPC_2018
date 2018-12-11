#include <iostream>
#include <iterator>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>
#include <stdlib.h>
#include "tbb/parallel_sort.h"
#include "omp.h"

#include "algorithms/filter_sollin.hpp"
#include "graph.hpp"
#include "common.hpp"
#include "pstl/execution"
#include "pstl/algorithm"

#define PAD 8

typedef vector<edge*>::iterator v_edge_it;

struct result_FAL_vector{
	result_FAL_vector() : firstSource(-1),lastSource(-1) {}
	list<component_FAL_vector*> liste;
	int firstSource,lastSource;
};

void merge_FAL_vector(result_FAL_vector& v1, result_FAL_vector& v2,const compWeight cW){

	// Get size of v1
	int t = v1.liste.size();
	int u = v2.liste.size();
	
	if(v1.lastSource == v2.firstSource){
		auto& ref1 = v1.liste.back()->liste;
		auto& ref2 = v2.liste.front()->liste;
		ref1.splice(ref1.end(),ref2);
		delete v2.liste.front();
		v2.liste.pop_front();
	}
	if(t == 0){
		v1.firstSource = v2.firstSource;
	}
	if(u != 0){
		v1.lastSource = v2.lastSource;
	}
	v1.liste.splice(v1.liste.end(),v2.liste);
		
}

int pivot(Graph& g, vector<vector<edge*>::iterator> starts, vector<vector<edge*>::iterator> ends){
	int n_samples = 128;
	vector<int> values(n_samples);

	for(int s = 0; s != n_samples; s++){
		// Choose random edge in starts-ends range
		int source = rand() % g.n;
		int size = std::distance(ends[source],starts[source]);
		int r = rand() % size;
		vector<edge*>::iterator it = starts[source];
		advance(it,r) ;
		values[s] = (*it)->weight;
	}

	tbb::parallel_sort(values.begin(),values.end());
	// caution number of nodes and edges
	int index = 5 * n_samples * g.n / g.n_edges;
        index = min(n_samples, index);
        return values[index];
}

const vector<v_edge_it> filter(Graph& g, const vector<vector<edge*>::iterator> starts, const vector<vector<edge*>::iterator> ends, union_find* u ){

	vector<v_edge_it> middles(g.n);
	for(int i = 0; i != g.n; i++){
	    middles[i] = partition(pstl::execution::par,
                     starts[i], ends[i],
                     [u](edge* e) {
                     return u->find(e->source) != u->find(e->target);
		     });
	}
	return middles;
}

const vector<v_edge_it> partition(Graph& g, const vector<v_edge_it> starts, const vector<v_edge_it> ends,int pivot){
	
	vector<v_edge_it> middles(g.n);

	for(int i = 0; i != g.n; i++){
	    middles[i] = partition(pstl::execution::par,
                     starts[i], ends[i],
                     [pivot](edge* e) {return e->weight <= pivot;});
	}

	return middles;
}

l_edge_t filter_sollin::algorithm(Graph& g, unsigned int n_threads){

	union_find* u = new union_find(g.n);
	compVertexFALvec cV(u);
	const compWeight cW;

	vector<component_FAL_vector*> edges;
	for(auto aux : g.adjacency_vector){
		component_FAL_vector* c = new component_FAL_vector;
		c->index = aux->index;
		c->liste.push_back(aux);
		edges.push_back(c);
	}

	auto starts = vector<vector<edge*>::iterator>(g.n);
	auto ends = vector<vector<edge*>::iterator>(g.n);
	for(int i = 0; i != g.n; i++){
		starts[i] = g.adjacency_vector[i]->adjacent_vertices.begin();
		ends[i] = g.adjacency_vector[i]->adjacent_vertices.end();
	}

	l_edge_t mst = main_func(g,n_threads,edges,starts,ends,u,cV,cW);
	return mst;

}



l_edge_t filter_sollin::base_func(Graph& g, unsigned int n_threads,vector<component_FAL_vector*>& edges, vector<vector<edge*>::iterator> starts, const vector<vector<edge*>::iterator> ends, union_find* u,compVertexFALvec cV,compWeight cW){

	l_edge_t mst;

	// Sort each list by weight
	#pragma omp parallel for num_threads(n_threads)
	for(int i = 0; i < g.n; i++){
		sort(starts[i],ends[i],cW);
	}

	// Internal value
	edge* einit = new edge();
	einit->target = -1;

	// While not connected
	while(u->numTrees > 1 ){	

		// Sort by parent vertex
		tbb::parallel_sort(edges.begin(),edges.end(),cV);
		//sort(edges.begin(),edges.end(),cV);

		// Update all parents
		u->update_parents();
		/*
		cout << endl;
		for(auto parent : u->parents){
			cout << parent << " ";
		}
		cout << endl;
		*/


		// Merge same parent vertex
		int nVertex = edges.size();
		unsigned int k;

		vector<result_FAL_vector> auxs(n_threads);
		#pragma omp parallel num_threads(n_threads)
		{
			int id = omp_get_thread_num();
			#pragma omp for
			for(k = 0; k < nVertex; k++){
				component_FAL_vector* adj = edges[k];
				int p1;
				#pragma omp critical
				{
					p1 = u->find(adj->index);
				}
				if(auxs[id].firstSource == -1){
					auxs[id].firstSource = p1;
				}
				if(p1 != auxs[id].lastSource){
					auxs[id].liste.push_back(adj);
					auxs[id].lastSource = p1;
				}else{
					auto& ref = auxs[id].liste.back()->liste;
					auto& ref0 = adj->liste;
					ref.splice(ref.end(),ref0);
					delete adj;
				}
			}
		}

		// Merge partial results
		result_FAL_vector aux;
		for(int k = 0; k != n_threads; k++){
			merge_FAL_vector(aux,auxs[k],cW);
		}


		// Copy list in vector
		edges.clear();
		list<component_FAL_vector*> & aLRef = aux.liste;
		for(auto it = aLRef.begin(); it != aLRef.end();it++){
			edges.push_back(*it);
		}


		// For all vertice find minimum outgoing edge
		int nComps = edges.size();
		vector<vector<edge*>> cheapest(nComps,vector<edge*>(PAD));

		# pragma omp parallel for num_threads(n_threads)
		for(k = 0; k < nComps; k++){
			component_FAL_vector* val = edges[k];
			int intra_size = val->liste.size();
			// Take min in parallel
			vector<edge*> intra_cheapest;
			for(auto v_edges : val->liste){
				int source_index = v_edges->index;
				auto it = starts[source_index];
				auto it_end = ends[source_index];
				int source = u->find_debug(source_index);
				int target = source;
				//cout << "Edge: " << source;
				//print_edge_vec(v_edges->adjacent_vertices);
				edge* e;
				while(target == source){
					if(it == it_end){
						e = einit;
						break;
					}
					e = *it; 
					target = u->find_debug(e->target);
					it++;
					//cout << (target == source) << endl;
				}	
				intra_cheapest.push_back(e);
				starts[source_index] = it;

			}
			// Find minimum in intra_cheapest
			int min_weight = std::numeric_limits<int>::max();
			int min_index = 0;
			for(unsigned int i = 0; i != intra_size; i++){
				edge* e = intra_cheapest[i];
				if(e->source != -1){
					int weight = intra_cheapest[i]->weight;
					if(weight < min_weight){
						min_index = i;
						min_weight = weight;
					}
				}
			}
			// Push back all others
			int aux_index = 0;
			for(auto v_edges : val->liste){
				edge* e = intra_cheapest[aux_index];
				if(e->source != -1){
					if(aux_index != min_index){
						int source_index = v_edges->index;
						starts[source_index]--;
					}
				}
				aux_index++;
			}
			cheapest[k][0] = intra_cheapest[min_index];
		}



		// For all vertices, connect components
		vector<edge*> add_to_mst;
		// Connect the components via pointer-jump
		#pragma omp parallel for num_threads(n_threads) reduction(addEdges:add_to_mst)
		for(k = 0; k < nComps; k++){
			edge* e = cheapest[k][0];
			bool b;
			#pragma omp critical
			{
				b = u->unite(e->source,e->target);
			}
			if (b) add_to_mst.push_back(e);
		}
		if(add_to_mst.size() == 0){
			return mst;
		}
		mst.insert(mst.end(),add_to_mst.begin(),add_to_mst.end());
	}
	return mst;
}

l_edge_t filter_sollin::main_func(Graph& g, unsigned int n_threads, vector<component_FAL_vector*>& edges,const vector<vector<edge*>::iterator> starts, const vector<vector<edge*>::iterator> ends,union_find* u, compVertexFALvec cV, compWeight cW){
	// Internal Time Measurements
	double constant_time_1 = 0;
	double constant_time_2 = 0;
	
	// Compute size and check if < 5000
	int sum_sizes = 0;
	for(int i = 0; i != g.n; i ++){
		sum_sizes += ends[i] - starts[i];
	}

	if(sum_sizes < 5000){
		cout << "Base started" << endl;
		return base_func(g,n_threads,edges,starts,ends,u,cV,cW);	
	}
	
	int piv = pivot(g,starts,ends);
	cout << "Pivot: " << piv << endl;
	
	const vector<vector<edge*>::iterator> middles = partition(g,starts,ends,piv);

	// Recursive call
	cout << "Recursive started" <<  (*starts[0])->weight << endl;
	cout << (*(middles[0]-1))->weight << endl;
	l_edge_t mst = main_func(g,n_threads,edges,starts,middles,u,cV,cW);
	cout << "Recursive ended" << endl;

	if(mst.size() < g.n - 1){
		cout << "Not complete" << endl;

		// Filter rest of list
		const vector<v_edge_it> new_end = filter(g,middles,ends,u);
		cout << (*new_end[0])->weight << endl;

		// Call Base Func again
		l_edge_t mst_bis = main_func(g,n_threads,edges,middles,new_end,u,cV,cW);

		// Merge rest of list
		mst.splice(mst.end(),mst_bis);

	}


	timing t_init_1("Init_time_1",constant_time_1);
	timing t_init_2("Init_time_2",constant_time_2);

	internal_timings.push_back(t_init_1);
	internal_timings.push_back(t_init_2);

	return mst;
}
