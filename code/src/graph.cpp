#include <graph.hpp>

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

Graph Graph::copy(){
	Graph h;
	for(v_node_it it = h.nodes.begin(); it != h.nodes.end(); it++){
		node* n = *it;
		h.nodes.push_back(n);
		for(v_edge_it ite = n->adjacentEdges.begin(); ite != n->adjacentEdges.end(); ite++){
				
	
		}
	}
	return h;
}

void Graph_EL::add_edge(int u, int v, int weight){
	edge_EL* e1 = new edge_EL;
	edge_EL* e2 = new edge_EL;
	e1->source = u;
	e2->source = v;
	e1->target = v;
	e2->target = u;
	e1->weight = weight;
	e2->weight = weight;
	edges.push_back(e1);
	edges.push_back(e2);
}

Graph_EL::Graph_EL(int nVertices, double edgeProba, int min, int max) :
    n(nVertices), boost_rep(n)
{

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
                edge_EL* ei = new edge_EL;
                edge_EL* ej = new edge_EL;
		ei->source = i; ej->source = j;
		ei->target = j; ej->target = i;
		ei->weight = w; ej->weight = w;
		edges.push_back(ei);
		edges.push_back(ej);

		// Add edge to boost graph
		boost::add_edge(i,j,w,boost_rep);
            }
        }
    }
}

