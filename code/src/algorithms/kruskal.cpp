#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>

#include "algorithms/kruskal.hpp"
#include "common.hpp"
#include "tbb/parallel_sort.h"

using namespace std;

bool compare(const edge* a, const edge* b){
    return a->weight < b->weight;
}

l_edge_t kruskal_main(vector<edge*>::iterator start, vector<edge*>::iterator end, union_find* u_find){
    l_edge_t result;

    auto it = start;
    while (it != end) {
        edge* e = *it;
        if (u_find->find(e->source) != u_find->find(e->target)) {
            u_find->unite(e->source, e->target);
            result.push_back(e);
        }
        it++;
    }

    return result;
}

l_edge_t kruskal::algorithm(Graph &g, unsigned int n_threads) {
    vector<edge*> edges {g.unique_edges.begin(), g.unique_edges.end()};
    union_find* u_find = new union_find(g.n);

    if (n_threads > 1) {
        tbb::parallel_sort(edges.begin(), edges.end(), compare);
    }
    else {
        sort(edges.begin(), edges.end(), compare);
    }

    auto result = kruskal_main(edges.begin(), edges.end(), u_find);
    delete u_find;
    return result;
}
