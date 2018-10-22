#include <iostream>
#include <liblsb.h>
#include <mpi.h>


#include <graph.hpp>
#include "algorithms.hpp"

#define N 100
#define RUNS 10


using namespace std;

int main(){
	int rank;

	cout << "Hello world !" << endl;
	Graph g(10,0.5,0,10);
	g.printGraph();
	Kruskal k(g);

	// Enter test section
	MPI_Init(&argc, &argv);
	LSB_Init("test", 0);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	LSB_Set_Rparam_int("rank", rank);
    LSB_Set_Rparam_int("runs", RUNS);
	for(int i=0;i<TEST_NB;++i){
		for(int j=0;j<INNER_TESTS;==j){
			LSB_Res();
			vector<edge_C> MST = k.compute();
			LSB_Rec(i);
	}}
	LSB_Finalize();
	MPI_Finalize();
	return 0;
}
