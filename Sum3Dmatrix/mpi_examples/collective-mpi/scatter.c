#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {

	float a[8], b[2];
	MPI_Init(&argc, &argv); // Initialization

	int rank, size, root = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(rank == root) {

		printf("Array to be scattered:\n");
		for(unsigned i; i < 8; i++) { 

			a[i] = (float)i;
			printf("a[%d] = %f\n", i, a[i]);

		}
	
	}

	if(rank == root) printf("About to begin the scattering\n");

	MPI_Scatter(a, 2, MPI_FLOAT, b, 2, MPI_FLOAT, root, MPI_COMM_WORLD);

	if(rank == root) printf("Scattering ended\n");
	
	printf("Processor %d now has b[0] = %f and b[1] = %f\n", rank, b[0], b[1]);
	
	// free(data);
	MPI_Finalize();

}
