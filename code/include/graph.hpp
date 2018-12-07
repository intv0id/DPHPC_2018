#ifndef GRAPH
#define GRAPH

#include <vector>
#include <list>

#include <boost/graph/adjacency_list.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS,
	boost::no_property,
	boost::property<boost::edge_weight_t,int> > Boost_Graph;


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
    string name;
    //Constructs a graph with n vertices and no edges 
    Graph(int n_);
    // Constructor to generate a Erdos-Renyi random graph
    // Proba of an edge p
    // min <= weight <= max
    Graph(int nVertices, double p, int min, int max);
    // Constructor to generate a Preferential Attachment graph
    // At each iteration, new vertex connect to m neighbours
    // min <= weight <= max
    Graph(int nVertices, int m, int min, int max);
    // Constructor to generate a graph based on one of the files found on
    // http://www.diag.uniroma1.it/challenge9/download.shtml
    // file name : "USA-road-"+ type + "." + name +".gr"
    Graph(string fname, string type);
    ~Graph();

    // Function to print a graph
    void printGraph();
    Graph copy();

    // Function to add an edge between u and v (note: add (u,v) and (v,u))
    void add_edge(int u, int v, int weight);

    int n;
    int n_edges;
    list<edge*> edges;
    list<edge*> unique_edges; // Only min -> max
    vector<vertex_adjacency_list*> adjacency_list;
    Boost_Graph boost_rep;
    int** adjacency_matrix;
};

typedef list<edge*> l_edge_t;
typedef vector<edge*> v_edge_t;
typedef vector<edge*>::iterator v_edge_it;
typedef list<edge*>::iterator l_edge_it;
typedef vector<edge*>::iterator v_edge_it;
typedef list<vertex_adjacency_list*>::iterator l_val_it;

#endif
