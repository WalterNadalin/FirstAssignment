/**
 * @author RookieHPC
 * @brief Original source code at https://www.rookiehpc.com/mpi/docs/mpi_cart_create.php
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <mpi.h>
 
/**
 * @brief Illustrates how to create a communicator representing a 2D torus
 * topology.
 **/

int main(int argc, char* argv[])
{

    MPI_Init(&argc, &argv);
 
    // Size of the default communicator
    int size;
    int dim = 3;
    int old_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &old_rank);  

    // printf("[MPI process %d] I am .\n",old_rank);
    // sleep(12);
    // MPI_Barrier(MPI_COMM_WORLD);

    // Ask MPI to decompose our processes in a 2D cartesian grid for us
    int dims[dim];
    //dims[0] = 24;
    //dims[1] = 1;
    //dims[2] = 1;
    for(unsigned i = 0; i < dim; i ++) dims[i] = 0;
    MPI_Dims_create(size, dim, dims);

    printf("Number of processors for each direction [");
    for(unsigned i = 0; i < dim; i ++) printf("%d, ", dims[i]);
    printf("\b\b]\n");
 
    // Make both dimensions periodic
    int periods[dim];
    for(unsigned i = 0; i < dim; i ++) periods[i] = true;
 
    // Let MPI assign arbitrary ranks if it deems it necessary
    int reorder = true;
 
    // Create a communicator given the 2D torus topology.
    MPI_Comm new_communicator;
    MPI_Cart_create(MPI_COMM_WORLD, dim, dims, periods, reorder, &new_communicator);
 
    // My rank in the new communicator
    int my_rank;
    MPI_Comm_rank(new_communicator, &my_rank);

    // Get my coordinates in the new communicator
    int my_coords[dim];
    MPI_Cart_coords(new_communicator, my_rank, dim, my_coords);  
    //MPI_Barrier(MPI_COMM_WORLD); 

    // Print my location in the 2D torus.
    printf("[MPI process, old %d, new %d] I am located at (", old_rank, my_rank);
    for(unsigned i = 0; i < dim; i ++) printf("%d, ", my_coords[i]);
    printf("\b\b)\n");

    MPI_Barrier(new_communicator);	

   /* int dummy_rank, dummy_coords[dim];
    
    if(my_rank == size - 1) {

	for(unsigned i = 0; i < dims[0]; i ++) {

		for(unsigned j = 0; j < dims[1]; j ++) {
		
			dummy_coords[0] = i;
			dummy_coords[1] = j;
			MPI_Cart_rank(new_communicator, dummy_coords, &dummy_rank);
			printf("%d - (%d, %d) \t", dummy_rank, dummy_coords[0], dummy_coords[1]);

        	}

		printf("\n");

  	 }

   }*/
   
   MPI_Barrier(new_communicator);
   int rank_prev, rank_next, direction = 0, displace = 1;
   MPI_Cart_shift(new_communicator, direction, displace, &rank_prev, &rank_next);

   printf("[MPI process %d] Considering a displacement %d on the %d direction: %d --> %d --> %d\n", my_rank, displace, direction, rank_prev, my_rank, rank_next);
 
   MPI_Finalize();
 
    return EXIT_SUCCESS;
}
