#include <boost/mpi.hpp>

#include <mpi.h>
#include "parser.hpp"

/*
 * MAIN: keep it simple
 */

int main(int argc, char *argv[]){
    // Initialize MPI
    
    boost::mpi::environment env{argc, argv};
    boost::mpi::communicator world;
    std::cout << world.rank() << ", " << world.size() << '\n';

    /* int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    */
    
    parser P(&argc, &argv, world.rank());

    // MPI_Finalize();
    return 0;
}
