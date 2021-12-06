// a simple implementation of the ring exercises
// taken from Bill Gropp's tutorial and slightly adapted
#include <stdio.h>
#include "mpi.h"
#include <stdbool.h>

int main(argc, argv)
int argc;
char **argv;
{

    int rank, value, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    
    value = 10;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    float start_time = MPI_Wtime();
 
    if(rank == 0) {

	 MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
	 MPI_Recv(&value, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);    

    } else {

	    MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);

	    if(rank < size - 1) MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
	    else MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    
    }

    printf("Process %d got %d from %d\n", rank, value, status.MPI_SOURCE);
 
    float end_time = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);
    printf("Walltime of process %d without reordering: %f\n", rank, end_time - start_time);

    int new_rank, periods = true;

    MPI_Comm new_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 1, &size, &periods, true, &new_comm);

    MPI_Comm_rank(new_comm, &new_rank);

    printf("I was %d and now I am %d\n", rank, new_rank);   

    MPI_Finalize();

    return 0;

}
