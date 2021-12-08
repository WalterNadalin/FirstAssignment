#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {

        unsigned dim_x = 24000, dim_y = 100, dim_z = 100, dim = dim_x * dim_y * dim_z;
        double a[dim], b[dim], sum[dim];

        MPI_Init(&argc, &argv);

        unsigned rank, size, root = 0;

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        
        unsigned elements = dim / size;
        if(rank < dim % size) elements ++;

        double scattered_a[elements], scattered_b[elements], scattered_sum[elements];

        if(rank == root) {

                for(unsigned i = 0; i < dim_z; i ++) {

                        for(unsigned j = 0; j < dim_x; j ++) {
				
				for(unsigned k = 0; k < dim_y; k ++) {

	                                a[i * dim_x * dim_y + j * dim_y + k] = 0.01 * i + j + k * 100;
        	                        b[i * dim_x * dim_y + j * dim_y + k] = 0.01 * i + j + k * 100;
                	                // printf("%d,%d,%d:%f ", k, j, i, a[i * dim_x * dim_y + j * dim_y + k]);
				}

				// printf("\n");

                        }

                        // printf("\n");

                }

        }
        
	double begin = MPI_Wtime();

        MPI_Scatter(a, elements, MPI_DOUBLE, scattered_a, elements, MPI_DOUBLE, root, MPI_COMM_WORLD);
        MPI_Scatter(b, elements, MPI_DOUBLE, scattered_b, elements, MPI_DOUBLE, root, MPI_COMM_WORLD);

	for(unsigned i = 0; i < elements; i++) {
	
		scattered_sum[i] = scattered_a[i] + scattered_b[i];
	
	}
	
	MPI_Gather(scattered_sum, elements, MPI_DOUBLE, sum, elements, MPI_DOUBLE, root, MPI_COMM_WORLD);

	double end = MPI_Wtime();

	printf("Time of processor %d: %f\n", rank, end - begin);
	/*
        if(rank == root) {

                for(unsigned i = 0; i < dim_z; i ++) {

                        for(unsigned j = 0; j < dim_x; j ++) {
				
				for(unsigned k = 0; k < dim_y; k ++) {

                	                printf("%d,%d,%d:%f ", k, j, i, sum[i * dim_x * dim_y + j * dim_y + k]);
				
				}

				printf("\n");

                        }

                        printf("\n");

                }

        }
	*/
	
	MPI_Finalize();
	
        return 0;

}
