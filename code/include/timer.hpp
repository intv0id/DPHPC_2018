#include <iostream>
#include <fstream>
#include <list>

#include "common.hpp"
#include "graph.hpp"

using namespace std;

class Timer{
	ofstream o;
	const list<mst_algorithm*> algorithms;
	public:
	// Open file, create list of algorithms
	Timer(string name,list<mst_algorithm*> algorithms); 
	// Print a string
	void printF(string field, string s) ;
	void printF(string field, int s) ;
	// Time a graph
	void time(Graph& g) ;
	~Timer(){
		o.close();
	}
};
