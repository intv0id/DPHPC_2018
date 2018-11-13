#include <iostream>
#include <fstream>
#include <list>

#include "common.hpp"
#include "graph.hpp"

using namespace std;

class LsbTimer{
	ofstream o;
	const list<mst_algorithm*> algorithms;
	public:
	// Open file, create list of algorithms
	Timer(string filename,list<mst_algorithm*> algorithms);
	// Time a list of graphs
	void time(list<Graph&> g_list) ;
	~Timer(){
		o.close();
	}
};
