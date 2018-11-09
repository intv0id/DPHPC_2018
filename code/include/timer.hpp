#include <iostream>
#include <fstream>

#include "graph.hpp"

using namespace std;

class Timer{
	public:
	ofstream o;
	Timer(string name); 
	void printF(int n);
	void time(l_edge_t (Graph&), Graph& g);
};
