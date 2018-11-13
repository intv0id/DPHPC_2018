#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>

#include "kruskal.hpp"
#include "common.hpp"
#include "tbb/parallel_sort.h"

using namespace std;

bool compare(const edge* a, const edge* b){
    return a->weight < b->weight;
}

l_edge_t kruskal_main(Graph &g, vector<edge*> &edges, union_find* u_find){
    l_edge_t result;

    for (auto e : edges){
        if (u_find->find(e->source) != u_find->find(e->target)) {
            u_find->unite(e->source, e->target);
            result.push_back(e);
        }
    }

    return result;
}

l_edge_t seq_kruskal(Graph &g){
    vector<edge*> edges {g.unique_edges.begin(), g.unique_edges.end()};
    sort(edges.begin(), edges.end(), compare);
    union_find* u_find = new union_find(g.n);
    return kruskal_main(g, edges, u_find);
}

l_edge_t par_kruskal(Graph &g){
    vector<edge*> edges {g.unique_edges.begin(), g.unique_edges.end()};
    tbb::parallel_sort(edges, compare);
    union_find* u_find = new union_find(g.n);
    return kruskal_main(g, edges, u_find);
}
