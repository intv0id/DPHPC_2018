#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>

#include "kruskal.hpp"
#include "common.hpp"

using namespace std;

bool compare(const edge* a, const edge* b){
    return a->weight < b->weight;
}

l_edge_t seq_kruskal(Graph& g){
    l_edge_t result;

    list<edge*> edges = g.unique_edges;
    edges.sort(compare);
    union_find* u_find = new union_find(g.n);

    // Merge trees
    for (auto e : edges){
        if (u_find->find(e->source) != u_find->find(e->target)) {
            u_find->unite(e->source, e->target);
            result.push_back(e);
        }
    }

    return result;
}
