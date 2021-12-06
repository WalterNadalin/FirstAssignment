#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

int main(int argc, char** argv) {

	// int num_elements = 2;
	// MPI(NULL, NULL);
	float a[2];
	MPI_Init(&argc, &argv); // Initialization

	int rank, root = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("Processor %d has initialized the array with a[0] = %f and a[1] = %f\n", rank, a[0], a[1]);

	// int* data = (int*)malloc(sizeof(int) * num_elements);
	// assert(data != NULL);
	
	if(rank == root) {

		a[0] = 1.0;
		a[1] = 2.0;
		printf("The master has modified the array\n");
	
	}

	if(rank == root) printf("The master is about to start the broadcasting\n");

	MPI_Bcast(a, 2, MPI_FLOAT, root, MPI_COMM_WORLD);

	if(rank == root) printf("The master has ended the broadcasting\n");
	
	printf("Processor %d now has a[0] = %f and a[1] = %f\n", rank, a[0], a[1]);
	
	// free(data);
	MPI_Finalize();

}
