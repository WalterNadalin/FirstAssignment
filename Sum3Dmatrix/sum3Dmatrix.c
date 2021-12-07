#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {

        unsigned cols = 10, rows = 240, dim = cols * rows;
        double a[dim], b[dim], sum[dim];

        MPI_Init(&argc, &argv);

        unsigned rank, size, root = 0;

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        
        unsigned elements = dim / size;
        if(rank < dim % size) elements ++;

        double scattered_a[elements], scattered_b[elements], scattered_sum[elements];

        if(rank == root) {

                for(unsigned i = 0; i < rows; i ++) {

                        for(unsigned j = 0; j < cols; j ++) {

                                a[i * cols + j] = i + j * 0.01;
                                b[i * cols + j] = i + j * 0.01;
                                // printf("%d,%d:%f  ", i, j, A[i * cols + j]);

                        }

                        // printf("\n");

                }

        }
        
        MPI_Scatter(a, elements, MPI_DOUBLE, scattered_a, elements, MPI_DOUBLE, root, MPI_COMM_WORLD);
        MPI_Scatter(b, elements, MPI_DOUBLE, scattered_b, elements, MPI_DOUBLE, root, MPI_COMM_WORLD);

	for(unsigned i = 0; i < elements; i++) {
	
		scattered_sum[i] = scattered_a[i] + scattered_b[i];
	
	}
	
	MPI_Gather(scattered_sum, elements, MPI_DOUBLE, sum, elements, MPI_DOUBLE, root, MPI_COMM_WORLD);
	
	if(rank == root) {

                for(unsigned i = 0; i < rows; i ++) {

                        for(unsigned j = 0; j < cols; j ++) {

                                printf("%d,%d:%f  ", i, j, sum[i * cols + j]);

                        }

                        printf("\n");

                }

        }
	
	MPI_Finalize();
	
        return 0;

}
