#include <graph.hpp>

#include <iostream>
#include <random>

using namespace std;


void edge::print(){
    cout << "source: " << source << endl;
    cout << "Weight: " << weight << endl;
}

void Graph::add_edge(int u, int v, int weight){
	edge* e1 = new edge;
	edge* e2 = new edge;
	e1->source = u;
	e2->source = v;
	e1->target = v;
	e2->target = u;
	e1->weight = weight;
	e2->weight = weight;
	edges.push_back(e1);
	edges.push_back(e2);

    if (u < v) {
        unique_edges.push_back(e1);
    }
    else {
        unique_edges.push_back(e2);
    }

    adjacency_matrix[u][v] = weight;
    /*
    adjacency_list[u].push_back(e1);
    adjacency_list[v].push_back(e2);
    */
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
            }
        }
    }
}

Graph::~Graph(){
	for(auto e : edges){
		delete e;
	}
	for(auto e : unique_edges){
		delete e;
	}
	for(auto v : adjacency_list){
		delete v;
	}
}
