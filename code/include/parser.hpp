#ifndef PARSER_H
#define PARSER_H

#include "common.hpp"
#include "graph.hpp"

using namespace std;

class parser {
	list<mst_algorithm*> selected_algorithms;
	list<Graph*> selected_graphs;

	int max_threads = 1;
	int runs = 5;
	string lsb_name = "measures";

	// Variables for graph generation
	const int edgePerVertex = 20;
	const int minWeight = 0;
	const int maxWeight = 100;

	public:
	parser(int *argc, char **argv[], int MPI_rank);
	void print_help(int MPI_rank, bool syntax_error=false);
	bool parse_algonames(string algoname);
	void print_algos();
    void compute();
};

#endif
