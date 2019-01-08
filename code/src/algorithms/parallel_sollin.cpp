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

#define PAD 8

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
	#ifdef DEBUG
	cout << "\nThread "<<omp_get_thread_num()<<" on cpu "<<sched_getcpu()<<endl;
	#endif
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



l_edge_t parallel_sollin_EL::algorithm(Graph& g, unsigned int n_threads){


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
	// Find minimum ingoing edge
	edge* einit = new edge();
	einit->source = -1;

	// While not connected
	while(u->numTrees > 1){	
		// Sort the edge list and supervertex
		tbb::parallel_sort(vectorEdges.begin(),vectorEdges.end(),c);
		//cout << "Sorted " << endl;

		// Remove self-loops and multiple edges (compact graph)
		result aux;
		unsigned int k;
		int nEdges = vectorEdges.size();
		#pragma omp parallel for ordered num_threads(n_threads) reduction(listEdges:aux)
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


		// Do loop in parallel

		nEdges = vectorEdges.size();
		v_edge_t cheapest(n,einit);
		#pragma omp parallel for num_threads(n_threads) reduction(findMin:cheapest)
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
		#pragma omp parallel for num_threads(n_threads) reduction(addEdges:add_to_mst)
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
	delete u;
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
	
	if(v1.lastSource == v2.firstSource){
		l_edge_t& ref1 = v1.liste.back()->adjacent_vertices;
		l_edge_t& ref2 = v2.liste.front()->adjacent_vertices;
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

#pragma omp declare reduction \
	(compactVertexAL:result_AL:merge_AL(omp_out,omp_in))

l_edge_t parallel_sollin_AL::algorithm(Graph& g, unsigned int n_threads){
	// Internal Time Measurements
	double constant_time_1 = 0;
	double constant_time_2 = 0;
	double time_compact_step_1 = 0;
	double time_compact_step_2 = 0;
	double time_compact_step_3 = 0;
	double time_compact_step_4 = 0;
	double time_compact_step_5 = 0;
	double time_compact_step_6 = 0;
	double time_find_min = 0;
	double time_connect = 0;

	double t1 = omp_get_wtime();

	// Copy adjacency list (NOW COPYING GRAPH)
	/*vector<vertex_adjacency_list*> edges(g.n);
	//#pragma omp parallel num_threads(n_threads)
	for(unsigned int i = 0; i != g.adjacency_list.size(); i++){
		vertex_adjacency_list* v = new vertex_adjacency_list();
		v->index = g.adjacency_list[i]->index;
		v->adjacent_vertices = g.adjacency_list[i]->adjacent_vertices;
		edges[i] = v;	
	}*/

	// Internal value
	edge* einit = new edge();
	einit->target = -1;

	auto& edges(g.adjacency_list);

	double t0 = omp_get_wtime();
	constant_time_1 = t0 - t1;


	// Create MST
	l_edge_t mst;

	// Create union-find structure
	union_find* u = new union_find(g.n);
	compVertex cV(u);
	compTargetVertex cTV(u);

	t1 = omp_get_wtime();
	constant_time_2 = t1 - t0;

	// While not connected
	while(u->numTrees > 1){	

		t0 = omp_get_wtime();

		// Sort by parent vertex
		//tbb::parallel_sort(edges.begin(),edges.end(),cV);
		sort(edges.begin(),edges.end(),cV);

		// Update all parents
		u->update_parents();

		t1 = omp_get_wtime();
		time_compact_step_1 += t1 - t0;
		

		// Merge same parent vertex
		int nVertex = edges.size();
		unsigned int k;

		vector<result_AL> auxs(n_threads);
		#pragma omp parallel num_threads(n_threads)
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

		t0 = omp_get_wtime();
		time_compact_step_2 += t0 - t1;
		
		// Merge partial results
		result_AL aux;
		for(int k = 0; k != n_threads; k++){
			merge_AL(aux,auxs[k]);
		}


		t1 = omp_get_wtime();
		time_compact_step_3 += t1 - t0;

		// Copy list in vector
		edges.clear();
		list<vertex_adjacency_list*> & aLRef = aux.liste;
		for(l_val_it it = aLRef.begin(); it != aLRef.end();it++){
			edges.push_back(*it);
		}


		t0 = omp_get_wtime();
		time_compact_step_4 += t0 - t1;

		// Sort each list by target parent vertex
		nVertex = edges.size();
		#pragma omp parallel for num_threads(n_threads)
		for(int i = 0; i < nVertex; i++){
			edges[i]->adjacent_vertices.sort(cTV);
		}

		t1 = omp_get_wtime();
		time_compact_step_5 += t1 - t0;
		t0 = omp_get_wtime();

		// For each list merge target vertex
		#pragma omp parallel for num_threads(n_threads)
		for(int i = 0; i < nVertex; i++){
			int target = -1;
			int p1 = u->find(edges[i]->index);
			l_edge_t& ref = edges[i]->adjacent_vertices;
			for(l_edge_it it = ref.begin(); it != ref.end();){
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
		

		t1 = omp_get_wtime();
		time_compact_step_6 += t1 - t0;

		// For all vertice find minimum outgoing edge
		int nComps = edges.size();
		vector<vector<edge*>> cheapest(nComps,v_edge_t(PAD));

		#pragma omp parallel for num_threads(n_threads)
		for(k = 0; k < nComps; k++){
			cheapest[k][0] = einit;
			vertex_adjacency_list* val = edges[k];
			l_edge_t& ref = val->adjacent_vertices;
			for(l_edge_it it = ref.begin(); it != ref.end(); it++){

				edge* e = *it;
				int target = u->find_debug(e->target);
				int weight = e->weight;

				edge* cheapest_edge = cheapest[k][0];
				
				if(cheapest_edge->target == -1 || weight < cheapest_edge->weight){
					cheapest[k][0] = e;
				}
			}
		}

		t0 = omp_get_wtime();
		time_find_min += t0 - t1;

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
		mst.insert(mst.end(),add_to_mst.begin(),add_to_mst.end());

		t1 = omp_get_wtime();
		time_connect += t1 - t0;
        
	}
	//timing t_init_1("Init_time_1",constant_time_1);
	//timing t_init_2("Init_time_2",constant_time_2);
	timing t_compact_1("Compact_step_1",time_compact_step_1);
	timing t_compact_2("Compact_step_2",time_compact_step_2);
	//timing t_compact_3("Compact_step_3",time_compact_step_3);
	//timing t_compact_4("Compact_step_4",time_compact_step_4);
	timing t_compact_5("Compact_step_5",time_compact_step_5);
	timing t_compact_6("Compact_step_6",time_compact_step_6);
	timing t_find_min("Find_min_step",time_find_min);
	timing t_connect("Connect_step",time_connect);

	//internal_timings.push_back(t_init_1);
	//internal_timings.push_back(t_init_2);
	internal_timings.push_back(t_compact_1);
	internal_timings.push_back(t_compact_2);
	//internal_timings.push_back(t_compact_3);
	//internal_timings.push_back(t_compact_4);
	internal_timings.push_back(t_compact_5);
	internal_timings.push_back(t_compact_6);
	internal_timings.push_back(t_find_min);
	internal_timings.push_back(t_connect);

	delete u;

	return mst;
}



struct result_FAL{
	result_FAL() : firstSource(-1),lastSource(-1) {}
	list<component_FAL*> liste;
	int firstSource,lastSource;
};

void merge_FAL(result_FAL& v1, result_FAL& v2,const compWeight cW){

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

bool check_sanity(list<edge*> pointers, list<edge> copy, string msg) {
    int n = pointers.size();
    if (n == copy.size()) {
        auto it_pointers = pointers.begin();
        auto it_copy = copy.begin();
        while (it_pointers != pointers.end()) {
            edge x = *(*it_pointers);
            edge y = *it_copy;
            if (x.source != y.source) {
                cout << msg << endl;
            }
            it_pointers++;
            it_copy++;
        }
    }
    else {
        cout << msg << endl;
        return false;
    }
    return true;
}

bool check_edges(edge* e1, edge* e2, string s) {
    if (e1->source != e2->source) {
        cout << endl << s << endl;
        return false;
    }
    else if (e1->target != e2->target) {
        cout << endl << s << endl;
        return false;
    }
    else if (e1->weight != e2->weight) {
        cout << endl << s << endl;
        return false;
    }
    return true;
}

edge* copy_edge(edge* e) {
    edge* new_e = new edge;
    new_e->source = e->source;
    new_e->target = e->target;
    new_e->weight = e->weight;
    return new_e;
}

l_edge_t parallel_sollin_FAL::algorithm(Graph& g, unsigned int n_threads){
	// Internal Time Measurements
	double constant_time_1 = 0;
	double constant_time_2 = 0;
	double time_compact_step_1 = 0;
	double time_compact_step_2 = 0;
	double time_compact_step_3 = 0;
	double time_compact_step_4 = 0;
	double time_compact_step_5 = 0;
	double time_compact_step_6 = 0;
	double time_find_min = 0;
	double time_connect = 0;

	double t1 = omp_get_wtime();

	// Create union-find structure
	union_find* u = new union_find(g.n);
	compVertexFAL cV(u);
	const compWeight cW;
	const compWeight_copy cW_copy;

	// Sort each list by weight
	#pragma omp parallel for num_threads(n_threads)
	for(int i = 0; i < g.n; i++){
		//cout << "Edge: " << i << endl;
		//print_edge_list(edges[i]->adjacent_vertices);
		g.adjacency_list[i]->adjacent_vertices_copy.sort(cW_copy);
	}

	// Internal value
	edge* einit = new edge();
	einit->source = -1;

	// Create MST
	l_edge_t mst;


	double t0 = omp_get_wtime();
	constant_time_1 = t0 - t1;

	//auto& edges = g.adjacency_list;
	vector<component_FAL*> edges;
	for(auto aux : g.adjacency_list){
		component_FAL* c = new component_FAL;
		c->index = aux->index;
		c->liste.push_back(aux);
		edges.push_back(c);
	}

	t1 = omp_get_wtime();
	constant_time_2 = t1 - t0;

    // OUTSIDE LOOP
    // Find maximum size for an adjacency list
    int max_val;

    int j;
    /*
    #pragma omp parallel for reduction(max : max_val)
    for(j = 0; j < edges.size(); j++) {
        int x = edges[j]->liste.size();
        if (x > max_val)
            max_val = x;
    }
    */

    max_val = g.n;

    vector<vector<edge*>> intra_cheapest_vector_copy(n_threads);
    for (j = 0; j < n_threads; j++) {
        intra_cheapest_vector_copy[j].reserve(max_val);
    }

    vector<edge*> cheapest(g.n);

    // cout << endl << "INIT TIME: " << omp_get_wtime() - t1 << endl;

	// While not connected
	while(u->numTrees > 1){	

		t0 = omp_get_wtime();

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

		t1 = omp_get_wtime();
		time_compact_step_1 += t1 - t0;
		

		// Merge same parent vertex
		int nVertex = edges.size();
		unsigned int k;

		vector<result_FAL> auxs(n_threads);
		#pragma omp parallel num_threads(n_threads)
		{
			int id = omp_get_thread_num();
			#pragma omp for
			for(k = 0; k < nVertex; k++){
				component_FAL* adj = edges[k];
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

		t0 = omp_get_wtime();
		time_compact_step_2 += t0 - t1;
		
		// Merge partial results
		result_FAL aux;
		for(int k = 0; k != n_threads; k++){
			merge_FAL(aux,auxs[k],cW);
		}



		t1 = omp_get_wtime();
		time_compact_step_3 += t1 - t0;

		// Copy list in vector
		edges.clear();
		list<component_FAL*> & aLRef = aux.liste;
		for(auto it = aLRef.begin(); it != aLRef.end();it++){
			edges.push_back(*it);
		}


		t0 = omp_get_wtime();
		time_compact_step_4 += t0 - t1;

		// For all vertice find minimum outgoing edge
		int nComps = edges.size();

		# pragma omp parallel for num_threads(n_threads)
		for(k = 0; k < nComps; k++){

			int intra_size = edges[k]->liste.size();

			// Take min in parallel
			vector<edge*>& intra_cheapest_copy = intra_cheapest_vector_copy[omp_get_thread_num()];
            intra_cheapest_copy.clear();

			for(auto list_edges : edges[k]->liste){
                
				list<edge>& ref_copy = list_edges->adjacent_vertices_copy;

				int source = u->find_debug(list_edges->index);
				int target = source;

				//cout << "Edge: " << source;
				//print_edge_list(ref);
				edge* e_copy;

				while(target == source){
					if(ref_copy.empty()){
						e_copy = einit;
						break;
					}
                    e_copy = &(ref_copy.front());
					target = u->find_debug(e_copy->target);
					ref_copy.pop_front();
				}

				intra_cheapest_copy.push_back(copy_edge(e_copy));
			}

			int min_weight = std::numeric_limits<int>::max();
			int min_index = 0;
			for(unsigned int i = 0; i != intra_size; i++){
				//edge* e = intra_cheapest[i];
				edge* e = intra_cheapest_copy[i];

				if(e->source != -1){
					int weight = intra_cheapest_copy[i]->weight;
					if(weight < min_weight){
						min_index = i;
						min_weight = weight;
					}
				}
			}

            // Push back all others
			int aux_index = 0;
			for(auto list_edges : edges[k]->liste){

				edge* e = intra_cheapest_copy[aux_index];
				if(e->source != -1){
					if(aux_index != min_index){
						list_edges->adjacent_vertices_copy.push_front(*intra_cheapest_copy[aux_index]);
					}
				}
				aux_index++;
			}
			cheapest[k] = intra_cheapest_copy[min_index];
		}

		t0 = omp_get_wtime();
		time_find_min += t0 - t1;

		// For all vertices, connect components
		// Connect the components via pointer-jump

        edge* e;
        edge* e_copy;
		for(k = 0; k < nComps; k++){

			e = cheapest[k];

			if(e->source != -1){
                if (u->unite(e->source,e->target)) {
                    e_copy = new edge;
                    e_copy->source =e->source;
                    e_copy->target = e->target;
                    e_copy->weight = e->weight;
                    mst.push_back(e_copy);
                }
			}
		}

		t1 = omp_get_wtime();
		time_connect += t1 - t0;

	}

	timing t_init_1("Init_time_1",constant_time_1);
	timing t_init_2("Init_time_2",constant_time_2);
	timing t_compact_1("Compact_step_1",time_compact_step_1);
	timing t_compact_2("Compact_step_2",time_compact_step_2);
	timing t_compact_3("Compact_step_3",time_compact_step_3);
	timing t_compact_4("Compact_step_4",time_compact_step_4);
	timing t_compact_5("Compact_step_5",time_compact_step_5);
	timing t_compact_6("Compact_step_6",time_compact_step_6);
	timing t_find_min("Find_min_step",time_find_min);
	timing t_connect("Connect_step",time_connect);

	internal_timings.push_back(t_init_1);
	internal_timings.push_back(t_init_2);
	internal_timings.push_back(t_compact_1);
	internal_timings.push_back(t_compact_2);
	internal_timings.push_back(t_compact_3);
	internal_timings.push_back(t_compact_4);
	internal_timings.push_back(t_compact_5);
	internal_timings.push_back(t_compact_6);
	internal_timings.push_back(t_find_min);
	internal_timings.push_back(t_connect);

	delete u;
    for(auto e : edges){
		delete e;
	}
	
	return mst;
}

