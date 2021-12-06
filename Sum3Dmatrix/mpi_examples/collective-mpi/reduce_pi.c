#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#define USE MPI
#define SEED 35791246

int main ( int argc , char *argv[ ] )
{
  // Coordinates
  double x, y;

  // Number of points inside the circle
  long long int M, local_M; 
  double pi;
   
  // Times and other informations about the communication 
  double start_time, end_time;   
  int myid, numprocs, proc;
  MPI_Status status;
  MPI_Request request;

  int master = 0; // master process
  int tag = 123;

  // Initialization phase
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  if (argc <=1) {

    fprintf(stderr, "Usage : mpi -np n %s number_of_iterations\n", argv[0]);
    MPI_Finalize() ;
    exit(-1) ;
 
  }

  long long int N = atoll(argv[1]) / numprocs;

  start_time = MPI_Wtime();
 
  srand48(SEED * (myid + 1));

  local_M = 0;

  long long int i;
  for (i = 0; i < N ; i ++) { 

    // Generating randomly the coordinates of a point inside the unite square
    x = drand48(); 
    y = drand48();      

    if ((x * x + y * y) < 1) // Checking if the point is inside the unit circle
      local_M ++;
  
  }

  // Reducing the sum 
  MPI_Reduce(&local_M, &M, 1, MPI_INT, MPI_SUM, master, MPI_COMM_WORLD);

  if (myid == master) { 
 
    pi = 4.0 * M / (N * numprocs);

    end_time = MPI_Wtime();

    printf("\n # of trials = %llu, estimate of pi is %1.9f \n", N*numprocs, pi);
    printf("\n # walltime on master processor : %10.8f \n", end_time - start_time);

  }
  else {

     end_time = MPI_Wtime();

     printf("\n # walltime on processor %i : %10.8f \n",myid, end_time - start_time);
 
  }

  MPI_Finalize(); 

}
