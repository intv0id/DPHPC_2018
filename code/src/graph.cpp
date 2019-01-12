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
	adjacency_list[i]->adjacent_vertices_copy.push_back(*ei);
	adjacency_list[j]->adjacent_vertices.push_back(ej);
	adjacency_list[j]->adjacent_vertices_copy.push_back(*ej);
	adjacency_vector[i]->adjacent_vertices.push_back(ei);
	adjacency_vector[j]->adjacent_vertices.push_back(ej);

	// Add edge to boost graph
    boost::add_edge(i,j,w,boost_rep);
    if (process_id(boost_distrib_rep.process_group()) == 0) {
        boost::add_edge(boost::vertex(i,boost_distrib_rep),boost::vertex(j,boost_distrib_rep),w,boost_distrib_rep);
    }
    synchronize(boost_distrib_rep.process_group());
	
	if (i < j) {
		unique_edges.push_back(ei);
		unique_edges_vector.push_back(ei);
	}
	else {
		unique_edges.push_back(ej);
		unique_edges_vector.push_back(ej);
	}

    n_edges++;
	
}
Graph::Graph(int n_) :
	n(n_){

    name="void";
	
    for(int i = 0; i != n; i++){
        vertex_adjacency_list* val = new vertex_adjacency_list;
        val->index = i;
        adjacency_list.push_back(val);
        vertex_adjacency_vector* val0 = new vertex_adjacency_vector;
        val0->index = i;
        adjacency_vector.push_back(val0);
    }
}

Graph::Graph(int nVertices, double edgeProba, int min, int max) :
    n(nVertices), boost_rep(n), boost_distrib_rep(n)
{
    name = "Erdos-Renyi_random_p=" + to_string(edgeProba);

    // For all pair of nodes, generate random edges
    
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(min,max);
    srand(42);
    
    if(edgeProba > 1){
	    edgeProba = 1;
    }
    std::geometric_distribution<> d(edgeProba);


    for(int i = 0; i != nVertices; i++){
        vertex_adjacency_list* val = new vertex_adjacency_list;
        val->index = i;
        adjacency_list.push_back(val);
        vertex_adjacency_vector* val0 = new vertex_adjacency_vector;
        val0->index = i;
        adjacency_vector.push_back(val0);
    }


    for( int i = 0; i !=nVertices; i++){
	int j = i+1;
	while(j < nVertices){
		int delta = d(rng);
		j += delta;
		if(j < nVertices){
			int w = uni(rng);
			
			add_edge(i,j,w);
		}
		j++;
	}
    }
}
Graph::Graph(int nVertices, int m, int min, int max) : n(nVertices)
{

    name = "Preferential_attachment_" + to_string(m) + "_edges_per_vertice";

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
        vertex_adjacency_vector* val0 = new vertex_adjacency_vector;
        val0->index = i;
        adjacency_vector.push_back(val0);
    }

    vector<int> vect{0,0,1,1,2,2};

    for (int i = 3; i != nVertices; i++){
        int new_neighbours[m];
        for (int k = 0; k != m; k++) {
            // Generate random nodes to connect
            uniform_int_distribution<int> dist(0, vect.size() - 1);
            new_neighbours[k] = vect[dist(rng)];	
        }
        for (int k = 0; k != m; k++) {
            int w = uni(rng);
            add_edge(i,new_neighbours[k],w);
            vect.push_back(i);
            vect.push_back(new_neighbours[k]);
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
        vertex_adjacency_vector* val0 = new vertex_adjacency_vector;
        val0->index = i;
        adjacency_vector.push_back(val0);
    }

    while (inFile >> a >> i >> j >> w) {
        add_edge(i-1,j-1,w);
    }
    
    inFile.close(); 
}

Graph::Graph(Graph& h) : name(h.name), n(h.n), n_edges(0), boost_rep(h.n), boost_distrib_rep(h.n) {
	    // Init adj list
	    for(int i = 0; i != n; i++){
		vertex_adjacency_list* val = new vertex_adjacency_list;
		val->index = i;
		adjacency_list.push_back(val);
		vertex_adjacency_vector* val0 = new vertex_adjacency_vector;
		val0->index = i;
		adjacency_vector.push_back(val0);
	    }
	    // Add all edges
	    for(edge* e : h.unique_edges){
		add_edge(e->source,e->target,e->weight);
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
	for(auto v : adjacency_vector){
		delete v;
	}
	boost_rep.clear();
    boost_distrib_rep.clear();
}
