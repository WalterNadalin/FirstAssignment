#include <stdio.h>
#include <mpi.h>

int main (int argc, char * argv[])
{

	int recvd_tag, recvd_from, recvd_count, rank, size, buffer;
	MPI_Status status;

	MPI_Init(&argc, &argv); // Initialization
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Getting the rank
	MPI_Comm_size(MPI_COMM_WORLD, &size); // Getting the size

	int left_processor = rank - 1, right_processor = rank + 1;

	if(rank == 0) left_processor = size - 1;
	if(rank == size - 1) right_processor = 0;

	if(rank % 2 == 0) {
	
		buffer = rank + 10;

		// Sending the buffer to processor on the right
		MPI_Send(&buffer, 1, MPI_INT, right_processor, 1, MPI_COMM_WORLD);

		// Receveing the buffer from processor on the left
		MPI_Recv(&buffer, 1, MPI_INT, left_processor, 1, MPI_COMM_WORLD, &status);		

		// Getting some informations about the comunication
		recvd_tag = status.MPI_TAG;
		recvd_from = status.MPI_SOURCE;
		MPI_Get_count(&status, MPI_INT, &recvd_count);

		// Printing some informations
		printf("\nReceiver: %d\tSender: %d\tTag: %d\tMessage lenght: %d\tMessage content: %d\n", rank, recvd_from, recvd_tag, recvd_count, buffer);

	} else { 

		// Receveing the buffer from processor on the left
		MPI_Recv(&buffer, 1, MPI_INT, left_processor, 1, MPI_COMM_WORLD, &status);

		// Getting some informations about the comunication
		recvd_tag = status.MPI_TAG;
		recvd_from = status.MPI_SOURCE;
		MPI_Get_count(&status, MPI_INT, &recvd_count);

		// Printing some informations
		printf("\nReceiver: %d\tSender: %d\tTag: %d\tMessage lenght: %d\tMessage content: %d\n", rank, recvd_from, recvd_tag, recvd_count, buffer);

		buffer = rank + 10;

		// Sending the buffer to processor to the right
		MPI_Send(&buffer, 1, MPI_INT, right_processor, 1, MPI_COMM_WORLD);

	}

	MPI_Finalize();
	return 0;

}
