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
	const int edgePerVertex = 1000;
<<<<<<< HEAD
	const int minWeight = 1;
=======
	const int minWeight = 0;
>>>>>>> de1ce2ef2aba81840d4de1425740482a62a72df6
	const int maxWeight = 100;

	public:
	parser(int *argc, char **argv[], int MPI_rank);
	void print_help(int MPI_rank, bool syntax_error=false);
	bool parse_algonames(string algoname);
	void print_algos();
    void compute();
	void check_correctness();
};

#endif
