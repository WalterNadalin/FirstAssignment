/*
 * Copyright (C) 2015 - 2016 Master in High Performance Computing
 *
 * Adapted from the net by  Giuseppe Brandino. 
 * Last modified by Alberto Sartori. 
 * Addedd time and promoted to long long all important variables
 */

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
  // coordinates
  double x, y ;

  // number of points inside the circle
  long long int M, local_M ; 
  double pi ;
   
  // times 
  double start_time, end_time;   
  int myid , numprocs , proc ;
  MPI_Status status;
  MPI_Request request;
  // master process
  int master = 0;
  int tag = 123;

  // initialization phase
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);

  if ( argc <=1) {
    fprintf (stderr , " Usage : mpi -np n %s number_of_iterations \n", argv[0] ) ;
    MPI_Finalize() ;
    exit(-1) ;
  }

  long long int N = atoll(argv[1])/numprocs;
// take time of processors after initial I/O operation
  start_time = MPI_Wtime(); // MPI Wall time: routine that gives the time

  // initialize random numbers 
  srand48(SEED*(myid+1)) ; // seed the number generator
  local_M=0;
  long long int i;
  for (i=0; i<N ; i++) { // doing the computation
    // take a point P(x,y) inside the unit square
    x = drand48(); 
    y = drand48();
      
    // check if the point P(x,y) is inside the circle
    if ((x*x + y*y)<1)
      local_M++;
  }

  // here everybody has completed the computation, now we have to collect data
  // just one processor (processor 0) is collecting the data: serial part of the computation
  if (myid == master) { // if I am the master process gather results from others
    M = local_M ; // local_M (number of points that locally are within the unit circle) becomes M that we consider to be the global M
    // now the processor 0 is ready to receive
    for (proc=1; proc<numprocs ; proc++) { // loop over all the processors but the processor 0 (th master)
      MPI_Recv(&local_M,1,MPI_LONG_LONG,proc /*from whom receive the data*/,tag,MPI_COMM_WORLD,&status ) ;
      // printf("\n %d", status.MPI_SOURCE);
      // sleep(1);
      M += local_M ;
    }
    pi = 4.0*M/(N*numprocs) ; // computation of pi: we multiply by the number of processors
    end_time=MPI_Wtime(); // getting the whole time that it takes to complete the computation
    printf ( "\n # of trials = %llu , estimate of pi is %1.9f \n", N*numprocs, pi ) ;
    printf ( "\n # walltime on master processor : %10.8f \n", end_time - start_time ) ;
  }
  else {   // for all the slave processes send results to the master /

 //   printf ( " Processor %d sending results = %llu to master process \n", myid, local_M) ;
//    int time_to_sleep=1*myid; // asking every processor to wait in order to get the correct order in the output
//    sleep(time_to_sleep);

//    MPI_Send(&local_M/*location where to store the data*/ , 1/*how many data*/ ,MPI_LONG_LONG/*long long int*/, master , tag ,MPI_COMM_WORLD) ; 
// MPI_Send is supposed to be blocking: this means that it returns only when the processor which is meant to receive aknowledge the data 
// this implies that after MPI_Send we can modify local_M with no worries	   

// In reality MPI_Send is not really blocking in the sense that the completition condition is just that all the data has been sent
// indeed if it was blocking we'll expect to see processors complete the task in order, that is to say processor 1, then 2 and then 3, but this does not happen
 
     MPI_Ssend(&local_M, 1, MPI_LONG_LONG, master, tag, MPI_COMM_WORLD); 
// Synchronous sending: returning only when the receveing operation is completed
// that is to say the local buffer is full but it takes some time to deal with the local buffer which is not account for in the blocking

// Even now the processors aren't in the correct order since now we have the same non blocking behaviour on the receveing side
// since the processor does not say when it has done receveing all the data

// So synchronous send is blocking but not for the receiver
  
     end_time=MPI_Wtime(); // MPI_Wtime() is called independently by all the processors
			   // so there is no interference between the processes and, in this way, this is a measure of the time of each particular process 
     printf ( "\n # walltime on processor %i : %10.8f \n",myid, end_time - start_time ) ;
  }

  MPI_Finalize() ; // let MPI finish up /

}
