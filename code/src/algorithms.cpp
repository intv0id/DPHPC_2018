#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>

#include "algorithms.hpp"


using namespace std;


Kruskal::Kruskal(Graph G){
    // Convert graph type
    this -> n_vertices = G.nVertices;
    for (auto n: G.nodes){
        for (auto e: n->adjacentEdges){
            struct edge_C new_edge = {
                    .source = n->index,
                    .dest = e->source,
                    .weight = e->weight
            };

            this->edges.push_back(new_edge);
        }
    }
}


bool Kruskal::Compare(const edge_C &a, const edge_C &b){
    return a.weight > b.weight;
}

vector<edge_C> Kruskal::compute(){
    vector<edge_C> result;

    // Hashmap which links the index of a node to a pointer its tree
    // To merge trees we just need to change the pointer address
    map<int, tree_C **> nodeTrees;

    // Sort edges by weight
    sort(this->edges.begin(), this->edges.end(), this->Compare);

    // Create a tree for each node
    for(int i=0;i<this->n_vertices;i++){
        tree_C * new_tree = new tree_C();
        new_tree->id = i;
        nodeTrees[i] = & new_tree;
    }


    // Merge trees
    for (auto &e : this->edges){
        // Check if nodes doesn't belong to the same tree
        if(nodeTrees[e.source] !=  nodeTrees[e.dest]){
            // Merge trees by changing pointer address
            nodeTrees[e.source] =  nodeTrees[e.dest];
            // Get the edge in the resulting edges list
            result.push_back(e);
        }
    }

    return result;
}
