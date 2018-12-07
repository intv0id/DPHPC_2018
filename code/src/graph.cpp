#include <graph.hpp>

#include <iomanip>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

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
		unique_edges_vector.push_back(ei);
	}
	else {
		unique_edges.push_back(ej);
		unique_edges_vector.push_back(ej);
	}
	
	//adjacency_matrix[u][v] = weight;
	/*
	adjacency_list[u].push_back(e1);
	adjacency_list[v].push_back(e2);
	*/
}
Graph::Graph(int n_) :
	n(n_){

    name="void";
	
    for(int i = 0; i != n; i++){
        vertex_adjacency_list* val = new vertex_adjacency_list;
        val->index = i;
        adjacency_list.push_back(val);
    }

	
}
Graph::Graph(int nVertices, double edgeProba, int min, int max) :
    n(nVertices), boost_rep(n)
{
    name = "Erdos-Renyi_random";

    // For all pair of nodes, generate random edges
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(min,max);
    srand(42);


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
Graph::Graph(int nVertices, int m, int min, int max) : n(nVertices)
{

    name = "Preferential attachment";

    if (nVertices < 3 || m > nVertices) {
        cout << "Preferential Attachment graph should have at least 3 vertices" << endl;
        exit(1);
    }
	
    // For all pair of nodes, generate random edges
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(min,max);

    for(int i = 0; i != nVertices; i++){
        vertex_adjacency_list* val = new vertex_adjacency_list;
        val->index = i;
        adjacency_list.push_back(val);
    }

    vector<int> vect{1,1,2,2,3,3};

    for (int i = 4; i != nVertices; i++){
	for (int k = 0; k != m; k++) {
	    // Generate random node to connect
	    uniform_int_distribution<int> dist(0, vect.size() - 1);
	    int j = vect[dist(rng)];	

	    // Generate weight
            int w = uni(rng);
            add_edge(i,j,w);
    	    vect.push_back(i);
	    vect.push_back(j);

	}
    }
}
Graph::Graph(string fname, string type){

    name = "USA-road-"+ type + "." + name;

    int nVertices = 0;
    char a;
    int i, j, w;
    ifstream inFile;

    inFile.open("USA_graphs/USA-road-"+ type + "." + fname +".gr");
    if (!inFile) {
        cout << "Unable to open file";
        exit(1);
    }

    for (int count=0; count !=7; count++) {
        string line;
        getline(inFile, line);
	
	if (count == 4){
	    string s1, s2;
	    int nEdges;
	    stringstream ss(line);
	    ss >> s1 >> s2 >> nVertices >> nEdges;
	    
	    n = nVertices;
	}
    }

    for(int i = 0; i != nVertices; i++){
        vertex_adjacency_list* val = new vertex_adjacency_list;
        val->index = i;
        adjacency_list.push_back(val);
    }

    while (inFile >> a >> i >> j >> w) {
        add_edge(i-1,j-1,w);
    }
    
    inFile.close(); 
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
