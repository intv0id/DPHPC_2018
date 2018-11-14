#include <graph.hpp>

#include <iostream>
#include <random>

using namespace std;


void edge::print(){
    cout << "source: " << source << endl;
    cout << "Weight: " << weight << endl;
}

void Graph::add_edge(int i, int j, int w){


	// Create edges
	edge* ei = new edge;
	edge* ej = new edge;
	ei->source = i; ej->source = j;
	ei->target = j; ej->target = i;
	ei->weight = w; ej->weight = w;
	edges.push_back(ei);
	edges.push_back(ej);
	adjacency_list[i]->adjacent_vertices.push_back(ei);
	adjacency_list[j]->adjacent_vertices.push_back(ej);

	// Add edge to boost graph
	boost::add_edge(i,j,w,boost_rep);

	
	if (i < j) {
		unique_edges.push_back(ei);
	}
	else {
		unique_edges.push_back(ej);
	}
	
	//adjacency_matrix[u][v] = weight;
	/*
	adjacency_list[u].push_back(e1);
	adjacency_list[v].push_back(e2);
	*/
}
Graph::Graph(int n_) :
	n(n_){
	
    for(int i = 0; i != n; i++){
        vertex_adjacency_list* val = new vertex_adjacency_list;
        val->index = i;
        adjacency_list.push_back(val);
    }

	
}
Graph::Graph(int nVertices, double edgeProba, int min, int max) :
    n(nVertices), boost_rep(n)
{

    // For all pair of nodes, generate random edges
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(min,max);


    for(int i = 0; i != nVertices; i++){
        vertex_adjacency_list* val = new vertex_adjacency_list;
        val->index = i;
        adjacency_list.push_back(val);
    }

    for(int i = 0; i !=nVertices; i++){
        for(int j = i+1; j!= nVertices; j++){

	    double test = rand() / (double) RAND_MAX;
            if(test <= edgeProba){
                // Generate weight
                int w = uni(rng);
		add_edge(i,j,w);

            }
        }
    }
}

Graph::~Graph(){
	cout << "Graph is destructed" << endl;
	for(auto e : edges){
		delete e;
	}
	for(auto v : adjacency_list){
		delete v;
	}
}
