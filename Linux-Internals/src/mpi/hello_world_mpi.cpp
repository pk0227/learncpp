// Program 1: Basic MPI Initialization and Finalization
#if __has_include(<mpi.h>)
#include <mpi.h>
#else
#include "mpi.h"
#endif
#include <iostream>
int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::cout << "Hello from rank " << rank << " out of " << size << std::endl;
    MPI_Finalize();
    return 0;
}
