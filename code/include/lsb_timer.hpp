#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <liblsb.h>

#include "common.hpp"
#include "graph.hpp"

using namespace std;

class LsbTimer{
	string filename;
	unsigned int runs;
    unsigned int max_threads;
	const list<mst_algorithm*> algorithms;
	public:
	// Open file, create list of algorithms
	LsbTimer(list<mst_algorithm*> algorithms, string fname, unsigned int m_threads, unsigned int n_runs);
	// Time a list of graphs
	void clock(list<Graph*> g_list);
    void print_times(vector<double> times);
    double mean(vector<double> times, int runs);
    double std(vector<double> times, double m);
    double median(vector<double> times);
	~LsbTimer(){}
};
