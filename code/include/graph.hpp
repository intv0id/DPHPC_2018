#ifndef GRAPH
#define GRAPH

#include <vector>
#include <list>

#include <boost/graph/adjacency_list.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS,
	boost::no_property,
	boost::property<boost::edge_weight_t,int>> Boost_Graph;


struct edge {

    void print();
    int source;
    int target;
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
    //Constructs a graph with n edges and no vertices
    Graph(int n_) : n(n_) {};
    // Constructor to generate a Erdos-Renyi random graph
    // Proba of an edge p
    // min <= weight <= max
    Graph(int nVertices, double p, int min, int max);

    // Function to print a graph
    void printGraph();
    Graph copy();

    // Function to add an edge between u and v (note: add (u,v) and (v,u))
    void add_edge(int u, int v, int weight);

    int n;
    list<edge*> edges;
    Boost_Graph boost_rep;
};

typedef vector<node*> v_node_t; 
typedef vector<edge*> v_edge_t;
typedef list<edge*> l_edge_t;
typedef vector<edge*> v_edge_t;
typedef vector<node*>::iterator v_node_it;
typedef vector<edge*>::iterator v_edge_it;
typedef list<edge*>::iterator l_edge_it;
typedef vector<edge*>::iterator v_edge_it;

#endif
