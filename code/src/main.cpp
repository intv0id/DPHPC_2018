#include <iostream>
#include <fstream>
#include <mpi.h>
#include <omp.h>

#include "graph.hpp"
#include "common.hpp"
#include "verifier.hpp"
#include "experiments.hpp"
#include "parser.hpp"


using namespace std;

/*
 * MAIN: keep it simple
 */

int main(int argc, char *argv[]){
    // Initialize MPI
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    parser P(&argc, &argv, rank);

    MPI_Finalize();
    return 0;
}
