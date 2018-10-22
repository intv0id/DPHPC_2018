
#include <graph.hpp>


// Structures

struct edge_C {
    int source;
    int dest;
    int weight;
};

struct tree_C {
    int id;
};


// Kruskal algorithm

class Kruskal {
private:
    std::vector<edge_C> edges;
    int n_vertices;

public:
    Kruskal(Graph G);
    static bool Compare(edge_C &a, edge_C &b);
    vector<edge_C> compute();
};


