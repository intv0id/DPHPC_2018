#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <stdlib.h> 
#include <iostream>

#include "filter_kruskal.hpp"
#include "kruskal.hpp"
#include "common.hpp"

using namespace std;

int find_pivot(list<edge*> &edges) {
    int n = edges.size();
    int pivot_id = rand() % n;
    int pivot = -1;
    int i = 0;
    for (auto e: edges) {
        if (i == pivot_id) {
            pivot = e->weight;
        }
        i++;
    }

    return pivot;
}

list<edge*> filter(list<edge*> &edges, union_find *u_find) {
    list<edge*> filtered;
    for (auto e : edges) {
        int u = e->source;
        int v = e->target;
        if (u_find->find(e->source) != u_find->find(e->target)) {
            filtered.push_back(e);
        }
    }
    return filtered;
}

pair<list<edge*>, list<edge*>> partition(list<edge*> &edges, int pivot) {

    list<edge*> e_minus;
    list<edge*> e_plus;

    for (auto e : edges){
        if (e->weight <= pivot) {
            e_minus.push_back(e);
        }
        else {
            e_plus.push_back(e);
        }
    }

    return make_pair(e_minus, e_plus);
}

l_edge_t seq_filter_kruskal_main(Graph &g, list<edge*> &edges, union_find *u) {

    if (edges.size() < 3) {
        return seq_kruskal_main(g, edges, u);
    }
    int pivot = find_pivot(edges);
    auto couple = partition(edges, pivot);

    l_edge_t partial_solution = seq_filter_kruskal_main(g, couple.first, u);

    auto e_plus = filter(couple.second, u);
    partial_solution.merge(seq_filter_kruskal_main(g, e_plus, u));

    return partial_solution;
}

l_edge_t seq_filter_kruskal(Graph &g){
    l_edge_t result;
    list<edge*> edges = g.unique_edges;
    union_find* u_find = new union_find(g.n);

    return seq_filter_kruskal_main(g, edges, u_find);
}
