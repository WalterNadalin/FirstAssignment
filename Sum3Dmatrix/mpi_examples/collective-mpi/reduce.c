#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {

	float a[2], sum[2];
	MPI_Init(&argc, &argv); // Initialization

	int rank, size, root = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	a[0] = 1;
	a[1] = 2;

	printf("Processor %d has a[0] = %f and a[1] = %f\n", rank, a[0], a[1]);
	
	if(rank == root) printf("About to begin the reducing\n");

	MPI_Reduce(a, sum, 2, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);

	if(rank == root) printf("The reducing has ended and the root now has sum[0] = %f and sum[1] = %f\n", sum[0], sum[1]);
	
	// free(data);
	MPI_Finalize();

}
