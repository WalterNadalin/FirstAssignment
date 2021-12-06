#include <stdio.h>
#include <mpi.h>

int main (int argc, char * argv[])
{

	int recvd_tag, recvd_from, recvd_count, rank, buffer;
	MPI_Status status;

	MPI_Init(&argc, &argv); // Initialization
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Getting the rank

	if(rank == 0) { // If we are on the processor 0
	
		buffer = 33;

		// Sending the buffer to processor 1
		MPI_Send(&buffer, 1, MPI_INT, 1, 4, MPI_COMM_WORLD);		

	} else if(rank == 1) { // If we are on processor 1

		// Receveing the buffer from processor 0
		MPI_Recv(&buffer, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);

		// Getting some informations about the comunication
		recvd_tag = status.MPI_TAG;
		recvd_from = status.MPI_SOURCE;
		MPI_Get_count(&status, MPI_INT, &recvd_count);

		// Printing some informations
		printf("Message received successfully\n");
		printf("Receiver: %d\n", rank);
		printf("Sender: %d\n", recvd_from);		
		printf("Tag: %d\n", recvd_tag);
		printf("Message length: %d\n", recvd_count);
		printf("Message content: %d\n", buffer);

	}

	MPI_Finalize();
	return 0;

}
