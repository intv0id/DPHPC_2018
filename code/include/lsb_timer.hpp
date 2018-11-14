#include <iostream>
#include <fstream>
#include <list>
#include <liblsb.h>

#include "common.hpp"
#include "graph.hpp"

using namespace std;

class LsbTimer{
	ofstream o;
	const list<mst_algorithm*> algorithms;
	public:
	// Open file, create list of algorithms
	LsbTimer(list<mst_algorithm*> algorithms);
	// Time a list of graphs
	void clock(list<Graph*> g_list);
	~LsbTimer(){
		o.close();
	}
};
