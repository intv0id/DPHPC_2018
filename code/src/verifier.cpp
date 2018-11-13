#include <stdio.h>
#include <iostream>
#include <graph.hpp>

#include "verifier.hpp"
#include "kruskal.hpp"

using namespace std;

int getSum(l_edge_t &mst){
    int c = 0;
    for(l_edge_it ite = mst.begin(); ite != mst.end(); ite++){
        c += (*ite)->weight;
    }   
	return c;
}

bool isEveryNodeReachable(int n, l_edge_t &mst) {
    bool* reached = new bool[n];
    reached[0] = true;
    int new_node = 1;
    while (new_node > 0) {
        new_node = 0;
        for (l_edge_it ite = mst.begin(); ite != mst.end(); ite++) {
            int u = (*ite)->source;
            int v = (*ite)->target;
            if (reached[u] && !reached[v]) {
                new_node++;
                reached[v] = true;
            }
            else if (reached[v] && !reached[u]) {
                new_node++;
                reached[u] = true;
            }
        }
    }

    bool reachable = true;
    for (int i = 0; i < n; i++) {
        reachable = reachable && reached[i];
    }

    return reachable;
}

bool verify(Graph &g, l_edge_t &solution) {
    bool verified = true;

    seq_kruskal s;
    l_edge_t other_solution = s.algorithm(g);

    // 1. Is it spanning?
    verified = verified && isEveryNodeReachable(g.n, solution);
    
    // 2. Is it minimum?
    verified = verified && (getSum(solution) == getSum(other_solution));

    return verified;
}
