# First assignment
*First Foundation of High Performance Computing assignment - University of Trieste - Academic Year 2021/2022*

## Section 1: MPI programming 

1. Implement in C an MPI program using P processors on a ring (i.e. a simple 1D topology where each processor has a left and right neighbour). The program should implement a stream of messages in both directions. Model network performance and discuss scalability in term of the number of processors. 

2. Implement a simple 3d matrix-matrix addition in parallel using a 1D,2D and 3D distribution of data using virtual topology and study its scalability in term of communication within a single THIN node. Use here just collective operations to communicate among MPI processes. Program should accept as input the sizes of the matrixes and should then allocate the matrix and initialize them using double precision random numbers.

## Section 2: measure MPI point to point performance
Use the Intel MPI benchmark to estimate latency and bandwidth of all available combinations of topologies and networks on ORFEO computational nodes. Use both IntelMPI and openmpi latest version libraries available on ORFEO. Report numbers, graph and also fit data obtained against the simple communication model we discussed. Compare estimated latency and bandwidth parameters against the one provided by a least-square fitting model. 

## Section 3: compare performance observed against performance model for Jacobi solver
The application we are using here is a Jacobi application already discussed in class.
