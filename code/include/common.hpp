#ifndef COMMON_H
#define COMMON_H

#include "graph.hpp"
#include <list>

using namespace std;

class mst_algorithm {
	public:
	const string name;
	virtual l_edge_t algorithm(Graph& g) = 0;
	mst_algorithm(string name_) : name(name_) {};
};

class union_find {
	public:
        vector<int> parents;
        int size;
        int numTrees;
        union_find(int n);
        int find(int x);
        bool unite(int x, int y);
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


class compTargetVertex{
	union_find* u;
	public:
	compTargetVertex(union_find*);
	bool operator() (edge*,edge*) const;
};

void print_edge_list(l_edge_t);

#endif
