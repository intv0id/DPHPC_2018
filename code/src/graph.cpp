#ifndef GRAPH
#define GRAPH

#include <graph.hpp>

#endif

#include <iostream>
#include <random>

using namespace std;

void edge::print(){
    cout << "source: " << source << endl;
    cout << "Weight: " << weight << endl;
}

Graph::Graph(int nVertices, double edgeProba, int min, int max) :
    nVertices(nVertices),
    nodes(nVertices)
{
    // Generate all nodes
    for(int i = 0; i != nVertices; i++){
        nodes[i] = new node(i);
    }

    // For all pair of nodes, generate random edges
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(min,max);

    for(int i = 0; i !=nVertices; i++){
        for(int j = i+1; j!= nVertices; j++){

	    double test = rand() / (double) RAND_MAX;
            if(test <= edgeProba){
                // Generate weight
                int w = uni(rng);

                // Create edges
                edge* ei = new edge(i,w);
                edge* ej = new edge(j,w);
                nodes[j]->adjacentEdges.push_back(ei);
                nodes[i]->adjacentEdges.push_back(ej);
            }
        }
    }
}

void Graph::printGraph(){
    for(vector<node*>::iterator it = nodes.begin(); it != nodes.end(); it++){
        cout << endl << "Vertice: " << (*it)->index << endl;
        for(vector<edge*>::iterator it0 = (*it)->adjacentEdges.begin(); it0 != (*it)->adjacentEdges.end(); it0++){
            (*it0)->print();
        }
    }
}
