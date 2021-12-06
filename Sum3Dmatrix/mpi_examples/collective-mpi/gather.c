#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {

	float a[8], b[2];
	MPI_Init(&argc, &argv); // Initialization

	int rank, size, root = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	b[0] = 2 * rank;
	b[1] = 2 * rank + 1;

	printf("Processor %d will sent b[0] = %f and b[1] = %f to be gathered\n", rank, b[0], b[1]);

	if(rank == root) printf("About to begin the gathering\n");

	MPI_Gather(b, 2, MPI_FLOAT, a, 2, MPI_FLOAT, root, MPI_COMM_WORLD);

	if(rank == root) printf("Gathering ended\n");	

	if(rank == root) {

		printf("Array gathered:\n");
		for(unsigned i; i < 8; i++) printf("a[%d] = %f\n", i, a[i]);
	
	}
	
	// free(data);
	MPI_Finalize();

}
