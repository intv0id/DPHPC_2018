#ifndef COMMON_H
#define COMMON_H

#include "graph.hpp"
#include <list>

using namespace std;

#pragma omp declare reduction \
	(addEdges:v_edge_t: omp_out.insert(omp_out.end(),omp_in.begin(),omp_in.end()))

struct component_FAL{
	int index;
	list<vertex_adjacency_list*> liste;
};

struct component_FAL_vector{
	int index;
	list<vertex_adjacency_vector*> liste;
};

class timing {
	public:
	string name;	
	double measurement;
	timing(string name_,double measurement_) : name(name_),measurement(measurement_) {}
};

class mst_algorithm {
	public:
	const string name;
	virtual l_edge_t algorithm(Graph& g, unsigned int n_threads = 1) = 0;
	vector<timing> internal_timings; 
	mst_algorithm(string name_) : name(name_) {};
	string get_measurements_names();
	string get_measurements_values();
};

class union_find {
	public:
        vector<int> parents;
        int size;
        int numTrees;
        union_find(int n);
        int find(int x);
        int find_debug(int x);
        bool unite(int x, int y);
	// Update all parents
	void update_parents();
};

class comp{
	union_find* u;
	public:
	comp(union_find*);
	bool operator() (edge*,edge*) const;
};

class compVertex{
	union_find* u;
	public:
	compVertex(union_find*);
	bool operator() (const vertex_adjacency_list*,const vertex_adjacency_list*) const;
};

class compVertexFAL{
	union_find* u;
	public:
	compVertexFAL(union_find*);
	bool operator() (const component_FAL*,const component_FAL*) const;
};

class compVertexFALvec{
	union_find* u;
	public:
	compVertexFALvec(union_find*);
	bool operator() (const component_FAL_vector*,const component_FAL_vector*) const;
};

class compWeight{
	public:
	bool operator() (edge*, edge*) const;
};

class compWeight_copy{
	public:
	bool operator() (edge&, edge&) const;
};

class compTargetVertex{
	union_find* u;
	public:
	compTargetVertex(union_find*);
	bool operator() (edge*,edge*) const;
};

void print_edge_list(l_edge_t);
void print_edge_vec(v_edge_t);

#endif
