#include <mpi.h>

#include "parser.hpp"

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
