#include <iostream>

#include <graph.hpp>


#include "algorithms.hpp"

using namespace std;

int main(){
	cout << "Hello world !" << endl;
	Graph g(10,0.5,0,10);
	g.printGraph();
	Kruskal k(g);
	vector<edge_C> MST = k.compute();
}
