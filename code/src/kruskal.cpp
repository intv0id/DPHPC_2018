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

l_edge_t seq_kruskal_main(Graph &g, list<edge*> &edges, union_find* u_find){
    l_edge_t result;

    edges.sort(compare);

    // Merge trees
    for (auto e : edges){
        if (u_find->find(e->source) != u_find->find(e->target)) {
            u_find->unite(e->source, e->target);
            result.push_back(e);
        }
    }

    return result;
}

l_edge_t seq_kruskal::algorithm(Graph &g){
    list<edge*> edges = g.unique_edges;
    union_find* u_find = new union_find(g.n);
    return seq_kruskal_main(g, edges, u_find);
}
