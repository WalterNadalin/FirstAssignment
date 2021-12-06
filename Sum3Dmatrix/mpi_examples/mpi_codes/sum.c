#include <stdio.h>
#include <mpi.h>

int main (int argc, char * argv[])
{

	unsigned N = 101;
	unsigned op_num, op_rem, gap;
	int array[N]; // Numbers to sum
	int sum, local_sum;

	for(unsigned i = 0; i < N; i ++) array[i] = i;

	// Variables containing some information about the comunication
	unsigned recvd_tag, recvd_from, recvd_count, rank, size;
	int master = 0; // The processor 0 is the master
	int buffer;
	MPI_Status status;

	MPI_Init(&argc, &argv); // Initialization
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Getting the rank
	MPI_Comm_size(MPI_COMM_WORLD, &size); // Getting the size

	op_num = N / size; // Number of operations to assign to each slave processor
	op_rem = N - (N / size) * size;
	gap = rank * op_num;

	local_sum = 0;

	for(unsigned i = 0; i < op_num; i ++) local_sum += array[gap + i];

	if(rank == master) { // If we are on the master processor
	
		sum = local_sum;

		for(unsigned proc = 1; proc < size; proc ++)  {

			MPI_Recv(&buffer, 1, MPI_INT, proc, MPI_ANY_TAG, MPI_COMM_WORLD, & status);
			
			// Informations about the comunication
			recvd_from = status.MPI_SOURCE;
			printf("The master received %d from %d\n", buffer, recvd_from);
		
			sum += buffer;

		}

		for(unsigned i = 0; i < op_rem; i++) sum += array[N - 1 - i];

		printf("The total sum is equal to %d \n", sum); 

	} else { // If we are on a slave processor

		buffer = local_sum;

		printf("Processor %d is about to send %d to the master\n", rank, buffer);

		// Sending to the master
		MPI_Send(&buffer, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);

		// printf("The master has aknowledged the data from processor %d\n", rank);

	}

	MPI_Finalize();
	return 0;

}
