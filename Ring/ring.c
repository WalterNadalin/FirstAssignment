// IMplementation of the first exercise

#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[])
{

        // Some informations about communication and processes
        int rank, size, recvd_cnt;
        MPI_Status left_sts, right_sts;
        FILE *info_file;
        FILE *time_file;

        info_file = fopen("info.txt", "w");
        fclose(info_file);

        MPI_Init(&argc, &argv); // Initialization
        MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Getting the rank
        MPI_Comm_size(MPI_COMM_WORLD, &size); // Getting the size

        // Defining who are the right and left processors on the ring
        int left_prcs = rank - 1, right_prcs = rank + 1;
        if(rank == 0) left_prcs = size - 1;  // The first processor has the last on its left
        if(rank == size - 1) right_prcs = 0; // The last processor has the first on its right

        // Defining some other useful variables
        int my_tag = rank * 10, left_tag = my_tag, right_tag = my_tag; // Initial tags to send out
        int msgleft = rank, msgright = -rank; // Creating the initial messagges to send out
        int left_bfr, right_bfr; // Variables that will contain the received messages from the left and from the right respectively
        int np = 0, count = 1, flag = 1;

        float start_time = MPI_Wtime();

        while(flag) {

                if(rank % 2 == 0) { // If the rank is even then first send and then receive for both the left and right stream

                        // Left stream
                        MPI_Send(&msgleft, count, MPI_INT, left_prcs, left_tag, MPI_COMM_WORLD); // Sending to the left
                        MPI_Recv(&right_bfr, count, MPI_INT, right_prcs, MPI_ANY_TAG, MPI_COMM_WORLD, &right_sts); // Receiving from the right

                        // Right stream
                        MPI_Send(&msgright, count, MPI_INT, right_prcs, right_tag, MPI_COMM_WORLD); // Sending to the right
                        MPI_Recv(&left_bfr, count, MPI_INT, left_prcs, MPI_ANY_TAG, MPI_COMM_WORLD, &left_sts); // Receiving from the left

                } else { // If the rank is odd then first receive and then send for both the left and right stream

                        // Left stream
                        MPI_Recv(&right_bfr, count, MPI_INT, right_prcs, MPI_ANY_TAG, MPI_COMM_WORLD, &right_sts); // Receiving from the right
                        MPI_Send(&msgleft, count, MPI_INT, left_prcs, left_tag, MPI_COMM_WORLD); // Sending to the left

	                // Right stream
                        MPI_Recv(&left_bfr, count, MPI_INT, left_prcs, MPI_ANY_TAG, MPI_COMM_WORLD, &left_sts); // Receiving from the left
                        MPI_Send(&msgright, count, MPI_INT, right_prcs, right_tag, MPI_COMM_WORLD); // Sending to the right

                }

                // Printing some informations about the messagge received
                // printf("\nSender: %d | Receiver: %d | Content: %d | Tag: %d\n", left_sts.MPI_SOURCE, rank, left_bfr, left_sts.MPI_TAG);
                // printf("\nSender: %d | Receiver: %d | Content: %d | Tag: %d\n", right_sts.MPI_SOURCE, rank, right_bfr, right_sts.MPI_TAG);

                // Updating properly the content of the messagges received
                msgleft = right_bfr - rank;
                msgright = left_bfr + rank;

                // Updating the tags with the ones received
                left_tag = right_sts.MPI_TAG;
                right_tag = left_sts.MPI_TAG;

                if(left_tag == my_tag)
                        if(right_tag == my_tag)
                                 flag = 0;

                // Increasing the count of messages received
                MPI_Get_count(&right_sts, MPI_INT, &recvd_cnt);
                np += recvd_cnt;
                MPI_Get_count(&left_sts, MPI_INT, &recvd_cnt);
                np += recvd_cnt;

	}

	MPI_Barrier(MPI_COMM_WORLD);
	float end_time = MPI_Wtime();

        info_file = fopen("info.txt", "a");
        fprintf(info_file, "I am process %d and i have received %d messages. My final messages have tag %d and value %d,%d\n", rank, np, left_tag, left_bfr, right_bfr);
        fclose(info_file);

        time_file = fopen("time.dat", "a");
        fprintf(time_file, "%d %d %f\n", size, rank, end_time - start_time);
        fclose(time_file);

        MPI_Finalize();

        return 0;

}
