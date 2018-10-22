#ifndef GRAPH
#define GRAPH

#include <vector>

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

    // Constructor to generate a Erdos-Renyi random graph
    // Proba of an edge p
    // min <= weight <= max
    Graph(int nVertices, double p, int min, int max);

    // Function to print a graph
    void printGraph();

    int nVertices;
    vector<node*> nodes;
};

#endif
