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

class vertex_adjacency_list{
	public:
	int index;
	list<edge*> adjacent_vertices;
};

class Graph {
    public:
    //Constructs a graph with n vertices and no edges 
    Graph(int n_) : n(n_) {
	int** adj = new int* [n];
        for (int i = 0; i < n; i++) {
            adj[i] = new int [n];
            for(int j = 0; j < n; j++)
                adj[i][j] = 0;
        }
	this->adjacency_matrix = adj;
    };
    // Constructor to generate a Erdos-Renyi random graph
    // Proba of an edge p
    // min <= weight <= max
    Graph(int nVertices, double p, int min, int max);
    ~Graph();

    // Function to print a graph
    void printGraph();
    Graph copy();

    // Function to add an edge between u and v (note: add (u,v) and (v,u))
    void add_edge(int u, int v, int weight);

    int n;
    list<edge*> edges;
    list<edge*> unique_edges; // Only min -> max
    vector<vertex_adjacency_list> adjacency_list;
    Boost_Graph boost_rep;
    int** adjacency_matrix;
};

typedef list<edge*> l_edge_t;
typedef vector<edge*> v_edge_t;
typedef vector<edge*>::iterator v_edge_it;
typedef list<edge*>::iterator l_edge_it;
typedef vector<edge*>::iterator v_edge_it;

#endif
