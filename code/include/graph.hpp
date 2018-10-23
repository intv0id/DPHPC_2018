#ifndef GRAPH
#define GRAPH

#include <vector>
#include <list>

using namespace std;

class edge {

    public:
    edge(int source_, double weight_) :
        source(source_),
        weight(weight_)
    {}
    void print();

    int source;
    int weight;
};

class node {

    public:
    node(int index_) :
        index(index_)
    {}

    int index;
    vector<edge*> adjacentEdges;
};

class Graph {

    public:
    Graph(){};

    // Constructor to generate a Erdos-Renyi random graph
    // Proba of an edge p
    // min <= weight <= max
    Graph(int nVertices, double p, int min, int max);

    // Function to print a graph
    Graph copy();
    void printGraph();

    int nVertices;
    vector<node*> nodes;
};

struct edge_EL {
    int source;
    int target;
    int weight;
};

class Graph_EL {
    public:
    //Constructs a graph with n edges and no vertices
    Graph_EL(int n_) : n(n_) {};
    // Constructor to generate a Erdos-Renyi random graph
    // Proba of an edge p
    // min <= weight <= max
    Graph_EL(int nVertices, double p, int min, int max);

    // Function to print a graph
    void printGraph();

    // Function to add an edge between u and v (note: add (u,v) and (v,u))
    void add_edge(int u, int v, int weight);
    int n;
    list<edge_EL*> edges;
};

#endif
