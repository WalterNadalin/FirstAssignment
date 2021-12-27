#include <stdio.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// generate a random floating point number from min to max 
double randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

int main(int argc, char** argv) {
	
	MPI_Init(&argc, &argv);

        unsigned dim_x, dim_y, dim_z, dim; // Dimension of the matrix along the 3 directions and dimension of the virtual topology	
        unsigned old_rank, size, root = 0;

        MPI_Comm_rank(MPI_COMM_WORLD, &old_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	// Reading from the standard input the various dimensions only from the processor 0
	if(old_rank == root) {
		 
		printf("Insert number of elements on the direction 0:\n");
		scanf("%d", &dim_x);
		printf("Insert number of elements on the direction 1:\n");
		scanf("%d", &dim_y);
		printf("Insert number of elements on the direction 2:\n");
		scanf("%d", &dim_z);
		printf("Insert virtual topology dimension:\n");
		scanf("%d", &dim);
		 
	 }
	
	// Broadcasting the value obtained to every other processors
	MPI_Bcast(&dim_x, 1, MPI_INT, root, MPI_COMM_WORLD);
	MPI_Bcast(&dim_y, 1, MPI_INT, root, MPI_COMM_WORLD);
	MPI_Bcast(&dim_z, 1, MPI_INT, root, MPI_COMM_WORLD);
	MPI_Bcast(&dim, 1, MPI_INT, root, MPI_COMM_WORLD);
	
	// Allocating all the variables which depends on the dimensions, in particular the matrices
	unsigned dim_mat = dim_x * dim_y * dim_z, elements = dim_mat / size;
	
	unsigned *dims = (unsigned *)calloc(dim, sizeof(unsigned)); 
	unsigned *periods = (unsigned *)calloc(dim, sizeof(unsigned)), *my_coords = (unsigned *)calloc(dim, sizeof(unsigned)); 
	
	double *a = (double *)calloc(dim_mat, sizeof(double)), *scattered_a = (double *)calloc(elements, sizeof(double)); // First matrix to sum
	double *b = (double *)calloc(dim_mat, sizeof(double)), *scattered_b = (double *)calloc(elements, sizeof(double)); // Second matrix to sum
	double *sum = (double *)calloc(dim_mat, sizeof(double)), *scattered_sum = (double *)calloc(elements, sizeof(double)); // Matrix containing the results of the sum
	
        if(old_rank == root) {

		srand (time ( NULL)); // Seeding with the time

		// Inizitializing the matrices to sum with random doubles between -100 and 100
                for(unsigned i = 0; i < dim_mat; i ++) { 

			a[i * dim_x * dim_y + j * dim_y + k] = randfrom(-100, 100);
			b[i * dim_x * dim_y + j * dim_y + k] = randfrom(-100, 100);

                }

		// Reading the number of processors on each direction of the virtual topology
		for(unsigned i = 0; i < dim; i++) {

   			printf("Insert the number of processors on the direction %d\n", i);
    			scanf("%d", &dims[i]);

		}

	}

	MPI_Bcast(dims, dim, MPI_INT, root, MPI_COMM_WORLD);

    	for(unsigned i = 0; i < dim; i ++) periods[i] = false; // Without PBC
    	int new_rank, reorder = true; // Let MPI assign arbitrary ranks if it deems it necessary
    	
    	MPI_Comm new_communicator; 
    	MPI_Cart_create(MPI_COMM_WORLD, dim, dims, periods, reorder, &new_communicator); // Virtual topology within the new communicator
   	MPI_Comm_rank(new_communicator, &new_rank); // Gets the rank in the new communicator
    	MPI_Cart_coords(new_communicator, new_rank, dim, my_coords); // Gets the coordinates in the topology

    	printf("[MPI process, old %d, new %d] I am located at (", old_rank, new_rank);
    	for(unsigned i = 0; i < dim; i ++) printf("%d, ", my_coords[i]);
    	printf("\b\b)\n");
    	
        MPI_Barrier(new_communicator);
	double begin = MPI_Wtime(); // Starting the measure of the time

	// -- SCATTERING, SUMMING AND GATHERING -------------------------------------------------------------- 
	
        MPI_Scatter(a, elements, MPI_DOUBLE, scattered_a, elements, MPI_DOUBLE, root, new_communicator);
        MPI_Scatter(b, elements, MPI_DOUBLE, scattered_b, elements, MPI_DOUBLE, root, new_communicator);

	for(unsigned i = 0; i < elements; i++) {
	
		scattered_sum[i] = scattered_a[i] + scattered_b[i];
	
	}
	
	MPI_Gather(scattered_sum, elements, MPI_DOUBLE, sum, elements, MPI_DOUBLE, root, new_communicator);
	
	// --------------------------------------------------------------------------------------------------- 

	double end = MPI_Wtime(); // Ending the measure

	printf(time_file, "Time take by the processor %d to perform the sum and the communication: %f\n", new_rank, end - begin);

	MPI_Finalize();
	
        return 0;

}
