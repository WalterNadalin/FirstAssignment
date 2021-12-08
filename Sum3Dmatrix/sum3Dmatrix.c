#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {

        unsigned dim_x = 24000, dim_y = 100, dim_z = 100, dim_mat = dim_x * dim_y * dim_z, dim = 3;
        double a[dim_mat], b[dim_mat], sum[dim_mat];

        MPI_Init(&argc, &argv);

        unsigned old_rank, size, root = 0;

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

	unsigned dims[dim];
 
  	for(unsigned i = 0; i < dim; i ++) dims[i] = 0;
  	MPI_Dims_create(size, dim, dims);

   	printf("Number of processors for each direction [");
    	for(unsigned i = 0; i < dim; i ++) printf("%d, ", dims[i]);
    	printf("\b\b]\n");
 
    	int periods[dim];
    	for(unsigned i = 0; i < dim; i ++) periods[i] = false;
 
    	// Let MPI assign arbitrary ranks if it deems it necessary
    	int reorder = true;
 
    	// Create a new communicator
    	MPI_Comm new_communicator;
    	MPI_Cart_create(MPI_COMM_WORLD, dim, dims, periods, reorder, &new_communicator);
 
   	// My rank in the new communicator
    	int my_rank;
   	MPI_Comm_rank(new_communicator, &my_rank);

    	// Get my coordinates in the new communicator
    	int my_coords[dim];
    	MPI_Cart_coords(new_communicator, my_rank, dim, my_coords);  

    	printf("[MPI process, old %d, new %d] I am located at (", old_rank, my_rank);
    	for(unsigned i = 0; i < dim; i ++) printf("%d, ", my_coords[i]);
    	printf("\b\b)\n");
        
        unsigned elements = dim_mat / size;
        if(rank < dim_mat % size) elements ++;

        double scattered_a[elements], scattered_b[elements], scattered_sum[elements];

	double begin = MPI_Wtime();

        MPI_Scatter(a, elements, MPI_DOUBLE, scattered_a, elements, MPI_DOUBLE, root, new_communicator);
        MPI_Scatter(b, elements, MPI_DOUBLE, scattered_b, elements, MPI_DOUBLE, root, new_communicator);

	for(unsigned i = 0; i < elements; i++) {
	
		scattered_sum[i] = scattered_a[i] + scattered_b[i];
	
	}
	
	MPI_Gather(scattered_sum, elements, MPI_DOUBLE, sum, elements, MPI_DOUBLE, root, new_communicator);

	double end = MPI_Wtime();

	printf("Time of processor %d: %f\n", new_rank, end - begin);
	
	MPI_Finalize();
	
        return 0;

}