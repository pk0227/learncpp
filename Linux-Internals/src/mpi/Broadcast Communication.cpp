// Program 3: Broadcast Communication
#if __has_include(<mpi.h>)
#include <mpi.h>
#else
#include "mpi.h"
#endif
#include <iostream>
int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, data;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        data = 42;
    }
    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
