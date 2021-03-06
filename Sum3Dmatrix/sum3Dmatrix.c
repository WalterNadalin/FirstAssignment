#include <stdio.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/* generate a random floating point number from min to max */
double randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

int main(int argc, char** argv) {
	
	MPI_Init(&argc, &argv);

        FILE *time_file;
        unsigned dim_x, dim_y, dim_z, dim;	
        unsigned old_rank, size, root = 0;

        MPI_Comm_rank(MPI_COMM_WORLD, &old_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(old_rank == root) {
		 
		printf("Insert number of elements on the direction 0: \n");
		scanf("%d", &dim_x);
		printf("Insert number of elements on the direction 1: \n");
		scanf("%d", &dim_y);
		printf("Insert number of elements on the direction 2: \n");
		scanf("%d", &dim_z);
		printf("Insert virtual topology dimensions: \n");
		scanf("%d", &dim);
		 
	 }
	
	MPI_Bcast(&dim_x, 1, MPI_INT, root, MPI_COMM_WORLD);
	MPI_Bcast(&dim_y, 1, MPI_INT, root, MPI_COMM_WORLD);
	MPI_Bcast(&dim_z, 1, MPI_INT, root, MPI_COMM_WORLD);
	MPI_Bcast(&dim, 1, MPI_INT, root, MPI_COMM_WORLD);
	
	unsigned dim_mat = dim_x * dim_y * dim_z;
	double *a = (double *)calloc(dim_mat, sizeof(double));
	double *b = (double *)calloc(dim_mat, sizeof(double));
	double *sum = (double *)calloc(dim_mat, sizeof(double));
	unsigned *dims = (unsigned *)calloc(dim, sizeof(unsigned));


	if(old_rank == root) {

		for(unsigned i = 0; i < dim; i++) {

   			printf("Insert the number of processors on the direction %d", i);
    			scanf("%d", &dims[i]);

		}

	}
	
  	// MPI_Dims_create(size, dim, dims);

	if(old_rank == root) {

   		printf("Number of processors for each direction [");
    		for(unsigned i = 0; i < dim; i ++) printf("%d, ", dims[i]);
    		printf("\b\b]\n");

	}

	MPI_Bcast(dims, dim, MPI_INT, root, MPI_COMM_WORLD);
/*
   	printf("Number of processors for each direction [");
    	for(unsigned i = 0; i < dim; i ++) printf("%d, ", dims[i]);
    	printf("\b\b]\n");
*/ 
    	unsigned *periods = (unsigned *)calloc(dim, sizeof(unsigned));
    	for(unsigned i = 0; i < dim; i ++) periods[i] = false;
	
	// Let MPI assign arbitrary ranks if it deems it necessary
    	int reorder = true;

        if(old_rank == root) {

		srand (time ( NULL));

                for(unsigned i = 0; i < dim_z; i ++) {

                        for(unsigned j = 0; j < dim_x; j ++) {

                                for(unsigned k = 0; k < dim_y; k ++) {

					a[i * dim_x * dim_y + j * dim_y + k] = randfrom(-100, 100);
					b[i * dim_x * dim_y + j * dim_y + k] = randfrom(-100, 100);
                                        //a[i * dim_x * dim_y + j * dim_y + k] = 0.01 * i + j + k * 100;
                                        //b[i * dim_x * dim_y + j * dim_y + k] = 0.01 * i + j + k * 100;
					
                                        //printf("a[%d, %d, %d]: %f\t", k, j, i, a[i * dim_x * dim_y + j * dim_y + k]);
                                        //printf("b[%d, %d, %d]: %f\t", k, j, i, b[i * dim_x * dim_y + j * dim_y + k]);

                                }

                                //printf("\n");

                        }

                        //printf("\n");
                                
                }

        }
 
    	// Create a new communicator
    	MPI_Comm new_communicator;
    	MPI_Cart_create(MPI_COMM_WORLD, dim, dims, periods, reorder, &new_communicator);
 
   	// My rank in the new communicator
    	int new_rank;
   	MPI_Comm_rank(new_communicator, &new_rank);

    	// Get my coordinates in the new communicator
    	unsigned *my_coords = (unsigned *)calloc(dim, sizeof(unsigned));
    	MPI_Cart_coords(new_communicator, new_rank, dim, my_coords);  

    	printf("[MPI process, old %d, new %d] I am located at (", old_rank, new_rank);
    	for(unsigned i = 0; i < dim; i ++) printf("%d, ", my_coords[i]);
    	printf("\b\b)\n");
        
        unsigned elements = dim_mat / size;
   	
	double *scattered_a = (double *)calloc(elements, sizeof(double));
	double *scattered_b = (double *)calloc(elements, sizeof(double));
	double *scattered_sum = (double *)calloc(elements, sizeof(double));

	double begin = MPI_Wtime();

        MPI_Scatter(a, elements, MPI_DOUBLE, scattered_a, elements, MPI_DOUBLE, root, new_communicator);
        MPI_Scatter(b, elements, MPI_DOUBLE, scattered_b, elements, MPI_DOUBLE, root, new_communicator);

	for(unsigned i = 0; i < elements; i++) {
	
		scattered_sum[i] = scattered_a[i] + scattered_b[i];
	
	}
	
	MPI_Gather(scattered_sum, elements, MPI_DOUBLE, sum, elements, MPI_DOUBLE, root, new_communicator);

	double end = MPI_Wtime();

	time_file = fopen("time_sum3Dmatrix.dat", "a");	
	fprintf(time_file, "Time of processor %d: %f\n", new_rank, end - begin);
	fclose(time_file);

	/*if(new_rank == root) {

                for(unsigned i = 0; i < dim_z; i ++) {

                        for(unsigned j = 0; j < dim_x; j ++) {

                                for(unsigned k = 0; k < dim_y; k ++) {

                                        printf("sum[%d, %d, %d]: %f\t", k, j, i, sum[i * dim_x * dim_y + j * dim_y + k]);

                                }

                                printf("\n");

                        }

                        printf("\n");

                }

	}*/
	
	MPI_Finalize();
	
        return 0;

}
