#include <iostream>

#include "algorithms/prim.hpp"
// #include "common.hpp"

using namespace std;

// Finds the minimum key value from vertices not in MST
int min_key(int key[], bool in_mst[], int n){
    int min = INT_MAX, i_min;

    for (int v = 0; v < n; v++)
        if (!in_mst[v] && key[v] < min)
            min = key[v], i_min = v;

    return i_min;
}

l_edge_t seq_prim::algorithm(Graph &g){
    l_edge_t result;
    int n = g.n;
    int key[n];
    int prev[n];
    bool in_mst[n];

    // Initialize all keys as infinite and vertices as not in MST
    for (int v = 0; v < n; v++)
        key[v] = INT_MAX, in_mst[v] = false;


    // Put 0 as root of MST by setting its key to 0
    key[0] = 0;
    prev[0] = -1;

    for (int count = 1; count < n; count++){
        // Choose the minimum key in the set of vertices not in MST
        int u = min_key(key, in_mst, n);
        in_mst[u] = true;

        // Update key values		
        for (int v = 0; v < n; v++){
            int w = g.adjacency_matrix[u][v];
            
            if (w > 0 && !in_mst[v] && w < key[v]) {
                prev[v] = u, key[v] = w;
            }
        }
    }

    list<edge*> edges = g.unique_edges;
    for (auto e : edges){
	int u = e->source;
        int v = e->target;
        if (prev[v] == u || prev[u] == v) {            
            result.push_back(e);
        }
    }

    return result;
}
