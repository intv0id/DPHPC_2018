#ifndef PARSER_H
#define PARSER_H

#include "common.hpp"
#include "graph.hpp"

using namespace std;

class parser {
	list<mst_algorithm*> selected_algorithms;
	list<Graph*> selected_graphs;

	vector<unsigned int> threads;
	int runs = 5;
	string lsb_name = "measures";

	// Variables for graph generation
	const int edgePerVertex = 1000;
	const int minWeight = 0;
	const int maxWeight = 100;

	public:
	parser(int *argc, char **argv[], int MPI_rank);
	void print_help(int MPI_rank, bool syntax_error=false);
	bool parse_algonames(string algoname);
	void print_algos();
    vector<unsigned int> thread_list(unsigned int min, unsigned int max, unsigned int step);
    vector<unsigned int> thread_list(unsigned int min, unsigned int max);
    void compute();
	void check_correctness();
};

#endif
